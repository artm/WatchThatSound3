#include "startpage/StartPage"
#include "utils/Stub"

QStandardItemModel models[4];
void init_models(StartPage * start_page)
{
#define SET_MODEL( area_name, model ) \
    do { \
    QAbstractItemView* area = start_page->findChild<QAbstractItemView*>(area_name); \
    if (area) area->setModel( model ); \
    } while(false)

    QStandardItem * item;

    models[0].setColumnCount( 1 );
    models[0].appendRow( item = new QStandardItem(QIcon(":/icons/bit6.jpg"), "WTS_Bit_1") );
    models[0].appendRow( item = new QStandardItem(QIcon(":/icons/bit10.jpg"), "WTS_Bit_2") );
    models[0].appendRow( item = new QStandardItem(QIcon(":/icons/bit6.jpg"), "WTS_Bit_3") );

    models[1].setColumnCount( 1 );
    models[1].appendRow( new QStandardItem(QIcon(":/icons/bit10.jpg"), "WTS_Bit_1") );
    models[1].appendRow( new QStandardItem(QIcon(":/icons/bit6.jpg"), "WTS_Bit_1") );
    models[1].appendRow( new QStandardItem(QIcon(":/icons/bit6.jpg"), "WTS_Bit_2") );

    models[2].setColumnCount( 1 );
    models[2].appendRow( new QStandardItem(QIcon(":/icons/bit6.jpg"), "Rhythm") );
    models[2].appendRow( new QStandardItem(QIcon(":/icons/bit6.jpg"), "Pitch") );
    models[2].appendRow( new QStandardItem(QIcon(":/icons/bit10.jpg"), "Surprises") );

    models[3].setColumnCount( 1 );
    models[3].appendRow( new QStandardItem(QIcon(":/icons/bit10.jpg"), "Maak je eigen") );
    models[3].appendRow( new QStandardItem(QIcon(":/icons/bit10.jpg"), "Jaques explains") );
    models[3].appendRow( new QStandardItem(QIcon(":/icons/bit6.jpg"), "Quick start") );

    SET_MODEL("library", &models[0]);
    SET_MODEL("projects", &models[1]);
    SET_MODEL("study_material", &models[2]);
    SET_MODEL("get_started", &models[3]);

    start_page->connect_signals();

#undef SET_MODEL
}

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

    init_models(start_page);

    main_win.show();
    return app.exec();
}
