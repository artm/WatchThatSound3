#include "ProjectEditor.hpp"

#include "utils/WidgetUtils"
#include "utils/Macros"
#include "utils/Exception"

struct ProjectEditor::Detail
{
    static const QStringList widget_presets;
    static const QStringList dynamic_widgets;    
    QList< QVector<int> > presets;

    Detail() {
        parse_presets();
    }

    void setup_tab_bar(ProjectEditor * self)
    {
        QTabBar * tabs = new QTabBar();
        WidgetUtils::replace_widget( self, "tabs", tabs );

        tabs->addTab("1 scenes");
        tabs->addTab("2 tension line");
        tabs->addTab("3 visual score");
        tabs->addTab("4 audio edit");
        tabs->setDrawBase(false);
    }

    void parse_presets() {
        foreach(const QString& preset, widget_presets ) {
            QVector<int> states(4);
            QStringList tokens = preset.split(QRegExp(" +"));
            for(int i = 0; i < tokens.length(); ++i) {
                states[i] = tokens[i].toInt();
            }
            presets << states;
        }
    }

    void populate_dynamic_widgets(ProjectEditor * self) {
        foreach(QString row_name, dynamic_widgets) {
            QString url = ":/forms/" + row_name + ".ui";
            QWidget * container = self->findChild<QWidget *>( row_name + "_container" );
            if (!container) continue;
            try {
                WidgetUtils::load_form(container, url);
            } catch ( const FileNotFound& ) {
                // no form, ignore...
                continue;
            }
        }
    }

    void set_widget_state( QWidget * container, int state )
    {
        QWidget * button_bar = container->findChild<QWidget*>("buttons");
        QGraphicsView * time_line = container->findChild<QGraphicsView*>();

        container->setProperty("active_state", state);

        QSizePolicy policy = container->sizePolicy();

        switch( state ) {
        case 0:
            container->hide();
            break;
        case 1:
            NOP_OR( button_bar )->hide();
            NOP_OR( time_line )->setProperty("editMode", false);
            container->show();
            container->resize(0,0);
            policy.setVerticalPolicy(QSizePolicy::Fixed);
            break;
        case 2:
            NOP_OR(button_bar)->show();
            NOP_OR( time_line )->setProperty("editMode", true);
            container->show();
            policy.setVerticalPolicy(QSizePolicy::Expanding);
            break;
        }

        container->setSizePolicy(policy);

        WidgetUtils::update_widget_style(container);
    }
};

const QStringList ProjectEditor::Detail::widget_presets = QStringList()
        << "2"
        << "1 2"
        << "1 0 2"
        << "1 0 1 2";

const QStringList ProjectEditor::Detail::dynamic_widgets = QStringList()
        << "storyboard"
        << "tension"
        << "score"
        << "record";

ProjectEditor::ProjectEditor(QWidget *parent)
    : QWidget(parent), detail(new Detail)
{
    WidgetUtils::load_form(this,":/forms/ProjectEditor.ui");
    detail->setup_tab_bar(this);
    detail->populate_dynamic_widgets(this);

    QMetaObject::connectSlotsByName(this);
    on_tabs_currentChanged( findChild<QTabBar*>("tabs")->currentIndex() );
}

ProjectEditor::~ProjectEditor()
{
}

void ProjectEditor::on_tabs_currentChanged(int index)
{
    QVector<int> preset = detail->presets[index];
    for(int i = 0; i < preset.size(); ++i) {
        QString widget_name = Detail::dynamic_widgets[i];
        QString container_name = widget_name + "_container";
        detail->set_widget_state( findChild<QWidget*>(container_name), preset[i] );
    }
}
