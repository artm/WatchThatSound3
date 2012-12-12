#ifndef WIDGETUTILS_HPP
#define WIDGETUTILS_HPP

#include "stable.h"

class WidgetUtils
{
public:
    static QWidget * load_form(QWidget * widget, const QString& url, bool use_layout = true);
    static QDialog * load_dialog(QWidget * widget, const QString& url);
    static void replace_widget(QWidget * container, const QString& widget_name, QWidget * new_widget);
};

#endif // WIDGETUTILS_HPP
