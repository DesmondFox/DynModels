#ifndef LorenzModel_H
#define LorenzModel_H

#include "../idynmodelplugin.h"


class LorenzModel : public QObject, public IDynModelPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IDynModelPlugin_iid FILE "lorenz.json")
    Q_INTERFACES(IDynModelPlugin)
#endif // QT_VERSION >= 0x050000

public:
    LorenzModel(QObject *parent = 0);

private:
    qreal a;    // Коэф. r
    qreal b;     // Коэф. убийства ж-вы х-ками
    qreal c;
    Point startValues;

    // IDynModelPlugin interface
public:
    QList<Element> differentiate(const DiffSettings &settings);
    QPixmap getFormulaPixmap();
    QList<StabilityPoint> getEquilibriumPoints();
    QList<PointComplex> getEigenvalues();
    QString getEigenvaluesSolve();
    Point getStartValues();
};

#endif // LorenzModel_H
