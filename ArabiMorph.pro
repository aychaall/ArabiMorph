QT += core gui widgets

CONFIG += c++17
TARGET = ArabiMorph
TEMPLATE = app

# Source files
SOURCES += \
    main_qt.cpp \
    src/Root.cpp \
    src/Node.cpp \
    src/BinarySearchTree.cpp \
    src/TreeVisualizationWidget.cpp \
    src/MainWindow.cpp

# Header files
HEADERS += \
    include/Root.h \
    include/Node.h \
    include/BinarySearchTree.h \
    include/TreeVisualizationWidget.h \
    include/MainWindow.h

# Include paths
INCLUDEPATH += include

# Windows: UTF-8
win32 {
    QMAKE_CXXFLAGS += /utf-8
    RC_ICONS = resources/app.ico
}

# macOS
macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 11.0
}

# Compiler flags
unix:!macx {
    QMAKE_CXXFLAGS += -std=c++17
}

# Output directory
DESTDIR = bin
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui
