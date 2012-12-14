#include "stable.h"

class timelinewidgetsTest: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase() {
    }
    
    void cleanupTestCase() {
    }
    
    void fails() {
        QFAIL("Implement tests for timelinewidgets");
    }
};

QTEST_MAIN(timelinewidgetsTest)
#include "timelinewidgets_test.moc"

