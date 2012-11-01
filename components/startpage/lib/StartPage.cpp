#include "StartPage.hpp"

StartPage::StartPage(QWidget *parent) :
    QWidget(parent)
{
    QUiLoader loader;
    QFile file(":/forms/startpage.ui");
    file.open(QFile::ReadOnly);
    QWidget * form = loader.load(&file, this);
    file.close();
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( form );
    setLayout( layout );

    QMetaObject::connectSlotsByName( this );
}

void StartPage::connect_signals()
{
#define CONNECT_SELECTION_CHANGED( area_name ) \
    do { \
    QAbstractItemView * area = findChild<QAbstractItemView*>( #area_name ); \
    Q_ASSERT(area); \
    QItemSelectionModel * sel_model = area->selectionModel(); \
    connect( sel_model, \
             SIGNAL( selectionChanged(QItemSelection,QItemSelection) ), \
               SLOT( handle_ ## area_name ## _selectionChanged(QItemSelection, QItemSelection) )); \
    } while(false)

    CONNECT_SELECTION_CHANGED( library );
    CONNECT_SELECTION_CHANGED( projects );
    CONNECT_SELECTION_CHANGED( study_material );
    CONNECT_SELECTION_CHANGED( get_started );

#undef CONNECT_SELECTION_CHANGED

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), SLOT(handle_focusChanged(QWidget*,QWidget*)));
}

void StartPage::handle_library_selectionChanged(const QItemSelection& current,const QItemSelection&)
{
    QWidget * button = findChild<QWidget *>("new_project");
    Q_ASSERT(button);
    button->setEnabled( current.count() > 0 );
}

void StartPage::handle_projects_selectionChanged(const QItemSelection& current,const QItemSelection&)
{
    QWidget * button = findChild<QWidget *>("continue_project");
    Q_ASSERT(button);
    button->setEnabled( current.count() > 0 );
}

void StartPage::handle_study_material_selectionChanged(const QItemSelection& current,const QItemSelection&)
{
    QWidget * button = findChild<QWidget *>("open_study_material");
    Q_ASSERT(button);
    button->setEnabled( current.count() > 0 );
}

void StartPage::handle_get_started_selectionChanged(const QItemSelection& current,const QItemSelection&)
{
    QWidget * button = findChild<QWidget *>("open_get_started");
    Q_ASSERT(button);
    button->setEnabled( current.count() > 0 );
}

void StartPage::on_new_project_clicked()
{
    QUiLoader loader;
    QFile file(":/forms/new_project_dialog.ui");
    file.open(QFile::ReadOnly);
    QDialog * dialog = qobject_cast<QDialog *>(loader.load(&file, this));
    file.close();

    Q_ASSERT(dialog);

    QDialogButtonBox* bbox = dialog->findChild<QDialogButtonBox*>("buttonBox");
    bbox->addButton("Create New Project", QDialogButtonBox::AcceptRole);

    if (dialog->exec() == QDialog::Accepted) {
        emit create_new_project( dialog->findChild<QLineEdit*>("project_name")->text(),
                                 selected_filename("library") );
    }
}

void StartPage::on_open_study_material_clicked()
{
    emit open_file( selected_filename("study_material") );
}

void StartPage::on_open_get_started_clicked()
{
    emit open_file( selected_filename("get_started") );
}

QString StartPage::selected_filename(const QString& area_name)
{
    QAbstractItemView * area = findChild<QAbstractItemView*>( area_name );
    Q_ASSERT( area );
    return area->model()->data( area->currentIndex() ).toString();
}

void StartPage::on_add_video_clicked()
{
    QString file_name =
            QFileDialog::getOpenFileName(
                this,
                "Add an Existing Video",
                "",
                "Video Files (*.mov *.avi)");
    emit import_video(file_name);
}

void StartPage::handle_focusChanged(QWidget * old, QWidget * now)
{
    QAbstractItemView * old_area = qobject_cast<QAbstractItemView *>(old);
    if ( old_area && isAncestorOf(old_area) && isAncestorOf(now) && !old->parentWidget()->isAncestorOf(now) ) {
        QModelIndex none;
        Q_ASSERT( !none.isValid() );
        old_area->setCurrentIndex( none );
    }
}
