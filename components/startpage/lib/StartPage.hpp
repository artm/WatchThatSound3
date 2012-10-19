#ifndef STARTPAGE_HPP
#define STARTPAGE_HPP

#include "stable.h"

class StartPage : public QWidget
{
    Q_OBJECT
public:
    explicit StartPage(QWidget *parent = 0);

    void connectSignals();
    QString selected_filename(const QString& area_name);

signals:
    void create_new_project(QString name, QString original_video);
    void open_file(QString file_name);

public slots:
    void handle_library_currentChanged(const QModelIndex& ,const QModelIndex& );
    void handle_projects_currentChanged(const QModelIndex& ,const QModelIndex& );
    void handle_study_material_currentChanged(const QModelIndex& ,const QModelIndex& );
    void handle_get_started_currentChanged(const QModelIndex& ,const QModelIndex& );

    // auto-connected
    void on_new_project_clicked();
    void on_open_study_material_clicked();

};

#endif // STARTPAGE_HPP
