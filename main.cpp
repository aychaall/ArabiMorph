#include <QApplication>
#include <QFont>
#include "./include/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Arabic Morphological Engine");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("ArabicMorph");

    // Set default application font
    QFont appFont("Segoe UI", 10);
    app.setFont(appFont);

    MainWindow window;
    window.show();

    return app.exec();
}
