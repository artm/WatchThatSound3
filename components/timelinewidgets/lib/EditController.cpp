#include "EditController.hpp"
#include "Project.hpp"

EditController::EditController(QObject *parent) :
    TimeLineController(parent)
{
}

void EditController::addMarkerAtCursor(Project::MarkerType type)
{
    m_project->addMarker(type, cursorTime(), 0.5f);
}

void EditController::addSceneMarkerAtCursor()
{
    addMarkerAtCursor(Project::SCENE);
}

void EditController::addEventMarkerAtCursor()
{
    addMarkerAtCursor(Project::EVENT);
}
