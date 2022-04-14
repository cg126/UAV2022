#include "MainWindow.h"
#include "UAV2022.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //UAV2022 w;
    //w.show();
	MainWindow m;
	m.show();

    return a.exec();
}
