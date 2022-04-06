#include "MyLabel.h"
#include <QWheelEvent>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
	// 设置新建右键菜单
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	
	NowH = 480;
	NowW = 640;
	RightMenu = new QMenu();
	Load = new QAction(QString::fromLocal8Bit("加载图片"));
	Save = new QAction(QString::fromLocal8Bit("保存图片"));
	
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(Label_RightMenu()));
	connect(Load, SIGNAL(triggered()), this, SLOT(LoadImage()));
	connect(Save, SIGNAL(triggered()), this, SLOT(SaveImage()));
}


MyLabel::~MyLabel()
{}


void MyLabel::Label_RightMenu()
{
	RightMenu->addAction(Load);
	RightMenu->addAction(Save);
	RightMenu->exec(QCursor::pos());
}


void MyLabel::LoadImage()
{
	QString ImagePath;

	ImagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromLocal8Bit(""), tr("Image Files (*.jpg *.png)"));	// 文件选择对话框

	showImage(ImagePath);

	//String name1 = ImagePath.toStdString();
	//outImage = src1 = imread(name1);

	update();
}


void MyLabel::SaveImage()
{

}


void MyLabel::showImage(QString ImagePath)
{
	pixmap = ImagePath;
	NowW = pixW = pixmap.width();
	NowH = pixH = pixmap.height();
	this->setScaledContents(false);
	this->setPixmap(pixmap);
}


void MyLabel::showImage_ZI(QString ImagePath)
{
	pixmap = ImagePath;
	this->setScaledContents(false);
	float ra = 1.5;
	int pW = pixmap.width() * ra;
	int pH = pixmap.height() * ra;
	pixmap = pixmap.scaled(pW, pH, Qt::IgnoreAspectRatio);
	this->setPixmap(pixmap);
}


void MyLabel::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);

	
	//int w = pixmap.width();
	//int h = pixmap.height();

	painter.drawTiledPixmap(XPtInterval, YPtInterval, NowW, NowH, pixmap);
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
	// event->pos()：鼠标坐标
	// this->frameGeometry()：Label框的范围
	QPoint point = event->pos();
	QRect rect = this->frameGeometry();
	QPoint point1;

	if (rect.x() == 0)
	{
		point1.setX(event->pos().x());
		point1.setY(event->pos().y());
	}
	else if(rect.x() == 326)
	{
		point1.setX(event->pos().x() + 326);
		point1.setY(event->pos().y());
	}
	else if (rect.x() == 652)
	{
		point1.setX(event->pos().x() + 652);
		point1.setY(event->pos().y());
	}
	else if (rect.x() == 10 && rect.y() == 260)
	{
		point1.setX(event->pos().x() + 10);
		point1.setY(event->pos().y() + 260);
	}
	else if (rect.x() == 660 && rect.y() == 260)
	{
		point1.setX(event->pos().x() + 660);
		point1.setY(event->pos().y() + 260);
	}

	// 如果鼠标所在的Label框的范围包含了鼠标的坐标，则滚轮功能生效
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

	//action = MyLabel::Move;
	OldPos = pos;

	this->update();
}