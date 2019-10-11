#ifndef VOLTERRA2VICTIMS_H
#define VOLTERRA2VICTIMS_H

#include "../idynmodelplugin.h"

class Volterra2Victims : public QObject, public IDynModelPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IDynModelPlugin_iid FILE "volterra2victims.json")
    Q_INTERFACES(IDynModelPlugin)
#endif // QT_VERSION >= 0x050000

public:
    explicit Volterra2Victims(QObject *parent = 0);
    virtual ~Volterra2Victims();

    // IDynModelPlugin interface
public:
    QList<Element> differentiate(const DiffSettings &settings);

    // IDynModelPlugin interface
public:
    QPixmap getFormulaPixmap();

private:
    qreal a1;// a1 - Рождаемость жертв1
    qreal a2;  // a2 - Рождаемость жертв2
    qreal b1;  // b1 - Коэф убийства жертвы1 хищником
    qreal b2;  // b2 - Коэф убийства жертвы2 хищником
    qreal c;  // c  - Смертность хищника
    qreal d1;  // d1 - Рождаемость хищников?
    qreal d2;

    // IDynModelPlugin interface
public:
    QList<StabilityPoint> getEquilibriumPoints();
    QList<PointComplex> getEigenvalues();
    Point getStartValues();

    // IDynModelPlugin interface
public:
    QString resolveLambdas(const PointComplex &complex);
};

#endif // VOLTERRA2VICTIMS_H
