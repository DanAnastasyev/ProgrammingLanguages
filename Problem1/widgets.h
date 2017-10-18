#ifndef WIDGETS_H
#define WIDGETS_H

#include <QApplication>
#include <QLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

struct Object : public virtual QObject {
    Object(const char* _name) : name(_name) {}

    const char* GetClassName() const {
        return name;
    }

private:
    const char* name;
};

inline const char* Object_GetClassName(Object* obj) {
    return obj->GetClassName();
}

struct Application : public virtual QApplication, public virtual Object {
    Application(int& argc, char** argv) :
        QApplication(argc, argv), Object("Application") {}
};

inline Application* Application_New(int& argc, char** argv) {
    return new Application(argc, argv);
}

inline int Application_Exec(Application* app) {
    return app->exec();
}

struct Widget : public virtual QWidget, public virtual Object {
    Widget(Widget* parent) : QWidget(parent), Object("Widget") {}
};

inline Widget* Widget_New(Widget* parent) {
    return new Widget(parent);
}

inline void Widget_SetWindowTitle(Widget* widget, const char* title) {
    widget->setWindowTitle(title);
}

inline void Widget_SetSize(Widget* widget, int w, int h) {
    widget->resize(w, h);
}

inline void Widget_SetVisible(Widget* widget, bool isVisible) {
    widget->setVisible(isVisible);
}

struct Layout : public virtual QLayout, public virtual Object {
    Layout(Widget* parent) : QLayout(parent), Object("Layout") {}
};

struct VBoxLayout : public virtual QVBoxLayout, public virtual Object {
    VBoxLayout(Widget* parent) : QVBoxLayout(parent), Object("VBoxLayout") {}
};

inline struct VBoxLayout* VBoxLayout_New(Widget* parent) {
    return new VBoxLayout(parent);
}

void Widget_SetLayout(Widget* widget, Layout* layout) {
    widget->setLayout(layout);
}

struct Label : public virtual QLabel, public virtual Object {
    Label(Widget* parent) : QLabel(parent), Object("Label") {}
};

inline Label* Label_New(Widget* parent) {
    return new Label(parent);
}

inline void Label_SetText(Label* label, const char* text) {
    label->setText(text);
}

inline void Layout_AddWidget(Layout* layout, Widget* widget) {
    layout->addWidget(widget);
}

struct PushButton : public virtual QPushButton, public virtual Object {
    PushButton(Widget* parent) : QPushButton(parent), Object("PushButton") {}
};

inline PushButton* PushButton_New(Widget* widget) {
    return new PushButton(widget);
}

inline void PushButton_SetText(PushButton* button, const char* text) {
    button->setText(text);
}

typedef void (*HandlerPointer)(Object*);
inline void PushButton_SetOnClicked(PushButton* button, HandlerPointer handler) {

    QObject::connect(button, &QPushButton::clicked, [button, handler]() {
        handler(button);
    });
}

inline void Object_Delete(Object* object) {
    delete object;
}

#endif // WIDGETS_H
