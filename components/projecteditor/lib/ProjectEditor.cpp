#include "ProjectEditor.hpp"
#include "utils/WidgetUtils"

struct ProjectEditor::Detail
{
    void setup_tab_bar(ProjectEditor * self)
    {
        QTabBar * tabs = new QTabBar();
        WidgetUtils::replace_widget( self, "tabs", tabs );

        tabs->addTab("1 scenes");
        tabs->addTab("2 tension line");
        tabs->addTab("3 visual score");
        tabs->addTab("4 audio edit");
    }
};

ProjectEditor::ProjectEditor(QWidget *parent)
    : QWidget(parent), detail(new Detail)
{
    setLayout( new QVBoxLayout );
    WidgetUtils::load_form(this,":/forms/ProjectEditor.ui");
    detail->setup_tab_bar(this);
}

ProjectEditor::~ProjectEditor()
{
}
