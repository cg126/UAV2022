#include "WidgetPic.h"

WidgetPic::WidgetPic(QMdiSubWindow *parent)
	: QMdiSubWindow(parent)
{
	ui.setupUi(this);

	ui.label->setText("hello");
	ui.label->setScaledContents(true);
	ui.label_2->clear();
	//ui.label->setAlignment(Qt::AlignHCenter);
	//ui.label->setAlignment(Qt::AlignVCenter);
}


WidgetPic::WidgetPic(int width, int height, QMdiSubWindow *parent = Q_NULLPTR)
{
	ui.setupUi(this);
	this->resize(width + 20, height + 45);
	ui.label->setText("hello");
	ui.label->setScaledContents(false);
	ui.label->setGeometry(9, 35, width, height);
	ui.label_2->clear();
}


WidgetPic::~WidgetPic()
{
}


void WidgetPic::SetPixmap(QString ImagePath)
{
	ui.label->showImage(ImagePath);
}