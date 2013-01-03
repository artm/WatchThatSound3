#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "Macros.hpp"
#include <QFile>
#include <QString>

struct Exception {
    const QString message;
    Exception(const QString& _message) : message(_message) {}
    Exception(const QString& _message, const char * file, int line)
        : message(message_with_location(_message, file, line)) {}
};

struct FileNotFound : public Exception {
    FileNotFound(const QString& url, const char * file, int line)
        : Exception( QString("File not found: %1").arg(url), file, line) {}
    FileNotFound(const QFile& not_found, const char * file, int line)
        : Exception( QString("File not found: %1").arg(not_found.fileName()), file, line) {}

    static bool test(const QString& path) { return !QFile(path).exists(); }
    static bool test(const QFile& file) { return !file.exists(); }
};

#define DEF_FORMAT_EXCEPTION(Type, format) struct Type : Exception { \
    Type(const QString& _message, const char * file, int line) \
    : Exception( QString(format).arg(_message), file, line ) {} \
}

#define RAISE_A(type, msg) throw type( msg, __FILE__, __LINE__ )
#define RAISE(msg) RAISE_A(Exception, msg)
#define REQUIRE_FILE(file) do { if (FileNotFound::test(file)) RAISE_A(FileNotFound, file); } while(false)

#endif // EXCEPTION_HPP
