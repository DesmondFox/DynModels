#include "phaseportrait.h"

PhasePortrait::PhasePortrait(QWidget *parent) : QCustomPlot(parent)
{
    setOpenGl(true);
    prepareSeries();
}

void PhasePortrait::prepareSeries()
{
    pEilersGraph    = addGraph();
    pModEilersGraph = addGraph();
    pRKuttaGraph    = addGraph();
    pAdamsGraph     = addGraph();

    pEilersGraph->setVisible(false);
    pModEilersGraph->setVisible(false);
    pRKuttaGraph->setVisible(false);
    pAdamsGraph->setVisible(false);

    pEilersGraph->setPen(QPen(Qt::blue, LineWidth));
    pModEilersGraph->setPen(QPen(Qt::red, LineWidth));
    pRKuttaGraph->setPen(QPen(Qt::green, LineWidth));
    pAdamsGraph->setPen(QPen(Qt::magenta, LineWidth));
}

void PhasePortrait::setRoles(const QStringList &roleslist)
{
    Q_ASSERT(roleslist.size() == 2);
    roles = roleslist;
    xAxis->setLabel(roleslist.at(0));
    yAxis->setLabel(roleslist.at(1));
}

QCPGraph *PhasePortrait::getGraph(const DiffMethod &method)
{
    if (method == DiffMethod::Eilers)
        return pEilersGraph;
    if (method == DiffMethod::ModifiedEilers)
        return pModEilersGraph;
    if (method == DiffMethod::RungeKutta4thOrder)
        return pRKuttaGraph;
    if (method == DiffMethod::AdamsBashforth4rdOrder)
        return pAdamsGraph;
}

bool PhasePortrait::isCorrected(const qreal &value)
{
    if (value < 100000)
        return true;
    return false;
}

void PhasePortrait::hideSeries(const DiffMethod &method)
{
    if (method == DiffMethod::Eilers)
        pEilersGraph->setVisible(false);
    if (method == DiffMethod::ModifiedEilers)
        pModEilersGraph->setVisible(false);
    if (method == DiffMethod::RungeKutta4thOrder)
        pRKuttaGraph->setVisible(false);
    if (method == DiffMethod::AdamsBashforth4rdOrder)
        pAdamsGraph->setVisible(false);
}

void PhasePortrait::draw(const DiffMethod &method, const QList<Element> &data)
{
    if (data.first().second.size() != 2)
    {
        qDebug() << "Data.size() != 2";
    }
    QCPGraph *series = getGraph(method);
    series->setVisible(true);

    QVector<qreal> x;
    QVector<qreal> y;
    for (const Element &el : data)
    {
        x << el.second.at(0);
        y << el.second.at(1);
    }
    series->setData(x, y, true);
    rescaleAxes();
    replot();
}

void PhasePortrait::clearPlot()
{
    hideSeries(DiffMethod::Eilers);
    hideSeries(DiffMethod::ModifiedEilers);
    hideSeries(DiffMethod::RungeKutta4thOrder);
    hideSeries(DiffMethod::AdamsBashforth4rdOrder);

    pEilersGraph->data().data()->clear();
    pModEilersGraph->data().data()->clear();
    pRKuttaGraph->data().data()->clear();
    pAdamsGraph->data().data()->clear();
}
