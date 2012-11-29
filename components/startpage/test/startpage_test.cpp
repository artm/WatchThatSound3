#include "stable.h"

#include "startpage/StartPage"
#include "utils/GuiTestingBase"
#include "utils/DialogSpecHelper"

class StartPageTest: public GuiTestingBase
{
    Q_OBJECT
    StartPage * start_page;
    QMap<QString, QList<QString> > items;

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
    void initTestCase()
    {
        start_page = new StartPage();

        items["Library"] << "WTS_Bit_1.mov" << "WTS_Bit_2.mov" << "WTS_Bit_3.mov";
        items["Projects"] << "WTS_Bit_1.mov" << "WTS_Bit_1.mov" << "WTS_Bit_2.mov";
        items["Study material"] << "Rhythm" << "Pitch" << "Surprises";
        items["Get started"] << "Maak je eigen.pdf" << "Jaques explains.mov" << "Quick start.pdf";

        foreach(QString area_title, items.keys()) {
            QStandardItemModel * model = new QStandardItemModel(0,1);
            foreach(QString file_name, items[area_title])
                model->appendRow( new QStandardItem(file_name) );
            area(area_title)->setModel( model );
        }
        start_page->connect_signals();
        start_page->show();
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
            QCOMPARE(spy.takeFirst().at(0).toString(), start_page->selected_filename( reference_area ));
        }
    }

    void deselect_on_loosing_focus_data()
    {
        open_buttons_data();
    }

    void deselect_on_loosing_focus()
    {
        QFETCH( QString, title );
        QFETCH( QString, button_title );

        QAbstractItemView * first_area = area(title);
        QAbstractButton * button = find_sibling_with_text<QAbstractButton *>( first_area, button_title );

        Q_ASSERT(first_area);
        Q_ASSERT(button);

        foreach( QAbstractItemView * next_area, start_page->findChildren<QAbstractItemView *>() ) {
            if (first_area == next_area)
                continue;
            // - Given an item is selected in an area
            first_area->selectionModel()->select( first_area->model()->index(0,0), QItemSelectionModel::Select );
            first_area->setFocus(Qt::MouseFocusReason);
            // - When I select an item in a different area
            next_area->selectionModel()->select( next_area->model()->index(0,0), QItemSelectionModel::Select );
            next_area->setFocus(Qt::MouseFocusReason);
            // - Then originally selected item is deselected
            QVERIFY( first_area->selectionModel()->selectedRows().count() == 0 );
            // - And corresponding "open" button becomes inactive
            QVERIFY( ! button->isEnabled() );
        }
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
        QTest::addColumn<QString>("area_title");
        QTest::addColumn<int>("row_num");
        QTest::addColumn<QString>("file_name");

        foreach( QString area_title, items.keys() ) {
            QTest::newRow(qPrintable(area_title + " no selection"))   << area_title << -1 << QString();
            for(int i = 0; i<items[area_title].count(); ++i) {
                QTest::newRow(qPrintable(area_title + " " + i))  << area_title <<  i << items[area_title][i];
            }
        }
    }

    void test_selected_filename()
    {
        QFETCH(QString, area_title);
        QFETCH(int, row_num);
        QFETCH(QString, file_name);

        QAbstractItemView * area_under_test = area(area_title);
        area_under_test->setCurrentIndex( (row_num >= 0) ? area_under_test->model()->index(row_num, 0) : QModelIndex() );
        QCOMPARE( start_page->selected_filename( area_under_test ) , file_name );
        QCOMPARE( start_page->selected_filename( area_under_test->objectName() ) , file_name );
    }

    void test_select_data()
    {
        test_selected_filename_data();
    }

    void test_select()
    {
        QFETCH(QString, area_title);
        QFETCH(int, row_num);

        QAbstractItemView * area_under_test = area(area_title);
        start_page->select(area_under_test->objectName(), row_num);
        QCOMPARE( area_under_test->currentIndex().row(), row_num );
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

