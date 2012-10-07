#include "stable.h"

#include "startpage/StartPage"

class StartPageTest: public QObject
{
    Q_OBJECT
    StartPage * start_page;

private slots:
    void initTestCase() {
        // given start page is displayed...
        start_page = new StartPage();
        start_page->show();
    }
    void cleanupTestCase() {
        // don't blink the window if unnecessary
        start_page->hide();
    }


    void contains_item_collections()
    {
        QWidget * area;

#define TEST_LABEL(where, name,label_text) \
    do { \
    QVERIFY( where->findChild<QLabel*>(name) ); \
    QVERIFY( where->findChild<QLabel*>(name)->isVisible() ); \
    QVERIFY( where->findChild<QLabel*>(name)->text().contains(label_text) ); \
    } while(false)

#define TEST_AREA(where, name, label_text) \
    do { \
    area = start_page->findChild<QWidget*>( name ); \
    QString label_name = QString(name) + "_label"; \
    QVERIFY( area ); \
    QVERIFY( area->isVisible() ); \
    TEST_LABEL( area->parent(), label_name, label_text ); \
    } while(false)


        TEST_AREA( start_page, "library", "Library" );
        TEST_AREA( start_page, "projects", "Projects" );
        TEST_AREA( start_page, "study_material", "Study material" );
        TEST_AREA( start_page, "get_started", "Get started" );
    }

    void buttons_under_start_page_areas()
    {
#define TEST_BUTTON( area_name, button_name, button_text ) \
    do { \
    QWidget * area = start_page->findChild<QWidget*>( area_name ); \
    QPushButton * button = area->parent()->findChild<QPushButton*>( button_name ); \
    QVERIFY( button ); \
    QVERIFY( button->text().contains( button_text ) ); \
    } while(false)
        TEST_BUTTON( "library", "add_video", "Add existing video" );
        TEST_BUTTON( "library", "new_project", "New Project" );
        TEST_BUTTON( "projects", "continue_project", "Continue Project" );
        TEST_BUTTON( "study_material", "open_study_material", "Open" );
        TEST_BUTTON( "get_started", "open_get_started", "Open" );
    }

    void open_inactive_on_startup() {
        QVERIFY( ! start_page->findChild<QWidget*>("new_project")->isEnabled() );
        QVERIFY( ! start_page->findChild<QWidget*>("continue_project")->isEnabled() );
        QVERIFY( ! start_page->findChild<QWidget*>("open_study_material")->isEnabled() );
        QVERIFY( ! start_page->findChild<QWidget*>("open_get_started")->isEnabled() );
    }
};

QTEST_MAIN(StartPageTest)
#include "startpage_test.moc"

