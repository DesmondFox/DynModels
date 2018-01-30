#ifndef PLOT2D_H
#define PLOT2D_H

#include <QWidget>
#include <qcustomplot.h>
#include "diffworker.h"

class Plot2D : public QCustomPlot
{
    Q_OBJECT
public:
    explicit Plot2D(QWidget *parent = nullptr);
    void drawSeries(const DiffMethod &method, const QList<Element> &data);
    void clearPlot();
    // Спрятать серию
    void hideSeries(const DiffMethod &method);
    // Задать легенду/названия серий
    void setRoles(const QStringList &roleslist);

private:
    static const quint8 SeriesNum = 3;
    static const quint8 LineWidth = 2;
    static const quint8 ColorGrad = 100;
    static const quint8 MarginPerc= 5;
    // Графики для методов
    std::array<QCPGraph*, SeriesNum> m_EilersGraphs;
    std::array<QCPGraph*, SeriesNum> m_ModEilersGraphs;
    std::array<QCPGraph*, SeriesNum> m_RKuttaGraphs;
    std::array<QCPGraph*, SeriesNum> m_AdamsGraphs;
    // подготовка серий
    void prepareGraphs();
    void colorsToGraphs();
    // Получение массива указателей на серии для опр.метода
    std::array<QCPGraph*, SeriesNum> getGraphs(const DiffMethod &method);
    // Максимальное значение Y
    std::array<qreal, DiffWorker::MethodNum> maximalY;
    quint8 getIndexByMethod(const DiffMethod &method) const;
    // Проверка на корректность значений
    bool isCorrected(const qreal &value);
    // Изменение range графика
    void setRange();
    // Роли
    QStringList roles;
    // Задать рили для метода
    void setLegend(const DiffMethod &method);
};

#endif // PLOT2D_H
