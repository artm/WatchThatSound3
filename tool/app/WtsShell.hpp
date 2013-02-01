#ifndef WTSSHELL_HPP
#define WTSSHELL_HPP

#include "stable.h"

class WtsShell : public QObject
{
    Q_OBJECT
public:
    explicit WtsShell(QObject *parent = 0);
    virtual ~WtsShell();

    void add_widget(const QString& tag, QWidget* widget);
    QWidget * widget(const QString& tag);
    template<typename T>
    T widget(const QString& tag)
    {
        return qobject_cast<T>(widget(tag));
    }

    QWidget * current_page();

signals:

public slots:
    void assemble();
    void start();
    void update_sender_style();
    void on_sampleNameEdit_editingFinished();

private:
    class Detail;
    QScopedPointer<Detail> detail;
};

#endif // WTSSHELL_HPP
