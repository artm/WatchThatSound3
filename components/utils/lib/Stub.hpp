#ifndef STUB_HPP
#define STUB_HPP

#include "stable.h"

class Stub : public QObject
{
    Q_OBJECT
public:
    explicit Stub(QObject *parent = 0);

signals:

public slots:
    void unimplemented();
};

#endif // STUB_HPP
