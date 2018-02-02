#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "diffworker.h"
#include "external/exportdialog.h"
#include "external/dataloader.h"

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
    ExportDialog *pExportDialog;
    DataLoader dataLoader;

private slots:
    void slotModelChanged(quint8 modelIndex);
    void slotSolve();
    void slotMouseHoverOn2DPlot(const QPointF &point);
    void on_acExit_triggered();
    void on_acExportTxt_triggered();
    void on_acLoad_triggered();
    void on_acSave_triggered();
};

#endif // MAINWINDOW_H
