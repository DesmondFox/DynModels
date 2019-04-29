#include "verhulstmodel.h"
#include <cmath>

VerhulstModel::VerhulstModel(QObject *parent) :
    QObject(parent)
{
}

QList<Element> VerhulstModel::differentiate(const DiffSettings &settings)
{
    qreal   start = settings.startTime,
            end = settings.endTime,
            h = settings.step;
    QList<Element> out;

    // Записать стартовые значения
    startValues = settings.startValues;

    // Метод интегрирования
    DiffMethod  method = settings.diffMethod;

    // Коэффицициенты
    r = settings.data.at(0);  // Коэф. рожд. жертв
    k = settings.data.at(1);  // Коэф. убийства ж-вы х-ками

    // Количества жертв и хищников
    qreal   prev_x  = settings.startValues.at(0);
    // Новый x
    qreal   x;

    quint32 iteration = 1; // итерация
    // Первый элемент добавляем в выход
    out << QPair<qreal, QVector<qreal>>(start, {prev_x});
    // Просчет для каждого шага
    for (qreal t = start+h; t <= end; t += h)
    {
        // текущие значения для каждого выражения на шаге h
        Element element;
        if (method == DiffMethod::Eilers)
        {
            x = prev_x + h*(r*prev_x*(1-prev_x/k));
//            x = prev_x + h* ((1.0 - (prev_x*r)/k) * powf(M_E, r*(1.0-prev_x/k)));
//            x = prev_x + h*(r*prev_x*(1.0-prev_x));
        }
        if (method == DiffMethod::ModifiedEilers)
        {
            double K[2];

            // X
            K[0] = h * (r*prev_x*(1-prev_x/k));
            K[1] = h * (r*(prev_x+K[0])*(1-(prev_x+K[0])/k));

            x = prev_x + (K[0]+K[1])/2.0;

        }
        // Для нахождения первых 5-х [0;5) точек для Адамса
        // Используется метод Р-К. Для избежания копипасты
        if (    method == DiffMethod::RungeKutta4thOrder ||
                method == DiffMethod::AdamsBashforth4rdOrder)
        {
            if ((iteration < 5) || (method == DiffMethod::RungeKutta4thOrder))
            {


                qreal K[4];

                K[0] = h* (r*prev_x*(1-prev_x/k));
                K[1] = h* (r*(prev_x + K[0]/2.0)*(1-(prev_x + K[0]/2.0)/k));
                K[2] = h* (r*(prev_x + K[1]/2.0)*(1-(prev_x + K[1]/2.0)/k));
                K[3] = h* (r*(prev_x + K[2])*(1-(prev_x + K[2])/k));
                x = prev_x + ((K[0] + 2.0*K[1] + 2.0*K[2] + K[3]))/6.0;
            }
        }
        if (method == DiffMethod::AdamsBashforth4rdOrder)
        {
            // После просчета первых 5-х точек идет метод Адамса
            if (iteration >= 5)
            {
                qreal   x_5 = out.at(iteration-5).second.at(0), // Xn-5
                        x_4 = out.at(iteration-4).second.at(0), // Xn-4
                        x_3 = out.at(iteration-3).second.at(0), // Xn-3
                        x_2 = out.at(iteration-2).second.at(0), // Xn-2
                        x_1 = out.at(iteration-1).second.at(0);

                x   = x_1 +h*((1901.0/720.0*(r*x_1*(1-x_1/k))) -
                              (1387.0/360.0*(r*x_1*(1-x_2/k))) +
                              (109.0/30.0*(r*x_1*(1-x_3/k)))  -
                              (637.0/360.0*(r*x_1*(1-x_4/k))) +
                              (251.0/720.0*(r*x_1*(1-x_5/k))));
            }
        }
        prev_x = x;

        // Добавляем к выходной последовательности
        element.first = t;  // Текущее время t
        element.second << x; // Значения популяции
        out << element;
        iteration++;
    }

    return out;
}

QPixmap VerhulstModel::getFormulaPixmap()
{
    return QPixmap();
}

QList<StabilityPoint> VerhulstModel::getEquilibriumPoints()
{
    return QList<StabilityPoint>();
}

QList<PointComplex> VerhulstModel::getEigenvalues()
{
    return QList<PointComplex>();
}

QString VerhulstModel::getEigenvaluesSolve()
{
    return "Eigen values todo";
}

Point VerhulstModel::getStartValues()
{
    return startValues;
}
