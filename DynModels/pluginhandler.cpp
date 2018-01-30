#include "pluginhandler.h"
#include <QPluginLoader>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QFileInfo>
#include <QApplication>

PluginHandler::PluginHandler(QObject *parent) : QObject(parent)
{

}

PluginHandler::~PluginHandler()
{
}

QList<Plugin> PluginHandler::loadPlugins()
{
    QList<Plugin> out;

    qDebug() << "Loading a plugins...";
    QDir dir(QApplication::applicationDirPath());
    if (!dir.cd("plugins"))
    {
        qCritical() << "Unable to entry \"plugin\" directory. Aborting...";
        return out;
    }
    qDebug() << "Plugin's path:" << dir.absolutePath();

    // Фильтр для библиотек: для Windows, Unix-based, macOS
    QStringList libFilesFilter;
    libFilesFilter << "*.dll" << "*.so" << "*.dylib";

    QPluginLoader loader;
    for (const QFileInfo &file : dir.entryInfoList(libFilesFilter))
    {
        loader.setFileName(file.absoluteFilePath());
        qDebug() << "Trying to load plugin" << file.fileName();

        IDynModelPlugin *pPlugin = qobject_cast<IDynModelPlugin*>(loader.instance());
        if (!pPlugin)
            qDebug() << "Failed: " << loader.errorString();
        else
        {
            Plugin plugin;
            plugin.plugin = pPlugin;
            plugin.metadata = loadMetadata(loader.metaData());
            out << plugin;
        }
    }
    return out;
}

Plugin::Metadata PluginHandler::loadMetadata(const QJsonObject &metadata)
{
    Plugin::Metadata mtd;
    mtd.IID         = metadata.value("IID").toString();
    QJsonObject modelData = metadata.value("MetaData").toObject();

    mtd.modelName   = modelData.value("ModelName").toString();
    mtd.expressions = modelData.value("NumberOfExpressions").toInt();
    mtd.description = modelData.value("Description").toString();

    // Загрузка данных для модели
    QJsonObject  data = modelData.value("Data").toObject();

    // Названия для входов
    QJsonArray  inputAttr = data.value("InputAttributes").toArray();
    for (const QJsonValue &attr : inputAttr)
        mtd.inputAttributes << attr.toString();

    // Коэффициенты
    QJsonArray  coefs = data.value("Coefs").toArray();
    for (const QJsonValue &coef : coefs)
    {
        QJsonObject obj = coef.toObject();
        Coef _coef;
        _coef.coefName  = obj.value("Name").toString();
        _coef.tip       = obj.value("Tip").toString();
        _coef.value     = obj.value("Value").toString().toDouble();
        mtd.coefs << _coef;
    }

    // Загрузка названий ролей
    QJsonArray roles  = data.value("RoleNames").toArray();
    for (const QJsonValue &role : roles)
        mtd.roleNames << role.toString();

    return mtd;
}
