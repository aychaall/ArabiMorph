#include <QApplication>
#include <QFont>
#include <QFontDatabase>
#include <QDir>
#include "../include/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("ArabiMorph");
    app.setApplicationDisplayName("ArabiMorph — Arabic Morphological Search Engine");
    app.setOrganizationName("ARABIMORPH");

    // Set Arabic-capable font
    QFont defaultFont("Segoe UI", 11);
    defaultFont.setHintingPreference(QFont::PreferFullHinting);
    app.setFont(defaultFont);

    MainWindow w;
    w.show();
    return app.exec();
}
