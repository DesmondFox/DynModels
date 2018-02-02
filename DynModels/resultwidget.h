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
    void drawGraphs();
    QPixmap getPlotPix() const;
    QPixmap getPhasePix() const;

private slots:
    void on_cbEulers_clicked();
    void on_cbModEulers_clicked();
    void on_cbRungeKutta_clicked();
    void on_cbAdams_clicked();
    void slotTabWidgetIndexChanged(int index);
private:
    Ui::ResultWidget        *ui;
    QList<ASolveByMethod>   results;
    QStringList roles;
    void checkClicked(const QCheckBox *cb, const DiffMethod &method, const quint8 &index);
    quint8 getSizeOfData() const;
    void draw2DPlots(const quint8 &index, const DiffMethod &method);
signals:
    void sigMouseHoveredOn2DPlots(const QPointF &point);
};

#endif // RESULTWIDGET_H
