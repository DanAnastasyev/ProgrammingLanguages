project(problem-1)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Widgets)
find_package(PythonLibs 3.4 REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(${PYTHON_INCLUDE_DIRS})

add_executable(
    widgets
    main.cpp widgets.h widgets.cpp
    )

python_add_module(
    _pywidgets
    PyWidgetsFunctionsModule.cpp widgets.h widgets.cpp PyWidgetsClasses.h
    PyWidgetsMacroses.h PyWidgetsFunctions.h
    )

python_add_module(
    pywidgets
    PyWidgetsModule.cpp widgets.h widgets.cpp PyWidgetsClasses.h
    PyWidgetsMacroses.h
    )

add_definitions(-DQT_NO_KEYWORDS)

target_link_libraries(widgets
    ${Qt5Core_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${PYTHON_LIBRARIES}
    )

target_link_libraries(_pywidgets
    ${Qt5Core_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${PYTHON_LIBRARIES}
    )

target_link_libraries(pywidgets
    ${Qt5Core_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${PYTHON_LIBRARIES}
    )
