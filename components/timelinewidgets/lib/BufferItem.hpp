#ifndef BUFFERITEM_H
#define BUFFERITEM_H

#include "WtsAudio.hpp"
#include <QGraphicsRectItem>
#include <QPointer>

class QGraphicsView;

class BufferItem : public QGraphicsRectItem
{
public:
    BufferItem(WtsAudio::BufferAt * buffer, qint64 duration, float height, QGraphicsView * view);
    WtsAudio::BufferAt * buffer() { return m_buffer; }
    void update();

    void bufferChanged();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    qint64 m_duration;
    WtsAudio::BufferAt * m_buffer;
    bool m_constrain;
    QPointer<QGraphicsView> m_view;
    QGraphicsPixmapItem * m_pixmap;
    QGraphicsSimpleTextItem * m_title;
};

#endif // BUFFERITEM_H
