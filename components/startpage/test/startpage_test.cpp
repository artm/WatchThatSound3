#include "stable.h"

#include "startpage/StartPage"

class StartPageTest: public QObject
{
    Q_OBJECT
    StartPage * start_page;
    QStandardItemModel library_items, projects_items, study_material_items, get_started_items;

private slots:
    void initTestCase() {
        // given start page is displayed...
        start_page = new StartPage();
        start_page->show();

        // some items for every area
        library_items.setColumnCount( 1 );
        library_items.appendRow( new QStandardItem("WTS_Bit_1.mov") );
        library_items.appendRow( new QStandardItem("WTS_Bit_2.mov") );
        library_items.appendRow( new QStandardItem("WTS_Bit_3.mov") );

        projects_items.setColumnCount( 1 );
        projects_items.appendRow( new QStandardItem("WTS_Bit_1.mov") );
        projects_items.appendRow( new QStandardItem("WTS_Bit_1.mov") );
        projects_items.appendRow( new QStandardItem("WTS_Bit_2.mov") );

        study_material_items.setColumnCount( 1 );
        study_material_items.appendRow( new QStandardItem("Rhythm") );
        study_material_items.appendRow( new QStandardItem("Pitch") );
        study_material_items.appendRow( new QStandardItem("Surprises") );

        get_started_items.setColumnCount( 1 );
        get_started_items.appendRow( new QStandardItem("Maak je eigen.pdf") );
        get_started_items.appendRow( new QStandardItem("Jaques explains.mov") );
        get_started_items.appendRow( new QStandardItem("Quick start.pdf") );

#define SET_MODEL( area_name, model ) \
    do { \
    QTreeView* area = start_page->findChild<QTreeView*>(area_name); \
    if (area) area->setModel( model ); \
    } while(false)

        SET_MODEL("library", &library_items);
        SET_MODEL("projects", &projects_items);
        SET_MODEL("study_material", &study_material_items);
        SET_MODEL("get_started", &get_started_items);

        start_page->connectSignals();

#undef SET_MODEL
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

#undef TEST_AREA
#undef TEST_LABEL
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

#undef TEST_BUTTON
    }

    void activate_open_buttons_on_selection() {
#define TEST_ACTIVATION( area_name, button_name ) \
    do { \
        QTreeView * area = start_page->findChild<QTreeView*>( #area_name ); \
        QPushButton * button = area->parent()->findChild<QPushButton*>( button_name ); \
        QVERIFY( ! button->isEnabled() ); \
        area->setCurrentIndex( area_name ## _items.index(0,0) ); \
        QVERIFY( button->isEnabled() ); \
        area->setCurrentIndex( QModelIndex() ); \
        QVERIFY( ! button->isEnabled() ); \
    } while(false)

        TEST_ACTIVATION( library, "new_project" );
        TEST_ACTIVATION( projects, "continue_project" );
        TEST_ACTIVATION( study_material, "open_study_material" );
        TEST_ACTIVATION( get_started, "open_get_started" );

#undef TEST_ACTIVATION
    }

    void new_project_opens_a_dialog()
    {
        QTreeView * area = start_page->findChild<QTreeView*>( "library" );
        area->setCurrentIndex( library_items.index(0,0) );
        QPushButton * button = area->parent()->findChild<QPushButton*>( "new_project" );
        // this will fire after the modal dialog is up
        QTimer::singleShot(1, this, SLOT( test_new_project_dialog_and_cancel() ));
        button->click();
        // test the application state after cancelling the dialog
        QVERIFY( ! qApp->activeModalWidget() );
        QCOMPARE( qApp->activeWindow(), start_page );
    }

    void creating_a_project()
    {
        // the same given as above...
        QTreeView * area = start_page->findChild<QTreeView*>( "library" );
        area->setCurrentIndex( library_items.index(0,0) );
        QPushButton * button = area->parent()->findChild<QPushButton*>( "new_project" );
        // this will fire after the modal dialog is up
        QTimer::singleShot(1, this, SLOT( fill_in_project_name_and_accept() ));
        QSignalSpy spy(start_page, SIGNAL(create_new_project(QString)));
        button->click();
        QCOMPARE(spy.count(), 1);
        QList<QVariant> arguments = spy.takeFirst();
        QCOMPARE(arguments.at(0).toString(), QString("Test Project"));
    }

public slots:
    // public slots don't get run as individual tests
    void fill_in_project_name_and_accept() {
        QDialog * active_modal = qobject_cast<QDialog*>(qApp->activeModalWidget());
        Acceptor acceptor( active_modal, true );
        QLineEdit* field = active_modal->findChild<QLineEdit*>("project_name");
        QVERIFY( field );
        field->setText("Test Project");
    }

    void test_new_project_dialog_and_cancel() {
        // see if the dialog looks right...
        QDialog * active_modal = qobject_cast<QDialog*>(qApp->activeModalWidget());
        QVERIFY( active_modal );
        Acceptor acceptor( active_modal, false );

        QVERIFY( active_modal->windowTitle() == "New Project from Video" );
        QVERIFY( find_widget_with_text<QLabel*>(active_modal, "Choose a name for your project" ) );
        QVERIFY( find_widget_with_text<QLineEdit*>(active_modal, "Untitled Project" ) );
        QVERIFY( find_widget_with_text<QPushButton*>( active_modal, "Cancel") );
        QVERIFY( find_widget_with_text<QPushButton*>( active_modal, "Create New Project") );
    }

private:
    // FIXME make this a shared utility
    template<typename WidgetType>
    WidgetType find_widget_with_text( QWidget * widget, const QString& text )
    {
        foreach(WidgetType child_widget, widget->findChildren<WidgetType>()) {
            if ( child_widget->text().contains(text) )
                return child_widget;
        }
        return NULL;
    }

    class Acceptor {
        QDialog * dialog;
        bool accept;
    public:
        Acceptor(QDialog * d, bool a) :
            dialog(d), accept(a)
        {}
        ~Acceptor() {
            if (dialog) {
                if (accept)
                    dialog->accept();
                else
                    dialog->reject();
            }
        }
    };

};

QTEST_MAIN(StartPageTest)
#include "startpage_test.moc"

