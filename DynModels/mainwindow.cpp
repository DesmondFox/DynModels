#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pExportDialog(new ExportDialog(this)),
    pResultDialog(new ResultDialog(this))
{
    ui->setupUi(this);
    if (worker.getModelNames().isEmpty())
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Плагінів не виявлено \n"
                                                     "Работа завершена"));
        return;
    }
    else
    {
        ui->integrationSettings->setModels(worker.getModelNames());
        ui->coefs->setCoefs(worker.getCoefs(ui->integrationSettings->getModelIdx()));
        ui->populationFrame->setFields(worker.getRoles(ui->integrationSettings->getModelIdx()));
        ui->description->setText(worker.getDescription(ui->integrationSettings->getModelIdx()));
    }

    pExportDialog->setMethods(worker.getMethods());

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
    ui->populationFrame->setFields(worker.getRoles(modelIndex));
    qDebug() << "Window:" << "New model changed";
}

void MainWindow::slotSolve()
{
    qDebug() << "Start solving...";

    auto solve = worker.solve(makeSettings(), ui->integrationSettings->getModelIdx());
    auto roles = worker.getRoles(ui->integrationSettings->getModelIdx());
    ui->resultWidget->setData(solve, roles);
    pExportDialog->setData(solve, roles);

    // Для задания последних значений и вывода окна
    makeResults(roles, solve);

    qDebug() << "Done...";
}

void MainWindow::slotMouseHoverOn2DPlot(const QPointF &point)
{
    if (point.x() == -1.0 || point.y() == -1.0)
        pStatusBarLabel->setText(tr(""));
    else
        pStatusBarLabel->setText(QString("X: %1\tY: %2").arg(point.x()).arg(point.y()));
}

void MainWindow::on_acExit_triggered()
{
    if (QMessageBox::question(this, tr("Підтвердження"),
                              tr("Ви дійсно бажаєте вийти?"),
                              QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        QApplication::exit(0);
}

void MainWindow::on_acExportTxt_triggered()
{
    QStringList coefList;
    auto raw = worker.getCoefs(ui->integrationSettings->getModelIdx());
    for (const Coef &coef : raw)
        coefList << coef.coefName;

    pExportDialog->setCoefs(coefList, ui->coefs->getValues());

    pExportDialog->setPopulations(worker.getRoles(ui->integrationSettings->getModelIdx()),
                                  ui->populationFrame->getValues());
    pExportDialog->setPlots(ui->resultWidget->getPlotPix(),
                            ui->resultWidget->getPhasePix());
    pExportDialog->setModel(worker.getModelNames().at(ui->integrationSettings->getModelIdx()),
                            worker.getDescription(ui->integrationSettings->getModelIdx()));
    if (pExportDialog->exec() == QDialog::Accepted)
        pStatusBarLabel->setText("Звіт збережено");
}

void MainWindow::on_acLoad_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Загрузка файла настроек"),
                                                    QDir::homePath(), tr("Dynamic Models File (*.dm)"));

    if (filename.isEmpty())
        return;
    if (!dataLoader.load(filename))
    {
        QMessageBox::critical(this, tr("Помилка"), tr("Виникла помилка завантаження"));
        return;
    }

    ui->integrationSettings->setModelIndex(worker.getModelIndexByID(dataLoader.getModelID()));
    ui->coefs->setValues(dataLoader.getCoefValues());
    ui->populationFrame->setValues(dataLoader.getPopulationValues());
    ui->integrationSettings->setStart(dataLoader.getStart());
    ui->integrationSettings->setEnd(dataLoader.getEnd());
    ui->integrationSettings->setStep(dataLoader.getStep());
}

void MainWindow::on_acSave_triggered()
{
    dataLoader.setModelID(worker.getModelID(ui->integrationSettings->getModelIdx()));
    dataLoader.setPopulationValues(ui->populationFrame->getValues());
    dataLoader.setCoefValues(ui->coefs->getValues());
    dataLoader.setIntegrationSettings(ui->integrationSettings->getStart(),
                                      ui->integrationSettings->getEnd(),
                                      ui->integrationSettings->getStep());
    QString filename = QFileDialog::getSaveFileName(this, tr("Збереження даних програми"),
                                                    QDir::homePath(), tr("Dynamic Models File (*.dm)"));
    if (filename.isEmpty())
        return;
   if (dataLoader.save(filename))
       pStatusBarLabel->setText("Дані збережені");
   else
       QMessageBox::critical(this, tr("Помилка"), tr("Виникла помилка збереження"));
}

void MainWindow::makeResults(const QStringList &roles, const QList<ASolveByMethod> &solve)
{
    LastValues values;
    values.eilers   = solve.at(0).elements.last().second;
    values.modEilers= solve.at(1).elements.last().second;
    values.RKutta   = solve.at(2).elements.last().second;
    values.adams    = solve.at(3).elements.last().second;
    pResultDialog->setValues(roles, values);
    pResultDialog->exec();
}
