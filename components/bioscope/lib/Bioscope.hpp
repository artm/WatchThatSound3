#ifndef BIOSCOPE_HPP
#define BIOSCOPE_HPP

#include "stable.h"

#include "utils/Exception"

/// Access to framebased video files
class Bioscope : public QObject
{
    Q_OBJECT
public:
    DEF_FORMAT_EXCEPTION( IOError, "IO Error: %1" );
    DEF_FORMAT_EXCEPTION( UnsupportedFile, "Unsupported file: %1" );
    DEF_FORMAT_EXCEPTION( AVError, "FFmpeg error: %1" );

    Bioscope(const QString& path, QObject *parent = 0);
    ~Bioscope();

    qint64 duration() const;
    int width() const;
    int height() const;
    void frame(QImage * frame);
    void seek( qint64 ms );
    qint64 time();

    static bool supportedFile(const QString& path);
signals:

    void streamEnd();

public slots:

private:
    class Detail;
    QScopedPointer<Detail> m_detail;
};

#endif // BIOSCOPE_HPP
