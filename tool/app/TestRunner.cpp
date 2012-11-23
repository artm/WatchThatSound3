#include "TestRunner.hpp"

#include <iostream>

TestRunner::TestRunner(const QStringList& _args) :
    args(_args)
{
}

int TestRunner::run()
{
    QDir test_dir("./test");
    if (!test_dir.exists()) {
        qCritical() << "test runner should be run in the build tree root";
        return 1;
    }

    QHash<QString, QStringList> test_args;
    QStringList test_list = test_dir.entryList(QDir::Executable|QDir::Files)
            .replaceInStrings(QRegExp("^"), test_dir.path() + "/");
    QString self = qApp->applicationFilePath();
    test_list.append( self );
    test_args[self] = QStringList() << "--test";

    int sum = 0;
    foreach(QString test, test_list) {
        QProcess process(0);
        process.start(test, test_args[test]);
        if (process.waitForStarted(500) && process.waitForFinished()) {
            std::cout << process.readAllStandardOutput().data();
            std::cerr << process.readAllStandardError().data();
            sum += abs( process.exitCode() );
        } else {
            sum++;
        }
    }
    return sum;
}
