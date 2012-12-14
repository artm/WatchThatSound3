#include "EditController.hpp"
#include "Project.hpp"

WTS::EditController::EditController(QObject *parent) :
    TimeLineController(parent)
{
}

void WTS::EditController::addMarkerAtCursor(Project::MarkerType type)
{
    m_project->addMarker(type, cursorTime(), 0.5f);
}

void WTS::EditController::addSceneMarkerAtCursor()
{
    addMarkerAtCursor(Project::SCENE);
}

void WTS::EditController::addEventMarkerAtCursor()
 {
    addMarkerAtCursor(Project::EVENT);
 }
