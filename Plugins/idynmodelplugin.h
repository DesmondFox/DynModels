#ifndef IDYNMODELPLUGIN_H
#define IDYNMODELPLUGIN_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QDebug>
#include <QPixmap>

// Метод дифференциирования
enum DiffMethod
{
    Eilers,
    ModifiedEilers,
    RungeKutta4thOrder,
    AdamsBashforth4rdOrder
};

struct DiffSettings
{
    // Начальные значения хищников, жертв,
    // прочих существ в системе
    QList<qreal>  startValues;
    // Начальное время
    qreal   startTime;
    // Конечное время
    qreal   endTime;
    // Шаг интегрирования
    qreal   step;
    // Метод интегрирования
    DiffMethod  diffMethod;

    // Коэффициенты системы
    // Задаются в соответствии с метаданными
    QList<qreal>    data;

};

// Алиас для указателя на функцию интегрирования. Для удобства
// Также передача предыдущих значений
//using DiffFunc = qreal(*)(const qreal, const QVector<qreal>);

// Один элемент из выхода. Равное числу популяции в данный момент t
typedef QPair<qreal, QVector<qreal>> Element;

// Point
typedef QList<qreal> Point;

struct StabilityPoint {
    StabilityPoint() {}

    StabilityPoint(Point p, QString formula) {
        this->point = p;
        this->formula = formula;
    }

    Point point;
    QString formula;
};

struct Complex
{
    Complex(qreal a, qreal l)
    {alpha = a; lambda = l; }
    qreal alpha;
    qreal lambda;
};

typedef QList<Complex> PointComplex;


class IDynModelPlugin
{
public:
    virtual QList<Element> differentiate(const DiffSettings &) = 0;
    virtual QPixmap getFormulaPixmap() = 0;
    // Точки равновесия
    virtual QList<StabilityPoint> getEquilibriumPoints() = 0;
    // Собственные значения
    virtual QList<PointComplex> getEigenvalues() = 0;
//    virtual ~IDynModelPlugin() = 0;
    // Показать как находятся собств. значения
    virtual QString getEigenvaluesSolve() = 0;
    // Начальные значения популяции
    virtual Point getStartValues() = 0;
};

#define IDynModelPlugin_iid "com.github.desmondfox.idynmodelplugin"
Q_DECLARE_INTERFACE(IDynModelPlugin, IDynModelPlugin_iid)

#endif // IDYNMODELPLUGIN_H
