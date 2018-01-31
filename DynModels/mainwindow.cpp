#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (worker.getModelNames().isEmpty())
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Ни один из плагинов не был загружен \n"
                                                     "Работа прекращена"));
        return;
    }
    else
    {
        ui->integrationSettings->setModels(worker.getModelNames());
        ui->coefs->setCoefs(worker.getCoefs(ui->integrationSettings->getModelIdx()));
        ui->populationFrame->setFields(worker.getRoles(ui->integrationSettings->getModelIdx()));
        ui->description->setText(worker.getDescription(ui->integrationSettings->getModelIdx()));
    }

    pStatusBarLabel = new QLabel(tr("Готово"), ui->statusbar);
    ui->statusbar->addWidget(pStatusBarLabel);

    connect(ui->integrationSettings, SIGNAL(modelHasChanged(quint8)), SLOT(slotModelChanged(quint8)));
    connect(ui->integrationSettings, SIGNAL(solveSig()), SLOT(slotSolve()));
    connect(ui->resultWidget,        SIGNAL(sigMouseHoveredOn2DPlots(QPointF)), SLOT(slotMouseHoverOn2DPlot(QPointF)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

DiffSettings MainWindow::makeSettings() const
{
    DiffSettings set;
    set.data        = ui->coefs->getValues();
    set.startValues = ui->populationFrame->getValues();
    set.startTime   = ui->integrationSettings->getStart();
    set.endTime     = ui->integrationSettings->getEnd();
    set.step        = ui->integrationSettings->getStep();
    return set;
}

void MainWindow::slotModelChanged(quint8 modelIndex)
{
    ui->coefs->setCoefs(worker.getCoefs(modelIndex));
    ui->description->setText(worker.getDescription(modelIndex));
    qDebug() << "Window:" << "New model changed";
}

void MainWindow::slotSolve()
{
    qDebug() << "Start solving...";
    ui->resultWidget->setData(worker.solve(makeSettings(), ui->integrationSettings->getModelIdx()),
                              worker.getRoles(ui->integrationSettings->getModelIdx()));
    qDebug() << "Done...";
}

void MainWindow::slotMouseHoverOn2DPlot(const QPointF &point)
{
    if (point.x() == -1.0 || point.y() == -1.0)
        pStatusBarLabel->setText(tr(""));
    else
        pStatusBarLabel->setText(QString("X: %1\tY: %2").arg(point.x()).arg(point.y()));
}
