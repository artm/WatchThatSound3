#ifndef TESTWTSINTEGRATION_HPP
#define TESTWTSINTEGRATION_HPP

#include "stable.h"
#include "utils/GuiTestingBase"

class WtsShell;

class TestWtsIntegration : public GuiTestingBase
{
    Q_OBJECT
public:
    TestWtsIntegration(const QStringList& args);

    int runTests();

private slots:
    void initTestCase(); // will be called before the first testfunction is executed.
    void cleanupTestCase(); // will be called after the last testfunction was executed.
    void init(); // will be called before each testfunction is executed.
    void cleanup(); // will be called after every testfunction.

    void application_starts_with_start_page();
    void clicking_continue_opens_project_editor();

protected:
    QStringList args;
    WtsShell * shell;
};

#endif // TESTWTSINTEGRATION_HPP
