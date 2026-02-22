QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = ArabicMorphEngine
TEMPLATE = app

SOURCES += \
    main.cpp \
    src/MainWindow.cpp \
    src/Root.cpp \
    src/Node.cpp \
    src/BinarySearchTree.cpp \
    src/hashtable.cpp \
    src/core_engine.cpp \
    src/TreeVisualizationWidget.cpp

HEADERS += \
    include/Root.h \
    include/Node.h \
    include/BinarySearchTree.h \
    include/hashtable.h \
    include/core_engine.h \
    include/MainWindow.h \
    include/TreeVisualizationWidget.h

INCLUDEPATH += include
