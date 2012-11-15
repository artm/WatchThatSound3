#ifndef WTSSHELL_HPP
#define WTSSHELL_HPP

#include <QObject>

class WtsShell : public QObject
{
    Q_OBJECT
public:
    explicit WtsShell(QObject *parent = 0);

signals:

public slots:
    void assemble();
    void start();

};

#endif // WTSSHELL_HPP
