#ifndef TESTRUNNER_HPP
#define TESTRUNNER_HPP

#include "stable.h"

class TestRunner
{
public:
    TestRunner(const QStringList& args);
    int run();
protected:
    QStringList args;
};

#endif // TESTRUNNER_HPP
