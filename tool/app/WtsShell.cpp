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
#include "timelinewidgets/SequencerTimeLine"
#include "timelinewidgets/WaveformWidget"
#include "timelinewidgets/SampleNameValidator"

#include "timelinewidgets/Project"

struct WtsShell::Detail {
    QMainWindow * main_window;
    SlidingStackedWidget * stacker;
    QStandardItemModel models[4];
    QHash<QString, QWidget *> widgets;

    Project * project;

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

    void open_project(const QString& project_file_name) {
        REQUIRE_FILE( project_file_name );
        project = new Project( project_file_name );

        foreach(TimeLineWidget* timeline,  main_window->findChildren<TimeLineWidget*>()) {
            timeline->setProject( project );
        }

        // setup validator
        QLineEdit * namer = main_window->findChild<QLineEdit *>("sampleNameEdit");
        WaveformWidget * wave = main_window->findChild<WaveformWidget *>();
        Q_ASSERT(namer);
        Q_ASSERT(wave);
        namer->setValidator( new SampleNameValidator(wave, project));

        project->load();
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
        WidgetUtils::replace_widget( project_editor, "storyboard", new StoryBoard );
        WidgetUtils::replace_widget( project_editor, "tension", new CurveEditor );
        WidgetUtils::replace_widget( project_editor, "score", new ScoreEditor );

        SequencerTimeLine  * seq = new SequencerTimeLine;
        WaveformWidget * wave = new WaveformWidget;
        WidgetUtils::replace_widget( project_editor, "sequencer", seq );
        WidgetUtils::replace_widget( project_editor, "waveform", wave );

        connect( seq, SIGNAL(bufferSelected(WtsAudio::BufferAt*)),
                 wave, SLOT(updateWaveform(WtsAudio::BufferAt*)) );
        connect( wave, SIGNAL(rangeChanged(SoundBuffer*)),
                 seq, SLOT(updateBuffer(SoundBuffer*)) );

        QLineEdit * namer = wave->findChild<QLineEdit*>("sampleNameEdit");
        if (namer) {
            connect( namer, SIGNAL(textEdited(QString)), this, SLOT(update_sender_style()) );
            connect( namer, SIGNAL(editingFinished()), this, SLOT(on_sampleNameEdit_editingFinished()) );
        }
    }
    NOP_OR(stacker)->addWidget(detail->widgets["project_editor"]);

#undef USE_OR_INIT_WIDGET
}

void WtsShell::start()
{
    Q_ASSERT(detail);
    Q_ASSERT(detail->main_window);

    QStringList args = QCoreApplication::arguments();
    int open_flag_index = args.indexOf("--open-project");
    if (open_flag_index >= 0) {
        if (open_flag_index + 1 < args.size()) {
            QString project_file_name = args[ open_flag_index + 1 ]
                    .replace("~", QProcessEnvironment::systemEnvironment().value("HOME","~") );

            detail->open_project( project_file_name );
        }

        detail->stacker->setCurrentWidget( widget("project_editor") );
    }

    detail->main_window->showMaximized();
}

void WtsShell::update_sender_style()
{
    QWidget * widget = qobject_cast<QWidget*>(sender());
    if (widget)
        WidgetUtils::update_widget_style( widget );
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

void WtsShell::on_sampleNameEdit_editingFinished()
{
    QLineEdit * widget = qobject_cast<QLineEdit *>(sender());
    SequencerTimeLine * time_line = detail->main_window->findChild<SequencerTimeLine *>();
    Q_ASSERT(time_line);
    WtsAudio::BufferAt * bat = time_line->selectedBufferAt();
    if (!bat) return;
    bat->buffer()->setName( widget->text() );
    time_line->updateBuffer( bat->buffer() );
    detail->project->save();
}
