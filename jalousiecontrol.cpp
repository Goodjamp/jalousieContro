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

    connect(rxTimer, &QTimer::timeout, this, &jalousieControl::rxData, Qt::QueuedConnection);
    rxTimer->setInterval(1);
    rxTimer->setTimerType(Qt::CoarseTimer);
    dataCnt = 0;
    voltageGraph1->setYMax(5000);
    voltageGraph1->setXMax(100);
    protocol = new generalProtocol();
    connect(protocol, &generalProtocol::gpADCCommandRx, this, &jalousieControl::rxADC, Qt::QueuedConnection);
    connect(protocol, &generalProtocol::gpSend, this, &jalousieControl::txData, Qt::QueuedConnection);
    connect(voltageGraph1, &channelControl::stop, this, &jalousieControl::chStop, Qt::QueuedConnection);
    connect(voltageGraph1, &channelControl::startClockWise, this, &jalousieControl::chStartClockWise, Qt::QueuedConnection);
    connect(voltageGraph1, &channelControl::startCounterClockwise, this, &jalousieControl::chStartCounterClockwise, Qt::QueuedConnection);
}

jalousieControl::~jalousieControl()
{
    delete ui;
}

#define READ_SIZE 64
#define CH_CNT    7
void jalousieControl::rxADC(QVector<uint16_t> adcData)
{
    uint32_t k = 0;
    while((k * CH_CNT) < static_cast<uint32_t>(adcData.size())) {
        dataCnt++;
        voltageGraph1->setXMax(dataCnt);
        voltageGraph1->addPoint(dataCnt, adcData[k * CH_CNT]);
        k++;
    }
}

void jalousieControl::rxData(void)
{
    uint32_t numRead;
    uint8_t  rxHIDBuff[READ_SIZE];
    if((numRead = hid->read(rxHIDBuff, sizeof(rxHIDBuff), 10)) == 0){
        return;
    }
    protocol->gpParse(rxHIDBuff, numRead);
}

void jalousieControl::txData(QVector<uint8_t> txData)
{
    hid->write(txData.data(), static_cast<uint32_t>(txData.size()), 10);
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


void jalousieControl::chStop(uint8_t channelIndex)
{
    protocol->gpStopCommandTx(channelIndex);
}

void jalousieControl::chStartClockWise(uint8_t channelIndex)
{
    protocol->gpStartClockWiseCommandTx(channelIndex);
}

void jalousieControl::chStartCounterClockwise(uint8_t channelIndex)
{
    protocol->gpStartContrClockWiseCommandTx(channelIndex);
}



