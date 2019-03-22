#ifndef VERHULSTMODEL_H
#define VERHULSTMODEL_H

#include "../idynmodelplugin.h"


class VerhulstModel : public QObject, public IDynModelPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IDynModelPlugin_iid FILE "verhulst.json")
    Q_INTERFACES(IDynModelPlugin)
#endif // QT_VERSION >= 0x050000

public:
    VerhulstModel(QObject *parent = 0);

private:
    qreal r;    // Коэф. r
    qreal k;     // Коэф. убийства ж-вы х-ками
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

#endif // VERHULSTMODEL_H
