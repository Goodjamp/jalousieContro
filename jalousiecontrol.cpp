#include "jalousiecontrol.h"
#include "ui_jalousiecontrol.h"
#include "channelcontrol.h"
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
    voltageGraph1 = new channelControl(1);
    static_cast<QHBoxLayout*>(ui->centralWidget->layout())->insertWidget(1,voltageGraph1);
    rxTimer = new QTimer();

    connect(rxTimer, &QTimer::timeout, this, &jalousieControl::rxHIDData, Qt::QueuedConnection);
    rxTimer->setInterval(1);
    rxTimer->setTimerType(Qt::CoarseTimer);
    dataCnt = 0;
    voltageGraph1->setYMax(5000);
    voltageGraph1->setXMax(100);
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
      voltageGraph1->addPoint(dataCnt, sendDataCommand.command.dataBuff[k * CH_CNT]);
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
