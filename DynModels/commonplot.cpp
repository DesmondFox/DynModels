#include "commonplot.h"

CommonPlot::CommonPlot(QWidget *parent) : QCustomPlot(parent)
{
    setOpenGl(true);
    setMouseTracking(true);
}

bool CommonPlot::isCorrected(const qreal &value)
{
    if (value < 100000)
        return true;
    qDebug() << "Value: " << value << "is not corrected";
    return false;
}

void CommonPlot::mouseMoveEvent(QMouseEvent *event)
{
    QCustomPlot::mouseMoveEvent(event);

    x = xAxis->pixelToCoord(event->pos().x());
    y = yAxis->pixelToCoord(event->pos().y());
    qreal xmax = xAxis->range().maxRange;
    qreal ymax = yAxis->range().maxRange;
    if (x > 0.0 && y > 0.0 && x < xmax && y < ymax)
        emit mouseHover(QPointF(x, y));
    else
        emit mouseHover(QPointF(-1.0, -1.0));
}
