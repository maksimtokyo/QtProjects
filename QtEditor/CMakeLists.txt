cmake_minimum_required(VERSION 3.19)
project(QtEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(QT_CREATOR_SKIP_MAINTENANCE_TOOL_PROVIDER ON)

# Путь к Qt5 (укажи свой, если нужно)
# set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5")

find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets Script Network)


add_executable(QtRunner
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)


target_link_libraries(QtRunner
    PRIVATE
        Qt5::Core
        Qt5::Widgets
        Qt5::Network
        Qt5::Script
)

include(GNUInstallDirs)

install(TARGETS QtRunner
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
