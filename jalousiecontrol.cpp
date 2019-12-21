#include "jalousiecontrol.h"
#include "ui_jalousiecontrol.h"
#include "channelcontrol.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include <QVector>
#include <QDebug>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <QFile>
#include <QTime>

#define VID_DEVICE 0x0483
#define PID_DEVICE 0x5711

#define NUMBER_OF_PACKET  10
#define PACKET_SIZE       28
#define IGNORE_PACKET_CNT 1

#define DEFAULT_FILE_PATH "C:\\jalousie.rez"

jalousieControl::jalousieControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::jalousieControl)
{
    ui->setupUi(this);

    voltageGraph1 = new channelControl(1);
    static_cast<QHBoxLayout*>(ui->centralWidget->layout())->insertWidget(1,voltageGraph1);

    voltageGraph1->setYMax(5000);
    voltageGraph1->setXMax(100);

    ui->pushButton_Open->setEnabled(true);
    ui->pushButton_Close->setEnabled(false);

    hid             = new hidInterface();
    protocol        = new generalProtocol();
    protocolThreadH = new protocolThread(hid, protocol);
    connect(protocol, &generalProtocol::gpDataRxSize16, this, &jalousieControl::gpCommandRxDataProcessing);
    connect(protocol, &generalProtocol::gpSend, this, &jalousieControl::txData, Qt::QueuedConnection);
    /*One channel signals*/
    connect(voltageGraph1, &channelControl::stop, this, &jalousieControl::chStop, Qt::QueuedConnection);
    connect(voltageGraph1, &channelControl::startClockWise, this, &jalousieControl::chStartClockWise, Qt::QueuedConnection);
    connect(voltageGraph1, &channelControl::startCounterClockwise, this, &jalousieControl::chStartCounterClockwise, Qt::QueuedConnection);

    dataFrameCnt = 0;
    dataFrameX.resize(PACKET_SIZE * NUMBER_OF_PACKET);
    dataFrameY.resize(PACKET_SIZE * NUMBER_OF_PACKET);
    for(uint16_t k = 0; k < dataFrameX.size(); k++) {
        dataFrameX[k] = k;
    }

    dataBuffCnt = 0;
    dataBuff =(uint8_t*)malloc(1024 * 1024);
    ignorePacketCnt = 0;

    isChannelRun = false;
}

jalousieControl::~jalousieControl()
{
    delete ui;
}

void jalousieControl::gpCommandRxDataProcessing(QVector<uint16_t> adcData)
{
    if(!isChannelRun) {
        return;
    }
    if(ignorePacketCnt < IGNORE_PACKET_CNT) {
        ignorePacketCnt++;
    }

    if(dataFrameCnt >= NUMBER_OF_PACKET) {
        voltageGraph1->clearAxes();
        voltageGraph1->addPoints(dataFrameX, dataFrameY);
        dataFrameCnt = 0;
    }
    for(int32_t k = 0; k < PACKET_SIZE; k++) {
        dataFrameY[k + dataFrameCnt * PACKET_SIZE] = adcData[k];
    }
    dataFrameCnt++;

    /*Copy new data to file buff*/
    memcpy((uint8_t*)&dataBuff[dataBuffCnt], (uint8_t*)adcData.data(), adcData.size() * sizeof(uint16_t));
    dataBuffCnt += adcData.size() * sizeof(uint16_t);
}

void jalousieControl::txData(QVector<uint8_t> txData)
{
    hid->write(txData.data(), 64, 10);
}


void jalousieControl::chStop(uint8_t channelIndex)
{
    isChannelRun = false;
    protocol->gpStopCommandTx(channelIndex);
    /*Save all Rx data to file*/
    FILE *file = NULL;
    file = fopen(DEFAULT_FILE_PATH, "w");
    if(!file) {
        qDebug()<<"Can't create file";
    }
    uint32_t rxDataCnt = dataBuffCnt / 2;
    qDebug()<<"rxData = "<<rxDataCnt;
    QVector<double> data(rxDataCnt);
    QVector<double> index(rxDataCnt);
    for (uint32_t k = 0; k < rxDataCnt; k += sizeof(uint16_t)) {
        data[k] = (double)((dataBuff[k * 2 + 1] << 8) | dataBuff[k * 2]);
        index[k] = (double)k;
        if(file) {
            fprintf(file, "%5u\n", (uint16_t)data[k]);
        }
    }
    if(file) {
        fclose(file);
    }
    voltageGraph1->clearAxes();
    voltageGraph1->setXMax(rxDataCnt);
    voltageGraph1->addPoints(index , data);
    dataBuffCnt = 0;
    ignorePacketCnt = 0;
}

void jalousieControl::chStartClockWise(uint8_t channelIndex)
{
    protocol->gpStartClockWiseCommandTx(channelIndex);
    isChannelRun = true;
}

void jalousieControl::chStartCounterClockwise(uint8_t channelIndex)
{
    protocol->gpStartContrClockWiseCommandTx(channelIndex);
    isChannelRun = true;
}

void jalousieControl::on_pushButton_Open_clicked()
{
    errno = 0;
    hid->initUSB();
    if(!hid->openInterface(VID_DEVICE, PID_DEVICE)) {
        qDebug()<<"Can't open device";
        return;
    }
    protocolThreadH->start();
    ui->pushButton_Open->setEnabled(false);
    ui->pushButton_Close->setEnabled(true);
    qDebug()<<"Device opened";
}

void jalousieControl::on_pushButton_Close_clicked()
{
    hid->closeInterface();
    ui->pushButton_Open->setEnabled(true);
    ui->pushButton_Close->setEnabled(false);
    protocolThreadH->quit();
}
