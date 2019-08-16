#ifndef JENERALPROTOCOL_H
#define JENERALPROTOCOL_H

#include "QObject"
#include "QWidget"
#include "QVector"
#include "stdint.h"

class generalProtocol: public QObject
{
    Q_OBJECT
public:
    generalProtocol();
    ~generalProtocol();

    uint32_t s;

    bool gpDecode(uint8_t data[], uint32_t size);
    void gpStopCommandTx(uint8_t channel);
    void gpStartClockWiseCommandTx(uint8_t channel);
    void gpStartContrClockWiseCommandTx(uint8_t channel);
signals:
    void gpADCCommandRx(QVector<uint16_t> ADC);
    void gpSend(QVector<uint8_t> commandBuff);
};

#endif // GENERALPROTOCOL_H
