#ifndef SEQUENCERTIMELINE_H
#define SEQUENCERTIMELINE_H

#include "TimeLineWidget.hpp"
#include "SoundBuffer.hpp"
#include "WtsAudio.hpp"

class BufferItem;

class SequencerTimeLine : public TimeLineWidget
{
    Q_OBJECT
    Q_PROPERTY( int sampleHeight READ sampleHeight WRITE setSampleHeight )
public:
    explicit SequencerTimeLine(QWidget *parent = 0);
    ~SequencerTimeLine();
    WtsAudio::BufferAt * selectedBufferAt();

    int sampleHeight() const;
    void setSampleHeight(int pixels);

signals:
    void bufferSelected(WtsAudio::BufferAt * buffer);
    void startSolo( WtsAudio::BufferAt * );

public slots:
    void insertBufferAt(WtsAudio::BufferAt * buffer);
    void showScratch(WtsAudio::BufferAt * scratchAt, bool on);
    void updateBuffer(SoundBuffer * buffer);
    virtual void updateSelection();
    virtual void setProject(Project * project);
    void startSolo();

protected:
    QPen m_pen;
    QBrush m_brush, m_muteBrush;
    QGraphicsRectItem * m_scratchRect;

    QGraphicsItem * m_dragItem;
    QPointF m_dragLastP;

    QList< BufferItem * > m_bufferItems;

    void restackItems();
    void showRange(QGraphicsItem * root, SoundBuffer * buffer);
    virtual void onRemoved(Synced * synced, QGraphicsItem * item);

    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void resizeEvent ( QResizeEvent * event );

private:
    class Detail;
    QScopedPointer< Detail > detail;
};

#endif // SEQUENCERTIMELINE_H
