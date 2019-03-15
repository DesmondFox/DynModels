#ifndef STABILITYUTIL_H
#define STABILITYUTIL_H

#include <QString>

class StabilityUtil
{
private:
    StabilityUtil();

public:
    static QString resolveLambda(const qreal lam1, const qreal lam2);
};

#endif // STABILITYUTIL_H
