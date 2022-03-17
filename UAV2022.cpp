#include "UAV2022.h"
#include "function.h"

String name1, name2;
Mat src1, src2;
Mat diff_result;
Mat greyFrame;
int boxwidth = 0, boxheight = 0;


UAV2022::UAV2022(QWidget *parent)	// 定义构造函数（用于为成员变量赋初值）
    : QWidget(parent)
{
	NowW = pixW = 320;
	NowH = pixH = 240;

	ui.setupUi(this);
	//ui.label->setScaledContents(true);
	//ui.label_2->setScaledContents(true);
	//ui.label_3->setScaledContents(true);
	//ui.label_4->setScaledContents(true);
	//ui.label_5->setScaledContents(true);
	//ui.label_6->setScaledContents(true);

	/*设置QLabel文本框对齐方式*/
	//ui.label->setAlignment(Qt::AlignCenter);
	//ui.label_2->setAlignment(Qt::AlignCenter);
	//ui.label_3->setAlignment(Qt::AlignCenter);
	//ui.label_4->setAlignment(Qt::AlignCenter);
	//ui.label_5->setAlignment(Qt::AlignCenter);
	//ui.label_6->setAlignment(Qt::AlignCenter);

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
	connect(ui.pBt_Diff, SIGNAL(clicked()), this, SLOT(Diff()));
	connect(ui.pBt_Locate, SIGNAL(clicked()), this, SLOT(Locate()));
	connect(ui.pBt_PolarRange, SIGNAL(clicked()), this, SLOT(PolarRange()));
	connect(ui.pBt_Polar, SIGNAL(clicked()), this, SLOT(Polar()));
	connect(ui.pBt_Edge, SIGNAL(clicked()), this, SLOT(Edge()));
}


/*图像放大*/
/*void UAV2022::ImageEnlarge()
{
	ratio += 0.2*ratio;
	int NowW = ratio * pixW;
	int NowH = ratio * pixH;

	ui.label->setScaledContents(false);		// ?
	Soure_pixmap.load("ironman.jpg");
	Soure_pixmap = Soure_pixmap.scaled(NowW, NowH, Qt::IgnoreAspectRatio);
	ui.label->setPixmap(Soure_pixmap);
}*/


/*显示图像*/
//void UAV2022::showImage(string path)
//{
//	QString qpath;
//	qpath = QString::fromStdString(path);
//	QPixmap pixmap(qpath);
//	ui.label_2->setPixmap(pixmap);
//	remove(path);
//}

// 加载第一张图像
void UAV2022::LoadImage()
{
	QString ImagePath;
	ImagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromLocal8Bit(""), tr("Image Files (*.jpg *.png)"));	// 文件选择对话框
	QPixmap pixmap(ImagePath);

	ui.label->setPixmap(pixmap);

	name1 = ImagePath.toStdString();
	src1 = imread(name1);
	
	/*QImage *img = new QImage;
	img->load("./resources/数据源.jpg");
	ui.label->setPixmap(QPixmap::fromImage(*img));*/	// didn't work

	//ui.label->update();		// ？
}


// 加载第二张图像
void UAV2022::LoadImage_2()
{
	QString ImagePath;
	ImagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromLocal8Bit(""), tr("Image Files (*.jpg *.png)"));	// 文件选择对话框

	name2 = ImagePath.toStdString();
	src2 = imread(name2);

	QPixmap pixmap(ImagePath);
	ui.label_2->setPixmap(pixmap);
}


void UAV2022::Diff()
{
	//diff_result.create(greyFrame.size(), greyFrame.type());
	Diff2frame(src1, src2, diff_result);
	imwrite("./tmp/tmp.jpg", diff_result);

	QPixmap pixmap("./tmp/tmp.jpg");
	ui.label_2->setPixmap(pixmap);
	//system("rm ./resources/tmp.jpg");
	remove("./tmp/tmp.jpg");
}


// 矩形框和中心点标定
void UAV2022::Locate()
{
	int width = src1.cols;
	int height = src1.rows;
	int x11 = 0, x22 = 0, y11 = 0, y22 = 0;
	int boxcenterx = 0, boxcentery = 0;

	unsigned char *img0 = (unsigned char  *)malloc(width * height * sizeof(unsigned char));	// 使指针img0指向一块已分配的内存；sizeof()返回数据类型大小
	unsigned char *img00 = (unsigned char  *)malloc(width * height * sizeof(unsigned char));
	Mat diff_temp = diff_result.clone();


	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			img00[i*width + j] = diff_temp.at<uchar>(i, j);
		}
	vector<int> Single_RowPos, Single_ColPos;		// 用两个向量分别记录行、列投影时像素出现的范围
	FindSingleTarget(img00, height, width, Single_RowPos, Single_ColPos);  //centerx是col号  ， centery是row号


	//单目标边界定位
	//col
	x11 = Single_ColPos.front();	// Single_ColPos.front()记录了第一次出现像素的列序号，即无人机最左边像素的横坐标
	x22 = Single_ColPos.back();		// 类比上句，即无人机最右边像素的横坐标

	//row
	y11 = Single_RowPos.front();
	y22 = Single_RowPos.back();

	//框选区域的中心点

	//针对单目标中心点进行微调
	centerx = (x11 + x22) / 2 /*- 4*/;
	centery = (y11 + y22) / 2;

	boxcenterx = centerx;
	boxcentery = centery;

	cvtColor(src1, greyFrame, CV_BGR2GRAY);
	cvtColor(greyFrame, image2, CV_GRAY2BGR);
	
	//标记中心点
	for (int i = -2; i <= 2; i++)
		for (int j = -2; j <= 2; j++)
		{
			image2.at<Vec3b>(centery + i, centerx + j)[0] = 255;
			image2.at<Vec3b>(centery + i, centerx + j)[1] = 0;
			image2.at<Vec3b>(centery + i, centerx + j)[2] = 0;
		}

	boxwidth = abs(x11 - x22);//abs(x11 - x22) + 70;//70//126;//
	boxheight = abs(y11 - y22); //abs(y11 - y22) + 70;//70//121;//

	for (int i = -boxwidth / 2 - 5; i < boxwidth / 2 + 5; i++)
	{
		image2.at<Vec3b>(boxcentery - boxheight / 2 - 5, boxcenterx + i)[0] = 0;
		image2.at<Vec3b>(boxcentery - boxheight / 2 - 5, boxcenterx + i)[1] = 0;
		image2.at<Vec3b>(boxcentery - boxheight / 2 - 5, boxcenterx + i)[2] = 255;

		image2.at<Vec3b>(boxcentery + boxheight / 2 + 5, boxcenterx + i)[0] = 0;
		image2.at<Vec3b>(boxcentery + boxheight / 2 + 5, boxcenterx + i)[1] = 0;
		image2.at<Vec3b>(boxcentery + boxheight / 2 + 5, boxcenterx + i)[2] = 255;
	}
	for (int i = -boxheight / 2 - 5; i <= boxheight / 2 + 5; i++)
	{
		image2.at<Vec3b>(boxcentery - i, boxcenterx - boxwidth / 2 - 5)[0] = 0;
		image2.at<Vec3b>(boxcentery - i, boxcenterx - boxwidth / 2 - 5)[1] = 0;
		image2.at<Vec3b>(boxcentery - i, boxcenterx - boxwidth / 2 - 5)[2] = 255;

		image2.at<Vec3b>(boxcentery + i, boxcenterx + boxwidth / 2 + 5)[0] = 0;
		image2.at<Vec3b>(boxcentery + i, boxcenterx + boxwidth / 2 + 5)[1] = 0;
		image2.at<Vec3b>(boxcentery + i, boxcenterx + boxwidth / 2 + 5)[2] = 255;
	}

	imwrite("./tmp/locate.jpg", image2);

	QPixmap pixmap("./tmp/locate.jpg");
	ui.label_3->setPixmap(pixmap);
	remove("./tmp/locate.jpg");
}


// 极坐标范围展示
void UAV2022::PolarRange()
{
	t = new int*[360 / ergodic];
	roi = floor(sqrt(boxwidth * boxwidth + boxheight * boxheight)) / 2;
	int max = 0;
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0, temp = 0;
	double the = 0, x0 = 0, y0 = 0, x = 0, y = 0;

	for (int i = 0; i < 360 / ergodic; ++i)
	{
		t[i] = new int[roi];
	}

	// 变量th以360°遍历，标出极坐标转换范围
	for (int th = 0; th < 360; th = th + ergodic)
	{
		for (int r = 1; r < roi; r++)  //在th角度上以距离r遍历
		{

			the = th * 3.14159 / 180;
			x0 = r * cos(the);
			y0 = r * sin(the);
			x = x0 + centerx;
			y = centery - y0;

			x1 = floor(x);
			x2 = ceil(x);
			y1 = floor(y);
			y2 = ceil(y);

			/************2021/07/15 标出极坐标转换区域*********************/

			if (r == roi - 1)
			{
				image2.at<Vec3b>(y1 + 1, x1 + 1)[0] = 0;
				image2.at<Vec3b>(y1 + 1, x1 + 1)[1] = 0;
				image2.at<Vec3b>(y1 + 1, x1 + 1)[2] = 255;
			}
			/************2021/07/15 标出极坐标转换区域*********************/


			if (x1 == x2 || y1 == y2)
			{
				t[th / ergodic][r] = greyFrame.at<uchar>(y1, x1);
			}
			else
			{
				if (x > 0 && x < greyFrame.cols - 1 && y > 0 && y < greyFrame.rows - 1)
				{
					temp = (x2 - x)*(y2 - y)*greyFrame.at<uchar>(y1, x1) + (x - x1)*(y - y1)*greyFrame.at<uchar>(y2, x2) + (x2 - x)*(y - y1)*greyFrame.at<uchar>(y1, x2) + (x - x1)*(y2 - y)*greyFrame.at<uchar>(y2, x1);
					t[th / ergodic][r] = temp;
				}
				else
				{
					if (x <= 0 | x > greyFrame.cols | y <= 0 | y >= greyFrame.rows)
					{
						int temp_x, temp_y;
						if (x <= 0)
						{
							temp_x = 0;
						}
						else if (x > greyFrame.cols)
						{
							temp_x = greyFrame.cols - 1;
						}
						else
						{
							temp_x = floor(x);
						}

						if (y <= 0)
						{
							temp_y = 0;
						}
						else if (y > greyFrame.rows)
						{
							temp_y = greyFrame.rows - 1;
						}
						else
						{
							temp_y = floor(y);
						}
						t[th / ergodic][r] = greyFrame.at<uchar>(temp_y, temp_x);

					}
				}
				// t[th/ergodic][r]=image.at<uchar>(y1,x1);  
			}
		}
	}

	imwrite("./tmp/PolarRange.jpg", image2);

	QPixmap pixmap("./tmp/PolarRange.jpg");
	ui.label_4->setPixmap(pixmap);
	//system("rm ./tmp/PolarRange.jpg");
}


// 极坐标图像
void UAV2022::Polar()
{
	cv::resize(greyFrame, imgg, Size(360 / ergodic, roi));
	for (int i = 0; i < 360 / ergodic; i = i++)
	{
		for (int j = 0; j < roi; j++)
		{
			imgg.at<uchar>(j, i) = t[i][j];
		}
	}

	imwrite("./tmp/polar.jpg", imgg);
	QPixmap pixmap("./tmp/Polar.jpg");
	ui.label_4->setPixmap(pixmap);
	//system("rm ./tmp/Polar.jpg");
}


// 边缘提取
void UAV2022::Edge()
{
	img1.create(imgg.size(), imgg.type());
	int thre = Otsu(imgg);
	threshold(imgg, img1, 220, 255, CV_THRESH_BINARY);

	for (int th = 0; th < 360 / ergodic; th = th++)
	{
		for (int r = roi - 1; r > 0; r--)
		{
			if (img1.at<uchar>(r, th) == 255)
			{
				m[th + 20] = r;
				break;
			}
		}
	}
	for (int i = 0; i < 20; i++)
	{
		m[(360 / ergodic) + 20 + i] = m[20 + i];
		m[0 + i] = m[360 / ergodic + i];
	}

	Mat img22;	// 极坐标边缘提取
	img22.create(imgg.size(), imgg.type());
	for (int i = 0; i < 360 / ergodic; i++)
	{
		for (int j = 1; j < roi; j++)
		{
			img22.at<uchar>(j, i) = 0;
		}
	}
	for (int i = 0; i < 360 / ergodic; i++)
	{
		img22.at<uchar>(m[i + 20], i) = 255;
	}

	Mat img2;	// 拟合结果
	img2.create(imgg.size(), imgg.type());


	// 各角度对应边界的直角坐标
	vector<int> y_pixel;
	double angle1;
	for (int i = 0; i < 360; i++)
	{
		angle1 = i * 3.14159 / 180;
		double xx0, yy0;
		int xx1, yy1;
		xx0 = centerx + m[i + 20] * cos(angle1);
		yy0 = centery - m[i + 20] * sin(angle1);

		yy1 = ceil(yy0);
		y_pixel.push_back(yy1);
	}

	ofstream outfile("./tmp/边界y坐标取反.txt");
	for (int i = 0; i < y_pixel.size(); i++)
	{
		outfile << i << "      " << 480 - y_pixel[i] << endl;
	}
	// 记录各角度对应边界数据
	ofstream outfile1(".\\新数据中间结果\\边界数据.txt");
	for (int i = 0; i < 360 / ergodic; i++)
	{
		outfile1 << i + 20 << "	" << m[i + 20] << endl;
	}

	MedFilterImage(ergodic, m, n);		// 边缘信息均值滤波

	for (int i = 0; i < 360 / ergodic; i++)
	{
		for (int j = 1; j < roi; j++)
		{
			img2.at<uchar>(j, i) = 0;
		}

	}
	for (int i = 0; i < 360 / ergodic; i++)
	{
		img2.at<uchar>(n[i + 20], i) = 255;
	}

	imwrite("./tmp/Edge.jpg", img2);
	QPixmap pixmap("./tmp/Edge.jpg");
	ui.label_4->setPixmap(pixmap);
	//system("rm ./tmp/Edge.jpg");
}


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