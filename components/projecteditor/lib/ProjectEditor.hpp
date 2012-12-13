#ifndef PROJECTEDITOR_HPP
#define PROJECTEDITOR_HPP

#include "stable.h"

class ProjectEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectEditor(QWidget *parent = 0);
    ~ProjectEditor();
    
signals:
    
public slots:
    void on_tabs_currentChanged(int index);

protected:
    class Detail;
    QScopedPointer<Detail> detail;
};

#endif // PROJECTEDITOR_HPP
