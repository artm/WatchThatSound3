#include "stable.h"

class ProjectEditorTest: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase() {
    }
    
    void cleanupTestCase() {
    }
    
    void fails() {
        QFAIL("Implement tests for ProjectEditor");
    }
};

QTEST_MAIN(ProjectEditorTest)
#include "projecteditor_test.moc"

