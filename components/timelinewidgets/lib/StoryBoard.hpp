#ifndef STORYBOARD_H
#define STORYBOARD_H

#include "TimeLineWidget.hpp"

namespace WTS {

class StoryBoard : public TimeLineWidget
{
    Q_OBJECT
public:
    explicit StoryBoard(QWidget *parent = 0);

public slots:
    void updateSnapshots();
    virtual void resizeEvent ( QResizeEvent * event );
    void setVideoSize(int w, int h) { m_videoWidth = w; m_videoHeight = h; }
    virtual void setCurrentTime(qint64 time);
    virtual void setProject(Project * project);

protected:
    float m_thumbWidth, m_thumbHeight;
    int m_videoWidth, m_videoHeight;
    float m_thumbScale;
    float m_marginX;

    QMap<qint64, QGraphicsItem *> m_msToItem;
    QGraphicsItem * m_selectedThumb;

    // drag drop
    QGraphicsItem * m_dragItem;
    QPointF m_dragLastP;

    static const int s_levelCount;
    static const float s_marginY, s_marginBottom;

    void drawBackground( QPainter * painter, const QRectF & rect ) ;
};

}

#endif // STORYBOARD_H
