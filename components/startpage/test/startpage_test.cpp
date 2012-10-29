#include "stable.h"

#include "startpage/StartPage"
#include "dialog_spec_helper.hpp"

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

        TEST_BUTTON( "library", "add_video", "Add an Existing Video" );
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
        DialogSpecHelper helper(false);
        connect(&helper, SIGNAL(run_injected_code(QWidget*)), SLOT( verify_new_project_dialog(QWidget*)));
        button->click();
        QVERIFY( helper.injected_method_succeded() );
    }

    void import_video()
    {
        QPushButton * add_button = find_widget_with_text<QPushButton*>(start_page, "Add an Existing Video");
        QVERIFY( add_button );

        QSignalSpy spy(start_page, SIGNAL(import_video(QString)));

        // test if the file dialog shows up
        DialogSpecHelper helper(true);
        connect(&helper, SIGNAL(run_injected_code(QWidget*)), SLOT(test_add_video_dialog(QWidget*)));
        add_button->click();
        QVERIFY( helper.injected_method_succeded() );
        QCOMPARE( spy.count(), 1 );
        QList<QVariant> arguments = spy.takeFirst();
        QVERIFY( arguments.at(0).toString().endsWith("fake.mov") );
    }

    void creating_a_project()
    {
        // the same given as above...
        QTreeView * area = start_page->findChild<QTreeView*>( "library" );
        QModelIndex index_00 = library_items.index(0,0);
        area->setCurrentIndex( index_00 );
        QPushButton * button = area->parent()->findChild<QPushButton*>( "new_project" );
        DialogSpecHelper helper(true);
        connect(&helper,SIGNAL(run_injected_code(QWidget*)), SLOT(fill_in_project_name(QWidget*)));
        QSignalSpy spy(start_page, SIGNAL(create_new_project(QString, QString)));
        button->click();
        QCOMPARE(spy.count(), 1);
        QList<QVariant> arguments = spy.takeFirst();
        QCOMPARE(arguments.at(0).toString(), QString("Test Project"));
        QCOMPARE(arguments.at(1), library_items.data( index_00 ));
    }

    void opens_reference_documents_data()
    {
        QTest::addColumn<QString>("area_name");
        QTest::addColumn<QString>("button_name");
        QTest::newRow("study material") << "study_material" << "open_study_material";
        QTest::newRow("get started") << "get_started" << "open_get_started";
    }

    void opens_reference_documents()
    {
        QFETCH( QString, area_name );
        QFETCH( QString, button_name );

        QTreeView * area = start_page->findChild<QTreeView*>( area_name );
        QAbstractItemModel * model = area->model();
        for(int row = 0; row < area->model()->rowCount(); ++row) {
            QModelIndex the_index = model->index(row,0);
            area->setCurrentIndex( the_index );
            QPushButton * button = area->parent()->findChild<QPushButton*>( button_name );
            // this will fire after the modal dialog is up
            QSignalSpy spy(start_page, SIGNAL(open_file(QString)));
            button->click();
            QCOMPARE(spy.count(), 1);
            QList<QVariant> arguments = spy.takeFirst();
            QCOMPARE(arguments.at(0), model->data( the_index ) );
        }
    }

    // unit tests
    void test_selected_filename_data()
    {
        QTest::addColumn<QString>("area_name");
        QTest::addColumn<int>("row_num");
        QTest::addColumn<QString>("file_name");

        QTest::newRow("none in library")   << "library" << -1 << QString();
        QTest::newRow("first in library")  << "library" <<  0 << "WTS_Bit_1.mov";
        QTest::newRow("second in library") << "library" <<  1 << "WTS_Bit_2.mov";
        QTest::newRow("third in library")  << "library" <<  2 << "WTS_Bit_3.mov";

        QTest::newRow("none in projects")   << "projects" << -1 << QString();
        QTest::newRow("first in projects")  << "projects" <<  0 << "WTS_Bit_1.mov";
        QTest::newRow("second in projects") << "projects" <<  1 << "WTS_Bit_1.mov";
        QTest::newRow("third in projects")  << "projects" <<  2 << "WTS_Bit_2.mov";

        QTest::newRow("none in study_material")   << "study_material" << -1 << QString();
        QTest::newRow("first in study_material")  << "study_material" <<  0 << "Rhythm";
        QTest::newRow("second in study_material") << "study_material" <<  1 << "Pitch";
        QTest::newRow("third in study_material")  << "study_material" <<  2 << "Surprises";

        QTest::newRow("none in get_started")   << "get_started" << -1 << QString();
        QTest::newRow("first in get_started")  << "get_started" <<  0 << "Maak je eigen.pdf";
        QTest::newRow("second in get_started") << "get_started" <<  1 << "Jaques explains.mov";
        QTest::newRow("third in get_started")  << "get_started" <<  2 << "Quick start.pdf";

    }

    void test_selected_filename()
    {
        QFETCH(QString, area_name);
        QFETCH(int, row_num);
        QFETCH(QString, file_name);

        QTreeView * area = start_page->findChild<QTreeView*>( area_name );
        area->setCurrentIndex( (row_num >= 0)
                               ? area->model()->index(row_num, 0)
                               : QModelIndex() );
        QCOMPARE( start_page->selected_filename( area_name ) , file_name );
    }

public slots:
    // public slots don't get run as individual tests
    void reject_active_modal() {
        QDialog * active_modal = qobject_cast<QDialog*>(qApp->activeModalWidget());
        QVERIFY( active_modal );
        active_modal->reject();
    }

    void fill_in_project_name(QWidget*active_modal)
    {
        QLineEdit* field = active_modal->findChild<QLineEdit*>("project_name");
        QVERIFY( field );
        field->setText("Test Project");
    }

    void verify_new_project_dialog(QWidget*active_modal)
    {
        // see if the dialog looks right...
        QVERIFY( active_modal->windowTitle() == "New Project from Video" );
        QVERIFY( find_widget_with_text<QLabel*>(active_modal, "Choose a name for your project" ) );
        QVERIFY( find_widget_with_text<QLineEdit*>(active_modal, "Untitled Project" ) );
        QVERIFY( find_widget_with_text<QPushButton*>( active_modal, "Cancel") );
        QVERIFY( find_widget_with_text<QPushButton*>( active_modal, "Create New Project") );
    }

    void test_add_video_dialog(QWidget * active_modal) {
        QCOMPARE( active_modal->windowTitle(), QString("Add an Existing Video") );
        QFileDialog * dialog = qobject_cast<QFileDialog *>(active_modal);
        QVERIFY( dialog );
        // make it accept non-existing file just for testing
        dialog->setFileMode(QFileDialog::AnyFile);
        dialog->setAcceptMode(QFileDialog::AcceptSave);
        dialog->selectFile( "fake.mov" );
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
};

QTEST_MAIN(StartPageTest)
#include "startpage_test.moc"

