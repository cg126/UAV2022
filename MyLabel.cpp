#include "MyLabel.h"
#include <QWheelEvent>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
	// �����½��Ҽ��˵�
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	
	NowH = 240;
	NowW = 320;
	RightMenu = new QMenu();
	Load = new QAction(QString::fromLocal8Bit("����ͼƬ"));
	Save = new QAction(QString::fromLocal8Bit("����ͼƬ"));
	
	connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(Label_RightMenu()));
	connect(Load, SIGNAL(triggered()), this, SLOT(LoadImage()));
	connect(Save, SIGNAL(triggered()), this, SLOT(SaveImage()));

	this->setAlignment(Qt::AlignCenter);

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

	ImagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromLocal8Bit(""), tr("Image Files (*.jpg *.png)"));	// �ļ�ѡ��Ի���

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
	Source_pixmap = pixmap = ImagePath;
	Pixmap_path = ImagePath;
	NowW = pixW = pixmap.width();
	NowH = pixH = pixmap.height();
	this->setScaledContents(true);
	this->update();
	//this->setPixmap(pixmap);
}


void MyLabel::showImage_ZI(QString ImagePath)
{
	pixmap = ImagePath;
	Pixmap_path = ImagePath;
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

	//cg
	/*if (action == MyLabel::Move)
	{
		int w = pixmap.width();
		int h = pixmap.height();

		painter.drawTiledPixmap(XPtInterval, YPtInterval, NowW, NowH, pixmap);
	}*/
	
	if (action == MyLabel::Move)                    //�ƶ�
	{
		int offsetx = Alloffset.x() + offset.x();
		Alloffset.setX(offsetx);

		int offsety = Alloffset.y() + offset.y();
		Alloffset.setY(offsety);
		action = MyLabel::None;
	}

	if (abs(Alloffset.x()) >= (this->width() / 2 + NowW / 2 - 10))    //����Xƫ��ֵ
	{
		if (Alloffset.x() > 0)
			Alloffset.setX(this->width() / 2 + NowW / 2 - 10);
		else
			Alloffset.setX(-this->width() / 2 + -NowW / 2 + 10);
	}
	if (abs(Alloffset.y()) >= (this->height() / 2 + NowH / 2 - 10))    //����Yƫ��ֵ
	{
		if (Alloffset.y() > 0)
			Alloffset.setY(this->height() / 2 + NowH / 2 - 10);
		else
			Alloffset.setY(-this->height() / 2 + -NowH / 2 + 10);

	}

	int x = this->width() / 2 + Alloffset.x() - NowW / 2;
	if (x < 0)
		x = 0;


	int y = this->height() / 2 + Alloffset.y() - NowH / 2;
	if (y < 0)
		y = 0;

	int  sx = NowW / 2 - this->width() / 2 - Alloffset.x();
	if (sx < 0)
		sx = 0;

	int  sy = NowH / 2 - this->height() / 2 - Alloffset.y();
	if (sy < 0)
		sy = 0;


	int w = (NowW - sx) > this->width() ? this->width() : (NowW - sx);
	if (w > (this->width() - x))
		w = this->width() - x;

	int h = (NowH - sy) > this->height() ? this->height() : (NowH - sy);
	if (h > (this->height() - y))
		h = this->height() - y;

	//painter.drawRect(this->x() - 1, this->y() - 1, this->width() + 1, this->height() + 1); //����
	painter.drawTiledPixmap(x + this->x(), y + this->y(), w, h, pixmap, sx, sy);             //�滭ͼ��
}

bool MyLabel::event(QEvent *event)
{
	static bool press = false;
	static QPoint PreDot;

	if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouse = dynamic_cast<QMouseEvent*>(event);

		//�ж�����Ƿ����������,�����λ���Ƿ��ڻ滭����
		if (mouse->button() == Qt::LeftButton && this->geometry().contains(mouse->pos()))
		{
			press = true;
			QApplication::setOverrideCursor(Qt::ClosedHandCursor); //���������ʽ

			PreDot = mouse->pos();
		}

	}
	else if (event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent *mouse = dynamic_cast<QMouseEvent*>(event);

		//�ж�����Ƿ�������ͷ�,��֮ǰ���ڻ滭����
		if (mouse->button() == Qt::LeftButton && press)
		{
			QApplication::setOverrideCursor(Qt::OpenHandCursor); //�Ļ������ʽ
			press = false;
		}
	}

	if (event->type() == QEvent::MouseMove)              //�ƶ�ͼƬ
	{
		if (press)
		{
			QMouseEvent *mouse = dynamic_cast<QMouseEvent*>(event);

			offset.setX(mouse->x() - PreDot.x());
			offset.setY(mouse->y() - PreDot.y());
			PreDot = mouse->pos();
			action = MyLabel::Move;
			this->update();
		}
	}
	return QWidget::event(event);
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
		pixW = Source_pixmap.width();
		pixH = Source_pixmap.height();
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
				pixmap.load(Pixmap_path);
				pixmap = pixmap.scaled(NowW, NowH, Qt::IgnoreAspectRatio);
				//this->setPixmap(pixmapTemp);

				this->update();

				ratio = 0.2;

			}
			else
			{
				ratio -= 0.1;
				NowW = ratio * pixW;
				NowH = ratio * pixH;

				this->setScaledContents(false);
				pixmap.load(Pixmap_path);
				pixmap = pixmap.scaled(NowW, NowH, Qt::IgnoreAspectRatio);
				//this->setPixmap(pixmapTemp);
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
			pixmap.load(Pixmap_path);
			pixmap = pixmap.scaled(NowW, NowH, Qt::IgnoreAspectRatio);
			//this->setPixmap(pixmapTemp);
			this->update();
		}
	}
}


//�������
//void MyLabel::mousePressEvent(QMouseEvent *event)
//{
//	OldPos = event->pos();
//	Pressed = true;
//}
//
//
////����ɿ�
//void MyLabel::mouseMoveEvent(QMouseEvent *event)
//{
//	if (!Pressed)
//		return QWidget::mouseMoveEvent(event);
//
//	this->setCursor(Qt::SizeAllCursor);
//	QPoint pos = event->pos();
//	int xPtInterval = pos.x() - OldPos.x();
//	int yPtInterval = pos.y() - OldPos.y();
//
//	XPtInterval += xPtInterval;
//	YPtInterval += yPtInterval;
//
//	action = MyLabel::Move;
//	OldPos = pos;
//
//	this->update();
//}