#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "external/exportdialog.h"
#include "external/dataloader.h"
#include "dialogs/resultdialog.h"

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
    ResultDialog *pResultDialog;

private slots:
    void slotModelChanged(quint8 modelIndex);
    void slotSolve();
    void slotMouseHoverOn2DPlot(const QPointF &point);
    void on_acExit_triggered();
    void on_acExportTxt_triggered();
    void on_acLoad_triggered();
    void on_acSave_triggered();
    void makeResults(const QStringList &roles,
                     const QList<ASolveByMethod> &solve);
    void makeDescription();

};

#endif // MAINWINDOW_H
