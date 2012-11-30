#ifndef WIDGETUTILS_HPP
#define WIDGETUTILS_HPP

#include "stable.h"

class WidgetUtils
{
public:
    static QWidget * load_form(QWidget * widget, const QString& url, bool add_to_layout = true);
    static QDialog * load_dialog(QWidget * widget, const QString& url);
};

#endif // WIDGETUTILS_HPP
