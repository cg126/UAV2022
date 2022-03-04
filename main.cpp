#include "UAV2022.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UAV2022 w;
    w.show();
    return a.exec();
}
