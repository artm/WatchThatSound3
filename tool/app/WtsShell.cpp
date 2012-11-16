#include "WtsShell.hpp"
#include "startpage/StartPage"
#include "utils/Stub"
#include "customwidgets/SlidingStackedWidget"

struct WtsShell::Detail {
    QMainWindow * main_window;
    QStandardItemModel models[4];

    Detail() :
        main_window(NULL)
    {}
    ~Detail() {
        if (main_window) delete main_window;
    }

    void setup_fake_models(StartPage * start_page)
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


};

WtsShell::WtsShell(QObject *parent) :
    QObject(parent), detail(new Detail)
{
}

WtsShell::~WtsShell()
{
}

void WtsShell::assemble()
{
    Q_ASSERT(detail);
    if (!detail->main_window) {
        detail->main_window = new QMainWindow();
        detail->main_window->setWindowTitle("Watch that sound");
    }

    SlidingStackedWidget * stacker = new SlidingStackedWidget();
    stacker->setSpeed(1000);
    stacker->setVerticalMode();
    detail->main_window->setCentralWidget(stacker);

    StartPage * start_page = new StartPage();
    detail->setup_fake_models( start_page );
    stacker->addWidget(start_page);

    QLabel * project_editor = new QLabel("Here be project editor");
    stacker->addWidget(project_editor);

    Stub * stub = new Stub(qApp);
    QObject::connect(start_page,SIGNAL(import_video(QString)),stub,SLOT(unimplemented()));
    QObject::connect(start_page,SIGNAL(open_file(QString)),stub,SLOT(unimplemented()));

    QObject::connect(start_page,SIGNAL(create_new_project(QString,QString)),stacker,SLOT(slideInNext()));
    QObject::connect(start_page,SIGNAL(open_project(QString)),stacker,SLOT(slideInNext()));
}

void WtsShell::start()
{
    Q_ASSERT(detail);
    Q_ASSERT(detail->main_window);

    detail->main_window->show();
}
