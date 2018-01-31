#ifndef TABPLOT3D_H
#define TABPLOT3D_H

#include <QWidget>
#include "diffworker.h"

namespace Ui {
class TabPlot3D;
}

class TabPlot3D : public QWidget
{
    Q_OBJECT

public:
    explicit TabPlot3D(QWidget *parent = 0);
    ~TabPlot3D();
    void setData(const QList<ASolveByMethod> &solve,
                 const QStringList &rolelist);
    void drawPlot();

private slots:
    void on_cbEulers3d_clicked();

    void on_cbModEulers3d_clicked();

    void on_cbRungeKutta3d_clicked();

    void on_cbAdams3d_clicked();

private:
    QList<ASolveByMethod> results;
    Ui::TabPlot3D *ui;
    QStringList roles;
};

#endif // TABPLOT3D_H
