#ifndef MACROS_HPP
#define MACROS_HPP

#define NOP_OR(object) if (object) object
#define NULL_OR(object) (object == NULL) ? NULL : object

inline QString message_with_location(const QString& message, const char * file, int line)
{
    return QString("%1\n   Loc: [%2(%3)]").arg(message).arg(file).arg(line);
}

#define HERE_MESSAGE(message) message_with_location(message, __FILE__, __LINE__)
#define TRACE(message) do { qDebug() << HERE_MESSAGE(message); } while(false)
#define FIXME(message) do { qWarning() << "FIXME:" << HERE_MESSAGE(message); } while(false)


#endif // MACROS_HPP
