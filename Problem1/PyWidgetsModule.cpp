#include "PyWidgetsFunctions.h"

extern PyMethodDef methods[];

//--------------------------------------------------------------------------

PyMODINIT_FUNC PyInit_pywidgets() {
    static PyModuleDef modDef = {
        PyModuleDef_HEAD_INIT,
        "pywidgets",
        "Qt-widgets module",
        -1,
        methods
    };

    PyObject* module = PyModule_Create(&modDef);
    if (module == NULL) {
        return NULL;
    }

    REGISTER_TYPE(module, Application);
    REGISTER_TYPE(module, Widget);
    REGISTER_TYPE(module, Label);
    REGISTER_TYPE(module, VBoxLayout);
    REGISTER_TYPE(module, PushButton);

    return module;
}
