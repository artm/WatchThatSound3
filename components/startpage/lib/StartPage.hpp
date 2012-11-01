#ifndef STARTPAGE_HPP
#define STARTPAGE_HPP

#include "stable.h"

class StartPage : public QWidget
{
    Q_OBJECT
public:
    explicit StartPage(QWidget *parent = 0);

    void connect_signals();
    QString selected_filename(const QString& area_name);

signals:
    void create_new_project(QString name, QString original_video);
    void open_file(QString file_name);
    void import_video(QString file_name);

public slots:
    void handle_library_selectionChanged(const QItemSelection& ,const QItemSelection& );
    void handle_projects_selectionChanged(const QItemSelection& ,const QItemSelection& );
    void handle_study_material_selectionChanged(const QItemSelection& ,const QItemSelection& );
    void handle_get_started_selectionChanged(const QItemSelection& ,const QItemSelection& );
    void handle_focusChanged(QWidget * old, QWidget * now);

    // auto-connected
    void on_new_project_clicked();
    void on_open_study_material_clicked();
    void on_open_get_started_clicked();
    void on_add_video_clicked();

};

#endif // STARTPAGE_HPP
