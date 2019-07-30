#include "voltagegraph.h"
#include "ui_voltagegraph.h"

voltageGraph::voltageGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::voltageGraph)
{
    ui->setupUi(this);
    ui->plote->addGraph();
}

voltageGraph::~voltageGraph()
{
    delete ui;
}

bool voltageGraph::addPoints(QVector<double> &x, QVector<double> &y)
{
    if(x.size() != y.size()) {
        return false;
    }
    for(uint32_t k = 0; k < x.size(); k++) {
        ui->plote->graph(0)->addData(x[k], y[k]);
    }
    ui->plote->replot();
}

bool voltageGraph::addPoint(double x, double y)
{
    ui->plote->graph(0)->addData(x, y);
    ui->plote->replot();
}

bool voltageGraph::setYMax(double max)
{
    QCPRange curenRange = ui->plote->yAxis->range();
    ui->plote->yAxis->setRange(curenRange.lower, max);
    ui->plote->replot();
}

bool voltageGraph::setYMin(double min)
{
    QCPRange curenRange = ui->plote->yAxis->range();
    ui->plote->yAxis->setRange(min, curenRange.upper);
    ui->plote->replot();
}



bool voltageGraph::setXMax(double max)
{
    QCPRange curenRange = ui->plote->xAxis->range();
    ui->plote->xAxis->setRange(curenRange.lower, max);
    ui->plote->replot();
}

bool voltageGraph::setXMin(double min)
{
    QCPRange curenRange = ui->plote->xAxis->range();
    ui->plote->xAxis->setRange(min, curenRange.upper);
    ui->plote->replot();
}







