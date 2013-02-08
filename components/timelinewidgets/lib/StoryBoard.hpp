#ifndef STORYBOARD_H
#define STORYBOARD_H

#include "TimeLineWidget.hpp"

class StoryBoard : public TimeLineWidget
{
    Q_OBJECT
    Q_PROPERTY( int tickHeight READ tickHeight WRITE setTickHeight )
    Q_PROPERTY( int thumbMargin READ thumbMargin WRITE setThumbMargin )
public:
    explicit StoryBoard(QWidget *parent = 0);
    ~StoryBoard();

    int tickHeight() const;
    void setTickHeight(int height);

    int thumbMargin() const;
    void setThumbMargin(int margin);

public slots:
    void updateSnapshots();
    void setVideoSize(int w, int h);
    virtual void setCurrentTime(qint64 time);
    virtual void setProject(Project * project);

protected:
    virtual void resizeEvent ( QResizeEvent * event );
    virtual void drawBackground( QPainter * painter, const QRectF & rect ) ;

private:
    class Detail;
    QScopedPointer<Detail> detail;
};

#endif // STORYBOARD_H
