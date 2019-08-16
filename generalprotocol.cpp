#include "QObject"
#include "QVector"
#include "QDebug"
#include "generalprotocol.h"
#include "stdint.h"
#include "string.h"

#define BUFF_SIZE 64
#define CHANNEL_CNT 0x7
#define CHANNEL_ALL 0xFF

typedef enum{
    GP_ADC                    = 0,
    GP_STOP                   = 1,
    GP_START_CLOCK_WISE       = 2,
    GP_START_CONTR_CLOCK_WISE = 3,
} GP_COMMAND;


#pragma pack(push, 1)
#define PROTOCOL_BUFF_SIZE (BUFF_SIZE - 4)
typedef struct GpCommand{
    uint8_t headr;
    uint8_t subcommand[PROTOCOL_BUFF_SIZE];
} GpCommand;

typedef struct {
    uint16_t size;
    uint16_t data[];
}GpADCSubcommand;

typedef union {
    GpCommand command;
    uint8_t buff[sizeof(GpCommand)];
} GpCommandBuff ;
#pragma pack(pop)

generalProtocol::generalProtocol()
{
    //
}

generalProtocol::~generalProtocol()
{
    //
}

bool generalProtocol::gpDecode(uint8_t data[], uint32_t size)
{
    GpCommandBuff gpCommandBuff;
    memcpy((gpCommandBuff.buff), static_cast<uint8_t*>(data), size);
    switch(gpCommandBuff.command.headr) {
        case GP_ADC:
            GpADCSubcommand *gpADCSubcommand = (GpADCSubcommand*)gpCommandBuff.command.subcommand;
            QVector<uint16_t> adcRezV;
            for(uint32_t k = 0; k < gpADCSubcommand->size; k++) {
                adcRezV.push_back(gpADCSubcommand->data[k]);
            }
            emit gpADCCommandRx(adcRezV);
            break;
    };
    return true;
}

void generalProtocol::gpStopCommandTx(uint8_t channel)
{
    QVector<uint8_t> gpCommandV;
    gpCommandV.resize(sizeof(GpCommand));
    GpCommand *gpCommand = (GpCommand*)(gpCommandV.data());
    gpCommand->headr = GP_STOP;
    gpCommand->subcommand[0] = static_cast<uint8_t>(channel);
    emit gpSend(gpCommandV);
}

void generalProtocol::gpStartClockWiseCommandTx(uint8_t channel)
{
    QVector<uint8_t> gpCommandV;
    gpCommandV.resize(sizeof(GpCommand));
    GpCommand *gpCommand = (GpCommand*)(gpCommandV.data());
    gpCommand->headr = GP_START_CLOCK_WISE;
    gpCommand->subcommand[0] = static_cast<uint8_t>(channel);
    emit gpSend(gpCommandV);
}

void generalProtocol::gpStartContrClockWiseCommandTx(uint8_t channel)
{
    QVector<uint8_t> gpCommandV;
    gpCommandV.resize(sizeof(GpCommand));
    GpCommand *gpCommand = (GpCommand*)(gpCommandV.data());
    gpCommand->headr = GP_START_CONTR_CLOCK_WISE;
    gpCommand->subcommand[0] = static_cast<uint8_t>(channel);
    emit gpSend(gpCommandV);
}
