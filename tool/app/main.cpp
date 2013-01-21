#include "startpage/StartPage"
#include "utils/Stub"
#include "utils/Exception"
#include "customwidgets/SlidingStackedWidget"

#include "TestWtsIntegration.hpp"
#include "TestRunner.hpp"
#include "WtsShell.hpp"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QStringList args = QCoreApplication::arguments();

    QFile application_css(":/css/application.css");
    if (application_css.exists()) {
        application_css.open(QFile::ReadOnly);
        QString style_sheet(application_css.readAll());
        application_css.close();
        qApp->setStyleSheet(style_sheet);
    } else {
        qWarning() << "Stylesheet not found among the resources...";
    }

    if (args.contains("--test")) {
        // integration testing
        QStringList slice = args.mid(args.indexOf("--test") + 1 );
        TestWtsIntegration suite(slice);
        return suite.runTests();
    } else if (args.contains("--test-suite")) {
        // test runner for the individual component tests
        QStringList slice = args.mid(args.indexOf("--test-suite") + 1 );
        TestRunner runner(slice);
        return runner.run();
    } else {
        try {
            WtsShell * shell = new WtsShell(qApp);
            shell->assemble();
            shell->start();
            return qApp->exec();
        } catch( const Exception& ex ) {
            qDebug() << ex.message;
            return 100;
        }
    }

}
