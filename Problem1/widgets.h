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

inline void Object_Delete(Object* object) {
    delete object;
}

//----------------------------------------------------------------------------------------

struct Application : public virtual QApplication, public virtual Object {
    Application();

private:
    // Затычка для поддержки заданного интерфейса (Application_New без параметров)
    static int argc;
    static char* argv[];
};

inline Application* Application_New() {
    return new Application();
}

inline int Application_Exec(Application* app) {
    return app->exec();
}

//----------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------

struct Layout : public virtual QLayout, public virtual Object {
};

inline void Layout_AddWidget(Layout* layout, Widget* widget) {
    layout->addWidget(widget);
}

inline void Widget_SetLayout(Widget* widget, Layout* layout) {
    widget->setLayout(layout);
}

//----------------------------------------------------------------------------------------

struct VBoxLayout : public virtual QVBoxLayout, public virtual Object {
    VBoxLayout(Widget* parent) : QVBoxLayout(parent), Object("VBoxLayout") {}
};

inline struct VBoxLayout* VBoxLayout_New(Widget* parent) {
    return new VBoxLayout(parent);
}

//----------------------------------------------------------------------------------------

struct Label : public virtual QLabel, public virtual Object {
    Label(Widget* parent) : QLabel(parent), Object("Label") {}
};

inline Label* Label_New(Widget* parent) {
    return new Label(parent);
}

inline void Label_SetText(Label* label, const char* text) {
    label->setText(text);
}

//----------------------------------------------------------------------------------------

struct PushButton : public virtual QPushButton, public virtual Object {
    PushButton(Widget* parent) : QPushButton(parent), Object("PushButton") {}
};

inline PushButton* PushButton_New(Widget* widget) {
    return new PushButton(widget);
}

inline void PushButton_SetText(PushButton* button, const char* text) {
    button->setText(text);
}

template <typename Callable>
inline void PushButton_SetOnClicked(PushButton* button, Callable handler) {
    QObject::connect(button, &QPushButton::clicked, [button, handler]() {
        handler(button);
    });
}

#endif // WIDGETS_H
