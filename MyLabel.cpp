#include "MyLabel.h"
#include <QWheelEvent>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
	// �����½��Ҽ��˵�
	this->setContextMenuPolicy(Qt::CustomContextMenu);

	
	NowH = 480;
	NowW = 640;
	RightMenu = new QMenu();
	Open1 = new QAction(QString::fromLocal8Bit("����ͼƬ"));
	
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(Label_RightMenu()));

	connect(Open1, SIGNAL(triggered()), this, SLOT(LoadImage()));

}


MyLabel::~MyLabel()
{}


void MyLabel::LoadImage()
{
	QString ImagePath;

	ImagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromLocal8Bit(""), tr("Image Files (*.jpg *.png)"));	// �ļ�ѡ��Ի���

	showImage(ImagePath);

	//String name1 = ImagePath.toStdString();
	//outImage = src1 = imread(name1);

	update();
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
	// ���������ʽ
}


void MyLabel::leaveEvent(QEvent *event)
{
	QApplication::setOverrideCursor(Qt::ArrowCursor);	// ���������ʽ
}


void MyLabel::wheelEvent(QWheelEvent * event)
{
	// event->pos()���������
	// this->frameGeometry()��Label��ķ�Χ
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

	// ���������ڵ�Label��ķ�Χ�������������꣬����ֹ�����Ч
	if (this->frameGeometry().contains(point1))
	{
		pixW = pixmap.width();
		pixH = pixmap.height();
		//QApplication::setOverrideCursor(Qt::OpenHandCursor); //���������ʽ
		if (event->delta() < 0)     //�»�����С
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
		else                         //�ϻ����Ŵ�
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


//�������
void MyLabel::mousePressEvent(QMouseEvent *event)
{
	OldPos = event->pos();
	Pressed = true;
}


//����ɿ�
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