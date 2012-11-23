#include "GuiTestingBase.hpp"

GuiTestingBase::GuiTestingBase() :
    QObject(0),
    widget_under_test(0)
{
}

void GuiTestingBase::press(const QString &button_text, QWidget * top_widget)
{
    set_default(top_widget);
    button(button_text, top_widget)->click();
}

QAbstractButton * GuiTestingBase::button(const QString &button_text, QWidget * top_widget)
{
    set_default(top_widget);
    return find_widget_with_text<QAbstractButton*>(top_widget, button_text);
}

QWidget * GuiTestingBase::find_main_window()
{
    foreach(QWidget * widget, QApplication::topLevelWidgets() ) {
        QMainWindow * window = qobject_cast<QMainWindow *>(widget);
        if (window)
            return window;
    }
    return NULL;
}

