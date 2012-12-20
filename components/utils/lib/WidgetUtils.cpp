#include "WidgetUtils.hpp"

QWidget * WidgetUtils::load_form(QWidget *widget, const QString &url, bool use_layout)
{
    QUiLoader loader;
    QFile file(url);

    if (!file.exists())
        RAISE_A(ResourceNotFound, url);

    file.open(QFile::ReadOnly);
    QWidget * form = loader.load(&file, widget);
    file.close();

    if (use_layout && widget && form->layout()) {
        if (widget->layout()) {
            delete widget->layout();
        }
        widget->setLayout( form->layout() );
    }

    return form;
}

QDialog *WidgetUtils::load_dialog(QWidget *widget, const QString &url)
{
    return qobject_cast<QDialog *>(load_form( widget, url, false ));
}

void WidgetUtils::replace_widget(QWidget *container, const QString &widget_name, QWidget *new_widget)
{
    new_widget->setObjectName(widget_name);

    QWidget * old_widget = container->findChild<QWidget*>(widget_name);
    // destroy old widget when this function returns
    QScopedPointer<QWidget> destroyer(old_widget);

    QWidget * parent = old_widget->parentWidget();
    Q_ASSERT(parent);
    QLayout * layout = parent->layout();
    if (layout) {
        QBoxLayout * box_layout = qobject_cast<QBoxLayout*>(layout);
        if (box_layout) {
            int index = box_layout->indexOf( old_widget );
            box_layout->removeWidget(old_widget );
            box_layout->insertWidget(index, new_widget);
            return;
        }

        QFormLayout * form_layout = qobject_cast<QFormLayout*>(layout);
        if (form_layout) {
            int row;
            QFormLayout::ItemRole role;
            form_layout->getWidgetPosition ( old_widget, &row, &role);
            form_layout->removeWidget(old_widget);
            form_layout->setWidget(row, role, new_widget);
            return;
        }

        QGridLayout * grid_layout = qobject_cast<QGridLayout *>(layout);
        if (grid_layout) {
            int row, column, rowSpan, columnSpan;
            int index = grid_layout->indexOf( old_widget );
            grid_layout->getItemPosition ( index, &row, &column, &rowSpan, &columnSpan );
            grid_layout->removeWidget( old_widget );
            grid_layout->addWidget( new_widget, row, column, rowSpan, columnSpan );
            return;
        }

        QStackedLayout * stacked_layout = qobject_cast<QStackedLayout *>(layout);
        if (stacked_layout) {
            int index = stacked_layout->indexOf( old_widget );
            stacked_layout->removeWidget(old_widget);
            stacked_layout->insertWidget(index, new_widget);
            return;
        }

        qFatal( "Unsupported layout in WidgetUtils::replace_widget" );
    } else {
        old_widget->setParent(NULL);
        new_widget->setParent(container);
    }

}

void WidgetUtils::update_widget_style(QWidget *widget)
{
    widget->style()->unpolish( widget );
    widget->style()->polish( widget );
}


