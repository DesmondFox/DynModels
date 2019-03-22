#include "stabilityutil.h"

StabilityUtil::StabilityUtil()
{

}

QString StabilityUtil::resolveLambda(const Complex l1, const Complex l2)
{
    qreal lam1 = l1.lambda;
    qreal lam2 = l2.lambda;
    qreal alp1 = l1.alpha;
    qreal alp2 = l2.alpha;

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
    if (alp1 < 0)
        return "Устойчивый фокус";
    if (alp2 > 0)
        return "Неустойчивый фокус";
    return "Уст. или неуст. центр или фокус";
}
