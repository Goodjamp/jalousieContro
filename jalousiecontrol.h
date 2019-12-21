#ifndef JALOUSIECONTROL_H
#define JALOUSIECONTROL_H

#include "stdio.h"


#include <QMainWindow>
#include "QTimer"
#include "QTime"
#include "QFile"
#include "QTextStream"
#include "QVector"

#include "channelcontrol.h"
#include "hidInterface.h"
#include "generalprotocol.h"
#include "protocolthread.h"


namespace Ui {
class jalousieControl;
}

class jalousieControl : public QMainWindow
{
    Q_OBJECT

public:
    explicit jalousieControl(QWidget *parent = nullptr);
    ~jalousieControl();

private:
    generalProtocol *protocol;
    protocolThread  *protocolThreadH;
    channelControl  *voltageGraph1;
    Ui::jalousieControl *ui;
    hidInterface *hid;

    uint32_t dataBuffCnt;
    uint8_t *dataBuff;
    uint32_t ignorePacketCnt;
    /**Vector for keep elements*/
    QVector<double> dataFrameX;
    QVector<double> dataFrameY;
    int32_t dataFrameCnt;
    bool isChannelRun;

private slots:
    void gpCommandRxDataProcessing(QVector<uint16_t> adcData);
    void txData(QVector<uint8_t> txData);
    /*channel command processing*/
    void chStop(uint8_t channelIndex);
    void chStartClockWise(uint8_t channelIndex);
    void chStartCounterClockwise(uint8_t channelIndex);
    void on_pushButton_Open_clicked();
    void on_pushButton_Close_clicked();
};

#endif // JALOUSIECONTROL_H
