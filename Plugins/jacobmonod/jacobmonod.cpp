#include "jacobmonod.h"


JacobMonod::JacobMonod(QObject *parent) :
    QObject(parent)
{
    qDebug() << ">> Jacob-Monod model has been loaded";
}

JacobMonod::~JacobMonod()
{
    qDebug() << "<< Jacob-Monod model has been destroyed";
}

QList<Element> JacobMonod::differentiate(const DiffSettings &settings)
{
    Q_ASSERT(settings.startValues.size() == settings.expressions);

    qreal   start = settings.startTime,
            end = settings.endTime,
            h = settings.step;
    QList<Element> out;

    // Метод интегрирования
    DiffMethod  method = settings.diffMethod;

    // Коэффицициенты
    qreal   V   = settings.data.at(0),  // V - Скорость поглощения
            K   = settings.data.at(1),  // K - Постоянная насыщения
            Y   = settings.data.at(2);  // Y - Доходность x на единицу y

    // Количества жертв и хищников
    qreal   prev_x  = settings.startValues.at(0),
            prev_y  = settings.startValues.at(1);

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
            x = prev_x + h*(V*prev_y/(K+prev_y)*prev_x);
            y = prev_y + h*(-1.0/Y*(V*prev_y)/(K+prev_y)*prev_x);
        }
        if (method == DiffMethod::ModifiedEilers)
        {
            double K_[2];
            // X
            K_[0] = h * (V*prev_y/(K+prev_y)*prev_x);
            K_[1] = h * (V*(prev_y+K_[0])/(K+(prev_y+K_[0]))*(prev_x+K_[0]));
            x = prev_x + (K_[0]+K_[1])/2.0;

            // Y
            K_[0] = h * (-1.0/Y*(V*prev_y)/(K+prev_y)*prev_x);
            K_[1] = h * (-1.0/Y*(V*(prev_y+K_[0]))/(K+(prev_y+K_[0]))*(prev_x+K_[0]));
            y = prev_y + (K_[0]+K_[1])/2.0;
        }
        // Для нахождения первых 5-х [0;5) точек для Адамса
        // Используется метод Р-К. Для избежания копипасты
        if (    method == DiffMethod::RungeKutta4thOrder ||
                method == DiffMethod::AdamsBashforth4rdOrder)
        {
            if ((iteration < 5) || (method == DiffMethod::RungeKutta4thOrder))
            {

                qreal K_[4];
                // X
                K_[0] = h* (V*prev_y/(K+prev_y)*prev_x);
                K_[1] = h* (V*(prev_y+0.5*K_[0])/(K+(prev_y+0.5*K_[0]))*(prev_x+0.5*K_[0]));
                K_[2] = h* (V*(prev_y+0.5*K_[1])/(K+(prev_y+0.5*K_[1]))*(prev_x+0.5*K_[1]));
                K_[1] = h* (V*(prev_y+K_[2])/(K+(prev_y+K_[2]))*(prev_x+K_[2]));
                x = prev_x + (K_[0] + 2.0*K_[1] + 2.0*K_[2] + K_[3])/6.0;

                // Y
                K_[0] = h* (-1.0/Y*(V*prev_y)/(K+prev_y)*prev_x);
                K_[1] = h* (-1.0/Y*(V*(prev_y+0.5*K_[0]))/(K+(prev_y+0.5*K_[0]))*(prev_x+0.5*K_[0]));
                K_[2] = h* (-1.0/Y*(V*(prev_y+0.5*K_[1]))/(K+(prev_y+0.5*K_[1]))*(prev_x+0.5*K_[1]));
                K_[3] = h* (-1.0/Y*(V*(prev_y+K_[2]))/(K+(prev_y+K_[2]))*(prev_x+K_[2]));
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

                x   = x_1 +h*((1901.0/720.0 *(V*y_1/(K+y_1)*x_1)) -
                              (1387.0/360.0 *(V*y_1/(K+y_1)*x_1)) +
                              (109.0/30.0   *(V*y_1/(K+y_1)*x_1)) -
                              (637.0/360.0  *(V*y_1/(K+y_1)*x_1)) +
                              (251.0/720.0  *(V*y_1/(K+y_1)*x_1)));

                y   = y_1 +h*((1901.0/720.0 *(-1.0/Y*(V*y_1)/(K+y_1)*x_1)) -
                              (1387.0/360.0 *(-1.0/Y*(V*y_2)/(K+y_2)*x_2)) +
                              (109.0/30.0   *(-1.0/Y*(V*y_3)/(K+y_3)*x_3)) -
                              (637.0/360.0  *(-1.0/Y*(V*y_4)/(K+y_4)*x_4)) +
                              (251.0/720.0  *(-1.0/Y*(V*y_5)/(K+y_5)*x_5)));
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
