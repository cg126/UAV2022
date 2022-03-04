#include "UAV2022.h"
#include "function.h"

String name, name1;
Mat src1, src2;
Mat outImage;
Mat drone_pos;
Mat diff_result;


UAV2022::UAV2022(QWidget *parent)		// 定义构造函数（用于为成员变量赋初值）
    : QWidget(parent)
{
	ratio = 1.0;
	NowW = pixW = 320;
	NowH = pixH = 240;

	ui.setupUi(this);
	//ui.label->setScaledContents(true);
	//ui.label_2->setScaledContents(true);
	//ui.label_3->setScaledContents(true);
	//ui.label_4->setScaledContents(true);
	//ui.label_5->setScaledContents(true);
	//ui.label_6->setScaledContents(true);

	// 设置QLabel文本框对齐方式
	ui.label->setAlignment(Qt::AlignCenter);
	ui.label_2->setAlignment(Qt::AlignCenter);
	ui.label_3->setAlignment(Qt::AlignCenter);
	ui.label_4->setAlignment(Qt::AlignCenter);
	ui.label_5->setAlignment(Qt::AlignCenter);
	ui.label_6->setAlignment(Qt::AlignCenter);


	//Soure_pixmap.load("ironman.jpg");
	ui.label->setText("Hello!");
	//int w = Soure_pixmap.width();	// no use?
	//Temp_pixmap = new QPixmap;
	//Temp_pixmap->load("ironman.jpg");
	//Soure_pixmap = Soure_pixmap.scaled(pixW, pixH, Qt::IgnoreAspectRatio);	// ?
	//ui.label->setPixmap(*Temp_pixmap);
	/*ui.label_2->setPixmap(Soure_pixmap);
	ui.label_3->setPixmap(Soure_pixmap);
	ui.label_4->setPixmap(Soure_pixmap);
	ui.label_5->setPixmap(Soure_pixmap);
	ui.label_6->setPixmap(Soure_pixmap);*/

	connect(ui.pBt_LoadImage, SIGNAL(clicked()), this, SLOT(LoadImage()));	// 点击ui.pBt_LoadImage时，进行LoadImage()操作
	connect(ui.pBt_LoadImage_2, SIGNAL(clicked()), this, SLOT(LoadImage_2()));
	connect(ui.pBt_Locate, SIGNAL(clicked()), this, SLOT(FirstLocate()));
}


void UAV2022::LoadImage()
{
	QString ImagePath;
	ImagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromLocal8Bit(""), tr("Image Files (*.jpg *.png)"));	// 文件选择对话框
	QPixmap pixmap(ImagePath);

	ui.label->setPixmap(pixmap);

	name = ImagePath.toStdString();
	src1 = imread(name);
	
	/*QImage *img = new QImage;
	img->load("./resources/数据源.jpg");
	ui.label->setPixmap(QPixmap::fromImage(*img));*/	// didn't work

	//ui.label->update();		// ？
}


void UAV2022::LoadImage_2()
{
	QString ImagePath;
	ImagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromLocal8Bit(""), tr("Image Files (*.jpg *.png)"));	// 文件选择对话框

	name1 = ImagePath.toStdString();
	src2 = imread(name1);
}

void UAV2022::FirstLocate()
{
	Mat greyFrame;

	diff_result.create(greyFrame.size(), greyFrame.type());
	Diff2frame(src1, src2, diff_result);
	imwrite("./resources/tmp.jpg", diff_result);

	QPixmap pixmap("./resources/tmp.jpg");
	ui.label_2->setPixmap(pixmap);

	//system("rm ./resources/tmp.jpg");
	remove("./resources/tmp.jpg");
}


//void UAV2022::ImageEnlarge()
//{
//	ratio += 0.2*ratio;
//	int NowW = ratio * pixW;
//	int NowH = ratio * pixH;
//
//	ui.label->setScaledContents(false);		// ?
//	Soure_pixmap.load("ironman.jpg");
//	Soure_pixmap = Soure_pixmap.scaled(NowW, NowH, Qt::IgnoreAspectRatio);
//	ui.label->setPixmap(Soure_pixmap);
//}


void UAV2022::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	//int NowW = ratio *pixW;
	//int NowH = ratio *pixH;

	//if (action == Drone2021::Amplification)           //缩小
	//{
	//	ratio -= 0.1*ratio;
	//	if (ratio < 0.18)
	//		ratio = 0.1;


	//}
	//else  if (action == Drone2021::Shrink)           //放大
	//{

	//	ratio += 0.1*ratio;
	//	if (ratio > 4.5)
	//		ratio = 5.000;


	//}
	//if (action == Drone2021::Amplification || action == Drone2021::Shrink)      //更新图片
	//{
	//	NowW = ratio *pixW;
	//	NowH = ratio *pixH;
	//	Soure_pixmap.load("D:/tu/test2.jpg");                 //重新装载,因为之前的图片已经被缩放过
	//	Soure_pixmap = Soure_pixmap.scaled(NowW, NowH, Qt::KeepAspectRatio);
	//	action = Drone2021::None;

	//}

	if (action == UAV2022::Move)                    //移动
	{
		int offsetx = Alloffset.x() + offset.x();
		Alloffset.setX(offsetx);

		int offsety = Alloffset.y() + offset.y();
		Alloffset.setY(offsety);
		action = UAV2022::None;
	}

	if (abs(Alloffset.x()) >= (ui.label->width() / 2 + NowW / 2 - 10))    //限制X偏移值
	{
		if (Alloffset.x() > 0)
			Alloffset.setX(ui.label->width() / 2 + NowW / 2 - 10);
		else
			Alloffset.setX(-ui.label->width() / 2 + -NowW / 2 + 10);
	}
	if (abs(Alloffset.y()) >= (ui.label->height() / 2 + NowH / 2 - 10))    //限制Y偏移值
	{
		if (Alloffset.y() > 0)
			Alloffset.setY(ui.label->height() / 2 + NowH / 2 - 10);
		else
			Alloffset.setY(-ui.label->height() / 2 + -NowH / 2 + 10);

	}

	int x = ui.label->width() / 2 + Alloffset.x() - NowW / 2;
	if (x < 0)
		x = 0;


	int y = ui.label->height() / 2 + Alloffset.y() - NowH / 2;
	if (y < 0)
		y = 0;

	int  sx = NowW / 2 - ui.label->width() / 2 - Alloffset.x();
	if (sx < 0)
		sx = 0;

	int  sy = NowH / 2 - ui.label->height() / 2 - Alloffset.y();
	if (sy < 0)
		sy = 0;


	int w = (NowW - sx) > ui.label->width() ? ui.label->width() : (NowW - sx);
	if (w > (ui.label->width() - x))
		w = ui.label->width() - x;

	int h = (NowH - sy) > ui.label->height() ? ui.label->height() : (NowH - sy);
	if (h > (ui.label->height() - y))
		h = ui.label->height() - y;

	//painter.drawRect(ui.label->x() - 1, ui.label->y() - 1, ui.label->width() + 1, ui.label->height() + 1); //画框
	//painter.drawPixmap(x + ui.label->x(), y + ui.label->y(), Soure_pixmap, 0, 0, sx, sy);
	painter.drawTiledPixmap(x + ui.label->x(), y + ui.label->y(), w, h, Soure_pixmap, sx, sy);             //绘画图形
}