#include "ui/mainwindow.hpp"
#include <QtWidgets/QApplication>

using namespace ass2srt;

auto main(int argc, char** argv) -> int
{
    const QApplication app(argc, argv); // NOLINT
    ui::MainWindow window;

    window.show();

    return app.exec(); // NOLINT
}
