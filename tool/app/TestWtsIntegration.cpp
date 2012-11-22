#include "TestWtsIntegration.hpp"
#include "WtsShell.hpp"
#include "startpage/StartPage"


TestWtsIntegration::TestWtsIntegration(const QStringList& _args) :
    args(_args), shell(NULL)
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

void TestWtsIntegration::initTestCase()
{
}

void TestWtsIntegration::cleanupTestCase()
{
}

void TestWtsIntegration::init()
{
    shell = new WtsShell();
}

void TestWtsIntegration::cleanup()
{
    delete shell;
}

void TestWtsIntegration::application_starts_with_start_page()
{
    // Given application has started
    shell->assemble();
    shell->start();
    // When I look at the main window
    QWidget * main_window = find_main_window();
    QVERIFY(main_window);
    // Then I see the start page

    StartPage * start_page = main_window->findChild<StartPage *>();
    QVERIFY(start_page);
    QVERIFY(start_page->isVisible());
}

void TestWtsIntegration::clicking_continue_opens_project_editor()
{
    // Given start page is displayed
    shell->assemble();
    shell->start();
    QWidget * main_window = find_main_window();
    StartPage * start_page = main_window->findChild<StartPage *>();
    // And a project is selected
    start_page->select("projects", 0);
    // When I click "Continue Project"
    press( "Continue Project", start_page );
    // Then I see project editor
    QFAIL("Test that this is project editor");
}

QWidget * TestWtsIntegration::find_main_window()
{
    foreach(QWidget * widget, QApplication::topLevelWidgets() ) {
        QMainWindow * window = qobject_cast<QMainWindow *>(widget);
        if (window)
            return window;
    }
    return NULL;
}

