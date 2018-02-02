#include "volterra2victims.h"


Volterra2Victims::Volterra2Victims(QObject *parent) :
    QObject(parent)
{
    qDebug() << ">> Volterra for 2 victims model has been loaded";
}

Volterra2Victims::~Volterra2Victims()
{
    qDebug() << "<< Volterra for 2 victims model has been destroyed";
}

QList<Element> Volterra2Victims::differentiate(const DiffSettings &settings)
{

    qreal   start = settings.startTime,
            end = settings.endTime,
            h = settings.step;
    QList<Element> out;

    // Метод интегрирования
    DiffMethod  method = settings.diffMethod;

    /// TODO: Разобраться с коэффициентами
    // Коэффицициенты
    qreal   a1  = settings.data.at(0),  // a1 - Рождаемость жертв1
            a2  = settings.data.at(1),  // a2 - Рождаемость жертв2
            b1  = settings.data.at(2),  // b1 - Коэф убийства жертвы1 хищником
            b2  = settings.data.at(3),  // b2 - Коэф убийства жертвы2 хищником
            c   = settings.data.at(4),  // c  - Смертность хищника
            d1  = settings.data.at(5),  // d1 - Рождаемость хищников?
            d2  = settings.data.at(6);  // d2 - Рождаемость хищников?

    // Количества жертв и хищников
    qreal   prev_x1 = settings.startValues.at(0),
            prev_x2 = settings.startValues.at(1),
            prev_y  = settings.startValues.at(2);

    qreal   x1,
            x2,
            y;

    quint32 iteration = 1; // итерация
    // Первый элемент добавляем в выход
    out << QPair<qreal, QVector<qreal>>(0.0, {prev_x1, prev_x2, prev_y});
    // Просчет для каждого шага
    for (qreal t = start+h; t <= end; t += h)
    {
        // текущие значения для каждого выражения на шаге h
        Element element;
        if (method == DiffMethod::Eilers)
        {
            x1  = prev_x1 + h*(a1*prev_x1-b1*prev_x1*prev_y);
            x2  = prev_x2 + h*(a2*prev_x2-b2*prev_x2*prev_y);
            y   = prev_y  + h*(-c*prev_y+d1*prev_x1*prev_y+d2*prev_x2*prev_y);
        }
        if (method == DiffMethod::ModifiedEilers)
        {
            double K[2];
            // X1
            K[0] = h * (a1*prev_x1-b1*prev_x1*prev_y);
            K[1] = h * (a1*(prev_x1+K[0])-b1*(prev_x1+K[0])*(prev_y+K[0]));
            x1 = prev_x1 + (K[0]+K[1])/2.0;

            // X2
            K[0] = h * (a2*prev_x2-b2*prev_x2*prev_y);
            K[1] = h * (a2*(prev_x2+K[0])-b2*(prev_x2+K[0])*(prev_y+K[0]));
            x2 = prev_x2 + (K[0]+K[1])/2.0;

            // Y
            K[0] = h * (-c*prev_y+d1*prev_x1*prev_y+d2*prev_x2*prev_y);
            K[1] = h * (-c*(prev_y+K[0])+d1*(prev_x1+K[0])*(prev_y+K[0])+d2*(prev_x2+K[0])*(prev_y+K[0]));
            y = prev_y + (K[0]+K[1])/2.0;
        }
        // Для нахождения первых 5-х [0;5) точек для Адамса
        // Используется метод Р-К. Для избежания копипасты
        if (    method == DiffMethod::RungeKutta4thOrder ||
                method == DiffMethod::AdamsBashforth4rdOrder)        {
            if ((iteration < 5) || (method == DiffMethod::RungeKutta4thOrder))
            {
                qreal K[4];
                // X1
                K[0] = h* (a1*prev_x1-b1*prev_x1*prev_y);
                K[1] = h* (a1*(prev_x1+0.5*K[0])-b1*(prev_x1+0.5*K[0])*(prev_y+0.5*K[0]));
                K[2] = h* (a1*(prev_x1+0.5*K[1])-b1*(prev_x1+0.5*K[1])*(prev_y+0.5*K[1]));
                K[3] = h* (a1*(prev_x1+K[2])-b1*(prev_x1+K[2])*(prev_y+K[2]));
                x1 = prev_x1 + (K[0] + 2.0*K[1] + 2.0*K[2] + K[3])/6.0;

                // X2
                K[0] = h* (a2*prev_x2-b2*prev_x2*prev_y);
                K[1] = h* (a2*(prev_x2+0.5*K[0])-b2*(prev_x2+0.5*K[0])*(prev_y+0.5*K[0]));
                K[2] = h* (a2*(prev_x2+0.5*K[1])-b2*(prev_x2+0.5*K[1])*(prev_y+0.5*K[1]));
                K[3] = h* (a2*(prev_x2+K[2])-b2*(prev_x2+K[2])*(prev_y+K[2]));
                x2 = prev_x2 + (K[0] + 2.0*K[1] + 2.0*K[2] + K[3])/6.0;

                // Y
                K[0] = h* (-c*prev_y+d1*prev_x1*prev_y+d2*prev_x2*prev_y);
                K[1] = h* (-c*(prev_y+0.5*K[0])+d1*(prev_x1+0.5*K[0])*(prev_y+0.5*K[0])+d2*(prev_x2+0.5*K[0])*(prev_y+0.5*K[0]));
                K[2] = h* (-c*(prev_y+0.5*K[1])+d1*(prev_x1+0.5*K[1])*(prev_y+0.5*K[1])+d2*(prev_x2+0.5*K[1])*(prev_y+0.5*K[1]));
                K[3] = h* (-c*(prev_y+K[2])+d1*(prev_x1+K[2])*(prev_y+K[2])+d2*(prev_x2+K[2])*(prev_y+K[2]));
                y   = prev_y + (K[0] + 2.0*K[1] + 2.0*K[2] + K[3])/6.0;
            }
        }
        if (method == DiffMethod::AdamsBashforth4rdOrder)
        {
            // После просчета первых 5-х точек идет метод Адамса
            if (iteration >= 5)
            {
                qreal   x1_5 = out.at(iteration-5).second.at(0), // X1n-5
                        x1_4 = out.at(iteration-4).second.at(0), // X1n-4
                        x1_3 = out.at(iteration-3).second.at(0), // X1n-3
                        x1_2 = out.at(iteration-2).second.at(0), // X1n-2
                        x1_1 = out.at(iteration-1).second.at(0), // X1n-1
                        x2_5 = out.at(iteration-5).second.at(1), // X2n-5
                        x2_4 = out.at(iteration-4).second.at(1), // X2n-4
                        x2_3 = out.at(iteration-3).second.at(1), // X2n-3
                        x2_2 = out.at(iteration-2).second.at(1), // X2n-2
                        x2_1 = out.at(iteration-1).second.at(1), // X2n-1
                        y_5 = out.at(iteration-5).second.at(2),  // Yn-5
                        y_4 = out.at(iteration-4).second.at(2),  // Yn-4
                        y_3 = out.at(iteration-3).second.at(2),  // Yn-3
                        y_2 = out.at(iteration-2).second.at(2),  // Yn-2
                        y_1 = out.at(iteration-1).second.at(2);  // Yn-1

                x1   = x1_1+h*((1901.0/720.0*(a1*x1_1-b1*x1_1*y_1)) -
                              (1387.0/360.0 *(a1*x1_2-b1*x1_2*y_2)) +
                              (109.0/30.0   *(a1*x1_3-b1*x1_3*y_3))   -
                              (637.0/360.0  *(a1*x1_4-b1*x1_4*y_4))  +
                              (251.0/720.0  *(a1*x1_5-b1*x1_5*y_5)));

                x2   = x2_1+h*((1901.0/720.0*(a2*x2_1-b2*x2_1*y_1)) -
                              (1387.0/360.0 *(a2*x2_2-b2*x2_2*y_2)) +
                              (109.0/30.0   *(a2*x2_3-b2*x2_3*y_3))   -
                              (637.0/360.0  *(a2*x2_4-b2*x2_4*y_4))  +
                              (251.0/720.0  *(a2*x2_5-b2*x2_5*y_5)));

                y   = y_1 +h*((1901.0/720.0 *(-c*y_1+d1*x1_1*y_1+d2*x2_1*y_1)) -
                              (1387.0/360.0 *(-c*y_2+d1*x1_2*y_1+d2*x2_2*y_2)) +
                              (109.0/30.0   *(-c*y_3+d1*x1_3*y_1+d2*x2_3*y_3)) -
                              (637.0/360.0  *(-c*y_4+d1*x1_4*y_1+d2*x2_4*y_4)) +
                              (251.0/720.0  *(-c*y_5+d1*x1_5*y_1+d2*x2_5*y_5)));
            }
        }
        prev_x1 = x1;
        prev_x2 = x2;
        prev_y  = y;

        // Добавляем к выходной последовательности
        element.first = t;  // Текущее время t
        element.second << x1 << x2 << y; // Значения популяции
        out << element;
        iteration++;
    }
    return out;
}
