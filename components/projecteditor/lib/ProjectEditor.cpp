#include "ProjectEditor.hpp"
#include "utils/WidgetUtils"

struct ProjectEditor::Detail
{

};

ProjectEditor::ProjectEditor(QWidget *parent)
    : QWidget(parent), detail(new Detail)
{
    setLayout( new QVBoxLayout );
    WidgetUtils::load_form(this,":/forms/ProjectEditor.ui");
}

ProjectEditor::~ProjectEditor()
{
}
