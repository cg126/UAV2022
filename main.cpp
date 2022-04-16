#include <QtWidgets/QApplication>
#include "MainWindow.h"
#include "UAV2022.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //UAV2022 w;
    //w.show();
	MainWindow m;
	// 将主界面大小固定为全屏的70%
	/*QDesktopWidget dw;
	int x = dw.width() * 0.7;
	int y = dw.height() * 0.7;
	m.setFixedSize(x, y);*/
	m.show();

    return a.exec();
}
