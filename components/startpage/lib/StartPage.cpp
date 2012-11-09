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
    QAbstractItemView* area;

#define SETUP_AREA(area_name, button_name) \
    area = findChild<QAbstractItemView*>( area_name ); \
    area->setProperty("open_button_name", button_name); \
    connect( area->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(handle_selectionChanged()) );

    SETUP_AREA( "library", "new_project" );
    SETUP_AREA( "projects", "continue_project" );
    SETUP_AREA( "study_material", "open_study_material");
    SETUP_AREA( "get_started", "open_get_started");
#undef SETUP_AREA

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), SLOT(handle_focusChanged(QWidget*,QWidget*)));
}

void StartPage::handle_selectionChanged()
{
    QItemSelectionModel * sel_model = qobject_cast<QItemSelectionModel *>(sender());
    Q_ASSERT(sel_model);
    QAbstractItemView * area = area_of(sel_model->model());
    Q_ASSERT(area);
    QString button_name = area->property("open_button_name").toString();
    QAbstractButton * button = findChild<QAbstractButton *>(button_name);
    Q_ASSERT(button);
    button->setEnabled( area->currentIndex().isValid() );
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

void StartPage::on_continue_project_clicked()
{
    emit open_project( selected_filename("projects") );
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
    return selected_filename(area);
}

QString StartPage::selected_filename(QAbstractItemView * area)
{
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

QAbstractItemView * StartPage::area_of( const QAbstractItemModel * model )
{
    foreach( QAbstractItemView * area, findChildren<QAbstractItemView *>() ) {
        if (area->model() == model)
            return area;
    }
    return NULL;
}
