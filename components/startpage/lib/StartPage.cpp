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

void StartPage::connectSignals()
{
#define CONNECT_CURRENT_CHANGED( area_name ) \
    do { \
    QTreeView * area = findChild<QTreeView*>( #area_name ); \
    Q_ASSERT(area); \
    QItemSelectionModel * sel_model = area->selectionModel(); \
    connect( sel_model, \
             SIGNAL( currentChanged(QModelIndex,QModelIndex) ), \
               SLOT( handle_ ## area_name ## _currentChanged(QModelIndex,QModelIndex) )); \
    } while(false)

    CONNECT_CURRENT_CHANGED( library );
    CONNECT_CURRENT_CHANGED( projects );
    CONNECT_CURRENT_CHANGED( study_material );
    CONNECT_CURRENT_CHANGED( get_started );

#undef CONNECT_CURRENT_CHANGED
}

void StartPage::handle_library_currentChanged(const QModelIndex& current,const QModelIndex&)
{
    QWidget * button = findChild<QWidget *>("new_project");
    Q_ASSERT(button);
    button->setEnabled( current.isValid() );
}

void StartPage::handle_projects_currentChanged(const QModelIndex& current,const QModelIndex&)
{
    QWidget * button = findChild<QWidget *>("continue_project");
    Q_ASSERT(button);
    button->setEnabled( current.isValid() );
}

void StartPage::handle_study_material_currentChanged(const QModelIndex& current,const QModelIndex&)
{
    QWidget * button = findChild<QWidget *>("open_study_material");
    Q_ASSERT(button);
    button->setEnabled( current.isValid() );
}

void StartPage::handle_get_started_currentChanged(const QModelIndex& current,const QModelIndex&)
{
    QWidget * button = findChild<QWidget *>("open_get_started");
    Q_ASSERT(button);
    button->setEnabled( current.isValid() );
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
        emit create_new_project( dialog->findChild<QLineEdit*>("project_name")->text() );
    }
}
