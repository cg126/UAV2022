#include "MyLabel.h"
#include <QWheelEvent>

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


void MyLabel::showImage(QString ImagePath)
{
	pixmap = ImagePath;
	this->setScaledContents(false);
	this->setPixmap(pixmap);
}


void MyLabel::paintEvent(QEvent * event)
{

}


void MyLabel::enterEvent(QEvent *event)
{
	// 设置鼠标样式
}


void MyLabel::leaveEvent(QEvent *event)
{
	QApplication::setOverrideCursor(Qt::ArrowCursor);	// 设置鼠标样式
}


void MyLabel::wheelEvent(QWheelEvent * event)
{
	QPoint point = event->pos();
	QRect rect = this->frameGeometry();
	QPoint point1;
	point1.setX(event->pos().x()+ 320);
	point1.setY(event->pos().y());

	//if (rect.x)
	//{
	//	point1; //label
	//}
	//else if()
	//{
	//	point.。。。。  //label2
	//}
	//else if 
	//{

	//}


	if (this->frameGeometry().contains(point1))
	{
		pixW = pixmap.width();
		pixH = pixmap.height();
		//QApplication::setOverrideCursor(Qt::OpenHandCursor); //设置鼠标样式
		if (event->delta() < 0)     //下滑，缩小
		{
			//action = Drone2021::Shrink;
			//this->update();
			if (ratio <= 0.22)
			{
				NowW = 0.2 * pixW;
				NowH = 0.2 * pixH;

				this->setScaledContents(false);
				QPixmap pixmapTemp = pixmap.scaled(NowW, NowH, Qt::IgnoreAspectRatio);
				this->setPixmap(pixmapTemp);
				this->update();

				ratio = 0.2;
			}
			else
			{
				ratio -= 0.1;
				NowW = ratio * pixW;
				NowH = ratio * pixH;

				this->setScaledContents(false);
				QPixmap pixmapTemp = pixmap.scaled(NowW, NowH, Qt::IgnoreAspectRatio);
				this->setPixmap(pixmapTemp);
				this->update();
			}		
		}
		else                         //上滑，放大
		{
			//action = Drone2021::Amplification;
			//this->update();
			ratio += 0.1;
			NowW = ratio * pixW;
			NowH = ratio * pixH;

			this->setScaledContents(false);
			QPixmap pixmapTemp = pixmap.scaled(NowW, NowH, Qt::IgnoreAspectRatio);
			this->setPixmap(pixmapTemp);
			this->update();
		}
	}
}


void MyLabel::Label_RightMenu()
{
	RightMenu->addAction(Open1);
	RightMenu->exec(QCursor::pos());
}


//鼠标摁下
void MyLabel::mousePressEvent(QMouseEvent *event)
{
	OldPos = event->pos();
	Pressed = true;
}
//鼠标松开
void MyLabel::mouseMoveEvent(QMouseEvent *event)
{
	if (!Pressed)
		return QWidget::mouseMoveEvent(event);

	this->setCursor(Qt::SizeAllCursor);
	QPoint pos = event->pos();
	int xPtInterval = pos.x() - OldPos.x();
	int yPtInterval = pos.y() - OldPos.y();

	XPtInterval += xPtInterval;
	YPtInterval += yPtInterval;

	OldPos = pos;
	update();
}