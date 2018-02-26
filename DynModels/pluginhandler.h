#ifndef PLUGINHANDLER_H
#define PLUGINHANDLER_H

#include <QObject>
#include "../plugins/idynmodelplugin.h"

struct Coef
{
    QString coefName;   // Название коэффициента
    QString tip;        // Подсказка при наведении
    qreal   value;      // Стартовое значение
    QString letter;     // Как выглядит в формуле
};

struct Plugin
{
    IDynModelPlugin *plugin;        // Указатель на плагин
    struct Metadata
    {
        QString     IID;            // IID плагина
        QString     modelName;      // Название модели
        QStringList inputAttributes;// Названия атрибутов входных значений
        quint8      expressions;    // Количество уравнений
        QStringList roleNames;      // Название ролей (для обозначения на графиках)
        QString     description;    // Описание модели
        QList<Coef> coefs;          // Коэффициенты
        quint8      id;         // ID плагина
    } metadata;
};

class PluginHandler : public QObject
{
    Q_OBJECT
public:
    explicit PluginHandler(QObject *parent = nullptr);
    ~PluginHandler();
    QList<Plugin>   loadPlugins();
private:
    Plugin::Metadata loadMetadata(const QJsonObject &metadata);
};

#endif // PLUGINHANDLER_H
