#include "mainwindow.hpp"
#include "../ass.hpp"
#include "../io.hpp"
#include "../merge.hpp"
#include "../srt.hpp"
#include <QFileDialog>
#include <QtWidgets>
#include <stdexcept>

using namespace ass2srt::ui;

void MainWindow::selectAssFiles()
{
    assFiles = QFileDialog::getOpenFileNames(this, "Select .ASS siles", QString(), "ASS files (*.ass)");
#ifdef WIN32
    for (auto& assFile : assFiles) {
        assFile.replace('/', '\\');
    }
#endif
    ui.fileListWidget->clear();
    ui.fileListWidget->addItems(assFiles);
    enableConversionIfNeded();
}

void MainWindow::selectOutputDir()
{
    outputDirectory = QFileDialog::getExistingDirectory(this, "Select .SRT files output directory");
#ifdef WIN32
    outputDirectory.replace('/', '\\');
#endif
    ui.outputFolderEdit->setText(outputDirectory);
    enableConversionIfNeded();
}

void MainWindow::processFiles()
{
    ui.progressBar->setRange(0, static_cast<int>(assFiles.size()));
    ui.progressBar->setValue(0);
    ui.processButton->setEnabled(false);

    QString lastFile;
    try {
        int progress = 0;

        for (const auto& file : assFiles) {
            lastFile = file;
            const QFileInfo fileInfo(file);
            const QString baseName = fileInfo.completeBaseName();
            const QString srtFilePath = QDir(outputDirectory).filePath(baseName + ".srt");
#ifdef WIN32
            auto input_file = io::open_istream(file.toStdWString());
#else
            auto input_file = io::open_istream(file.toStdString());
#endif
            auto raw_lines = ass::parse_ass_stream(*input_file);
            auto merged_lines = merge::merge_subtitles_parts(raw_lines);

#ifdef WIN32
            auto output_file = io::open_ostream(srtFilePath.toStdWString());
#else
            auto output_file = io::open_ostream(srtFilePath.toStdString());
#endif
            srt::write_srt_stream(merged_lines, *output_file);

            ui.progressBar->setValue(++progress);
            QApplication::processEvents();
        }

        QMessageBox::information(this, "Success", "All files are processed successfully!");

    } catch (const std::runtime_error& e) {
        QMessageBox::critical(this, "Error", QString("Error processing the file %1: %2").arg(lastFile, e.what()));
        ui.progressBar->setValue(0);
        QApplication::processEvents();
    }

    enableConversionIfNeded();
}

void MainWindow::enableConversionIfNeded()
{
    ui.processButton->setEnabled(!assFiles.isEmpty() && !outputDirectory.isEmpty());
}
