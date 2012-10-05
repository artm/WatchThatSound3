#include "StartPage.hpp"

StartPage::StartPage(QWidget *parent) :
    QWidget(parent)
{
    Q_INIT_RESOURCE( startpage );

    QUiLoader loader;
    QFile file(":/forms/startpage.ui");
    file.open(QFile::ReadOnly);
    QWidget * form = loader.load(&file, this);
    file.close();
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( form );
    setLayout( layout );
}
