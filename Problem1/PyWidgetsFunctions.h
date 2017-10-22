#ifndef PY_WIDGETS_FUNCTIONS_H
#define PY_WIDGETS_FUNCTIONS_H

#include "PyWidgetsClasses.h"

static PyObject* PyWidgets_Application_New(PyObject* module, PyObject* noargs) {
    return PyApplication_new(&Py_TypeApplication, NULL, NULL);
}

static PyObject* PyWidgets_Widget_New(PyObject* module, PyObject* noargs) {
    return PyWidget_new(&Py_TypeWidget, NULL, NULL);
}

static PyObject* PyWidgets_Widget_SetWindowTitle(PyObject* module, PyObject* args) {
    PyWidget* pyWidget;
    const char* title;
    if (!PyArg_ParseTuple(args, "Os", &pyWidget, &title)) {
        return NULL;
    }

    Widget_SetWindowTitle(pyWidget->pImpl, title);
    Py_RETURN_NONE;
}

static PyObject* PyWidgets_Widget_SetSize(PyObject* module, PyObject* args) {
    PyWidget* pyWidget;
    int width = 0, height = 0;
    if (!PyArg_ParseTuple(args, "Oii", &pyWidget, &width, &height)) {
        return NULL;
    }

    Widget_SetSize(pyWidget->pImpl, width, height);
    Py_RETURN_NONE;
}

static PyObject* PyWidgets_Widget_SetVisible(PyObject* module, PyObject* args) {
    PyWidget* pyWidget;
    bool isVisible = 0;
    if (!PyArg_ParseTuple(args, "Op", &pyWidget, &isVisible)) {
        return NULL;
    }
    Widget_SetVisible(pyWidget->pImpl, isVisible);
    Py_RETURN_NONE;
}

static PyObject* PyWidgets_Object_GetClassName(PyObject* module, PyObject* args) {
    PyApplication* pyApp;
    if (PyArg_ParseTuple(args, "O", &pyApp)) {
        return PyApplication_GetClassName(pyApp);
    }
    PyWidget* pyWidget;
    if (PyArg_ParseTuple(args, "O", &pyWidget)) {
        return PyWidget_GetClassName(pyWidget);
    }
    return NULL;
}

static PyMethodDef methods[] = {
  {"Application_New", PyWidgets_Application_New, METH_NOARGS, "Application_New"},
  {"Widget_New", PyWidgets_Widget_New, METH_NOARGS, "Widget_New"},
  {"Widget_SetWindowTitle", PyWidgets_Widget_SetWindowTitle, METH_VARARGS, "Widget_SetWindowTitle"},
  {"Widget_SetSize", PyWidgets_Widget_SetSize, METH_VARARGS, "Widget_SetSize"},
  {"Widget_SetVisible", PyWidgets_Widget_SetVisible, METH_VARARGS, "Widget_SetVisible"},
  {"Object_GetClassName", PyWidgets_Object_GetClassName, METH_VARARGS, "Object_GetClassName"},
  {NULL, NULL, 0, NULL}
};

#endif // PY_WIDGETS_FUNCTIONS_H
