#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

struct Exception {
    const QString message;
    Exception(const QString& _message) : message(_message) {}
    Exception(const QString& _message, const char * file, int line)
        : message(QString("%1\n   Loc: [%2(%3)]").arg(_message).arg(file).arg(line)) {}
};

#define RAISE_A(type, msg) throw type( msg, __FILE__, __LINE__ )
#define RAISE(msg) RAISE_A(Exception, msg)

#endif // EXCEPTION_HPP
