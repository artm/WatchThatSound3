#include "stable.h"

class %ComponentName%Test: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase() {
    }

    void cleanupTestCase() {
    }

    void fails() {
        QFAIL("Implement tests for %ComponentName%");
    }
};

QTEST_MAIN(%ComponentName%Test)
#include "%ComponentName:l%_test.moc"
