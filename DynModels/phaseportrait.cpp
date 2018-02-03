#include "phaseportrait.h"

PhasePortrait::PhasePortrait(QWidget *parent) : CommonPlot(parent)
{
    prepareItems();
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

void PhasePortrait::prepareItems()
{
    try
    {
        pEilersCurve    = new QCPCurve(xAxis, yAxis);
        pModEilersCurve = new QCPCurve(xAxis, yAxis);
        pRungeKuttaCurve= new QCPCurve(xAxis, yAxis);
        pAdamsCurve     = new QCPCurve(xAxis, yAxis);
    }
    catch(std::bad_alloc &ex)
    {
        qDebug() << "Alloc failed: " << ex.what();
        QApplication::exit(-10);
    }
    ///
    /// Синий       - метод Эйлера
    /// Красный     - мод. метод Эйлера
    /// Зеленый     - метод Р-Кутты
    /// Фиолетовый  - метод Адамса
    ///
    pEilersCurve->setPen(QPen(QBrush(Qt::blue), LineWidth));
    pModEilersCurve->setPen(QPen(QBrush(Qt::red), LineWidth));
    pRungeKuttaCurve->setPen(QPen(QBrush(Qt::green), LineWidth));
    pAdamsCurve->setPen(QPen(QBrush(Qt::magenta), LineWidth));

    pEilersCurve->setVisible(false);
    pModEilersCurve->setVisible(false);
    pRungeKuttaCurve->setVisible(false);
    pAdamsCurve->setVisible(false);
}

QCPCurve *PhasePortrait::getCurve(const DiffMethod &method)
{
    if (method == DiffMethod::Eilers)
        return pEilersCurve;
    if (method == DiffMethod::ModifiedEilers)
        return pModEilersCurve;
    if (method == DiffMethod::RungeKutta4thOrder)
        return pRungeKuttaCurve;
    if (method == DiffMethod::AdamsBashforth4rdOrder)
        return pAdamsCurve;
    return nullptr;
}

void PhasePortrait::setRoles(const QStringList &roleslist)
{
    Q_ASSERT(roleslist.size() == 2);
    roles = roleslist;
    xAxis->setLabel(roleslist.at(0));
    yAxis->setLabel(roleslist.at(1));
}

void PhasePortrait::hide(const DiffMethod &method)
{
    auto curve = getCurve(method);
    /// WARNING: Костыль
    curve->data().data()->clear();
    curve->setVisible(false);
    replot();
}

void PhasePortrait::clearPlot()
{
    hide(DiffMethod::Eilers);
    hide(DiffMethod::ModifiedEilers);
    hide(DiffMethod::RungeKutta4thOrder);
    hide(DiffMethod::AdamsBashforth4rdOrder);
    pEilersCurve->data().data()->clear();
    pModEilersCurve->data().data()->clear();
    pRungeKuttaCurve->data().data()->clear();
    pAdamsCurve->data().data()->clear();
}

void PhasePortrait::draw(const DiffMethod &method, const QList<Element> &data)
{
    Q_ASSERT(data.first().second.size() == 2);
    auto curve = getCurve(method);
    curve->setVisible(true);

    QVector<QCPCurveData> convertedData;
    for (const Element &el : data)
    {
        qreal x = el.second.at(0);
        qreal y = el.second.at(1);
        if (isCorrected(x) && isCorrected(y))
            convertedData << QCPCurveData(el.first, x, y);
    }
    curve->data().data()->set(convertedData, true);
    axisRect()->setupFullAxesBox();
    rescaleAxes();
    replot();
}
