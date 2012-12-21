#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "Macros.hpp"

struct Exception {
    const QString message;
    Exception(const QString& _message) : message(_message) {}
    Exception(const QString& _message, const char * file, int line)
        : message(message_with_location(_message, file, line)) {}
};

#define RAISE_A(type, msg) throw type( msg, __FILE__, __LINE__ )
#define RAISE(msg) RAISE_A(Exception, msg)

#endif // EXCEPTION_HPP
