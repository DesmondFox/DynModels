#include "diffworker.h"

DiffWorker::DiffWorker()
{
    PluginHandler hdlr;
    plugins = hdlr.loadPlugins();
}

DiffWorker::~DiffWorker()
{
    /// WARNING: Удалить плагины
}

QList<ASolveByMethod> DiffWorker::solve(DiffSettings settings, const quint8 &modelIndex) const
{
    Q_ASSERT(modelIndex < plugins.size());
    QList<ASolveByMethod> out;
    QVector<DiffMethod> sequence = {DiffMethod::Eilers,
                                   DiffMethod::ModifiedEilers,
                                   DiffMethod::RungeKutta4thOrder,
                                   DiffMethod::AdamsBashforth4rdOrder};

    ASolveByMethod solve;
    for (const DiffMethod &currentMethod : sequence)
    {
        settings.diffMethod  = currentMethod;
        solve.method    = currentMethod;
        solve.elements  = plugins.at(modelIndex).plugin->differentiate(settings);
        out << solve;
    }
    return out;
}

QStringList DiffWorker::getModelNames() const
{
    QStringList out;
    for (const Plugin &plugin : plugins)
        out << plugin.metadata.modelName;
    return out;
}

QList<Coef> DiffWorker::getCoefs(const quint8 &modelIndex) const
{
    Q_ASSERT(modelIndex < plugins.size());
    return plugins.at(modelIndex).metadata.coefs;
}

QStringList DiffWorker::getRoles(const quint8 &modelIndex) const
{
    Q_ASSERT(modelIndex < plugins.size());
    return plugins.at(modelIndex).metadata.roleNames;
}

QString DiffWorker::getDescription(const quint8 &modelIndex) const
{
    Q_ASSERT(modelIndex < plugins.size());
    return plugins.at(modelIndex).metadata.description;
}

QStringList DiffWorker::getMethods() const
{
    QStringList out = {
        "Метод Эйлера",
        "Модифицированный метод Эйлера",
        "Метод Рунге-Кутты 4-го порядка",
        "Метод Адамса-Башфорта 4-го порядка"
    };
    return out;
}

quint8 DiffWorker::getModelID(const quint8 &modelIndex) const
{
    Q_ASSERT(modelIndex < plugins.size());
    return plugins.at(modelIndex).metadata.id;
}

qint8 DiffWorker::getModelIndexByID(const quint8 &modelID) const
{
    qint8 idx = -1;
    for (int i = 0; i < plugins.size(); i++)
    {
        if (plugins.at(i).metadata.id == modelID)
            idx = i;
    }
    return idx;
}
