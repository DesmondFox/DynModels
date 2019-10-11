#include "ResslerModel.h"
#include <cmath>

ResslerModel::ResslerModel(QObject *parent) :
    QObject(parent)
{
}

QList<Element> ResslerModel::differentiate(const DiffSettings &settings)
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
    a = settings.data.at(0);  // Коэф. a
    b = settings.data.at(1);  // Коэф. b
    c = settings.data.at(2);  // Коэф. c

    // Количества жертв и хищников
    qreal   prev_x  = settings.startValues.at(0);
    qreal   prev_y  = settings.startValues.at(1);
    qreal   prev_z  = settings.startValues.at(2);
    // Новый x
    qreal   x;
    qreal   y;
    qreal   z;

    int iteration = 1; // итерация
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
            x = prev_x + h*(a*(prev_y-prev_x));
            y = prev_y + h*(b*prev_x-prev_y-prev_x*prev_z);
            z = prev_z + h*(-c*prev_z+prev_x*prev_y);

            ///
            x = prev_x + h*(-prev_y-prev_z);
            y = prev_y + h*(prev_x + a*prev_y);
            z = prev_z + h*(b + prev_z*(prev_x - c));

        }
        if (method == DiffMethod::ModifiedEilers)
        {
            double K[2];

            // X
            K[0] = h*(a*(prev_y-prev_x));
            K[0] = h*(a*((prev_y+K[0])-(prev_x+K[0])));
            x = prev_x + (K[0]+K[1])/2.0;

            // Y
            K[0] = h * (b*prev_x-prev_y-prev_x*prev_z);
            K[1] = h * (b*(prev_x+K[0])-(prev_y+K[0])-(prev_x+K[0])*(prev_z+K[0]));
            y = prev_y + (K[0]+K[1])/2.0;

            // Z
            K[0] = h * (-c*prev_z+prev_x*prev_y);
            K[1] = h * (-c*(prev_z+K[0])+(prev_x+K[0])*(prev_y+K[0]));
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
                K[0] = h* (a*(prev_y-prev_x));
                K[1] = h* (a*((prev_y+0.5*K[0])-(prev_x+0.5*K[0])));
                K[2] = h* (a*((prev_y+0.5*K[1])-(prev_x+0.5*K[1])));
                K[3] = h* (a*((prev_y+K[2])-(prev_x+K[2])));
                x = prev_x + (K[0] + 2.0*K[1] + 2.0*K[2] + K[3])/6.0;

                // y
                K[0] = h* (b*prev_x-prev_y-prev_x*prev_z);
                K[1] = h* (b*(prev_x+0.5*K[0])-(prev_y+0.5*K[0])-(prev_x+0.5*K[0])*(prev_z+0.5*K[0]));
                K[2] = h* (b*(prev_x+0.5*K[1])-(prev_y+0.5*K[1])-(prev_x+0.5*K[1])*(prev_z+0.5*K[1]));
                K[3] = h* (b*(prev_x+K[2])-(prev_y+K[2])-(prev_x+K[2])*(prev_z+K[2]));
                y = prev_y + (K[0] + 2.0*K[1] + 2.0*K[2] + K[3])/6.0;

                // Z
                K[0] = h* (-c*prev_z+prev_x*prev_y);
                K[1] = h* (-c*(prev_z+0.5*K[0])+(prev_x+0.5*K[0])*(prev_y+0.5*K[0]));
                K[2] = h* (-c*(prev_z+0.5*K[1])+(prev_x+0.5*K[1])*(prev_y+0.5*K[1]));
                K[3] = h* (-c*(prev_z+K[2])+(prev_x+K[2])*(prev_y+K[2]));
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

                x   = x_1+h*((1901.0/720.0*(a*(y_1-x_1))) -
                             (1387.0/360.0 *(a*(y_2-x_2))) +
                             (109.0/30.0   *(a*(y_3-x_3)))   -
                             (637.0/360.0  *(a*(y_4-x_4)))  +
                             (251.0/720.0  *(a*(y_5-x_5))));

                y   = y_1+h*((1901.0/720.0*(b*x_1-y_1-x_1*z_1)) -
                             (1387.0/360.0 *(b*x_2-y_2-x_2*z_2)) +
                             (109.0/30.0   *(b*x_3-y_3-x_3*z_3))   -
                             (637.0/360.0  *(b*x_4-y_4-x_4*z_4))  +
                             (251.0/720.0  *(b*x_5-y_5-x_5*z_5)));

                z   = z_1 +h*((1901.0/720.0 *(-c*z_1+x_1*y_1)) -
                              (1387.0/360.0 *(-c*z_2+x_2*y_2)) +
                              (109.0/30.0   *(-c*z_3+x_3*y_3)) -
                              (637.0/360.0  *(-c*z_4+x_4*y_4)) +
                              (251.0/720.0  *(-c*z_5+x_5*y_5)));
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

QPixmap ResslerModel::getFormulaPixmap()
{
    return QPixmap(":/formula.png");
}

QList<StabilityPoint> ResslerModel::getEquilibriumPoints()
{
    QList<StabilityPoint> stab;
    stab.append(StabilityPoint(
                QList<qreal>()  << (c + sqrt(c*c - 4*a*b)) / 2
                                << (-c - sqrt(c*c - 4*a*b)) / (2*a)
                                << (c + sqrt(c*c - 4*a*b)) / (2*a), ""));
    stab.append(StabilityPoint(
                QList<qreal>()  << (c - sqrt(c*c - 4*a*b)) / 2
                                << (-c + sqrt(c*c - 4*a*b)) / (2*a)
                                << (c - sqrt(c*c - 4*a*b)) / (2*a), ""));
    return stab;
}

QList<PointComplex> ResslerModel::getEigenvalues()
{
    QList<PointComplex> eig;

//    eig.append(QList<Complex>() <<
//               Complex(0, (a+sqrt(a*a - 4))/2));
//    eig.append(QList<Complex>() <<
//               Complex(0, (a-sqrt(a*a - 4))/2));
    return eig;
}

Point ResslerModel::getStartValues()
{
    return startValues;
}

QString ResslerModel::resolveLambdas(const PointComplex &complex)
{
    return "";
}
