#ifndef TESTWTSINTEGRATION_HPP
#define TESTWTSINTEGRATION_HPP

#include "stable.h"

class TestWtsIntegration : public QObject
{
    Q_OBJECT
public:
    TestWtsIntegration(const QStringList& args);

    int runTests();
signals:

public slots:
protected:
    QStringList args;
};

#endif // TESTWTSINTEGRATION_HPP
