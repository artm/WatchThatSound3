#ifndef STARTPAGE_HPP
#define STARTPAGE_HPP

#include "stable.h"

class StartPage : public QWidget
{
    Q_OBJECT
public:
    explicit StartPage(QWidget *parent = 0);

    void connectSignals();
signals:
    void create_new_project(QString);

public slots:
    void handle_library_currentChanged(const QModelIndex& ,const QModelIndex& );
    void handle_projects_currentChanged(const QModelIndex& ,const QModelIndex& );
    void handle_study_material_currentChanged(const QModelIndex& ,const QModelIndex& );
    void handle_get_started_currentChanged(const QModelIndex& ,const QModelIndex& );
    void on_new_project_clicked();
};

#endif // STARTPAGE_HPP
