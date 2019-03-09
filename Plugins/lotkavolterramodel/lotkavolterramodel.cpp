#include "lotkavolterramodel.h"
#include <QDebug>
#include <cmath>

LotkaVolterraModel::LotkaVolterraModel(QObject *parent) :
    QObject(parent)
{
    qDebug() << ">> Lotka-Volterra model has been loaded";
}

LotkaVolterraModel::~LotkaVolterraModel()
{
    qDebug() << "<< Lotka-Volterra model has been destroyed";
}

QList<Element> LotkaVolterraModel::differentiate(const DiffSettings &settings)
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
    alpha   = settings.data.at(0),  // Коэф. рожд. жертв
    beta    = settings.data.at(1),  // Коэф. убийства ж-вы х-ками
    delta   = settings.data.at(2),  // Коэф. рожд. х-ков
    gamma   = settings.data.at(3);  // Коэф. убыли х-ков

    // Количества жертв и хищников
    qreal   prev_x  = settings.startValues.at(0),
            prev_y  = settings.startValues.at(1);
    // Новые x и y
    qreal   x,
            y;

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
            x = prev_x + h*((alpha-beta*prev_y)*prev_x);
            y = prev_y + h*((-gamma+delta*prev_x)*prev_y);
        }
        if (method == DiffMethod::ModifiedEilers)
        {
            double K[2];
            // X
            K[0] = h * ((alpha-beta*prev_y)*prev_x);
            K[1] = h * ((alpha-beta*(prev_y+K[0]))*(prev_x+K[0]));
            x = prev_x + (K[0]+K[1])/2.0;

            // Y
            K[0] = h * ((-gamma+delta*prev_x)*prev_y);
            K[1] = h * ((-gamma+delta*(prev_x+K[0]))*(prev_y+K[0]));
            y = prev_y + (K[0]+K[1])/2.0;
        }
        // Для нахождения первых 5-х [0;5) точек для Адамса
        // Используется метод Р-К. Для избежания копипасты
        if (    method == DiffMethod::RungeKutta4thOrder ||
                method == DiffMethod::AdamsBashforth4rdOrder)
        {
            if ((iteration < 5) || (method == DiffMethod::RungeKutta4thOrder))
            {
                qreal K[4];
                // X
                K[0] = h* ((alpha-beta*prev_y)*prev_x);
                K[1] = h* ((alpha-beta*(prev_y + K[0]/2.0))  * (prev_x + K[0]/2.0));
                K[2] = h* ((alpha-beta*(prev_y + K[1]/2.0))  * (prev_x + K[1]/2.0));
                K[3] = h* ((alpha-beta*(prev_y + K[2]))    * (prev_x + K[2]));
                x = prev_x + ((K[0] + 2.0*K[1] + 2.0*K[2] + K[3]))/6.0;

                // Y
                K[0] = h* ((-gamma + delta*prev_x)*prev_y);
                K[1] = h* ((-gamma + delta*(prev_x + K[0]/2.0))  * (prev_y + K[0]/2.0));
                K[2] = h* ((-gamma + delta*(prev_x + K[1]/2.0))  * (prev_y + K[1]/2.0));
                K[3] = h* ((-gamma + delta*(prev_x + K[2]))  * (prev_y + K[2]));
                y   = prev_y + ((K[0] + 2.0*K[1] + 2.0*K[2] + K[3]))/6.0;
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

                x   = x_1 +h*((1901.0/720.0*(alpha-beta*y_1)*x_1) -
                              (1387.0/360.0*(alpha-beta*y_2)*x_2) +
                              (109.0/30.0*(alpha-beta*y_3)*x_3)   -
                              (637.0/360.0*(alpha-beta*y_4)*x_4)  +
                              (251.0/720.0*(alpha-beta*y_5)*x_5));

                y   = y_1 +h*((1901.0/720.0*(-gamma*y_1+delta*x_1*y_1)) -
                              (1387.0/360.0*(-gamma*y_2+delta*x_2*y_2)) +
                              (109.0/30.0*(-gamma*y_3+delta*x_3*y_3)) -
                              (637.0/360.0*(-gamma*y_4+delta*x_4*y_4)) +
                              (251.0/720.0*(-gamma*y_5+delta*x_5*y_5)));
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

QPixmap LotkaVolterraModel::getFormulaPixmap()
{
    QPixmap px = QPixmap(":/formula/img/lotkavolterra.png");
    return px;
}

QList<Point> LotkaVolterraModel::getEquilibriumPoints()
{
    QList<Point> out;
    out.append(QList<qreal>()
               << 0.0f << 0.0f);
    out.append(QList<qreal>()
               << gamma/delta << alpha/beta);
    return out;
}

QList<Point> LotkaVolterraModel::getEigenvalues()
{
    QList<Point> out;
    out.append(QList<qreal>()
               << alpha << -gamma);
    out.append(QList<qreal>() <<
               sqrt(alpha*gamma) << -sqrt(alpha*gamma));
    return out;
}

QString LotkaVolterraModel::getEigenvaluesSolve()
{
    return QString("\/Тут показывается процесс поисказ значений. Это вбито в саму модель\/");
}

Point LotkaVolterraModel::getStartValues()
{
    return startValues;
}

