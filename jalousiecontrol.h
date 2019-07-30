#ifndef JALOUSIECONTROL_H
#define JALOUSIECONTROL_H

#include <QMainWindow>
#include "QTimer"
#include "channelcontrol.h"
#include "hidInterface.h"


#pragma pack(push, 1)
#define PROTOCOL_BUFF_SIZE ((64 - 4 - 4) / 2)
typedef struct SendDataCommandDesc{
    uint32_t headr;
    uint32_t size;
    uint16_t dataBuff[PROTOCOL_BUFF_SIZE];
} PSendDataCommand;

typedef union {
    PSendDataCommand command;
    uint8_t buff[sizeof(PSendDataCommand)];
} SendDataCommand ;
#pragma pack(pop)

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
    channelControl *voltageGraph1;
    Ui::jalousieControl *ui;
    hidInterface *hid;
    QTimer *rxTimer;
    uint32_t dataCnt;

private slots:
    void rxHIDData(void);

    void on_pushButton_clicked();
};

#endif // JALOUSIECONTROL_H
