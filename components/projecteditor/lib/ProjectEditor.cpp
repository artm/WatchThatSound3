#include "ProjectEditor.hpp"

struct ProjectEditor::Detail
{

};

ProjectEditor::ProjectEditor(QWidget *parent)
    : QWidget(parent), detail(new Detail)
{
}

ProjectEditor::~ProjectEditor()
{
}
