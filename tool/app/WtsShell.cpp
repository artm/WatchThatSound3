#include "WtsShell.hpp"
#include "startpage/StartPage"
#include "projecteditor/ProjectEditor"
#include "customwidgets/SlidingStackedWidget"
#include "utils/Macros"
#include "utils/Stub"
#include "utils/WidgetUtils"

#include "timelinewidgets/StoryBoard"
#include "timelinewidgets/CurveEditor"
#include "timelinewidgets/ScoreEditor"

struct WtsShell::Detail {
    QMainWindow * main_window;
    SlidingStackedWidget * stacker;
    QStandardItemModel models[4];
    QHash<QString, QWidget *> widgets;

    Detail() :
        main_window(NULL),
        stacker(NULL)
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

#define USE_OR_INIT_WIDGET(ClassName, name, ...) \
    if (!detail->widgets.contains(#name)) detail->widgets[#name] = new ClassName(__VA_ARGS__); \
    ClassName * name = qobject_cast<ClassName *>(detail->widgets[#name]); \
    name->setObjectName(#name); \
    if (name)

    USE_OR_INIT_WIDGET(QMainWindow, main_window) {
        detail->main_window = main_window;
        main_window->setWindowTitle("Watch that sound");
    }

    USE_OR_INIT_WIDGET(SlidingStackedWidget, stacker) {
        detail->stacker = stacker;
        stacker->setSpeed(1000);
        stacker->setVerticalMode();
    }
    NOP_OR(main_window)->setCentralWidget(detail->widgets["stacker"]);

    USE_OR_INIT_WIDGET(StartPage, start_page) {
        // extra setup for real start page
        detail->setup_fake_models( start_page );
        Stub * stub = new Stub(qApp);
        QObject::connect(start_page,SIGNAL(import_video(QString)),stub,SLOT(unimplemented()));
        QObject::connect(start_page,SIGNAL(open_file(QString)),stub,SLOT(unimplemented()));

        if (stacker) {
            QObject::connect(start_page,SIGNAL(create_new_project(QString,QString)),stacker,SLOT(slideInNext()));
            QObject::connect(start_page,SIGNAL(open_project(QString)),stacker,SLOT(slideInNext()));
        }
    }
    NOP_OR(stacker)->addWidget(detail->widgets["start_page"]);

    USE_OR_INIT_WIDGET(ProjectEditor, project_editor) {
        // extra setup for real project editor
        // TODO

        WidgetUtils::replace_widget( project_editor, "storyboard", new WTS::StoryBoard );
        WidgetUtils::replace_widget( project_editor, "tension", new WTS::CurveEditor );
        WidgetUtils::replace_widget( project_editor, "score", new WTS::ScoreEditor );

    }
    NOP_OR(stacker)->addWidget(detail->widgets["project_editor"]);

#undef USE_OR_INIT_WIDGET
}

void WtsShell::start()
{
    Q_ASSERT(detail);
    Q_ASSERT(detail->main_window);

    QStringList args = QCoreApplication::arguments();
    if (args.contains("--open-project")) {
        detail->stacker->setCurrentWidget( widget("project_editor") );
    }

    detail->main_window->showMaximized();
}

void WtsShell::add_widget(const QString& tag, QWidget* widget)
{
    Q_ASSERT(detail);
    detail->widgets[tag] = widget;
}

QWidget * WtsShell::current_page()
{
    Q_ASSERT(detail);
    return NULL_OR(detail->stacker)->currentWidget();
}

QWidget * WtsShell::widget(const QString &tag)
{
    Q_ASSERT(detail);
    return detail->widgets.value(tag,NULL);
}
