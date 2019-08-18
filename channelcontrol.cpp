#include <channelcontrol.h>
#include <ui_channelcontrol.h>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

#include <stdbool.h>
#include <stdint.h>

#define DEF_X_MAX   100
#define DEF_X_MIM     0
#define DEF_Y_MAX  5000
#define DEF_Y_MIM     0

channelControl::channelControl(char inChannelIndex, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::channelControl)
{
    channelIndex = inChannelIndex;
    ui->setupUi(this);
    ui->plote->addGraph();
    ui->plote->xAxis->setRange(DEF_X_MIM, DEF_X_MAX);
    ui->plote->yAxis->setRange(DEF_Y_MIM, DEF_Y_MAX);
    QCPRange curenXRange = ui->plote->xAxis->range();
    QCPRange curenYRange = ui->plote->yAxis->range();
    ui->lineEditMaxXAxes->setText(QString::number(static_cast<int>(curenXRange.upper), 10));
    ui->lineEditMinXAxes->setText(QString::number(static_cast<int>(curenXRange.lower), 10));
    ui->lineEditMaxYAxes->setText(QString::number(static_cast<int>(curenYRange.upper), 10));
    ui->lineEditMinYAxes->setText(QString::number(static_cast<int>(curenYRange.lower), 10));
    xCnt = 0;
}

channelControl::~channelControl()
{
    delete ui;
}

bool channelControl::addPoints(QVector<double> &x, QVector<double> &y)
{
    if(x.size() != y.size()) {
        return false;
    }
    for(uint32_t k = 0; k < x.size(); k++) {
        ui->plote->graph(0)->addData(x[k], y[k]);
    }
    ui->plote->replot();
    return true;
}

bool channelControl::addPoint(double x, double y)
{
    setXMax(xCnt);
    ui->plote->graph(0)->addData(xCnt, y);
    ui->plote->replot();
    xCnt = xCnt + 1;
    return true;
}

bool channelControl::setYMax(double max)
{
    QCPRange curenRange = ui->plote->yAxis->range();
    ui->plote->yAxis->setRange(curenRange.lower, max);
    ui->plote->replot();
    return true;
}

bool channelControl::setYMin(double min)
{
    QCPRange curenRange = ui->plote->yAxis->range();
    ui->plote->yAxis->setRange(min, curenRange.upper);
    ui->plote->replot();
    return true;
}

bool channelControl::setXMax(double max)
{
    QCPRange curenRange = ui->plote->xAxis->range();
    ui->plote->xAxis->setRange(curenRange.lower, max);
    ui->plote->replot();
    return true;
}

bool channelControl::setXMin(double min)
{
    QCPRange curenRange = ui->plote->xAxis->range();
    ui->plote->xAxis->setRange(min, curenRange.upper);
    ui->plote->replot();
    return true;
}


void channelControl::on_pushButtonClearAxes_clicked()
{
    ui->plote->graph(0)->data()->clear();
    //ui->plote->yAxis->setRange(DEF_Y_MIM, DEF_Y_MAX);
    //ui->plote->xAxis->setRange(DEF_X_MIM, DEF_X_MAX);
    xCnt = 0;
    setYMin(static_cast<double>(DEF_Y_MIM));
    setYMax(static_cast<double>(DEF_Y_MAX));
    setXMin(static_cast<double>(DEF_X_MIM));
    setXMax(static_cast<double>(DEF_X_MAX));
    ui->plote->replot();
}

void channelControl::on_pushButtonSetAxesRanges_clicked()
{
    bool rezConvert = false;
    double maxX;
    double minX;
    double maxY;
    double minY;
    maxX = ui->lineEditMaxXAxes->text().toDouble(&rezConvert);
    if(!rezConvert) {
        return;
    }
    minX = ui->lineEditMinXAxes->text().toDouble(&rezConvert);
    if(!rezConvert) {
        return;
    }
    maxY = ui->lineEditMaxYAxes->text().toDouble(&rezConvert);
    if(!rezConvert) {
        return;
    }
    minY = ui->lineEditMinYAxes->text().toDouble(&rezConvert);
    if(!rezConvert) {
        return;
    }

    ui->plote->xAxis->setRange(minX, maxX);
    ui->plote->yAxis->setRange(minY, maxY);
}

void channelControl::on_pushButtonStop_clicked()
{
    emit stop(channelIndex);
}

void channelControl::on_pushButtonStartClockWise_clicked()
{
    emit startClockWise(channelIndex);
}

void channelControl::on_pushButtonStartContrClockWise_clicked()
{
    emit startCounterClockwise(channelIndex);
}
