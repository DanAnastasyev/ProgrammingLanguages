#include "widgets.h"

const char* ApplicationClassName = "Application";

// Определяем конструкторы в .cpp файле, чтобы не мусорить наружу
//  констатными строками с названиями классов (хоть и жертвуем inline-ом)
Application::Application() :
    QApplication(argc, argv), Object(ApplicationClassName) {}

int Application::argc = 1;
char* Application::argv[] = {"Widget.exe"};
