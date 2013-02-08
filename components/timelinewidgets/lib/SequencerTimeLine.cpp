#include "SequencerTimeLine.hpp"
#include "BufferItem.hpp"
#include "Project.hpp"

#include "utils/Macros"



struct SequencerTimeLine::Detail {
    SequencerTimeLine * self;
    QGraphicsItem * samples;
    int sample_height;

    Detail(SequencerTimeLine * _self)
        : self(_self)
        , samples( new QGraphicsItemGroup )
        , sample_height( 10 )
    {
        self->scene()->addItem(samples);
    }

};

SequencerTimeLine::SequencerTimeLine(QWidget *parent)
    : TimeLineWidget(parent)
    , m_pen(Qt::black)
    , m_brush(Qt::white)
    , m_muteBrush(QColor(0,0,0,70))
    , m_dragItem(0)
    , detail( new Detail(this) )
{
    FIXME("connect(m_mainWindow,SIGNAL(scratchUpdated(WtsAudio::BufferAt*,bool)), SLOT(showScratch(WtsAudio::BufferAt*,bool)))");

    QBrush scratchBrush(QColor(255,200,200,127));
    m_scratchRect = scene()->addRect(0,0,0,0);
    m_scratchRect->setBrush(scratchBrush);
    m_scratchRect->hide();
    m_scratchRect->setZValue(1.0);

    setSeekOnDrag(true);
}

SequencerTimeLine::~SequencerTimeLine()
{
}

void SequencerTimeLine::setProject(Project * project)
{
    TimeLineWidget::setProject(project);
    connect(m_project,SIGNAL(newBufferAt(WtsAudio::BufferAt*)),
            SLOT(insertBufferAt(WtsAudio::BufferAt*)));
    connect(m_project,SIGNAL(sampleChanged(SoundBuffer*)),
            SLOT(updateBuffer(SoundBuffer*)));
}

void SequencerTimeLine::insertBufferAt(WtsAudio::BufferAt * bufferAt)
{
    SoundBuffer * buffer = bufferAt->buffer();

    BufferItem * item = new BufferItem(bufferAt, project()->duration(), 1.0f, this);
    //scene()->addItem(item);
    item->setParentItem( detail->samples );

    showRange(item, buffer);

    m_bufferItems.append(item);

    restackItems();
}

bool lefterItem(const QGraphicsItem * a, const QGraphicsItem * b)
{ return a->x() < b->x(); }

void SequencerTimeLine::restackItems()
{
    QList<float> levelRight;
    qStableSort(m_bufferItems.begin(), m_bufferItems.end(), lefterItem);
    QList< BufferItem * >::iterator it;

    for(it = m_bufferItems.begin(); it != m_bufferItems.end(); ++it) {
        QGraphicsItem * item = *it;
        int until = levelRight.size();
        float x = item->x();
        for(int level = 0; level <= until ; ++level) {
            if (level == until)
                levelRight.append( x );

            if (x >= levelRight[level]) {
                // insert on level
                item->setY(level);
                levelRight[level] = x + item->boundingRect().width();
                break;
            }
        }
    }
    updateSelection();
}

void SequencerTimeLine::showScratch(WtsAudio::BufferAt * scratchAt, bool on)
{
    if (on) {
        qint64 at = scratchAt->at();
        qint64 duration = WtsAudio::sampleCountToMs(scratchAt->buffer()->m_writePos);

        float tt = (float)project()->duration();
        float relX = (float)at / tt;
        float relW = (float)duration / tt;
        m_scratchRect->setRect(relX, .1, relW, .8 );
        m_scratchRect->show();
    } else {
        m_scratchRect->hide();
    }
}

void SequencerTimeLine::mouseReleaseEvent ( QMouseEvent * event )
{
    TimeLineWidget::mouseReleaseEvent(event);
    restackItems();
    NOP_OR(project())->save();
}

void SequencerTimeLine::resizeEvent(QResizeEvent *event)
{
    TimeLineWidget::resizeEvent(event);

    QTransform transform( QTransform::fromScale(1, (float)sampleHeight() / height() ) );

    detail->samples->setTransform( transform );
    foreach(BufferItem * item, m_bufferItems) {
        item->update();
    }
}

void SequencerTimeLine::showRange(QGraphicsItem * root, SoundBuffer *buffer)
{
    float tt = (float)project()->duration();
    float relW = (float)buffer->duration() / tt;
    float selX1 = (float)WtsAudio::sampleCountToMs(buffer->rangeStart()) / tt;
    float selX2 = (float)WtsAudio::sampleCountToMs(buffer->rangeEnd()) / tt;

    QGraphicsRectItem * recti = scene()->addRect(
                0, 0, selX1, 1.0f,
                Qt::NoPen, m_muteBrush );
    recti->setZValue(.5);
    recti->setParentItem(root);

    recti = scene()->addRect(
                selX2, 0, relW-selX2, 1.0f,
                Qt::NoPen, m_muteBrush );
    recti->setZValue(.5);
    recti->setParentItem(root);

    recti = scene()->addRect(
                0, 0,
                relW, 1.0f,
                m_pen, Qt::NoBrush );
    recti->setZValue(.5);
    recti->setParentItem(root);

}

void SequencerTimeLine::onRemoved(Synced * , QGraphicsItem * item)
{
    // remove the item from m_bufferItems first...
    BufferItem * bitem = dynamic_cast<BufferItem *>(item);
    if (bitem) {
        m_bufferItems.removeAll(bitem);
        restackItems();
    }
}

void SequencerTimeLine::updateBuffer(SoundBuffer *buffer)
{
    foreach(BufferItem * bi, m_bufferItems) {

        if (bi->buffer()->buffer() == buffer) {
            scene()->clearSelection();
            bi->bufferChanged();
            showRange(bi, buffer);
            bi->setSelected(true);
        }
    }
}

void SequencerTimeLine::updateSelection()
{
    TimeLineWidget::updateSelection();

    WtsAudio::BufferAt * selected = selectedBufferAt();
    emit bufferSelected( selected );
}

void SequencerTimeLine::startSolo()
{
    WtsAudio::BufferAt * selected = selectedBufferAt();
    if (selected)
        emit startSolo(selected);
}

WtsAudio::BufferAt * SequencerTimeLine::selectedBufferAt()
{
    QList<QGraphicsItem *> sel = scene()->selectedItems();
    if (sel.length() > 0) {
        TRACE( QString("sel.length: %1").arg(sel.length()));
        BufferItem * bi = dynamic_cast<BufferItem*>(sel[0]);
        if (bi)
            return bi->buffer();
    }
    return 0;
}

int SequencerTimeLine::sampleHeight() const
{
    return detail->sample_height;
}

void SequencerTimeLine::setSampleHeight(int pixels)
{
    detail->sample_height = pixels;
}


