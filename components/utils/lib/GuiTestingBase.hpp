#ifndef GUITESTINGBASE_HPP
#define GUITESTINGBASE_HPP

#include "stable.h"

class GuiTestingBase : public QObject
{
    Q_OBJECT

public:
    GuiTestingBase();

    void set_default(QWidget*& widget) {
        if (widget == NULL)
            widget = widget_under_test;
    }

    // defaulting to widget_under_test
    QAbstractButton * button(const QString& button_text, QWidget * top_widget = NULL);
    void press(const QString& button_text, QWidget * top_widget = NULL);

    template<typename WidgetType>
    WidgetType find_widget_with_text( QWidget * container, const QString& text )
    {
        Q_ASSERT(container);
        foreach(WidgetType child_widget, container->findChildren<WidgetType>()) {
            if ( child_widget->objectName() == text || child_widget->text().contains(text) )
                return child_widget;
        }
        return NULL;
    }

    template<typename WidgetType>
    QList<WidgetType> find_label_siblings( QWidget * container, const QString& label_text )
    {
        QLabel * label = find_widget_with_text<QLabel*>(container, label_text);
        Q_ASSERT( label );
        return label->parentWidget()->findChildren<WidgetType>();
    }

    template<typename WidgetType>
    WidgetType find_sibling_with_text( QWidget * widget, const QString& sibling_text )
    {
        return find_widget_with_text<WidgetType>(widget->parentWidget(), sibling_text);
    }

protected:
    QWidget * widget_under_test;
};

#endif // GUITESTINGBASE_HPP
