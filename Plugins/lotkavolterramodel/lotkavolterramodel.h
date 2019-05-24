#ifndef LOTKAVOLTERRAMODEL_H
#define LOTKAVOLTERRAMODEL_H

#include "../idynmodelplugin.h"

class LotkaVolterraModel : public QObject, public IDynModelPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IDynModelPlugin_iid FILE "lotkavolterramodel.json")
    Q_INTERFACES(IDynModelPlugin)
#endif // QT_VERSION >= 0x050000

public:
    explicit LotkaVolterraModel(QObject *parent = 0);
    virtual ~LotkaVolterraModel();
    // IDynModelPlugin interface
public:
    QList<Element> differentiate(const DiffSettings &settings);
    QPixmap getFormulaPixmap();

private:
    qreal alpha;    // Коэф. рожд. жертв
    qreal beta;     // Коэф. убийства ж-вы х-ками
    qreal gamma;    // Коэф. убыли х-ков
    qreal delta;    // Коэф. рожд. х-ков
    Point startValues;

    // IDynModelPlugin interface
public:
    QList<StabilityPoint> getEquilibriumPoints();
    QList<PointComplex> getEigenvalues();
    Point getStartValues();

    QString resolveLambdas(const PointComplex &complex);
};

#endif // LOTKAVOLTERRAMODEL_H
