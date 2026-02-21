cmake_minimum_required(VERSION 3.16)
project(ArabiMorph VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Auto-process Qt MOC/UIC/RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 (or fall back to Qt5)
find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Gui Widgets)
    message(STATUS "Using Qt5")
else()
    message(STATUS "Using Qt6")
endif()

set(SOURCES
    main_qt.cpp
    src/Root.cpp
    src/Node.cpp
    src/BinarySearchTree.cpp
    src/TreeVisualizationWidget.cpp
    src/MainWindow.cpp
)

set(HEADERS
    include/Root.h
    include/Node.h
    include/BinarySearchTree.h
    include/TreeVisualizationWidget.h
    include/MainWindow.h
)

add_executable(ArabiMorph ${SOURCES} ${HEADERS})

target_include_directories(ArabiMorph PRIVATE include)

if(Qt6_FOUND)
    target_link_libraries(ArabiMorph PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
else()
    target_link_libraries(ArabiMorph PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
endif()

# Windows: UTF-8
if(MSVC)
    target_compile_options(ArabiMorph PRIVATE /utf-8)
endif()

# macOS bundle
if(APPLE)
    set_target_properties(ArabiMorph PROPERTIES MACOSX_BUNDLE TRUE)
endif()
