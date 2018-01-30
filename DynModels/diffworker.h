#ifndef DIFFWORKER_H
#define DIFFWORKER_H

#include "pluginhandler.h"

struct ASolveByMethod
{
    DiffMethod method;
    QList<Element> elements;
};


class DiffWorker
{
public:
    DiffWorker();
    QList<ASolveByMethod> solve(DiffSettings settings, const quint8 &modelIndex) const;
    QStringList getModelNames() const;
    QList<Coef> getCoefs(const quint8 &modelIndex) const;
    QStringList getRoles(const quint8 &modelIndex) const;
    QString     getDescription(const quint8 &modelIndex) const;

    static const quint8 MethodNum = 4;
private:
    QList<Plugin> plugins;
};

#endif // DIFFWORKER_H
