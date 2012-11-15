#include "TestWtsIntegration.hpp"
#include "startpage/StartPage"

TestWtsIntegration::TestWtsIntegration(const QStringList& _args) :
    QObject(qApp), args(_args), shell(NULL)
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

void TestWtsIntegration::shouldStartWithStartPage()
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

QWidget * TestWtsIntegration::find_main_window()
{
    foreach(QWidget * widget, QApplication::topLevelWidgets() ) {
        QMainWindow * window = qobject_cast<QMainWindow *>(widget);
        if (window)
            return window;
    }
    return NULL;
}
