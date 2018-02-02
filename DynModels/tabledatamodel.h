#ifndef TABLEDATAMODEL_H
#define TABLEDATAMODEL_H

#include <QStandardItemModel>
#include "diffworker.h"

class TableDataModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit TableDataModel(QObject *parent = nullptr);
    void print(const QList<Element> elements, const QStringList &roles);
    void setRoles(const QStringList &roles);

private:
    static const quint16 SkipInterval = 10;
};

#endif // TABLEDATAMODEL_H
