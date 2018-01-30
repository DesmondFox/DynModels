#include "plot2d.h"

Plot2D::Plot2D(QWidget *parent) : QCustomPlot(parent)
{
    maximalY = {0};
    prepareGraphs();
    setOpenGl(true);
    xAxis->setLabel("Продолжительность, года");
    yAxis->setLabel("Популяция, шт");
    legend->setVisible(false);
}

void Plot2D::drawSeries(const DiffMethod &method, const QList<Element> &data)
{
    std::array<QCPGraph*, SeriesNum> series = getGraphs(method);
    quint8 methodIndex = getIndexByMethod(method);

    quint8 seriesCount = data.first().second.size();
    series.at(0)->setVisible(true);
    series.at(1)->setVisible(true);
    if (seriesCount == 3)
        series.at(2)->setVisible(true);

    // Задать роли
    setLegend(method);

    for (int ser = 0; ser < seriesCount; ser++)
    {
        for (const Element &el : data)
        {
            qreal t = el.first;
            qreal y = el.second.at(ser);
            if (!isCorrected(y))
            {
                qDebug() << "Uncorrected value";
                return;
            }
            if (maximalY.at(methodIndex) < y)
                maximalY.at(methodIndex) = y;

            series.at(ser)->addData(t, y);
        }
    }
    xAxis->setRange(data.first().first, data.back().first);
    setRange();
    replot();
}

void Plot2D::clearPlot()
{
    QVector<DiffMethod> methodSequention = {
        DiffMethod::Eilers,
        DiffMethod::ModifiedEilers,
        DiffMethod::RungeKutta4thOrder,
        DiffMethod::AdamsBashforth4rdOrder
    };
    for (const DiffMethod &method : methodSequention)
    {
        auto series = getGraphs(method);
        for (quint8 i = 0; i < series.size(); i++)
        {
            series.at(i)->setVisible(false);
            series.at(i)->data().data()->clear();
            series.at(i)->removeFromLegend(legend);
        }
    }

    for (quint8 i = 0; i < maximalY.size(); i++)
        maximalY.at(i) = 0.0;
    roles.clear();
    qDebug() << "Plot cleared";
}

void Plot2D::prepareGraphs()
{
    for (quint8 i = 0; i < m_EilersGraphs.size(); i++)
        m_EilersGraphs.at(i) = this->addGraph();
    for (quint8 i = 0; i < m_ModEilersGraphs.size(); i++)
        m_ModEilersGraphs.at(i) = this->addGraph();
    for (quint8 i = 0; i < m_RKuttaGraphs.size(); i++)
        m_RKuttaGraphs.at(i) = this->addGraph();
    for (quint8 i = 0; i < m_AdamsGraphs.size(); i++)
        m_AdamsGraphs.at(i) = this->addGraph();
    colorsToGraphs();
}

void Plot2D::colorsToGraphs()
{
    ///
    /// Синий       - метод Эйлера
    /// Красный     - мод. метод Эйлера
    /// Зеленый     - метод Р-Кутты
    /// Фиолетовый  - метод Адамса
    ///
    /// 1 значение  - SolidLine
    /// 2 значение  - DashLine
    /// 3 значение  - DashDotLine
    ///

    QVector<Qt::PenStyle> styleSequence = {
        Qt::SolidLine,
        Qt::DashLine,
        Qt::DashDotLine
    };
    for (int i = 0; i < SeriesNum; i++)
    {
        m_EilersGraphs.at(i)->setPen(QPen(QBrush(QColor(0, 0, 255-i*ColorGrad, 240-i*ColorGrad)), LineWidth, styleSequence.at(i)));
        m_ModEilersGraphs.at(i)->setPen(QPen(QBrush(QColor(255-i*ColorGrad, 0, 0, 240-i*ColorGrad)), LineWidth, styleSequence.at(i)));
        m_RKuttaGraphs.at(i)->setPen(QPen(QBrush(QColor(0, 255-i*ColorGrad, 0, 240-i*ColorGrad)), LineWidth, styleSequence.at(i)));
        m_AdamsGraphs.at(i)->setPen(QPen(QBrush(QColor(255-i*ColorGrad, 0, 255-i*ColorGrad, 240-i*ColorGrad)), LineWidth, styleSequence.at(i)));
    }
}

std::array<QCPGraph *, Plot2D::SeriesNum> Plot2D::getGraphs(const DiffMethod &method)
{
    if (method == DiffMethod::Eilers)
        return m_EilersGraphs;
    if (method == DiffMethod::ModifiedEilers)
        return m_ModEilersGraphs;
    if (method == DiffMethod::RungeKutta4thOrder)
        return m_RKuttaGraphs;
    if (method == DiffMethod::AdamsBashforth4rdOrder)
        return m_AdamsGraphs;
    return std::array<QCPGraph *, Plot2D::SeriesNum>();
}

quint8 Plot2D::getIndexByMethod(const DiffMethod &method) const
{
    if (method == DiffMethod::Eilers)
        return 0;
    if (method == DiffMethod::ModifiedEilers)
        return 1;
    if (method == DiffMethod::RungeKutta4thOrder)
        return 2;
    if (method == DiffMethod::AdamsBashforth4rdOrder)
        return 3;
    return -1;
}

bool Plot2D::isCorrected(const qreal &value)
{
    if (value < 100000)
        return true;
    return false;
}

void Plot2D::setRange()
{
    qreal max = *std::max_element(maximalY.begin(), maximalY.end());
    yAxis->setRange(0.0, max+(MarginPerc/100)*max);
}

void Plot2D::setLegend(const DiffMethod &method)
{
    if (roles.isEmpty())
        return;
    auto series = getGraphs(method);
    for (quint8 i = 0; i < roles.size(); i++)
    {
        series.at(i)->setName(roles.at(i));
        series.at(i)->addToLegend(legend);
    }
}

void Plot2D::hideSeries(const DiffMethod &method)
{
    auto series = getGraphs(method);
    for (quint8 i = 0; i < series.size(); i++)
    {
        series.at(i)->setVisible(false);
        series.at(i)->removeFromLegend(legend);
        /// WARNING: Костыль
        series.at(i)->data().data()->clear();
    }
    replot();
}

void Plot2D::setRoles(const QStringList &roleslist)
{
    Q_ASSERT(roleslist.size() <= SeriesNum);
    roles.clear();
    roles = roleslist;
}
