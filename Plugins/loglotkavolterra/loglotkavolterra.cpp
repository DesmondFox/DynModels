#include "loglotkavolterra.h"
#include <cmath>

LogLotkaVolterra::LogLotkaVolterra(QObject *parent) :
    QObject(parent)
{
    qDebug() << ">> Logistic Lotka-Volterra model has been loaded";
}

LogLotkaVolterra::~LogLotkaVolterra()
{
    qDebug() << "<< Logistic Lotka-Volterra model has been destroyed";
}

QList<Element> LogLotkaVolterra::differentiate(const DiffSettings &settings)
{
    qreal   start = settings.startTime,
            end = settings.endTime,
            h = settings.step;
    QList<Element> out;

    // Метод интегрирования
    DiffMethod  method = settings.diffMethod;

    // Коэффицициенты
    qreal   a   = settings.data.at(0),  // a - Коэф. рожд. жертв
            b   = settings.data.at(1),  // b - Коэф. убийства ж-вы х-ками
            c   = settings.data.at(2),  // c - Коэф. рожд. х-ков
            d   = settings.data.at(3),  // d - Коэф. убыли х-ков
            alpha   = settings.data.at(4); // alp - Коэф. затухания

    // Количества жертв и хищников
    qreal   prev_x  = settings.startValues.at(0),
            prev_y  = settings.startValues.at(1);

//    qreal   y0  = settings.startValues(1);

    // Новые x и y
    qreal   x,
            y;

    quint32 iteration = 1; // итерация
    // Первый элемент добавляем в выход
    out << QPair<qreal, QVector<qreal>>(0.0, {prev_x, prev_y});
    // Просчет для каждого шага
    for (qreal t = start+h; t <= end; t += h)
    {
        // текущие значения для каждого выражения на шаге h
        Element element;
        if (method == DiffMethod::Eilers)
        {
            x = prev_x + h*((a-b*prev_y)*prev_y-alpha*pow(prev_x, 2.0));
            y = prev_y + h*((-c+d*prev_x)*prev_y-alpha*pow(prev_y, 2.0));
        }
        if (method == DiffMethod::ModifiedEilers)
        {
            double K[2];
            // X
            K[0] = h * ((a-b*prev_y)*prev_y-alpha*(prev_x*prev_x));
            K[1] = h * ((a-b*(prev_y+K[0]))*(prev_y+K[0])-alpha*pow(prev_x+K[0], 2.0));
            x = prev_x + (K[0]+K[1])/2.0;

            // Y
            K[0] = h * ((-c+d*prev_x)*prev_y-alpha*(prev_y*prev_y));
            K[1] = h * ((-c+d*(prev_x+K[0]))*(prev_y+K[0])-alpha*pow(prev_y+K[0], 2.0));
            y = prev_y + (K[0]+K[1])/2.0;
        }
        if (    method == DiffMethod::RungeKutta4thOrder ||
                method == DiffMethod::AdamsBashforth4rdOrder)
        {
            if ((iteration < 5) || (method == DiffMethod::RungeKutta4thOrder))
            {
                qreal K[4];
                // X
                K[0] = h* ((a-b*prev_y)*prev_y-alpha*pow(prev_x, 2.0));
                K[1] = h* ((a-b*(prev_y + 0.5*K[0]))*(prev_y + 0.5*K[0])-alpha*pow(prev_x + 0.5*K[0], 2.0));
                K[2] = h* ((a-b*(prev_y + 0.5*K[1]))*(prev_y + 0.5*K[1])-alpha*pow(prev_x + 0.5*K[1], 2.0));
                K[3] = h* ((a-b*(prev_y + K[2]))*(prev_y + K[2])-alpha*pow(prev_x + K[2], 2.0));
                x = prev_x + (K[0] + 2.0*K[1] + 2.0*K[2] + K[3])/6.0;

                // Y
                K[0] = h* ((-c+d*prev_x)*prev_y-alpha*pow(prev_y, 2.0));
                K[1] = h* ((-c+d*(prev_x + 0.5*K[0]))*(prev_y + 0.5*K[0]) - alpha*pow(prev_y + 0.5*K[0], 2.0));
                K[2] = h* ((-c+d*(prev_x + 0.5*K[1]))*(prev_y + 0.5*K[1]) - alpha*pow(prev_y + 0.5*K[1], 2.0));
                K[3] = h* ((-c+d*(prev_x + K[2]))*(prev_y + K[2]) - alpha*pow(prev_y + K[2], 2.0));
                y   = prev_y + (K[0] + 2.0*K[1] + 2.0*K[2] + K[3])/6.0;
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

                x   = x_1 +h*((1901.0/720.0 *((a-b*y_1)*y_1-alpha*pow(x_1, 2.0))) -
                              (1387.0/360.0 *((a-b*y_2)*y_2-alpha*pow(x_2, 2.0))) +
                              (109.0/30.0   *((a-b*y_3)*y_3-alpha*pow(x_3, 2.0)))   -
                              (637.0/360.0  *((a-b*y_4)*y_4-alpha*pow(x_4, 2.0)))  +
                              (251.0/720.0  *((a-b*y_5)*y_5-alpha*pow(x_5, 2.0))));

                y   = y_1 +h*((1901.0/720.0 *((-c+d*x_1)*y_1-alpha*pow(y_1, 2.0))) -
                              (1387.0/360.0 *((-c+d*x_2)*y_2-alpha*pow(y_2, 2.0))) +
                              (109.0/30.0   *((-c+d*x_3)*y_3-alpha*pow(y_3, 2.0))) -
                              (637.0/360.0  *((-c+d*x_4)*y_4-alpha*pow(y_4, 2.0))) +
                              (251.0/720.0  *((-c+d*x_5)*y_5-alpha*pow(y_5, 2.0))));
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

