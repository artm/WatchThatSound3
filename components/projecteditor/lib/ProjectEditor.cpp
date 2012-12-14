#include "ProjectEditor.hpp"
#include "utils/WidgetUtils"

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
        QWidget * container = findChild<QWidget*>(container_name);
        QSizePolicy policy = container->sizePolicy();

        switch( preset[i] ) {
        case 0:
            container->hide();
            break;
        case 1:
            policy.setVerticalStretch(0);
            container->setSizePolicy(policy);
            container->show();
            break;
        case 2:
            policy.setVerticalStretch(1);
            container->setSizePolicy(policy);
            container->show();
            break;
        }
    }
}
