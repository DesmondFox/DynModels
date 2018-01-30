#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QChart>
#include <QSplineSeries>
#include "plot2d.h"

namespace Ui {
class ResultWidget;
}

class ResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultWidget(QWidget *parent = 0);
    ~ResultWidget();
    void setData(const QList<ASolveByMethod> &solve, const QStringList &roleslist);
private slots:
    void on_cbEulers_clicked();
    void on_cbModEulers_clicked();
    void on_cbRungeKutta_clicked();
    void on_cbAdams_clicked();
private:
    Ui::ResultWidget        *ui;
    QList<ASolveByMethod>   results;
    QStringList roles;
    void drawPlots();
};

#endif // RESULTWIDGET_H
