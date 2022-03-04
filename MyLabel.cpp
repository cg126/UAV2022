#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
	// 设置新建右键菜单
	this->setContextMenuPolicy(Qt::CustomContextMenu);

	RightMenu = new QMenu();
	Open1 = new QAction(QString::fromLocal8Bit("显示数据"));
	
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(Label_RightMenu()));
}


MyLabel::~MyLabel()
{}

void MyLabel::enterEvent(QEvent *event)
{
	// 设置鼠标样式
}

void MyLabel::leaveEvent(QEvent *event)
{
	QApplication::setOverrideCursor(Qt::ArrowCursor);	// 设置鼠标样式
}

void MyLabel::Label_RightMenu()
{
	RightMenu->addAction(Open1);
	RightMenu->exec(QCursor::pos());
}