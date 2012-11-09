#include "TestWtsIntegration.hpp"

TestWtsIntegration::TestWtsIntegration(const QStringList& _args) :
    QObject(qApp), args(_args)
{
}

int TestWtsIntegration::runTests()
{
    try {
        return QTest::qExec( this, args );
    } catch (...) {
        return 1;
    }
}
