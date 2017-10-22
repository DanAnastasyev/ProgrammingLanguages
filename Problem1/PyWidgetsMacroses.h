/*
 * Макросы, используемые для генерации кода классов-оберток
 */

#ifndef PY_WIDGETS_MACROSES_H
#define PY_WIDGETS_MACROSES_H

#define PY_CLASS_WRAPPER(ClassName, methods, new_method) \
struct Py##ClassName { \
    PyObject_HEAD \
    ClassName* pImpl; \
}; \
 \
static void Py_Dealloc##ClassName(Py##ClassName* self) { \
    delete self->pImpl; \
    Py_TYPE(self)->tp_free((PyObject*)self); \
} \
 \
static PyTypeObject Py_Type##ClassName = { \
    PyVarObject_HEAD_INIT(NULL, 0) \
    "pywidgets."#ClassName,            /* tp_name */ \
    sizeof(Py##ClassName),             /* tp_basicsize */ \
    0,                                 /* tp_itemsize */ \
    (destructor)Py_Dealloc##ClassName, /* tp_dealloc */ \
    0,                                 /* tp_print */ \
    0,                                 /* tp_getattr */ \
    0,                                 /* tp_setattr */ \
    0,                                 /* tp_reserved */ \
    0,                                 /* tp_repr */ \
    0,                                 /* tp_as_number */ \
    0,                                 /* tp_as_sequence */ \
    0,                                 /* tp_as_mapping */ \
    0,                                 /* tp_hash  */ \
    0,                                 /* tp_call */ \
    0,                                 /* tp_str */ \
    0,                                 /* tp_getattro */ \
    0,                                 /* tp_setattro */ \
    0,                                 /* tp_as_buffer */ \
    Py_TPFLAGS_DEFAULT | \
        Py_TPFLAGS_BASETYPE,           /* tp_flags */ \
    #ClassName" object",               /* tp_doc */ \
    0,                                 /* tp_traverse */ \
    0,                                 /* tp_clear */ \
    0,                                 /* tp_richcompare */ \
    0,                                 /* tp_weaklistoffset */ \
    0,                                 /* tp_iter */ \
    0,                                 /* tp_iternext */ \
    methods,                           /* tp_methods */ \
    NULL,                              /* tp_members */ \
    0,                                 /* tp_getset */ \
    0,                                 /* tp_base */ \
    0,                                 /* tp_dict */ \
    0,                                 /* tp_descr_get */ \
    0,                                 /* tp_descr_set */ \
    0,                                 /* tp_dictoffset */ \
    NULL,                              /* tp_init */ \
    0,                                 /* tp_alloc */ \
    new_method,                        /* tp_new */ \
};

#define REGISTER_TYPE(module, ClassName) \
if (PyType_Ready(&Py_Type##ClassName) < 0) { \
    return NULL; \
} \
Py_INCREF(&Py_Type##ClassName); \
PyModule_AddObject(module, #ClassName, (PyObject*)&Py_Type##ClassName);

#endif // PY_WIDGETS_MACROSES_H
