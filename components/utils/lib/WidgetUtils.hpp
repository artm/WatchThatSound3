#ifndef WIDGETUTILS_HPP
#define WIDGETUTILS_HPP

#include "stable.h"
#include "Exception.hpp"

class WidgetUtils
{
public:
    struct ResourceNotFound : public Exception {
        ResourceNotFound(const QString& url, const char * file, int line)
            : Exception( QString("Resource not found: %1").arg(url), file, line)
        {}
    };

    static QWidget * load_form(QWidget * widget, const QString& url, bool use_layout = true);
    static QDialog * load_dialog(QWidget * widget, const QString& url);
    static void replace_widget(QWidget * container, const QString& widget_name, QWidget * new_widget);
};

#endif // WIDGETUTILS_HPP
