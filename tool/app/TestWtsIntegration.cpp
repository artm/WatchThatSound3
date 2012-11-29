#include "customwidgets/SlidingStackedWidget"
#include "startpage/StartPage"
#include "utils/Macros"
#include "utils/DialogSpecHelper"

#include "TestWtsIntegration.hpp"
#include "WtsShell.hpp"


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
    QWidget * start_page = new QWidget();
    shell->add_widget("start_page", start_page);
    // Given application has started
    shell->assemble();
    shell->start();

    // When I look at the main window
    QWidget * main_window = find_main_window();
    QVERIFY(main_window);

    // Then I see the start page
    QCOMPARE( shell->current_page(), start_page );
}

void TestWtsIntegration::clicking_continue_opens_project_editor()
{

    QWidget * fake_editor = new QWidget;
    shell->add_widget("project_editor", fake_editor);
    // Given start page is displayed
    shell->assemble();
    NOP_OR(shell->widget<SlidingStackedWidget *>("stacker"))->setSpeed(0);
    shell->start();

    // And a project is selected
    QWidget * main_window = find_main_window();
    StartPage * start_page = main_window->findChild<StartPage *>();
    start_page->select("projects", 0);

    // When I click "Continue Project"
    press( "Continue Project", start_page );

    // Then I see project editor
    QCOMPARE( shell->current_page(), fake_editor );
}

void TestWtsIntegration::clicking_new_project_opens_project_editor()
{
    // prepare to catch the dialog
    DialogSpecHelper helper(true);
    connect(&helper, SIGNAL(run_injected_code(QWidget*)), SLOT( verify_new_project_dialog(QWidget*)));

    QWidget * fake_editor = new QWidget;
    shell->add_widget("project_editor", fake_editor);
    // Given start page is displayed
    shell->assemble();
    NOP_OR(shell->widget<SlidingStackedWidget *>("stacker"))->setSpeed(0);
    shell->start();

    // And a video in library is selected
    QWidget * main_window = find_main_window();
    StartPage * start_page = main_window->findChild<StartPage *>();
    start_page->select("library", 0);

    // When I click "New Project"
    press( "New Project", start_page );

    // Then "New Project" dialog appears
    QVERIFY( helper.injected_method_succeded() );

    // Then I see project editor
    QCOMPARE( shell->current_page(), fake_editor );
}

void TestWtsIntegration::verify_new_project_dialog(QWidget*active_modal)
{
    // see if the dialog looks right...
    QVERIFY( active_modal->windowTitle() == "New Project from Video" );
    QVERIFY( find_widget_with_text<QLabel*>(active_modal, "Choose a name for your project" ) );
    QVERIFY( find_widget_with_text<QPushButton*>( active_modal, "Cancel") );
    QVERIFY( find_widget_with_text<QPushButton*>( active_modal, "Create New Project") );

    QLineEdit* field = find_widget_with_text<QLineEdit*>(active_modal, "Untitled Project" );
    QVERIFY( field );
    field->setText("Test Project");
}
