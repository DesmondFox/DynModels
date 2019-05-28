#ifndef HOLLINGTANNER_H
#define HOLLINGTANNER_H

#include "../idynmodelplugin.h"

class HollingTanner : public QObject, public IDynModelPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IDynModelPlugin_iid FILE "holingtanner.json")
    Q_INTERFACES(IDynModelPlugin)
#endif // QT_VERSION >= 0x050000

public:

    explicit HollingTanner(QObject *parent = 0);
    virtual ~HollingTanner();

    // IDynModelPlugin interface
public:
    QList<Element> differentiate(const DiffSettings &settings);
    QPixmap getFormulaPixmap();

private:
    Point startValues;
    qreal r;  // r
    qreal s; // s
    qreal K;
    qreal D;
    qreal J;
    qreal w;
    // IDynModelPlugin interface
public:
    QList<StabilityPoint> getEquilibriumPoints();
    QList<PointComplex> getEigenvalues();
    Point getStartValues();

    // IDynModelPlugin interface
public:
    QString resolveLambdas(const PointComplex &complex);
};

#endif // HOLLINGTANNER_H
