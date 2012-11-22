#include "startpage/StartPage"
#include "utils/Stub"
#include "customwidgets/SlidingStackedWidget"

#include "TestWtsIntegration.hpp"
#include "WtsShell.hpp"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QStringList args = QCoreApplication::arguments();

    if (args.contains("--test")) {
        // integration testing
        QStringList slice = args.mid(args.indexOf("--test") + 1 );
        TestWtsIntegration suite(slice);
        return suite.runTests();
    } else {
        WtsShell * shell = new WtsShell(qApp);
        shell->assemble();
        shell->start();
        return qApp->exec();
    }

}
