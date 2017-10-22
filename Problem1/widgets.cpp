/*
 * С++ библиотека-обертка над QT
 */

#include "widgets.h"

// Определяем конструкторы в .cpp файле, чтобы не мусорить наружу
//  констатными строками с названиями классов (хоть и жертвуем inline-ом)

const char* ApplicationClassName = "Application";
const char* WidgetClassName = "Widget";
const char* VBoxLayoutClassName = "VBoxLayout";
const char* PushButtonClassName = "PushButton";

Application::Application() :
    QApplication(argc, argv), Object(ApplicationClassName) {}

int Application::argc = 1;
char* Application::argv[] = {"Widget.exe"};

Widget::Widget(Widget* parent) :
    QWidget(parent), Object(WidgetClassName) {}

VBoxLayout::VBoxLayout(Widget* parent) :
    QVBoxLayout(parent), Object(VBoxLayoutClassName) {}

PushButton::PushButton(Widget* parent) :
    QPushButton(parent), Object(PushButtonClassName) {}
