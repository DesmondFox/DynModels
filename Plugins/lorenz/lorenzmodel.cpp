#include "LorenzModel.h"


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
            x = prev_x + h*(a*(prev_y-prev_x));
            y = prev_y + h*(b*prev_x-prev_y-prev_x*prev_z);
            z = prev_z + h*(-c*prev_z+prev_x*prev_y);
        }
        if (method == DiffMethod::ModifiedEilers)
        {
            double K[2];

//            // X
//            K[0] = h * (r*prev_x*(1-prev_x/k));
//            K[1] = h * (r*(prev_x+K[0])*(1-(prev_x+K[0])/k));

//            x = prev_x + (K[0]+K[1])/2.0;

        }
        // Для нахождения первых 5-х [0;5) точек для Адамса
        // Используется метод Р-К. Для избежания копипасты
        if (    method == DiffMethod::RungeKutta4thOrder ||
                method == DiffMethod::AdamsBashforth4rdOrder)
        {
            if ((iteration < 5) || (method == DiffMethod::RungeKutta4thOrder))
            {


//                qreal K[4];

//                K[0] = h* (r*prev_x*(1-prev_x/k));
//                K[1] = h* (r*(prev_x + K[0]/2.0)*(1-(prev_x + K[0]/2.0)/k));
//                K[2] = h* (r*(prev_x + K[1]/2.0)*(1-(prev_x + K[1]/2.0)/k));
//                K[3] = h* (r*(prev_x + K[2])*(1-(prev_x + K[2])/k));
//                x = prev_x + ((K[0] + 2.0*K[1] + 2.0*K[2] + K[3]))/6.0;
            }
        }
        if (method == DiffMethod::AdamsBashforth4rdOrder)
        {
            // После просчета первых 5-х точек идет метод Адамса
            if (iteration >= 5)
            {
//                qreal   x_5 = out.at(iteration-5).second.at(0), // Xn-5
//                        x_4 = out.at(iteration-4).second.at(0), // Xn-4
//                        x_3 = out.at(iteration-3).second.at(0), // Xn-3
//                        x_2 = out.at(iteration-2).second.at(0), // Xn-2
//                        x_1 = out.at(iteration-1).second.at(0);

//                x   = x_1 +h*((1901.0/720.0*(r*x_1*(1-x_1/k))) -
//                              (1387.0/360.0*(r*x_1*(1-x_2/k))) +
//                              (109.0/30.0*(r*x_1*(1-x_3/k)))  -
//                              (637.0/360.0*(r*x_1*(1-x_4/k))) +
//                              (251.0/720.0*(r*x_1*(1-x_5/k))));
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
    return QPixmap();
}

QList<StabilityPoint> LorenzModel::getEquilibriumPoints()
{
    return QList<StabilityPoint>();
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
