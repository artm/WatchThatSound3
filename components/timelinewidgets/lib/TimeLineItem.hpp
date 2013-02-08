#ifndef TIMELINEITEM_H
#define TIMELINEITEM_H

#include "stable.h"

class Synced;

class TimeLineItem : public QGraphicsItem
{
public:
    TimeLineItem(Synced * synced, QGraphicsScene * scene);

    Synced * synced() { return m_synced; }
    bool editModeOnly() const { return m_editModeOnly; }
    void setEditModeOnly(bool on) { m_editModeOnly = on; }

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

protected:

    Synced * m_synced;
    bool m_editModeOnly;
};

#endif // TIMELINEITEM_H
