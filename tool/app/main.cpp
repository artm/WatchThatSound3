#include "startpage/StartPage"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QMainWindow main_win;
    main_win.setWindowTitle("Watch that sound");

    StartPage * start_page = new StartPage();
    main_win.setCentralWidget(start_page);

    main_win.show();
    return app.exec();
}
