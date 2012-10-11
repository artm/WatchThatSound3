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
               SLOT( on_ ## area_name ## _currentChanged(QModelIndex,QModelIndex) )); \
    } while(false)

    CONNECT_CURRENT_CHANGED( library );
    CONNECT_CURRENT_CHANGED( projects );
    CONNECT_CURRENT_CHANGED( study_material );
    CONNECT_CURRENT_CHANGED( get_started );

#undef CONNECT_CURRENT_CHANGED
}

void StartPage::on_library_currentChanged(const QModelIndex& current,const QModelIndex&)
{
    QWidget * button = findChild<QWidget *>("new_project");
    Q_ASSERT(button);
    button->setEnabled( current.isValid() );
}

void StartPage::on_projects_currentChanged(const QModelIndex& current,const QModelIndex&)
{
    QWidget * button = findChild<QWidget *>("continue_project");
    Q_ASSERT(button);
    button->setEnabled( current.isValid() );
}

void StartPage::on_study_material_currentChanged(const QModelIndex& current,const QModelIndex&)
{
    QWidget * button = findChild<QWidget *>("open_study_material");
    Q_ASSERT(button);
    button->setEnabled( current.isValid() );
}

void StartPage::on_get_started_currentChanged(const QModelIndex& current,const QModelIndex&)
{
    QWidget * button = findChild<QWidget *>("open_get_started");
    Q_ASSERT(button);
    button->setEnabled( current.isValid() );
}
