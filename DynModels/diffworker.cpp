#include "diffworker.h"

DiffWorker::DiffWorker()
{
    PluginHandler hdlr;
    plugins = hdlr.loadPlugins();
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
