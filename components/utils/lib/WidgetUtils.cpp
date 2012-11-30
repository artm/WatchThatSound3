#include "WidgetUtils.hpp"

QWidget * WidgetUtils::load_form(QWidget *widget, const QString &url, bool add_to_layout)
{
    QUiLoader loader;
    QFile file(url);
    Q_ASSERT( file.exists() );
    file.open(QFile::ReadOnly);
    QWidget * form = loader.load(&file, widget);
    file.close();

    if (add_to_layout && widget->layout())
        widget->layout()->addWidget( form );

    return widget;
}

QDialog *WidgetUtils::load_dialog(QWidget *widget, const QString &url)
{
    return qobject_cast<QDialog *>(load_form( widget, url, false ));
}


