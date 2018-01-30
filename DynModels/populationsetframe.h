#ifndef POPULATIONSETFRAME_H
#define POPULATIONSETFRAME_H

#include <QFrame>

namespace Ui {
class PopulationSetFrame;
}

class PopulationSetFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PopulationSetFrame(QWidget *parent = 0);
    ~PopulationSetFrame();
    void setFields(const QStringList &roles);
    QList<qreal> getValues() const;

private:
    Ui::PopulationSetFrame *ui;
};

#endif // POPULATIONSETFRAME_H
