#ifndef STORYBOARD_H
#define STORYBOARD_H

#include "TimeLineWidget.hpp"

namespace WTS {

class StoryBoard : public TimeLineWidget
{
    Q_OBJECT
    Q_PROPERTY( int tickHeight READ tickHeight WRITE setTickHeight )
    Q_PROPERTY( int thumbMargin READ thumbMargin WRITE setThumbMargin )
public:
    explicit StoryBoard(QWidget *parent = 0);

    int tickHeight() const { return m_tickHeight; }
    void setTickHeight(int height) { m_tickHeight = height; }

    int thumbMargin() const { return m_thumbMargin; }
    void setThumbMargin(int margin) { m_thumbMargin = margin; }

public slots:
    void updateSnapshots();
    void setVideoSize(int w, int h) { m_videoWidth = w; m_videoHeight = h; }
    virtual void setCurrentTime(qint64 time);
    virtual void setProject(Project * project);

protected:
    float m_thumbWidth, m_thumbHeight;
    int m_videoWidth, m_videoHeight;
    float m_thumbScale;
    float m_thumbMargin;

    QMap<qint64, QGraphicsItem *> m_msToItem;
    QGraphicsItem * m_selectedThumb;

    // drag drop
    QGraphicsItem * m_dragItem;
    QPointF m_dragLastP;

    int m_tickHeight;

    static const int s_levelCount;

    virtual void resizeEvent ( QResizeEvent * event );
    virtual void drawBackground( QPainter * painter, const QRectF & rect ) ;
};

}

#endif // STORYBOARD_H
