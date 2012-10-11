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
    QTreeView * area = findChild<QTreeView*>("library");
    QItemSelectionModel * sel_model = area->selectionModel();
    connect( sel_model,
             SIGNAL(            currentChanged(QModelIndex,QModelIndex) ),
               SLOT( on_library_currentChanged(QModelIndex,QModelIndex) ));

}

void StartPage::on_library_currentChanged(const QModelIndex& current,const QModelIndex&)
{
    findChild<QWidget *>("new_project")->setEnabled( current.isValid() );
}
