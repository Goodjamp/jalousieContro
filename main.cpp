#include "jalousiecontrol.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    jalousieControl w;
    w.show();

    return a.exec();
}
