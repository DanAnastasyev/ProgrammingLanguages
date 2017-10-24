/*
 * Набор классов-оберток над сущностями из библиотеки
 */

#ifndef PY_WIDGETS_CLASSES_H
#define PY_WIDGETS_CLASSES_H

#include <Python.h>
#include "PyWidgetsMacroses.h"
#include "widgets.h"

struct PyApplication;

static PyObject* PyApplication_GetClassName(PyApplication* self);
static PyObject* PyApplication_Exec(PyApplication* self);

static PyMethodDef PyApplication_methods[] = {
    {"get_class_name", (PyCFunction)PyApplication_GetClassName, METH_NOARGS, "Return class name"},
    {"exec", (PyCFunction)PyApplication_Exec, METH_NOARGS, "Runs application"},
    {NULL}
};
static PyObject* PyApplication_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

PY_CLASS_WRAPPER(Application, PyApplication_methods, PyApplication_new)

static PyObject* PyApplication_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyApplication* self = (PyApplication*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->pImpl = Application_New();
    }

    return (PyObject*)self;
}

static PyObject* PyApplication_GetClassName(PyApplication* self) {
    return PyUnicode_FromFormat("%s", Object_GetClassName((Object*) self->pImpl));
}

static PyObject* PyApplication_Exec(PyApplication* self) {
    return PyLong_FromLong(Application_Exec(self->pImpl));
}

//----------------------------------------------------------------------------------------

struct PyWidget;

static PyObject* PyWidget_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
static PyObject* PyWidget_GetClassName(PyWidget* self);
static PyObject* PyWidget_SetWindowTitle(PyWidget* self, PyObject* args);
static PyObject* PyWidget_SetSize(PyWidget* self, PyObject* args);
static PyObject* PyWidget_Visible(PyWidget* self, PyObject* args);
static PyObject* PyWidget_SetLayout(PyWidget* self, PyObject* args);

static PyMethodDef PyWidget_methods[] = {
    {"get_class_name", (PyCFunction)PyWidget_GetClassName, METH_NOARGS, "Returns class name"},
    {"set_window_title", (PyCFunction)PyWidget_SetWindowTitle, METH_VARARGS, "Sets window title"},
    {"set_size", (PyCFunction)PyWidget_SetSize, METH_VARARGS, "Sets window width and height"},
    {"set_visible", (PyCFunction)PyWidget_Visible, METH_VARARGS, "Sets window visibility"},
    {"set_layout", (PyCFunction)PyWidget_SetLayout, METH_VARARGS, "Sets layout"},
    {NULL}
};

PY_CLASS_WRAPPER(Widget, PyWidget_methods, PyWidget_new)

static PyObject* PyWidget_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyWidget* self = (PyWidget*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->pImpl = Widget_New(NULL);
    }

    return (PyObject*)self;
}

static PyObject* PyWidget_GetClassName(PyWidget* self) {
    return PyUnicode_FromFormat("%s", Object_GetClassName((Object*) self->pImpl));
}

static PyObject* PyWidget_SetWindowTitle(PyWidget* self, PyObject* args) {
    const char* title;
    if (!PyArg_ParseTuple(args, "s", &title)) {
        return NULL;
    }
    Widget_SetWindowTitle(self->pImpl, title);
    Py_RETURN_NONE;
}

static PyObject* PyWidget_SetSize(PyWidget* self, PyObject* args) {
    int width = 0, height = 0;
    if (!PyArg_ParseTuple(args, "ii", &width, &height)) {
        return NULL;
    }
    Widget_SetSize(self->pImpl, width, height);
    Py_RETURN_NONE;
}

static PyObject* PyWidget_Visible(PyWidget* self, PyObject* args) {
    bool isVisible = 0;
    if (!PyArg_ParseTuple(args, "p", &isVisible)) {
        return NULL;
    }
    Widget_SetVisible(self->pImpl, isVisible);
    Py_RETURN_NONE;
}

//----------------------------------------------------------------------------------------

struct PyVBoxLayout;

static PyObject* PyVBoxLayout_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
static PyObject* PyVBoxLayout_GetClassName(PyVBoxLayout* self);
static PyObject* PyVBoxLayout_AddWidget(PyVBoxLayout* self, PyObject* args);

static PyMethodDef PyVBoxLayout_methods[] = {
    {"get_class_name", (PyCFunction)PyVBoxLayout_GetClassName, METH_NOARGS, "Returns class name"},
    {"add_widget", (PyCFunction)PyVBoxLayout_AddWidget, METH_VARARGS, "Adds widget"},
    {NULL}
};

PY_CLASS_WRAPPER(VBoxLayout, PyVBoxLayout_methods, PyVBoxLayout_new)

static PyObject* PyVBoxLayout_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyVBoxLayout* self = (PyVBoxLayout*)type->tp_alloc(type, 0);

    PyWidget* parent = NULL;
    if (!PyArg_ParseTuple(args, "O", &parent)) {
        return NULL;
    }
    Py_INCREF(parent);
    if (self != NULL) {
        self->pImpl = VBoxLayout_New(parent->pImpl);
    }

    return (PyObject*)self;
}

static PyObject* PyVBoxLayout_GetClassName(PyVBoxLayout* self) {
    return PyUnicode_FromFormat("%s", Object_GetClassName((Object*) self->pImpl));
}

//----------------------------------------------------------------------------------------

static PyObject* PyWidget_SetLayout(PyWidget* self, PyObject* args) {
    PyVBoxLayout* layout = NULL;
    if (!PyArg_ParseTuple(args, "O", &layout)) {
        return NULL;
    }
    Py_INCREF(layout);
    Widget_SetLayout(self->pImpl, (Layout*) layout->pImpl);
    Py_RETURN_NONE;
}

//----------------------------------------------------------------------------------------

struct PyLabel;

static PyObject* PyLabel_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
static PyObject* PyLabel_GetClassName(PyLabel* self);
static PyObject* PyLabel_SetText(PyLabel* self, PyObject* args);

static PyMethodDef PyLabel_methods[] = {
    {"get_class_name", (PyCFunction)PyLabel_GetClassName, METH_NOARGS, "Returns class name"},
    {"set_text", (PyCFunction)PyLabel_SetText, METH_VARARGS, "Sets text"},
    {NULL}
};

PY_CLASS_WRAPPER(Label, PyLabel_methods, PyLabel_new)

static PyObject* PyLabel_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyLabel* self = (PyLabel*)type->tp_alloc(type, 0);

    PyWidget* parent = NULL;
    if (!PyArg_ParseTuple(args, "O", &parent)) {
        return NULL;
    }
    Py_INCREF(parent);
    if (self != NULL) {
        self->pImpl = Label_New(parent->pImpl);
    }

    return (PyObject*)self;
}

static PyObject* PyLabel_GetClassName(PyLabel* self) {
    return PyUnicode_FromFormat("%s", Object_GetClassName((Object*) self->pImpl));
}

static PyObject* PyLabel_SetText(PyLabel* self, PyObject* args) {
    const char* text;
    if (!PyArg_ParseTuple(args, "s", &text)) {
        return NULL;
    }
    Label_SetText(self->pImpl, text);
    Py_RETURN_NONE;
}

//----------------------------------------------------------------------------------------

struct PyPushButton;

static PyObject* PyPushButton_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
static PyObject* PyPushButton_GetClassName(PyPushButton* self);
static PyObject* PyPushButton_SetText(PyPushButton* self, PyObject* args);
static PyObject* PyPushButton_SetOnClicked(PyPushButton* self, PyObject* args);

static PyMethodDef PyPushButton_methods[] = {
    {"get_class_name", (PyCFunction)PyPushButton_GetClassName, METH_NOARGS, "Returns class name"},
    {"set_text", (PyCFunction)PyPushButton_SetText, METH_VARARGS, "Sets text"},
    {"set_on_clicked", (PyCFunction)PyPushButton_SetOnClicked, METH_VARARGS, "Sets onClick callback"},
    {NULL}
};

PY_CLASS_WRAPPER(PushButton, PyPushButton_methods, PyPushButton_new)

static PyObject* PyPushButton_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyPushButton* self = (PyPushButton*)type->tp_alloc(type, 0);

    PyWidget* parent = NULL;
    if (!PyArg_ParseTuple(args, "O", &parent)) {
        return NULL;
    }
    Py_INCREF(parent);
    if (self != NULL) {
        self->pImpl = PushButton_New(parent->pImpl);
    }

    return (PyObject*)self;
}

static PyObject* PyPushButton_GetClassName(PyPushButton* self) {
    return PyUnicode_FromFormat("%s", Object_GetClassName((Object*) self->pImpl));
}

static PyObject* PyPushButton_SetText(PyPushButton* self, PyObject* args) {
    const char* text;
    if (!PyArg_ParseTuple(args, "s", &text)) {
        return NULL;
    }
    PushButton_SetText(self->pImpl, text);
    Py_RETURN_NONE;
}

static PyObject* PyPushButton_SetOnClicked(PyPushButton* self, PyObject* args) {
    PyObject* callable = NULL;
    if (!PyArg_ParseTuple(args, "O", &callable)) {
        return NULL;
    }
    if (!PyCallable_Check(callable)) {
        return NULL;
    }
    Py_INCREF(callable);
    PyObject* callbackArg = Py_BuildValue("(O)", self);
    PushButton_SetOnClicked(self->pImpl, [callbackArg, callable](Object*) {
        PyObject_CallObject(callable, callbackArg);
    });
    Py_RETURN_NONE;
}

//----------------------------------------------------------------------------------------

static PyObject* PyVBoxLayout_AddWidget(PyVBoxLayout* self, PyObject* args) {
    PyLabel* label = NULL;
    if (PyArg_ParseTuple(args, "O", &label)) {
        Layout_AddWidget((Layout*) self->pImpl, (Widget*) label->pImpl);
        Py_RETURN_NONE;
    }
    PyPushButton* pushButton = NULL;
    if (PyArg_ParseTuple(args, "O", &pushButton)) {
        Layout_AddWidget((Layout*) self->pImpl, (Widget*) pushButton->pImpl);
        Py_RETURN_NONE;
    }
    return NULL;
}

#endif // PY_WIDGETS_CLASSES_H
