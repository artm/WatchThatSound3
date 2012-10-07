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

    void contains_item_collections() {
        QVERIFY( start_page->findChild<QWidget*>("projects") );
        QVERIFY( start_page->findChild<QWidget*>("projects")->isVisible() );

        QVERIFY( start_page->findChild<QWidget*>("study_material") );
        QVERIFY( start_page->findChild<QWidget*>("study_material")->isVisible() );

        QVERIFY( start_page->findChild<QWidget*>("help") );
        QVERIFY( start_page->findChild<QWidget*>("help")->isVisible() );
    }

    void open_inactive_on_startup() {
        QVERIFY( start_page->findChild<QWidget*>("start_project") );
        QVERIFY( ! start_page->findChild<QWidget*>("start_project")->isEnabled() );

        QVERIFY( start_page->findChild<QWidget*>("open_study_material") );
        QVERIFY( ! start_page->findChild<QWidget*>("open_study_material")->isEnabled() );

        QVERIFY( start_page->findChild<QWidget*>("open_help") );
        QVERIFY( ! start_page->findChild<QWidget*>("open_help")->isEnabled() );
    }
};

QTEST_MAIN(StartPageTest)
#include "startpage_test.moc"

