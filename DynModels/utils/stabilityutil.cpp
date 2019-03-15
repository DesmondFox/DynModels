#include "stabilityutil.h"

StabilityUtil::StabilityUtil()
{

}

QString StabilityUtil::resolveLambda(const qreal lam1, const qreal lam2)
{
    if ((lam1 < lam2) && (lam2 < 0.0f))
        return "Устойчивый несобственный узел";
    if (qFuzzyCompare(lam1, lam2) && (lam2 < 0.0f))
        return "Устойчивый узел или фокус";
    if ((lam1 < 0.0f) && (0.0f < lam2))
        return "Неустйчивая седловая точка";
    if (qFuzzyCompare(lam1, lam2) && (lam2 > 0.0f))
        return "Неустойчивый узел или фокус";
    if ((lam1 > lam2) && (lam2 > 0.0f))
        return "Неустойчивый несобственный узел";
    // TODO: Остальные уловия
    return "TODO";
}
