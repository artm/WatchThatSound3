#include "stable.h"

class IntegrationTest: public QObject
{
    Q_OBJECT
private slots:
    void app_start() {
        QFAIL("TODO write a test that application starts");
    }
};

QTEST_MAIN(IntegrationTest)
#include "integration.moc"
