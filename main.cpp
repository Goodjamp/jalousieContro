#include "jalousiecontrol.h"
#include <QApplication>
#include <QVector>
#include <QMetaType>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    jalousieControl w;
    w.show();
    qRegisterMetaType< QVector<uint16_t> >();
    qRegisterMetaType< QVector<uint8_t> >();
    qRegisterMetaType< uint8_t >();
    return a.exec();
}
