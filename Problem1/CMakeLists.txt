project(qt-cmake-project)
cmake_minimum_required(VERSION 3.0)
find_package(Qt5 COMPONENTS Core Widgets)

find_package(PythonLibs 3.4 REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(${PYTHON_INCLUDE_DIRS})

message(${PYTHON_INCLUDE_DIRS})

set(SOURCES
    main.cpp
    )

set(HEADERS widgets.h)

qt5_wrap_ui(UI_SRC ${FORMS})
qt5_wrap_cpp(MOC_SRC ${HEADERS})

add_executable(
    window_app
    ${SOURCES} ${UI_SRC} ${MOC_SRC}
    )

add_definitions(-DQT_NO_KEYWORDS)

target_link_libraries(window_app
    ${Qt5Core_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${PYTHON_LIBRARIES}
    )
