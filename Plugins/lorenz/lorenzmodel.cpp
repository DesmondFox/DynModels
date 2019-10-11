#include "lorenzmodel.h"
#include <cmath>

LorenzModel::LorenzModel(QObject *parent) :
    QObject(parent)
{
}

QList<Element> LorenzModel::differentiate(const DiffSettings &settings)
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
    d = settings.data.at(0);  // Коэф. a
    r = settings.data.at(1);  // Коэф. b
    b = settings.data.at(2);  // Коэф. c

    // Количества жертв и хищников
    qreal   prev_x  = settings.startValues.at(0);
    qreal   prev_y  = settings.startValues.at(1);
    qreal   prev_z  = settings.startValues.at(2);
    // Новый x
    qreal   x;
    qreal   y;
    qreal   z;

    quint32 iteration = 1; // итерация
    // Первый элемент добавляем в выход
    out << QPair<qreal, QVector<qreal>>(start, {prev_x, prev_y, prev_z});
    // Просчет для каждого шага
    for (qreal t = start+h; t <= end; t += h)
    {
        // текущие значения для каждого выражения на шаге h
        Element element;
        if (method == DiffMethod::Eilers)
        {
            //            x = prev_x + h*(r*prev_x*(1-prev_x/k));
            x = prev_x + h*(d*(prev_y-prev_x));
            y = prev_y + h*(r*prev_x-prev_y-prev_x*prev_z);
            z = prev_z + h*(-b*prev_z+prev_x*prev_y);
        }
        if (method == DiffMethod::ModifiedEilers)
        {
            double K[2];

            // X
            K[0] = h*(d*(prev_y-prev_x));
            K[0] = h*(d*((prev_y+K[0])-(prev_x+K[0])));
            x = prev_x + (K[0]+K[1])/2.0;

            // Y
            K[0] = h * (r*prev_x-prev_y-prev_x*prev_z);
            K[1] = h * (r*(prev_x+K[0])-(prev_y+K[0])-(prev_x+K[0])*(prev_z+K[0]));
            y = prev_y + (K[0]+K[1])/2.0;

            // Z
            K[0] = h * (-b*prev_z+prev_x*prev_y);
            K[1] = h * (-b*(prev_z+K[0])+(prev_x+K[0])*(prev_y+K[0]));
            z = prev_z + (K[0]+K[1])/2.0;
        }
        // Для нахождения первых 5-х [0;5) точек для Адамса
        // Используется метод Р-К. Для избежания копипасты
        if (    method == DiffMethod::RungeKutta4thOrder ||
                method == DiffMethod::AdamsBashforth4rdOrder)
        {
            if ((iteration < 5) || (method == DiffMethod::RungeKutta4thOrder))
            {
                qreal K[4];

                // X1
                K[0] = h* (d*(prev_y-prev_x));
                K[1] = h* (d*((prev_y+0.5*K[0])-(prev_x+0.5*K[0])));
                K[2] = h* (d*((prev_y+0.5*K[1])-(prev_x+0.5*K[1])));
                K[3] = h* (d*((prev_y+K[2])-(prev_x+K[2])));
                x = prev_x + (K[0] + 2.0*K[1] + 2.0*K[2] + K[3])/6.0;

                // y
                K[0] = h* (r*prev_x-prev_y-prev_x*prev_z);
                K[1] = h* (r*(prev_x+0.5*K[0])-(prev_y+0.5*K[0])-(prev_x+0.5*K[0])*(prev_z+0.5*K[0]));
                K[2] = h* (r*(prev_x+0.5*K[1])-(prev_y+0.5*K[1])-(prev_x+0.5*K[1])*(prev_z+0.5*K[1]));
                K[3] = h* (r*(prev_x+K[2])-(prev_y+K[2])-(prev_x+K[2])*(prev_z+K[2]));
                y = prev_y + (K[0] + 2.0*K[1] + 2.0*K[2] + K[3])/6.0;

                // Z
                K[0] = h* (-b*prev_z+prev_x*prev_y);
                K[1] = h* (-b*(prev_z+0.5*K[0])+(prev_x+0.5*K[0])*(prev_y+0.5*K[0]));
                K[2] = h* (-b*(prev_z+0.5*K[1])+(prev_x+0.5*K[1])*(prev_y+0.5*K[1]));
                K[3] = h* (-b*(prev_z+K[2])+(prev_x+K[2])*(prev_y+K[2]));
                z = prev_z + (K[0] + 2.0*K[1] + 2.0*K[2] + K[3])/6.0;
            }
        }
        if (method == DiffMethod::AdamsBashforth4rdOrder)
        {
            // После просчета первых 5-х точек идет метод Адамса
            if (iteration >= 5)
            {
                qreal   x_5 = out.at(iteration-5).second.at(0), // X1n-5
                        x_4 = out.at(iteration-4).second.at(0), // X1n-4
                        x_3 = out.at(iteration-3).second.at(0), // X1n-3
                        x_2 = out.at(iteration-2).second.at(0), // X1n-2
                        x_1 = out.at(iteration-1).second.at(0), // X1n-1
                        y_5 = out.at(iteration-5).second.at(1), // X2n-5
                        y_4 = out.at(iteration-4).second.at(1), // X2n-4
                        y_3 = out.at(iteration-3).second.at(1), // X2n-3
                        y_2 = out.at(iteration-2).second.at(1), // X2n-2
                        y_1 = out.at(iteration-1).second.at(1), // X2n-1
                        z_5 = out.at(iteration-5).second.at(2),  // Yn-5
                        z_4 = out.at(iteration-4).second.at(2),  // Yn-4
                        z_3 = out.at(iteration-3).second.at(2),  // Yn-3
                        z_2 = out.at(iteration-2).second.at(2),  // Yn-2
                        z_1 = out.at(iteration-1).second.at(2);  // Yn-1

                x   = x_1+h*((1901.0/720.0*(d*(y_1-x_1))) -
                             (1387.0/360.0 *(d*(y_2-x_2))) +
                             (109.0/30.0   *(d*(y_3-x_3)))   -
                             (637.0/360.0  *(d*(y_4-x_4)))  +
                             (251.0/720.0  *(d*(y_5-x_5))));

                y   = y_1+h*((1901.0/720.0*(r*x_1-y_1-x_1*z_1)) -
                             (1387.0/360.0 *(r*x_2-y_2-x_2*z_2)) +
                             (109.0/30.0   *(r*x_3-y_3-x_3*z_3))   -
                             (637.0/360.0  *(r*x_4-y_4-x_4*z_4))  +
                             (251.0/720.0  *(r*x_5-y_5-x_5*z_5)));

                z   = z_1 +h*((1901.0/720.0 *(-b*z_1+x_1*y_1)) -
                              (1387.0/360.0 *(-b*z_2+x_2*y_2)) +
                              (109.0/30.0   *(-b*z_3+x_3*y_3)) -
                              (637.0/360.0  *(-b*z_4+x_4*y_4)) +
                              (251.0/720.0  *(-b*z_5+x_5*y_5)));
            }
        }
        prev_x = x;
        prev_y = y;
        prev_z = z;

        // Добавляем к выходной последовательности
        element.first = t;  // Текущее время t
        element.second << x
                       << y
                       << z; // Значения популяции
        out << element;
        iteration++;
    }

    return out;
}

QPixmap LorenzModel::getFormulaPixmap()
{
    return QPixmap(":/lorenz_formula.png");
}

QList<StabilityPoint> LorenzModel::getEquilibriumPoints()
{
    StabilityPoint p1;
    p1.point << 0.0 << 0.0 << 0.0;

    StabilityPoint p2;
    p2.point << -sqrt(b)*sqrt(r-1)
             << -sqrt(b)*sqrt(r-1)
             << r - 1;
    return QList<StabilityPoint>()
            << p1
            << p2;
}

QList<PointComplex> LorenzModel::getEigenvalues()
{
    return QList<PointComplex>();
}

QString LorenzModel::getEigenvaluesSolve()
{
    return "Eigen values todo";
}

Point LorenzModel::getStartValues()
{
    return startValues;
}

QString LorenzModel::resolveLambdas(const PointComplex &complex)
{
    return QString();
}
