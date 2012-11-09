#include "stable.h"

class CustomWidgetsTest: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase() {
    }

    void cleanupTestCase() {
    }

    void fails() {
        QFAIL("Implement tests for CustomWidgets");
    }
};

QTEST_MAIN(CustomWidgetsTest)
#include "customwidgets_test.moc"

