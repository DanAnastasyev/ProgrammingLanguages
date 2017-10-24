/*
 * Набор функций-оберток над сущностями из библиотеки
 */

#ifndef PY_WIDGETS_FUNCTIONS_H
#define PY_WIDGETS_FUNCTIONS_H

#include "PyWidgetsClasses.h"

static PyObject* PyWidgets_Application_New(PyObject* module, PyObject* noargs) {
    return PyApplication_new(&Py_TypeApplication, NULL, NULL);
}

static PyObject* PyWidgets_Widget_New(PyObject* module, PyObject* noargs) {
    return PyWidget_new(&Py_TypeWidget, NULL, NULL);
}

static PyObject* PyWidgets_VBoxLayout_New(PyObject* module, PyObject* args) {
    return PyVBoxLayout_new(&Py_TypeVBoxLayout, args, NULL);
}

static PyObject* PyWidgets_Label_New(PyObject* module, PyObject* args) {
    return PyLabel_new(&Py_TypeLabel, args, NULL);
}

static PyObject* PyWidgets_PushButton_New(PyObject* module, PyObject* args) {
    return PyPushButton_new(&Py_TypePushButton, args, NULL);
}

//----------------------------------------------------------------------------------------

static PyObject* PyWidgets_Application_Exec(PyObject* module, PyObject* args) {
    PyApplication* pyApplication = NULL;
    if (!PyArg_ParseTuple(args, "O", &pyApplication)) {
        return NULL;
    }

    return PyApplication_Exec(pyApplication);
}

//----------------------------------------------------------------------------------------

static PyObject* PyWidgets_Widget_SetWindowTitle(PyObject* module, PyObject* args) {
    PyWidget* pyWidget = NULL;
    const char* title;
    if (!PyArg_ParseTuple(args, "Os", &pyWidget, &title)) {
        return NULL;
    }

    Widget_SetWindowTitle(pyWidget->pImpl, title);
    Py_RETURN_NONE;
}

static PyObject* PyWidgets_Widget_SetSize(PyObject* module, PyObject* args) {
    PyWidget* pyWidget = NULL;
    int width = 0, height = 0;
    if (!PyArg_ParseTuple(args, "Oii", &pyWidget, &width, &height)) {
        return NULL;
    }

    Widget_SetSize(pyWidget->pImpl, width, height);
    Py_RETURN_NONE;
}

static PyObject* PyWidgets_Widget_SetVisible(PyObject* module, PyObject* args) {
    PyWidget* pyWidget = NULL;
    bool isVisible = 0;
    if (!PyArg_ParseTuple(args, "Op", &pyWidget, &isVisible)) {
        return NULL;
    }
    Widget_SetVisible(pyWidget->pImpl, isVisible);
    Py_RETURN_NONE;
}

static PyObject* PyWidgets_Widget_SetLayout(PyObject* module, PyObject* args) {
    PyWidget* pyWidget = NULL;
    PyVBoxLayout* pyLayout = NULL;
    if (!PyArg_ParseTuple(args, "OO", &pyWidget, &pyLayout)) {
        return NULL;
    }
    Widget_SetLayout(pyWidget->pImpl, (Layout*) pyLayout->pImpl);
    Py_RETURN_NONE;
}

//----------------------------------------------------------------------------------------

static PyObject* PyWidgets_Layout_AddWidget(PyObject* module, PyObject* args) {
    PyObject* pyWidget = NULL;
    PyVBoxLayout* pyLayout = NULL;
    if (!PyArg_ParseTuple(args, "OO", &pyLayout, &pyWidget)) {
        return NULL;
    }
    PyVBoxLayout_AddWidget(pyLayout, Py_BuildValue("(O)", pyWidget));
    Py_RETURN_NONE;
}

//----------------------------------------------------------------------------------------

static PyObject* PyWidgets_Label_SetText(PyObject* module, PyObject* args) {
    PyLabel* pyLabel = NULL;
    const char* text;
    if (!PyArg_ParseTuple(args, "Os", &pyLabel, &text)) {
        return NULL;
    }

    Label_SetText(pyLabel->pImpl, text);
    Py_RETURN_NONE;
}

//----------------------------------------------------------------------------------------

static PyObject* PyWidgets_PushButton_SetText(PyObject* module, PyObject* args) {
    PyPushButton* pyPushButton = NULL;
    const char* text;
    if (!PyArg_ParseTuple(args, "Os", &pyPushButton, &text)) {
        return NULL;
    }

    PushButton_SetText(pyPushButton->pImpl, text);
    Py_RETURN_NONE;
}

//----------------------------------------------------------------------------------------

static PyObject* PyWidgets_Object_GetClassName(PyObject* module, PyObject* args) {
    PyApplication* pyApp;
    if (PyArg_ParseTuple(args, "O", &pyApp)) {
        return PyApplication_GetClassName(pyApp);
    }
    PyWidget* pyWidget;
    if (PyArg_ParseTuple(args, "O", &pyWidget)) {
        return PyWidget_GetClassName(pyWidget);
    }
    PyVBoxLayout* pyVBoxLayout;
    if (PyArg_ParseTuple(args, "O", &pyVBoxLayout)) {
        return PyVBoxLayout_GetClassName(pyVBoxLayout);
    }
    PyPushButton* pyPushButton;
    if (PyArg_ParseTuple(args, "O", &pyPushButton)) {
        return PyPushButton_GetClassName(pyPushButton);
    }
    PyLabel* pyLabel;
    if (PyArg_ParseTuple(args, "O", &pyLabel)) {
        return PyLabel_GetClassName(pyLabel);
    }
    return NULL;
}

//----------------------------------------------------------------------------------------

static PyMethodDef methods[] = {
    {"Application_New", PyWidgets_Application_New, METH_NOARGS, "Application_New"},
    {"Application_Exec", PyWidgets_Application_Exec, METH_VARARGS, "Application_Exec"},
    {"Widget_New", PyWidgets_Widget_New, METH_NOARGS, "Widget_New"},
    {"VBoxLayout_New", PyWidgets_VBoxLayout_New, METH_VARARGS, "VBoxLayout_New"},
    {"Label_New", PyWidgets_Label_New, METH_VARARGS, "Label_New"},
    {"PushButton_New", PyWidgets_PushButton_New, METH_VARARGS, "PushButton_New"},
    {"Widget_SetWindowTitle", PyWidgets_Widget_SetWindowTitle, METH_VARARGS, "Widget_SetWindowTitle"},
    {"Widget_SetSize", PyWidgets_Widget_SetSize, METH_VARARGS, "Widget_SetSize"},
    {"Widget_SetVisible", PyWidgets_Widget_SetVisible, METH_VARARGS, "Widget_SetVisible"},
    {"Widget_SetLayout", PyWidgets_Widget_SetLayout, METH_VARARGS, "Widget_SetLayout"},
    {"Layout_AddWidget", PyWidgets_Layout_AddWidget, METH_VARARGS, "Layout_AddWidget"},
    {"Label_SetText", PyWidgets_Label_SetText, METH_VARARGS, "Label_SetText"},
    {"PushButton_SetText", PyWidgets_PushButton_SetText, METH_VARARGS, "PushButton_SetText"},
    {"Object_GetClassName", PyWidgets_Object_GetClassName, METH_VARARGS, "Object_GetClassName"},
    {NULL, NULL, 0, NULL}
};

#endif // PY_WIDGETS_FUNCTIONS_H
