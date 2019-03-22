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
    pointsGraph = nullptr;
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

void PhasePortrait::drawArrows(const QList<Element> &data)
{
    qDebug() << "Drawing arrows at phase portrait";
    qDebug() << "Received" << data.size() << "items";
    int size = data.size();

    for (QCPAbstractItem *item : arrows)
        this->removeItem(item);
    arrows.clear();

    int step = 2;
    if (size < 2)
        return;
    if (size > 2 && size < 50)
        step = 2;
    if (size > 50 && size < 100)
        step = 10;

    if (size > 100 && size < 400)
        step = 20;
    if (size > 400 && size < 1000)
        step = 60;
    if (size > 1000)
        step = 100;
    if (size > 5000)
        step = 400;

//     step = size / 10;

    for (int i = 0; i < data.size() - 5; i+= step) {
        QCPItemLine *arrow = new QCPItemLine(this);
        arrow->start->setCoords(data[i].second.at(0),
                                data[i].second.at(1));
        arrow->end->setCoords(data[i+2].second.at(0),
                                data[i+2].second.at(1));
        arrow->setHead(QCPLineEnding::esSpikeArrow);
        arrows << arrow;
    }
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
    for (QCPItemText *text : textItems) {
        this->removeItem(text);
    }
    textItems.clear();
}

void PhasePortrait::setEquilibriumPoints(QList<StablePointForPhasePortrait> points)
{
    int dbg_size = points.front().point.size();
    Q_ASSERT(points.front().point.size() == 2);

    equilPoints = points;

    if (pointsGraph != nullptr) {
        pointsGraph->data().data()->clear();
        for (QCPItemText *text : textItems) {
            this->removeItem(text);
        }
        textItems.clear();
    }

    pointsGraph = this->addGraph(this->xAxis, this->yAxis);
    pointsGraph->setPen(QPen(QColor(Qt::black)));
    pointsGraph->setBrush(QBrush(QColor(Qt::black)));
    pointsGraph->setLineStyle(QCPGraph::lsNone);
    pointsGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,
                                                 QColor(Qt::black),
                                                 QColor(Qt::black), 8));
    for (StablePointForPhasePortrait p : points) {
        pointsGraph->addData(p.point.at(0), p.point.at(1));
        QCPItemText *text = new QCPItemText(this);
        text->setClipToAxisRect(true);
        text->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
        text->position->setAxes(xAxis, yAxis);
        text->position->setCoords(p.point.at(0), p.point.at(1));
        text->setFont(QFont(font().family(), 12));
        text->setText(p.description);
        textItems << text;
    }
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

    drawArrows(data);

    axisRect()->setupFullAxesBox();
    rescaleAxes(true);
    replot();
}
