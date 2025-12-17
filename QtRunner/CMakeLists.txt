cmake_minimum_required(VERSION 3.19)
project(QtRunner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(QT_CREATOR_SKIP_MAINTENANCE_TOOL_PROVIDER ON)

find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets Script Network)

add_executable(QtRunner
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    scriptpainter.h scriptpainter.cpp
)

target_link_libraries(QtRunner
    PRIVATE
        Qt5::Core
        Qt5::Widgets
        Qt5::Script
        Qt5::Network
)

include(GNUInstallDirs)

install(TARGETS QtRunner
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
