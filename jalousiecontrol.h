#ifndef JALOUSIECONTROL_H
#define JALOUSIECONTROL_H

#include <QMainWindow>
#include "QTimer"
#include "channelcontrol.h"
#include "hidInterface.h"
#include "generalprotocol.h"

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

private slots:
    void rxData(void);
    void rxADC(QVector<uint16_t> adcData);
    void txData(QVector<uint8_t> txData);
    void on_pushButton_clicked();
    /*channel command processing*/
    void chStop(uint8_t channelIndex);
    void chStartClockWise(uint8_t channelIndex);
    void chStartCounterClockwise(uint8_t channelIndex);
};

#endif // JALOUSIECONTROL_H
