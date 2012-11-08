#include "stable.h"

#include "startpage/StartPage"
#include "dialog_spec_helper.hpp"

class StartPageTest: public QObject
{
    Q_OBJECT
    StartPage * start_page;
    QStandardItemModel library_items, projects_items, study_material_items, get_started_items;


    void open_buttons_data()
    {
        QTest::addColumn<QString>("title");
        QTest::addColumn<QString>("button_title");
        QTest::newRow("New Project") << "Library" << "New Project";
        QTest::newRow("Continue Project") << "Projects" << "Continue Project";
        QTest::newRow("Get started / Open") << "Get started" << "Open";
        QTest::newRow("Study material / Open") << "Study material" << "Open";
    }

    void all_buttons_data()
    {
        open_buttons_data();
        QTest::newRow("Add an Existing Video") << "Library" << "Add an Existing Video";
    }

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
    QAbstractItemView* area = start_page->findChild<QAbstractItemView*>(area_name); \
    if (area) area->setModel( model ); \
    } while(false)

        SET_MODEL("library", &library_items);
        SET_MODEL("projects", &projects_items);
        SET_MODEL("study_material", &study_material_items);
        SET_MODEL("get_started", &get_started_items);

        start_page->connect_signals();

#undef SET_MODEL
    }

    void cleanupTestCase() {
        // don't blink the window if unnecessary
        start_page->hide();
    }

    void contains_item_collections()
    {
        QVERIFY( area("Library") );
        QVERIFY( area("Projects") );
        QVERIFY( area("Study material") );
        QVERIFY( area("Get started") );
    }

    void buttons_under_start_page_areas_data()
    {
        all_buttons_data();
    }

    void buttons_under_start_page_areas()
    {
        QFETCH(QString, title);
        QFETCH(QString, button_title);
        QVERIFY( find_sibling_with_text<QPushButton*>(area(title), button_title ) );
    }

    void activate_open_buttons_on_selection_data() {
        open_buttons_data();
    }

    void activate_open_buttons_on_selection() {
        QFETCH(QString, title);
        QFETCH(QString, button_title);

        QPushButton * button = find_sibling_with_text<QPushButton *>( area(title), button_title );
        deselect(title);
        QVERIFY( ! button->isEnabled() );
        select(title);
        QVERIFY( button->isEnabled() );
    }

    void new_project_opens_a_dialog()
    {
        DialogSpecHelper helper(false);
        connect(&helper, SIGNAL(run_injected_code(QWidget*)), SLOT( verify_new_project_dialog(QWidget*)));

        select("Library");
        press("New Project");

        QVERIFY( helper.injected_method_succeded() );
    }

    void clicking_add_opens_a_file_dialog()
    {
        QSignalSpy spy(start_page, SIGNAL(import_video(QString)));
        DialogSpecHelper helper(true);
        connect(&helper, SIGNAL(run_injected_code(QWidget*)), SLOT(test_add_video_dialog(QWidget*)));

        press("Add an Existing Video");

        QCOMPARE( spy.count(), 1 );
        QVERIFY( spy.takeFirst().at(0).toString().endsWith("fake.mov") );
    }

    void creating_a_project()
    {
        // the same given as above...
        DialogSpecHelper helper(true);
        connect(&helper,SIGNAL(run_injected_code(QWidget*)), SLOT(fill_in_project_name(QWidget*)));
        QSignalSpy spy(start_page, SIGNAL(create_new_project(QString, QString)));

        select("Library");
        press("New Project");

        QCOMPARE(spy.count(), 1);
        QList<QVariant> args = spy.takeFirst();
        QCOMPARE(args.at(0).toString(), QString("Test Project"));
        QCOMPARE(args.at(1).toString(), start_page->selected_filename("library") );
    }

    void opens_reference_documents_data()
    {
        QTest::addColumn<QString>("area_title");
        QTest::newRow("Study material") << "Study material";
        QTest::newRow("Get started") << "Get started";
    }

    void opens_reference_documents()
    {
        QFETCH( QString, area_title );

        QAbstractItemView * reference_area = area(area_title);
        QVERIFY(reference_area);
        for(int row = 0; row < reference_area->model()->rowCount(); ++row) {
            // this will fire after the modal dialog is up
            QSignalSpy spy(start_page, SIGNAL(open_file(QString)));

            select( area_title, row );
            press( area_title, "Open" );

            QCOMPARE(spy.count(), 1);
            QCOMPARE(spy.takeFirst().at(0).toString(), start_page->selected_filename( reference_area->objectName() ));
        }
    }

    void deselect_on_loosing_focus_data()
    {
        QTest::addColumn<QString>("area_name");
        QTest::addColumn<QString>("next_area_name");
        QTest::addColumn<QString>("button_name");

        QTest::newRow("library") << "library" << "projects" << "new_project";
        QTest::newRow("projects") << "projects" << "library" << "continue_project";
        QTest::newRow("study_material") << "study_material" << "get_started" << "open_study_material";
        QTest::newRow("get_started") << "get_started" << "study_material" << "open_get_started";
    }

    void deselect_on_loosing_focus()
    {
        QFETCH( QString, area_name );
        QFETCH( QString, next_area_name );
        QFETCH( QString, button_name );

        QAbstractItemView * area = start_page->findChild<QAbstractItemView*>( area_name );
        QAbstractItemView * next_area = start_page->findChild<QAbstractItemView*>( next_area_name );
        QWidget * button = start_page->findChild<QWidget*>( button_name );

        Q_ASSERT(area);
        Q_ASSERT(next_area);
        Q_ASSERT(button);

        // - Given an item is selected in an area
        area->selectionModel()->select( area->model()->index(0,0), QItemSelectionModel::Select );
        area->setFocus(Qt::MouseFocusReason);
        // - When I select an item in a different area
        next_area->selectionModel()->select( next_area->model()->index(0,0), QItemSelectionModel::Select );
        next_area->setFocus(Qt::MouseFocusReason);
        // - Then originally selected item is deselected
        QVERIFY( area->selectionModel()->selectedRows().count() == 0 );
        // - And corresponding "open" button becomes inactive
        QVERIFY( ! button->isEnabled() );
    }

    void continue_project_emits_open_project()
    {
        QSignalSpy spy(start_page, SIGNAL(open_project(QString)));

        select("Projects");
        press("Continue Project");

        QCOMPARE( spy.count(), 1);
        QCOMPARE( spy.takeFirst().at(0).toString() , start_page->selected_filename("projects") );
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

        QAbstractItemView * area = start_page->findChild<QAbstractItemView*>( area_name );
        area->setCurrentIndex( (row_num >= 0) ? area->model()->index(row_num, 0) : QModelIndex() );
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
    WidgetType find_widget_with_text( QWidget * container, const QString& text )
    {
        Q_ASSERT(container);
        foreach(WidgetType child_widget, container->findChildren<WidgetType>()) {
            if ( child_widget->objectName() == text || child_widget->text().contains(text) )
                return child_widget;
        }
        return NULL;
    }

    template<typename WidgetType>
    QList<WidgetType> find_label_siblings( QWidget * container, const QString& label_text )
    {
        QLabel * label = find_widget_with_text<QLabel*>(container, label_text);
        Q_ASSERT( label );
        return label->parentWidget()->findChildren<WidgetType>();
    }

    template<typename WidgetType>
    WidgetType find_sibling_with_text( QWidget * widget, const QString& sibling_text )
    {
        return find_widget_with_text<WidgetType>(widget->parentWidget(), sibling_text);
    }

    void select(const QString& area_name, int index = 0)
    {
        QAbstractItemView * a = area( area_name );
        QVERIFY( a );
        a->setCurrentIndex( index >= 0 ? a->model()->index(index,0) : QModelIndex() );
    }

    void deselect(const QString& area_name)
    {
        select(area_name, -1);
    }

    void press(const QString& button_text)
    {
        QAbstractButton * button = find_widget_with_text<QAbstractButton*>( start_page, button_text );
        QVERIFY(button);
        button->click();
    }

    void press(const QString& label_text, const QString& button_text )
    {
        QLabel * label = find_widget_with_text<QLabel*>(start_page, label_text);
        QVERIFY(label);
        QAbstractButton* button = find_sibling_with_text<QAbstractButton*>( label, button_text );
        QVERIFY(button);
        button->click();
    }

    QAbstractItemView * area( const QString& title )
    {
        QList<QAbstractItemView *> candidates = find_label_siblings<QAbstractItemView *>(start_page, title);
        Q_ASSERT( candidates.count() == 1 );
        return candidates[0];
    }
};

QTEST_MAIN(StartPageTest)
#include "startpage_test.moc"

