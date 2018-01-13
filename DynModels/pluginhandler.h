#ifndef PLUGINHANDLER_H
#define PLUGINHANDLER_H

#include <QObject>

class PluginHandler : public QObject
{
    Q_OBJECT
public:
    explicit PluginHandler(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PLUGINHANDLER_H