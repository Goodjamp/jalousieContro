#include "jalousiecontrol.h"
#include "ui_jalousiecontrol.h"
#include "voltagegraph.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include <QVector>
#include <QDebug>
#include <stdint.h>

#define VID_DEVICE 0x0483
#define PID_DEVICE 0x5711


jalousieControl::jalousieControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::jalousieControl)
{
    ui->setupUi(this);
    hid = new hidInterface();
    hid->initUSB();
    voltageGraph1 = new voltageGraph();
    static_cast<QHBoxLayout*>(ui->centralWidget->layout())->insertWidget(1,voltageGraph1);
    rxTimer = new QTimer();

    connect(rxTimer, &QTimer::timeout, this, &jalousieControl::rxHIDData, Qt::QueuedConnection);
    rxTimer->setInterval(1);
    rxTimer->setTimerType(Qt::CoarseTimer);
    dataCnt = 0;
    //voltageGraph1->setYMax(5000);

    QVector<double> x;
    QVector<double> y;
    voltageGraph1->setYMax(3.0);
    voltageGraph1->setYMin(-3.0);
    for(uint32_t k = 0; k < 3000; k++) {
        x.push_back(k * 0.001);
        y.push_back(3 * cos(2 * 3.141*k * 0.001));
    }
    voltageGraph1->addPoints(x, y);
    x.clear();
    y.clear();
    for(uint32_t k = 4000; k < 5000; k++) {
        x.push_back(k * 0.001);
        y.push_back(3 * sin(2 * 3.141*k * 0.001));
    }

    voltageGraph1->addPoints(x, y);

}

#define READ_SIZE 64
#define CH_CNT    7
void jalousieControl::rxHIDData(void)
{
    uint32_t numRead;
    SendDataCommand sendDataCommand;
    if((numRead = hid->read(sendDataCommand.buff, sizeof(sendDataCommand), 100)) == 0){
        return;
    }
    uint32_t k = 0;
    while((k * CH_CNT)  < sizeof(sendDataCommand.command.dataBuff) / sizeof(sendDataCommand.command.dataBuff[0])) {
      dataCnt++;
      voltageGraph1->setXMax(dataCnt);
      voltageGraph1->addPoint(sendDataCommand.command.dataBuff[k * CH_CNT], dataCnt);
      k++;
    }
}

jalousieControl::~jalousieControl()
{
    delete ui;
}


void jalousieControl::on_pushButton_clicked()
{
    if(!hid->openInterface(VID_DEVICE, PID_DEVICE)) {
        qDebug()<<"Can't open device";
        return;
    }
    rxTimer->start();
    qDebug()<<"Device opened";
}
