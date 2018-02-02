#ifndef INTERGRATIONSETTINGFRAME_H
#define INTERGRATIONSETTINGFRAME_H

#include <QFrame>

namespace Ui {
class IntergrationSettingFrame;
}

class IntergrationSettingFrame : public QFrame
{
    Q_OBJECT

public:
    explicit IntergrationSettingFrame(QWidget *parent = 0);
    ~IntergrationSettingFrame();

    qreal   getStart()  const;
    qreal   getEnd()    const;
    qreal   getStep()   const;
    quint8 getModelIdx()const;

    void    setModels(const QStringList &modellist);
    void    setModelIndex(const quint8 &modelIndex);

    void setStart(const qreal &value);
    void setEnd(const qreal &value);
    void setStep(const qreal &value);

signals:
    void    solveSig();
    void    modelHasChanged(quint8 modelIndex);

private slots:
    void on_pushSolve_clicked();
    void on_cbModel_currentIndexChanged(int index);

    void on_dsbStart_valueChanged(double value);

private:
    Ui::IntergrationSettingFrame *ui;
};

#endif // INTERGRATIONSETTINGFRAME_H
