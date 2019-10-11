#include "hollingtanner.h"
#include <QDebug>
#include <cmath>

HollingTanner::HollingTanner(QObject *parent) :
    QObject(parent)
{
    qDebug() << ">> Holling-Tanner model has been loaded";
}

HollingTanner::~HollingTanner()
{
    qDebug() << "<< Holling-Tanner model has been destroyed";

}

QList<Element> HollingTanner::differentiate(const DiffSettings &settings)
{
    qreal   start = settings.startTime,
            end = settings.endTime,
            h = settings.step;
    QList<Element> out;

    startValues = settings.startValues;

    // Метод интегрирования
    DiffMethod  method = settings.diffMethod;

    // Коэффицициенты
    r   = settings.data.at(0);  // r
    s   = settings.data.at(1);  // s
    K   = settings.data.at(2);  // K
    D   = settings.data.at(3);  // D
    J   = settings.data.at(4);  // J
    w   = settings.data.at(5);  // w

    // Количества жертв и хищников
    qreal   prev_x  = settings.startValues.at(0),
            prev_y  = settings.startValues.at(1);

    // Новые x и y
    qreal   x = 0.0,
            y = 0.0;
    quint32 iteration = 1; // итерация
    // Первый элемент добавляем в выход
    out << QPair<qreal, QVector<qreal>>(start, {prev_x, prev_y});
    // Просчет для каждого шага
    for (qreal t = start+h; t <= end; t += h)
    {
        // текущие значения для каждого выражения на шаге h
        Element element;
        if (method == DiffMethod::Eilers)
        {
            x = prev_x + h*(r*(1-prev_x/K)*prev_x - (w*prev_x*prev_y)/(D+prev_x));
            y = prev_y + h*(s*(1-J*prev_y/prev_x)*prev_y);
        }
        if (method == DiffMethod::ModifiedEilers)
        {
            double K_[2];
            // X
            K_[0] = h * (r*(1-prev_x/K)*prev_x - (w*prev_x*prev_y)/(D+prev_x));
            K_[1] = h * ((r*(1-(prev_x+K_[0])/K))*(prev_x+K_[0]) - (w*(prev_x+K_[0])*(prev_y+K_[0]))/(D+(prev_x+K_[0])));
            x = prev_x + (K_[0]+K_[1])/2.0;

            // Y
            K_[0] = h * (s*(1-J*prev_y/prev_x)*prev_y);
            K_[1] = h * (s*(1-J*(prev_y+K_[0])/(prev_x+K_[0]))*(prev_y+K_[0]));
            y = prev_y + (K_[0]+K_[1])/2.0;
        }
        // Для нахождения первых 5-х [0;5) точек для Адамса
        // Используется метод Р-К. Для избежания копипасты
        if (    method == DiffMethod::RungeKutta4thOrder ||
                method == DiffMethod::AdamsBashforth4rdOrder)        {
            if ((iteration < 5) || (method == DiffMethod::RungeKutta4thOrder))
            {
                qreal K_[4];
                // X
                K_[0] = h* (r*(1-prev_x/K)*prev_x - (w*prev_x*prev_y)/(D+prev_x));
                K_[1] = h* (r*(1-(prev_x+0.5*K_[0])/K)*(prev_x+0.5*K_[0]) - (w*(prev_x+0.5*K_[0])*(prev_y+0.5*K_[0]))/(D+(prev_x+0.5*K_[0])));
                K_[2] = h* (r*(1-(prev_x+0.5*K_[1])/K)*(prev_x+0.5*K_[1]) - (w*(prev_x+0.5*K_[1])*(prev_y+0.5*K_[1]))/(D+(prev_x+0.5*K_[1])));
                K_[3] = h* (r*(1-(prev_x+K_[2])/K)*(prev_x+K_[2]) - (w*(prev_x+K_[2])*(prev_y+K_[2]))/(D+(prev_x+K_[2])));
                x = prev_x + (K_[0] + 2.0*K_[1] + 2.0*K_[2] + K_[3])/6.0;

                // Y
                K_[0] = h* (s*(1-J*prev_y/prev_x)*prev_y);
                K_[1] = h* (s*(1-J*(prev_y+0.5*K_[0])/(prev_x+0.5*K_[0]))*(prev_y+0.5*K_[0]));
                K_[2] = h* (s*(1-J*(prev_y+0.5*K_[1])/(prev_x+0.5*K_[1]))*(prev_y+0.5*K_[1]));
                K_[3] = h* (s*(1-J*(prev_y+K_[2])/(prev_x+K_[2]))*(prev_y+K_[2]));
                y   = prev_y + (K_[0] + 2.0*K_[1] + 2.0*K_[2] + K_[3])/6.0;
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
                        x_1 = out.at(iteration-1).second.at(0), // Xn-1
                        y_5 = out.at(iteration-5).second.at(1), // Yn-5
                        y_4 = out.at(iteration-4).second.at(1), // Yn-4
                        y_3 = out.at(iteration-3).second.at(1), // Yn-3
                        y_2 = out.at(iteration-2).second.at(1), // Yn-2
                        y_1 = out.at(iteration-1).second.at(1); // Yn-1

                x   = x_1 +h*((1901.0/720.0 *(r*(1-x_1/K)*x_1 - (w*x_1*y_1)/(D+x_1))) -
                              (1387.0/360.0 *(r*(1-x_2/K)*x_2 - (w*x_1*y_2)/(D+x_2))) +
                              (109.0/30.0   *(r*(1-x_3/K)*x_3 - (w*x_1*y_3)/(D+x_3)))   -
                              (637.0/360.0  *(r*(1-x_4/K)*x_4 - (w*x_1*y_4)/(D+x_4)))  +
                              (251.0/720.0  *(r*(1-x_5/K)*x_5 - (w*x_1*y_5)/(D+x_5))));

                y   = y_1 +h*((1901.0/720.0 *(s*(1-J*y_1/x_1)*y_1)) -
                              (1387.0/360.0 *(s*(1-J*y_2/x_2)*y_2)) +
                              (109.0/30.0   *(s*(1-J*y_3/x_3)*y_3)) -
                              (637.0/360.0  *(s*(1-J*y_4/x_4)*y_4)) +
                              (251.0/720.0  *(s*(1-J*y_5/x_5)*y_5)));
            }
        }
        prev_x = x;
        prev_y = y;

        // Добавляем к выходной последовательности
        element.first = t;  // Текущее время t
        element.second << x << y; // Значения популяции
        out << element;
        iteration++;
    }
    return out;
}

QPixmap HollingTanner::getFormulaPixmap()
{
    return QPixmap(":/formula/img/formula.png");
}

QList<StabilityPoint> HollingTanner::getEquilibriumPoints()
{
    QList<StabilityPoint> stab;
    qreal y = startValues[1];

    qreal x1, x2, y1, y2;

    y1 = (-(r*J*(J*K-D*J) - K*J*w) + sqrt(pow(r*J*(J*K-D*J) - K*J*w, 2) - 4*(-pow(J, 3)*pow(y, 2))*(r*J*D*K)))
            / (-2*r*pow(J, 3));
    x1 = J*y1;

    y2 = (-(r*J*(J*K-D*J) - K*J*w) - sqrt(pow(r*J*(J*K-D*J) - K*J*w, 2) - 4*(-pow(J, 3)*pow(y, 2))*(r*J*D*K)))
            / (-2*r*pow(J, 3));
    x2 = J*y2;

    stab.append(StabilityPoint(
                    QList<qreal>()  << 0
                                    << 0
                    , ""));
    stab.append(StabilityPoint(
                    QList<qreal>()  << x1
                                    << y1
                    , ""));
    stab.append(StabilityPoint(
                    QList<qreal>()  << x2
                                    << y2
                    , ""));
    return stab;
}

QList<PointComplex> HollingTanner::getEigenvalues()
{
    return QList<PointComplex>();
}

Point HollingTanner::getStartValues()
{
    return startValues;
}

QString HollingTanner::resolveLambdas(const PointComplex &complex)
{
    return QString();
}
