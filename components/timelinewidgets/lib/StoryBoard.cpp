#include "StoryBoard.hpp"
#include "Project.hpp"
#include "TimeLineItem.hpp"

#include "bioscope/Bioscope"

#include <cmath>

using namespace WTS;

const int StoryBoard::s_levelCount = 5;

StoryBoard::StoryBoard(QWidget *parent)
    : TimeLineWidget(parent)
    , m_videoWidth(0)
    , m_videoHeight(0)
    , m_thumbMargin(5)
    , m_selectedThumb(0)
    , m_dragItem(0)
    , m_tickHeight(4)
{
    FIXME("connect(m_mainWindow,SIGNAL(storyBoardChanged()),SLOT(updateSnapshots()))");
}

void StoryBoard::drawBackground ( QPainter * painter, const QRectF & rect )
{
    TimeLineWidget::drawBackground(painter, rect);

    if (!project()) return;

    painter->setFont( QFont("Monaco", 9) );

    int maxLines = width() / 5;

    float totalMin = (float)project()->duration() / 60000.0f;
    float N[] = { 60, 30, 15, 12, 6, 5, 4, 3, 2 };
    int Ncount = sizeof(N)/sizeof(N[0]);

    int i;
    for(i = 0; i<(Ncount-1) && ((totalMin * N[i]) > maxLines) ; ++i) {}
    float dx = 1.0f / (N[i] * totalMin);

    float tick_height = (float)tickHeight() / height();
    painter->setPen(QColor(50,60,50));
    for(int j = 0; j<maxLines; ++j) {
        float x = dx * j;

        int rem = j % (int)N[i];
        float y = 1.0 - (rem ? tick_height : 2.0 * tick_height);

        painter->drawLine(QPointF(x,y),QPointF(x,1.0));

        if (!rem) {
            QString text = QString("%1:00").arg( floor(j / N[i]) );

            double x_pix = x * width() + 3;
            double y_pix = y * height() - 2;

            painter->setMatrixEnabled(false);
            painter->drawText(QPointF(x_pix,y_pix), text);
            painter->setMatrixEnabled(true);
        }
    }

}

void StoryBoard::updateSnapshots()
{
    if (!project()) return;

    float tt = project()->duration();

    foreach(QGraphicsItem * item, m_msToItem) {
        scene()->removeItem(item);
    }
    m_msToItem.clear();
    m_dragItem = 0;
    m_selectedThumb = 0;

    int n = 0;
    float margin_bottom = 4.0 * m_tickHeight / height();
    foreach(Project::Marker * m,
            project()->getMarkers(Project::ANY, false)) {
        float x = (float)m->at() / tt - 0.5 * m_thumbWidth;

        float margin_x = (float)m_thumbMargin / width();
        float margin_y = (float)m_thumbMargin / height();

        int gapCount = s_levelCount - 1;
        float y = margin_y
                  // zig zag function
                  + fabs(n++ % (2*gapCount) - gapCount )
                  // fit centers between two margins + two halves
                  * (1.0 - margin_y - margin_bottom - m_thumbHeight) / (float)gapCount;

        TimeLineItem * tli = new TimeLineItem(m, scene());
        tli->setEditModeOnly(false);

        QGraphicsItem * frameItem = scene()->addRect(
                    QRectF(x - margin_x,
                       y - margin_y,
                       m_thumbWidth + margin_x * 2.0,
                       m_thumbHeight + margin_y * 2.0),
                QPen(Qt::NoPen),QBrush(QColor(255,255,255,150)));
        frameItem->setParentItem(tli);

        QGraphicsPixmapItem * gpi = scene()->addPixmap( m->snapshot() );
        gpi->setPos(x,y);
        gpi->scale(m_thumbScale/(float)width(), m_thumbScale/(float)height());
        gpi->setParentItem(frameItem);

        m_msToItem[m->at()] = tli;
    }

}

void StoryBoard::resizeEvent ( QResizeEvent * event )
{
    TimeLineWidget::resizeEvent(event);

    float rel_thumb_height = 0.55f;
    float pixH = std::min( 100.0f,  (float)height() * rel_thumb_height );
    float pixW = pixH * (float)m_videoWidth / (float)m_videoHeight;
    m_thumbWidth = pixW / (float)width();
    m_thumbHeight = pixH / (float)height();

    m_thumbScale = pixH/(float) m_videoHeight;
    m_thumbMargin = m_thumbMargin;

    updateSnapshots();
}

void StoryBoard::setCurrentTime(qint64 time)
{
    TimeLineWidget::setCurrentTime(time);

    if (m_msToItem.size() == 0)
        return; // no thumbs to select

    // find the last marker / item that we've passed
    QMap<qint64, QGraphicsItem *>::iterator iter = m_msToItem.upperBound(time);
    if (iter != m_msToItem.begin())
        iter--;
    QGraphicsItem * lastItem = iter.value();

    if (lastItem && lastItem != m_selectedThumb) {

        if (m_selectedThumb) {
            m_selectedThumb->setZValue(0.0);
            QGraphicsRectItem * ri = dynamic_cast<QGraphicsRectItem *>(m_selectedThumb);
            if (ri)
                ri->setPen(Qt::NoPen);
        }

        m_selectedThumb = lastItem;
        m_selectedThumb->setZValue(1.0);
        QGraphicsRectItem * ri = dynamic_cast<QGraphicsRectItem *>(m_selectedThumb);
        if (ri) ri->setPen(QPen(Qt::blue));
    }
}

void StoryBoard::setProject(Project *project)
{
    TimeLineWidget::setProject(project);
    connect(project, SIGNAL(storyBoardChanged()), SLOT(updateSnapshots()));
    setVideoSize( project->bioscope()->width(), project->bioscope()->height() );
}

