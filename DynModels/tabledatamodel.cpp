#include "tabledatamodel.h"

TableDataModel::TableDataModel(QObject *parent) : QStandardItemModel(parent)
{

}

void TableDataModel::print(const QList<Element> elements, const QStringList &roles)
{
    if (elements.isEmpty())
        return;

    removeRows(0, rowCount());

    setRoles(roles);
    // Учитывается размер + столбец со временем
    quint8 colCount = elements.first().second.size() + 1;
    quint8 elementCount = elements.first().second.size();

    if (columnCount() != colCount)
        setColumnCount(colCount);

    quint32 size = elements.size();


    for (quint32 i = 0; i < size; i++)
    {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(elements.at(i).first));
        for (quint8 el = 0; el < elementCount; el++)
            row << new QStandardItem(QString::number(elements.at(i).second.at(el)));
        insertRow(rowCount(), row);
    }
}

void TableDataModel::setRoles(const QStringList &roles)
{
    if (roles.isEmpty())
        return;
    QStringList r;
    r.append("Время");
    r.append(roles);
    this->setHorizontalHeaderLabels(r);
}
