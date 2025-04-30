#ifndef ASS2SRT_UI_MAINWINDOW_H
#define ASS2SRT_UI_MAINWINDOW_H

#include "config.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtWidgets> // NOLINT

namespace ass2srt::ui {
class MainWindow : public QMainWindow { // NOLINT
Q_OBJECT // NOLINT

    public : explicit MainWindow(QWidget* parent = nullptr) // NOLINT
    : QMainWindow(parent)
    , ui()
    , assFiles({})
    {
        const auto appTitle = QString("ass2srt converter v%1.%2.%3").arg(ASS2SRT_VERSION_MAJOR).arg(ASS2SRT_VERSION_MINOR).arg(ASS2SRT_VERSION_PATCH);

        ui.setupUi(this);
        setWindowTitle(appTitle);

        connect(ui.selectAssButton, &QPushButton::clicked, this, &MainWindow::selectAssFiles); // NOLINT
        connect(ui.selectOutputFolderButton, &QPushButton::clicked, this, &MainWindow::selectOutputDir); // NOLINT
        connect(ui.processButton, &QPushButton::clicked, this, &MainWindow::processFiles); // NOLINT
    }

private:
    Ui_MainWindow ui;

    QStringList assFiles;
    QString outputDirectory = "";

    void selectAssFiles();
    void selectOutputDir();
    void processFiles();
    void enableConversionIfNeded();
};
}

#endif
