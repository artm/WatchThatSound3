#include "startpage/StartPage"
#include "utils/Stub"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QMainWindow main_win;
    main_win.setWindowTitle("Watch that sound");

    StartPage * start_page = new StartPage();
    main_win.setCentralWidget(start_page);

    Stub * stub = new Stub(&app);
    QObject::connect(start_page,SIGNAL(import_video(QString)),stub,SLOT(unimplemented()));
    QObject::connect(start_page,SIGNAL(create_new_project(QString,QString)),stub,SLOT(unimplemented()));
    QObject::connect(start_page,SIGNAL(open_file(QString)),stub,SLOT(unimplemented()));

    main_win.show();
    return app.exec();
}
