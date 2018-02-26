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

class IDynModelPlugin
{
public:
    virtual QList<Element> differentiate(const DiffSettings &) = 0;
    virtual QPixmap getFormulaPixmap() = 0;
//    virtual ~IDynModelPlugin() = 0;
};

#define IDynModelPlugin_iid "com.github.desmondfox.idynmodelplugin"
Q_DECLARE_INTERFACE(IDynModelPlugin, IDynModelPlugin_iid)

#endif // IDYNMODELPLUGIN_H
