#ifndef EDITCONTROLLER_HPP
#define EDITCONTROLLER_HPP

#include "stable.h"
#include "TimeLineController.hpp"
#include "Project.hpp"

/* Main controller of the editor. Because we have single cursor for everything this controller is
 * responsible for driving the sequencer, knowing where to insert things etc.
 * It contains <DO SOMETHING>AtCursor() slots.
 */
class EditController : public TimeLineController
{
    Q_OBJECT
public:
    explicit EditController(QObject *parent = 0);
    
signals:
    
public slots:
    void addMarkerAtCursor(Project::MarkerType type);
    void addSceneMarkerAtCursor();
    void addEventMarkerAtCursor();

};

#endif // EDITCONTROLLER_HPP
