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
    ~DiffWorker();
    QList<ASolveByMethod> solve(DiffSettings settings, const quint8 &modelIndex);
    QStringList getModelNames() const;
    QList<Coef> getCoefs(const quint8 &modelIndex) const;
    QStringList getRoles(const quint8 &modelIndex) const;
    QString     getDescription(const quint8 &modelIndex) const;
    QStringList getMethods() const;
    quint8      getModelID(const quint8 &modelIndex) const;
    qint8       getModelIndexByID(const quint8 &modelID) const;
    QPixmap     getFormulaPixmap(const quint8 &modelIndex) const;
    IDynModelPlugin* getSelectedPlugin();

    static const quint8 MethodNum = 4;
private:
    QList<Plugin> plugins;

    IDynModelPlugin *selectedPlugin;
};

#endif // DIFFWORKER_H
