#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>

class DataLoader : public QObject
{
    Q_OBJECT
public:
    explicit DataLoader(QObject *parent = nullptr);
    void setPopulationValues(const QList<qreal> &values);
    void setModelID(const quint8 &modelID);
    void setCoefValues(const QList<qreal> &values);
    void setIntegrationSettings(const qreal &start,
                                const qreal &end,
                                const qreal &step);
    bool save(const QString &filename);
    bool load(const QString &filename);

    quint8 getModelID() const;
    QList<qreal> getCoefValues() const;
    QList<qreal> getPopulationValues() const;
    qreal getStart() const;
    qreal getEnd() const;
    qreal getStep() const;

private:
    QList<qreal> m_populations;
    QList<qreal> m_coefs;
    quint8  m_modelID;
    qreal   m_start;
    qreal   m_end;
    qreal   m_step;
};

#endif // DATALOADER_H
