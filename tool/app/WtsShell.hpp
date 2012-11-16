#ifndef WTSSHELL_HPP
#define WTSSHELL_HPP

#include "stable.h"

class WtsShell : public QObject
{
    Q_OBJECT
public:
    explicit WtsShell(QObject *parent = 0);
    virtual ~WtsShell();

signals:

public slots:
    void assemble();
    void start();
private:
    class Detail;
    QScopedPointer<Detail> detail;
};

#endif // WTSSHELL_HPP
