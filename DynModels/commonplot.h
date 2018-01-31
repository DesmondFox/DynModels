#ifndef COMMONPLOT_H
#define COMMONPLOT_H

#include <qcustomplot.h>
#include "diffworker.h"

class CommonPlot : public QCustomPlot
{
    Q_OBJECT
public:
    explicit CommonPlot(QWidget *parent = nullptr);
    virtual void draw(const DiffMethod&, const QList<Element>&) = 0;
    virtual void clearPlot() = 0;
    virtual void setRoles(const QStringList&) = 0;
    virtual void hide(const DiffMethod&) = 0;
protected:
    static const quint8 LineWidth = 2;
    QStringList roles;
    bool isCorrected(const qreal &value);
signals:
    void mouseHover(const QPointF &point);

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event);
    qreal x;
    qreal y;
};

#endif // COMMONPLOT_H
