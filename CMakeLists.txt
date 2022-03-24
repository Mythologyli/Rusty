cmake_minimum_required(VERSION 3.21)
project(Rusty)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        WebEngineWidgets
        REQUIRED
        )

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Rusty WIN32
        main.cpp
        mainwindow.cpp
        utility/utility.cpp
        binarizeimage/binarizeimage.cpp
        playvideo/playvideo.cpp
        resource.qrc
        )

target_link_libraries(Rusty
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::WebEngineWidgets
        )

target_link_libraries(Rusty ${OpenCV_LIBS})
