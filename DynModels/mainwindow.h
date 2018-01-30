#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "diffworker.h"

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
private slots:
    void slotModelChanged(quint8 modelIndex);
    void slotSolve();
};

#endif // MAINWINDOW_H
