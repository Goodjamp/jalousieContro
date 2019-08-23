#ifndef JALOUSIECONTROL_H
#define JALOUSIECONTROL_H

#include <QMainWindow>
#include "QTimer"
#include "QFile"
#include "QTextStream"
#include "channelcontrol.h"
#include "hidInterface.h"
#include "generalprotocol.h"
#include "stdio.h"


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
    channelControl *voltageGraph1;
    Ui::jalousieControl *ui;
    hidInterface *hid;
    QTimer *rxTimer;
    uint32_t dataCnt;
    FILE *file;
private slots:
    void rxData(void);
    void rxADC(QVector<uint16_t> adcData);
    void txData(QVector<uint8_t> txData);
    /*channel command processing*/
    void chStop(uint8_t channelIndex);
    void chStartClockWise(uint8_t channelIndex);
    void chStartCounterClockwise(uint8_t channelIndex);
    void on_pushButton_Open_clicked();
    void on_pushButton_Close_clicked();
};

#endif // JALOUSIECONTROL_H
