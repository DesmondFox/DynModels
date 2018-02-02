#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>

class DataLoader : public QObject
{
    Q_OBJECT
public:
    explicit DataLoader(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DATALOADER_H