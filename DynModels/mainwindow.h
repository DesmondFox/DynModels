#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "diffworker.h"

class QLabel;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DiffWorker worker;
    DiffSettings makeSettings() const;
    QLabel *pStatusBarLabel;

private slots:
    void slotModelChanged(quint8 modelIndex);
    void slotSolve();
    void slotMouseHoverOn2DPlot(const QPointF &point);
};

#endif // MAINWINDOW_H
