#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
	// �����½��Ҽ��˵�
	this->setContextMenuPolicy(Qt::CustomContextMenu);

	RightMenu = new QMenu();
	Open1 = new QAction(QString::fromLocal8Bit("��ʾ����"));
	
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(Label_RightMenu()));
}


MyLabel::~MyLabel()
{}

void MyLabel::enterEvent(QEvent *event)
{
	// ���������ʽ
}

void MyLabel::leaveEvent(QEvent *event)
{
	QApplication::setOverrideCursor(Qt::ArrowCursor);	// ���������ʽ
}

void MyLabel::Label_RightMenu()
{
	RightMenu->addAction(Open1);
	RightMenu->exec(QCursor::pos());
}