#ifndef STABILITYUTIL_H
#define STABILITYUTIL_H

#include <QString>
#include "aboutmodelwidget.h"

class StabilityUtil
{
private:
    StabilityUtil();

public:
    static QString resolveLambda(const Complex l1, const Complex l2);
};

#endif // STABILITYUTIL_H
