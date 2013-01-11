//#include "wts_version.h"

#include "utils/Macros"

#include "Project.hpp"
#include "Common.hpp"
#include "SoundBuffer.hpp"
#include "Rainbow.hpp"
#include "ScoreSymbol.hpp"
#include "ScoreEditor.hpp"

#include "ThumbsSpread.hpp"

#include "bioscope/Bioscope"

#include <QPainter>
#include <QPrinter>

#define THROW_IF_INVALID() do { if (this!=NULL && !isValid()) RAISE_A(InvalidProject, QString("$1").arg((long long)this)); } while(false)

using namespace WTS;

struct Project::Detail {
    Bioscope * bioscope;
    QString path;

    Detail()
        : bioscope(0)
    {}
};

const float MM_PER_INCH = 0.0393700787;

Project::Project(QObject * parent)
    : QObject(parent), m_detail(new Detail)
{
    setup();
}

Project::~Project() {

}

/* initialization common across constructors */
void Project::setup()
{
    m_duration = 0;
    m_loading = false;
    m_finalTension = 0.5;
    m_lastSampleNameNum = 0;
    m_msPerPage = 15000;

    // connect to itself...
    connect(this,SIGNAL(saveSection(QXmlStreamWriter&)),
            SLOT(saveSequence(QXmlStreamWriter&)));
    connect(this,SIGNAL(saveSection(QXmlStreamWriter&)),
            SLOT(saveStoryboard(QXmlStreamWriter&)));
    connect(this,SIGNAL(loadSection(QXmlStreamReader&)),
            SLOT(loadSequence(QXmlStreamReader&)));
    connect(this,SIGNAL(loadSection(QXmlStreamReader&)),
            SLOT(loadStoryboard(QXmlStreamReader&)));

}

Project::Project(const QString& path, QObject * parent)
    : QObject(parent)
{
    setup();

    m_detail->path = path;
    m_detail->bioscope = new Bioscope( path, this );

    QDir movieDir = QFileInfo(path).dir();
    m_dataDir = QDir( movieDir.filePath( QFileInfo(path).completeBaseName()
                + ".data") );
    if (! m_dataDir.exists() )
        movieDir.mkdir( m_dataDir.dirName() );
}

void Project::saveStoryboard(QXmlStreamWriter& xml)
{
    xml.writeStartElement("storyboard");
    xml.writeAttribute("final_tension", QString("%1").arg(m_finalTension));
    foreach(Marker * m, m_markers) {
        xml.writeStartElement("marker");
        QString ms;
        ms.setNum(m->at());
        xml.writeAttribute("ms",ms);
        xml.writeAttribute("type", m->type() == SCENE ? "scene" : "event");
        xml.writeAttribute("tension", QString("%1").arg(m->tension()));
        xml.writeEndElement();
    }
    xml.writeEndElement();
}

bool Project::loadStoryboard(QXmlStreamReader& xml)
{
    if (xml.name() != "storyboard")
        return false;

    m_finalTension = xml.attributes().hasAttribute("final_tension")
        ? xml.attributes().value("final_tension").toString().toFloat()
        : 0.5;

    while(xml.readNextStartElement()) {
        float tension = xml.attributes().hasAttribute("tension")
            ? xml.attributes().value("tension").toString().toFloat()
            : 0.5;

        addMarker(xml.attributes().value("type") == "scene" ? SCENE : EVENT,
                xml.attributes().value("ms").toString().toLongLong(),
                tension);

        // this is one way to read a flat list of <foo/> not recursing
        xml.readElementText();
    }

    emit tensionChanged();
    return true;
}

void Project::saveSequence(QXmlStreamWriter& xml)
{
    xml.writeStartElement("sequence");
    xml.writeAttribute("counter",QString("%1").arg(m_lastSampleNameNum));
    foreach(WtsAudio::BufferAt * buffer, m_sequence) {
        xml.writeStartElement("sample");
        qint64 at_int = buffer->at();
        QString at_s = QString("%1").arg(at_int);
        xml.writeAttribute("ms",at_s);
        xml.writeAttribute("id",buffer->buffer()->name());

        xml.writeAttribute("range_start", QString("%1").arg(buffer->buffer()->rangeStart()));
        xml.writeAttribute("range_end", QString("%1").arg(buffer->buffer()->rangeEnd()));
        xml.writeAttribute("gain", QString("%1").arg( buffer->buffer()->gain() ));
        xml.writeAttribute("color", QString("%1").arg(buffer->buffer()->color().name() ));
        xml.writeEndElement();

        buffer->buffer()->save( m_dataDir );
    }
    xml.writeEndElement();
}

bool Project::loadSequence(QXmlStreamReader& xml)
{
    if (xml.name() != "sequence")
        return false;

    QRegExp idRe = QRegExp("sample_(\\d+)");
    m_lastSampleNameNum =
        xml.attributes().value("counter").toString().toInt();
    while(xml.readNextStartElement()) {
        QString id = xml.attributes().value("id").toString();

        id.replace(".wav","");

        SoundBuffer * sb = new SoundBuffer( id );
        try {
            sb->load( m_dataDir );
            sb->setRange(xml.attributes().value("range_start").toString().toLongLong(),
                    xml.attributes().value("range_end").toString().toLongLong());

            QString at_s =  xml.attributes().value("ms").toString();
            qint64 at_int = at_s.toLongLong();
            WtsAudio::BufferAt * buffer =
                new WtsAudio::BufferAt(sb,
                        at_int,
                        this);
            m_sequence.append( buffer );

            // find out original number / color
            if (xml.attributes().hasAttribute("color")) {
                buffer->buffer()->setColor( QColor( xml.attributes().value("color").toString() ) );
            } else if (idRe.indexIn(id) > -1) {
                int color_index = idRe.cap(1).toInt();
                buffer->buffer()->setColor( Rainbow::getColor( color_index ) );
            } else {
                buffer->buffer()->setColor( Rainbow::getColor( rand() ) );
            }

            buffer->buffer()->initGains();
            if (xml.attributes().hasAttribute("gain"))
                buffer->buffer()->setGain( xml.attributes().value("gain").toString().toFloat() );

            emit newBufferAt(buffer);
        } catch (FileNotFound& e) {
            delete sb;
        }
        // finish off the element...
        xml.readElementText();
    }
    return true;
}

void Project::addMarker(Project::MarkerType type, qint64 when, float tension)
{
    THROW_IF_INVALID();

    m_markers[when] = new Marker(type, when, this);
    m_markers[when]->setTension( tension );
    m_detail->bioscope->seek(when);
    QImage img;
    m_detail->bioscope->frame(&img);
    m_markers[when]->setSnapshot( QPixmap::fromImage( img ) );

    emit storyBoardChanged();
    emit tensionChanged();

    save();
}

QList<Project::Marker *> Project::getMarkers(MarkerType type, bool forward) const
{
    THROW_IF_INVALID();

    QList<Marker *> scenes;
    foreach(Marker * m, m_markers) {
        if (type == ANY || m->type() == type) {
            if (forward)
                scenes.append(m);
            else
                scenes.prepend(m);
        }
    }

    return scenes;
}

void Project::removeMarkerAt(quint64 at)
{
    if (at > 0) {
        m_markers.remove(at);
        emit storyBoardChanged();
        emit tensionChanged();
        save();
    }
}

QPainterPath Project::tensionCurve(float width, float height, Marker * from, Marker * to) {
    THROW_IF_INVALID();

    QPainterPath curve;
    QMapIterator<qint64, Project::Marker *> iter(m_markers);
    bool init = true;

    qint64 start = from ? from->at() : 0;
    qint64 end = to ? to->at() : duration();

    // skip until 'from'
    if (from) {
        while(iter.hasNext()) {
            if (iter.peekNext().value() == from)
                break;
            iter.next();
        }
    }

    while(iter.hasNext()){
        iter.next();
        Project::Marker * m = iter.value();

        QPointF xy(width * (m->at()-start) / (end-start),
                   height * m->tension());

        if (init) {
            curve.moveTo(xy);
            init = false;
        } else
            curve.lineTo(xy);

        if (to && iter.value() == to)
            break;
    }

    if (!to)
        curve.lineTo( QPointF(width, height * finalTension()));

    return curve;
}

void Project::setMarkerTension(int markerIndex, float tension)
{
    if (markerIndex < m_markers.size() )
        m_markers[ m_markers.keys()[markerIndex] ]->setTension(tension);
    else if (markerIndex  == m_markers.size())
        setFinalTension(tension);
    emit tensionChanged();
}

void Project::removalRequested(Synced *synced)
{
    {
        Marker * marker = dynamic_cast<Marker*>(synced);
        if (marker) {
            removeMarkerAt(marker->at());
            return;
        }
    }

    {
        WtsAudio::BufferAt * buffer = dynamic_cast<WtsAudio::BufferAt *>(synced);
        if (buffer) {
            removeBufferAt(buffer);
            return;
        }
    }

    qWarning("Requested removal of unknown synced type");
}

void Project::save()
{
    // make sure we don't overwrite while loading...
    if (m_loading)
        return;

    QFile dataFile( dataDir().filePath("metadata.xml.tmp") );
    dataFile.open(QFile::WriteOnly | QFile::Text);

    QXmlStreamWriter xml( &dataFile );

    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE soundtrack>");
    xml.writeStartElement("soundtrack");
    //xml.writeAttribute("version", WTS_VERSION);
    FIXME("output version");
    xml.writeAttribute("movie", movieFilename());

    emit saveSection(xml);


    xml.writeEndElement();
    xml.writeEndDocument();
    dataFile.close();
    QFile::remove( dataDir().filePath("metadata.xml") );
    dataFile.rename(  dataDir().filePath("metadata.xml") );
}

void Project::load()
{
    m_loading = true;

    // always have a scene starting at 0
    addMarker(Project::SCENE, 0, 0.5);

    QFile dataFile( dataDir().filePath("metadata.xml") );
    dataFile.open(QFile::ReadOnly | QFile::Text);
    QXmlStreamReader xml( &dataFile );
    xml.readNextStartElement();
    if (xml.name() == "soundtrack") {
        while (xml.readNextStartElement()) {
            // try every section (a bit stupid, but simple to implement)
            // the following calls ATTEMPT to load, or do nothing if it's not their sections
            emit loadSection(xml);
        }

    } else {
        qDebug() << dataFile.fileName() << " is not a soundtrack file";
    }

    m_loading = false;
}

#if defined(__APPLE__)
#define nextToExe(p) QCoreApplication::applicationDirPath () + "/../../.." + p
#else
#define nextToExe(p) QCoreApplication::applicationDirPath () + p
#endif

QDir Project::s_movDir;
bool Project::s_movDirFound = false;

QDir Project::movDir()
{
    if (!s_movDirFound) {

        QString stdMoviesPath = QDesktopServices::storageLocation(
                QDesktopServices::MoviesLocation );

        s_movDir = QDir(stdMoviesPath).filePath("Watch That Sound Movies");

        if (!s_movDir.exists()) {
            // first try pre 3.0.3 convention

            QDir tryDir;
            tryDir = QDir(nextToExe("/WTSmovie"));
            if (!tryDir.exists())
                // then try 3-beta convention
                tryDir = QDir(nextToExe("/movie"));

            // upgrade from beta to actual version
            if (tryDir.exists()) {
                QDir().rename(tryDir.path(),s_movDir.path());

                QString info =
                    QString("Oude movie map %1 verplaatst naar de nieuwe locatie: %2")
                    .arg(tryDir.path()).arg(s_movDir.path());

                QMessageBox message(QMessageBox::Information, "Upgrade info",
                        info, QMessageBox::Ok);
                message.exec();
            } else {
                s_movDir.mkdir(s_movDir.path());
                // see if we have sample films installed
                QDir distVideos = QDir(nextToExe("/../video"));
                if (distVideos.exists()) {
                    foreach(QString path,
                            distVideos.entryList(QStringList()
                                << QString("*.%1").arg(VIDEO_FMT))) {
                        // copy sample films to movDir
                        qDebug() << "cp " << path << " to " << s_movDir.filePath(path);
                        QFile(distVideos.filePath(path)).copy(s_movDir.filePath(path));
                    }
                } else {
                    qWarning() << "No dist videos at " << distVideos.path();
                }
            }
        }
    }

    return s_movDir;
}

void Project::addBufferAt(WtsAudio::BufferAt * newBuff)
{
    m_sequence.append(newBuff);
    emit newBufferAt(newBuff);
}

WtsAudio::BufferAt *  Project::copyScratch(WtsAudio::BufferAt * scratch)
{
    WtsAudio::BufferAt * newBuff =
        new WtsAudio::BufferAt(
                new SoundBuffer(makeSampleName(),
                    *scratch->buffer(),
                    scratch->buffer()->m_writePos),
                scratch->at(),
                this);
    newBuff->buffer()->setColor( Rainbow::getColor(m_lastSampleNameNum) );
    newBuff->buffer()->initGains();
    addBufferAt(newBuff);
    return newBuff;
}

void Project::removeBufferAt(WtsAudio::BufferAt * bufferAt)
{
    m_sequence.removeAll(bufferAt);
    emit syncedItemRemoved(bufferAt);
    save();
}

QString Project::makeSampleName()
{
    return QString("sample_%1").arg(++m_lastSampleNameNum);
}

void Project::reScanSamples()
{
    foreach(WtsAudio::BufferAt * bufat, m_sequence) {
        SoundBuffer * sample = bufat->buffer();
        // see if corresponding sample changed on disk...
        if ( sample->maybeReload( m_dataDir ) )
            emit sampleChanged(sample);
    }
}

void Project::openInExternalApp(SoundBuffer * buffer)
{
    QString path = buffer->savedAs();
    if (QFile(path).exists()) {
        QDesktopServices::openUrl( QUrl( QString("file:///%0").arg(path)));
    }
}

void Project::setDuration(qint64 duration)
{
    if (!m_detail->bioscope)
        m_duration = duration;
    else
        RAISE_A(CantChangeDuration, "bioscope not set");
}

bool Project::isValid() const
{
    return m_detail->bioscope || m_duration > 0;
}

QString WTS::Project::pdfPath()
{
    return m_dataDir.filePath( movieFilename() + ".pdf" );
}

void WTS::Project::print(QPrinter &printer, const QList<WTS::ScoreSymbol *>& score)
{
    // setup
    QPainter painter;
    painter.begin(&printer);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing|
                           QPainter::SmoothPixmapTransform|
                           QPainter::HighQualityAntialiasing);

    QList<Marker *> sceneMarks = getMarkers(SCENE);

    m_markerPen = QPen(QColor(127,127,127));
    QPen helperPen = QPen(Qt::black);
    QPen tensionPen( Qt::red );
    tensionPen.setWidth( 0.5 * MM_PER_INCH * printer.resolution() );

    int staveMargin = printer.pageRect().height() / 10;
    int staveH = (printer.pageRect().height() - staveMargin) / 2, staveW = printer.pageRect().width();
    int hPart = staveH / 9;
    int staveIdx = 0;
    bool printStart = true;

    for(QList<Marker*>::iterator sceneIter = sceneMarks.begin();
        sceneIter != sceneMarks.end();
        ++sceneIter)
    {
        bool last = ( sceneIter+1== sceneMarks.end() );
        qint64  sceneStartTime = (*sceneIter)->at(),
                sceneEndTime = last ? duration() : (*(sceneIter+1))->at();

        qint64 startTime = sceneStartTime, endTime;
        int sceneStave = 0;

        while( startTime < sceneEndTime ) {
            if (staveIdx==0 && !printStart)
                printer.newPage();
            printStart = false;

            endTime = std::min( sceneEndTime, startTime + m_msPerPage );
            // print portion ....
            QRect target = QRect(0, 0, staveW * (endTime-startTime) / m_msPerPage, staveH);
            painter.resetTransform();
            if (staveIdx) painter.translate(0, staveIdx* (staveH+staveMargin));

            // first strip - thumbs
            target.setHeight( hPart * 3 / 2 );
            drawSceneThumbs( *sceneIter,  startTime, endTime, painter, target );

            // next strip - scale
            target.setY( target.y()+target.height() );
            target.setHeight( 10 * MM_PER_INCH * painter.device()->logicalDpiY() );
            painter.setPen(helperPen);
            drawTimeScale( startTime, endTime, painter, target );

            QRect frame = target;
            frame.setHeight( staveH - frame.y() );

            // next strip - tension curve AND score
            target.setY(  target.y() + target.height() );
            target.setHeight( staveH - target.y() );

            // draw the tension curve
            // curve fragment for the whole scene...
            QPainterPath curve = tensionCurve( staveW,
                                               target.height(),
                                               *sceneIter,
                                               (sceneIter+1 == sceneMarks.end())?0:*(sceneIter+1) );
            painter.setPen( tensionPen );
            painter.save();
            painter.setClipRect(target);
            painter.translate( - staveW * sceneStave, target.y() );
            painter.scale( (double)(sceneEndTime - sceneStartTime) / m_msPerPage, 1.0);
            painter.drawPath( curve );
            painter.restore();

            // last strip - score
            painter.save();
            painter.setClipRect(target);
            drawScore( score, startTime, endTime, painter, target );
            painter.restore();

            // frame
            painter.setPen(helperPen);
            painter.save();
            painter.drawRect( frame );
            painter.restore();

            startTime = endTime;
            sceneStave ++;
            staveIdx = staveIdx ? 0 : 1 ;
        }
    }

    painter.end();
}

void WTS::Project::drawTimeScale(qint64 start, qint64 end,
                                 QPainter &painter, const QRect &target)
{
    start /= 1000;//sec
    end /= 1000;

    float minTickDelta = 3.0 * MM_PER_INCH * painter.device()->logicalDpiX() ; // in dots...

    QList<float> secPerTicks;
    secPerTicks << 1 << 2 << 3 << 4 << 5 << 6 << 10 << 12 << 15 << 20 << 30 << 60 << 90 << 120;

    // chose a distance...
    qint64 duration = end - start;
    float dotPerSec = (float)target.width() / duration;
    float secPerTick;
    foreach(float spt, secPerTicks) {
        secPerTick = spt;
        if (spt * dotPerSec > minTickDelta)
            break;
    }

    float dotPerTick = dotPerSec * secPerTick;
    float x = target.x();
    float tickHeight = target.height()*0.3;
    float longTickHeight = target.height()*0.6;

    QFont oldFont = painter.font();
    QFont smallfont(oldFont);
    smallfont.setPointSizeF(9);
    painter.setFont(smallfont);

    float textHOffs = MM_PER_INCH * painter.device()->logicalDpiX(); // 1mm

    int tickIndex = 0;
    while( x<target.width()) {
        float h = (tickIndex%10) ? tickHeight : longTickHeight;

        painter.drawLine( QPointF(x, target.y()), QPointF(x, target.y()+h)  );

        if (h==longTickHeight) {
            // text next to long tick...
            int time = start + tickIndex * secPerTick;
            QString strTime = QString("%0:%1").arg(time/60).arg(time%60,2,10,QLatin1Char('0'));
            painter.drawText( QPointF(x + textHOffs, target.y() + longTickHeight ), strTime );
        }

        x += dotPerTick;
        tickIndex++;
    }
    painter.setFont(oldFont);
}


void WTS::Project::drawSceneThumbs(WTS::Project::Marker *sceneMarker,  qint64 start, qint64 end,
                                   QPainter &painter, const QRect &target)
{
    //int gap = 3.0f * MM_PER_INCH * painter.device()->logicalDpiX(); // 3 mm in dots
    QList<Marker*> markers = getMarkers();

    painter.setPen(m_markerPen);

    // 1. collect markers to draw
    // and 2. find desired centers
    QList<Marker*> drawMarkers;
    QList< int > anchers;
    for(QList<Marker*>::iterator markerIter = qFind( markers.begin(), markers.end(), sceneMarker);
        markerIter != markers.end() && (*markerIter == sceneMarker || (*markerIter)->type() != SCENE);
        ++markerIter)
    {
        Marker * marker = *markerIter;

        if (marker->at() < start || marker->at() > end)
            continue;
        else {
            drawMarkers << marker;
            anchers << target.width() * (marker->at() - start) / (end - start);
        }
    }

    // 3. spread the thumbnails...
    int thumb_height = target.height() * 4 / 5;
    int thumb_margin = thumb_height / 7;

    int thumb_width = thumb_height * m_detail->bioscope->width() / m_detail->bioscope->height();
    QList< int > offsets;
    WTS::spread( target.width(), thumb_width + 2*thumb_margin, anchers, offsets );

    // 4. draw
    for(int i = 0; i<drawMarkers.count(); i++)
    {
        Marker * marker = drawMarkers[i];

        m_detail->bioscope->seek( marker->at() );
        QImage frame;
        m_detail->bioscope->frame(&frame);
        QRect thumbRect(target.x() + offsets[i] + thumb_margin, target.y(), thumb_width, thumb_height);
        painter.drawImage( thumbRect, frame);

        // a line from the thumb to it's time ...
        QPoint lineOrig( thumbRect.center().x(), thumbRect.bottom() );
        painter.drawLine( lineOrig, QPoint( target.x() + anchers[i], target.bottom() ) );
    }
}

void WTS::Project::drawScore( const QList<WTS::ScoreSymbol *>& score, qint64 startTime, qint64 endTime,
                              QPainter &painter, const QRect &target)
{
    painter.save();

    // translate to target
    painter.translate(target.topLeft());
    // scale so that scene duration fits inside target width and (0;1) inside target height:
    painter.scale( (float)target.width() * (float)duration() / (float)(endTime-startTime), target.height() );
    // translate to the start of the current scene
    painter.translate(-(float)startTime / duration(), 0);

    float startX = (float)startTime/duration(), endX = (float)endTime/duration();

    QWidget * widget = parent()->findChild<QWidget *>("score");
    ScoreEditor * scoreEditor = qobject_cast<ScoreEditor *>(widget);

    if (!widget->isVisible()) {
        // very sneaky of me, but storyboard is always visible and is the same height as the score
        widget = parent()->findChild<QWidget *>("storyboard");
    }

    scoreEditor->drawBackground( &painter, target );

    float unit = (float)target.height() / (float)widget->height();

    foreach(ScoreSymbol * sym, score) {

        if (!sym->inHRange(startX, endX))
            continue;

        sym->print( painter,
                    unit * (float)(endX - startX) / target.width(),
                    unit / target.height());
    }
    painter.restore();
}

qint64 WTS::Project::duration() const {
    THROW_IF_INVALID();
    return m_detail->bioscope ? m_detail->bioscope->duration() : m_duration;
}

int WTS::Project::videoWidth() const {
    THROW_IF_INVALID();
    return m_detail->bioscope->width();
}

int WTS::Project::videoHeight() const {
    THROW_IF_INVALID();
    return m_detail->bioscope->height();
}

Bioscope * WTS::Project::bioscope()
{
    return m_detail->bioscope;
}

QString WTS::Project::moviePath() const
{
    return m_detail->path;
}
