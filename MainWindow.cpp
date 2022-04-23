#pragma once
#pragma execution_character_set("utf-8")		// 防止乱码

#include "MainWindow.h"
#include "function.h"
#include "MyLabel.h"
#include "UAV2022.h"
#include "WidgetPic.h"


MainWindow::MainWindow(QMainWindow *parent)
	: QMainWindow(parent)			// 对父类进行初始化
{
	ui.setupUi(this);				// ?
	this->resize(QDesktopWidget().availableGeometry(this).size() * 0.8);	// 设定主界面大小初始值为全屏的80%
	this->setWindowTitle("无人机要害点检测系统");
	//this->setWindowIcon(QIcon(":/new/prefix1/2.jpg"));

	// Central Widget
	pMDI = new QMdiArea();
	this->setCentralWidget(pMDI);	// 中心区域
	
	createDockWindows();
	
	//m_picWidget = new WidgetPic();
	//pMDI->addSubWindow(m_picWidget);

	connect(ui.old, SIGNAL(triggered()), this, SLOT(old()));
	connect(ui.frame_1, SIGNAL(triggered()), this, SLOT(frame_1()));
	connect(ui.frame_2, SIGNAL(triggered()), this, SLOT(frame_2()));
	connect(ui.diff, SIGNAL(triggered()), this, SLOT(diff()));
	connect(ui.locate, SIGNAL(triggered()), this, SLOT(locate()));
	connect(ui.polar_range, SIGNAL(triggered()), this, SLOT(polar_range()));
	connect(ui.polar, SIGNAL(triggered()), this, SLOT(polar()));
	connect(ui.edge, SIGNAL(triggered()), this, SLOT(edge()));
}


void MainWindow::createDockWindows()
{
	QDockWidget *dock = new QDockWidget(tr("控制窗口"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea |
		Qt::RightDockWidgetArea);
	WidgetControl *wc_1 = new WidgetControl();
	dock->setWidget(wc_1);		// 调用此函数之前必须为 widget 添加布局
	addDockWidget(Qt::LeftDockWidgetArea, dock);

	//connect(ui.view_wc, SIGNAL(triggered()), dock, SLOT(toggleViewAction()));
	ui.view->addAction(dock->toggleViewAction());
}


void MainWindow::showMistake()
{
	switch (flag)
	{
	case 0:
		if (f_1 != 1 || f_2 != 1)
		{
			QMessageBox::about(this, "提示", "请先载入图像");
		}
		else
		{
			QMessageBox::about(this, "提示", "请先进行帧间差分操作");
		}
		break;
	case 1:
		QMessageBox::about(this, "提示", "请先进行矩形框和中心点标定");
		break;
	case 2:
		QMessageBox::about(this, "提示", "请先显示极坐标范围");
		break;
	case 3:
		QMessageBox::about(this, "提示", "请先显示极坐标图像");
		break;
	case 4:
		QMessageBox::about(this, "提示", "请先进行边缘提取");
		break;
	}
}


void MainWindow::old()
{
	UAV2022 *m_UAVwidget;
	m_UAVwidget = new UAV2022();
	m_UAVwidget->show();
	//pMDI->addSubWindow(m_UAVwidget);
}


void MainWindow::frame_1()
{
	widget_1 = new WidgetPic();
	widget_1->setWindowTitle("第 1 帧图像");

	QString ImagePath;
	ImagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromLocal8Bit(""), tr("Image Files (*.jpg *.png)"));	// 文件选择对话框
	name_1 = ImagePath.toStdString();

	if (name_1 != "")
	{
		outImage = src_1 = imread(name_1);

		widget_1->SetPixmap(ImagePath);
		pMDI->addSubWindow(widget_1);			// 嵌入到MainWindow中？
		widget_1->show();

		f_1 = 1;
	}
}


void MainWindow::frame_2()
{
	widget_2 = new WidgetPic();
	widget_2->setWindowTitle("第 2 帧图像");

	QString ImagePath;
	ImagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromLocal8Bit(""), tr("Image Files (*.jpg *.png)"));	// 文件选择对话框
	name_2 = ImagePath.toStdString();
	
	if (name_2 != "")
	{
		outImage = src_2 = imread(name_2);

		widget_2->SetPixmap(ImagePath);
		pMDI->addSubWindow(widget_2);
		widget_2->show();

		f_2 = 1;
	}
}


void MainWindow::diff()
{
	if (f_1 && f_2)
	{
		widget_3 = new WidgetPic();
		widget_3->setWindowTitle("1. 帧间差分");

		Diff2frame(src_1, src_2, diff_result);
		imwrite("./tmp/tmp.jpg", diff_result);

		widget_3->SetPixmap("./tmp/tmp.jpg");
		pMDI->addSubWindow(widget_3);
		widget_3->show();
		remove("./tmp/tmp.jpg");

		flag = 1;
	}
	else
	{
		QMessageBox::about(this, "提示", "请先载入图像");
	}
}


void MainWindow::locate()
{
	if (flag != 1)
	{
		showMistake();
	}
	else
	{
		widget_4 = new WidgetPic();
		widget_4->setWindowTitle("2. 矩形框和中心点定位");

		int width = src_1.cols;
		int height = src_1.rows;
		int x11 = 0, x22 = 0, y11 = 0, y22 = 0;		// 
		int boxcenterx = 0, boxcentery = 0;			// 中心点坐标

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

		// 单目标边界定位
		x11 = Single_ColPos.front();	// Single_ColPos.front()记录了第一次出现像素的列序号，即无人机最左边像素的横坐标
		x22 = Single_ColPos.back();		// 类比上句，即无人机最右边像素的横坐标
		y11 = Single_RowPos.front();
		y22 = Single_RowPos.back();

		// 针对单目标中心点进行微调
		centerx = (x11 + x22) / 2 /*- 4*/;
		centery = (y11 + y22) / 2;

		boxcenterx = centerx;
		boxcentery = centery;

		cvtColor(src_1, greyFrame, CV_BGR2GRAY);
		cvtColor(greyFrame, image_2, CV_GRAY2BGR);

		// 标记中心点
		for (int i = -2; i <= 2; i++)
			for (int j = -2; j <= 2; j++)
			{
				image_2.at<Vec3b>(centery + i, centerx + j)[0] = 255;
				image_2.at<Vec3b>(centery + i, centerx + j)[1] = 0;
				image_2.at<Vec3b>(centery + i, centerx + j)[2] = 0;
			}

		boxwidth = abs(x11 - x22);	//abs(x11 - x22) + 70;//70//126;//
		boxheight = abs(y11 - y22); //abs(y11 - y22) + 70;//70//121;//

		// 绘制矩形框
		for (int i = -boxwidth / 2 - 5; i < boxwidth / 2 + 5; i++)
		{
			image_2.at<Vec3b>(boxcentery - boxheight / 2 - 5, boxcenterx + i)[0] = 0;
			image_2.at<Vec3b>(boxcentery - boxheight / 2 - 5, boxcenterx + i)[1] = 0;
			image_2.at<Vec3b>(boxcentery - boxheight / 2 - 5, boxcenterx + i)[2] = 255;

			image_2.at<Vec3b>(boxcentery + boxheight / 2 + 5, boxcenterx + i)[0] = 0;
			image_2.at<Vec3b>(boxcentery + boxheight / 2 + 5, boxcenterx + i)[1] = 0;
			image_2.at<Vec3b>(boxcentery + boxheight / 2 + 5, boxcenterx + i)[2] = 255;
		}
		for (int i = -boxheight / 2 - 5; i <= boxheight / 2 + 5; i++)
		{
			image_2.at<Vec3b>(boxcentery - i, boxcenterx - boxwidth / 2 - 5)[0] = 0;
			image_2.at<Vec3b>(boxcentery - i, boxcenterx - boxwidth / 2 - 5)[1] = 0;
			image_2.at<Vec3b>(boxcentery - i, boxcenterx - boxwidth / 2 - 5)[2] = 255;

			image_2.at<Vec3b>(boxcentery + i, boxcenterx + boxwidth / 2 + 5)[0] = 0;
			image_2.at<Vec3b>(boxcentery + i, boxcenterx + boxwidth / 2 + 5)[1] = 0;
			image_2.at<Vec3b>(boxcentery + i, boxcenterx + boxwidth / 2 + 5)[2] = 255;
		}

		imwrite("./tmp/locate.jpg", image_2);

		widget_4->SetPixmap("./tmp/locate.jpg");
		pMDI->addSubWindow(widget_4);
		widget_4->show();
		remove("./tmp/locate.jpg");

		flag = 2;
	}	
}


void MainWindow::polar_range()
{
	if (flag != 2)
	{
		showMistake();
	}
	else
	{
		widget_5 = new WidgetPic;
		widget_5->setWindowTitle("3. 极坐标转换范围");

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
					image_2.at<Vec3b>(y1 + 1, x1 + 1)[0] = 0;
					image_2.at<Vec3b>(y1 + 1, x1 + 1)[1] = 0;
					image_2.at<Vec3b>(y1 + 1, x1 + 1)[2] = 255;
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

		imwrite("./tmp/PolarRange.jpg", image_2);

		widget_5->SetPixmap("./tmp/PolarRange.jpg");
		pMDI->addSubWindow(widget_5);
		widget_5->show();
		remove("./tmp/PolarRange.jpg");
		
		flag = 3;
	}
}


void MainWindow::polar()
{
	if (flag != 3)
	{
		showMistake();
	}
	else
	{
		widget_6 = new WidgetPic(720, 58, nullptr);
		widget_6->setWindowTitle("4. 极坐标图象");

		cv::resize(greyFrame, imgg, Size(360 / ergodic, roi));
		for (int i = 0; i < 360 / ergodic; i = i++)
		{
			for (int j = 0; j < roi; j++)
			{
				imgg.at<uchar>(j, i) = t[i][j];
			}
		}

		imwrite("./tmp/polar.jpg", imgg);

		widget_6->SetPixmap("./tmp/polar.jpg");
		pMDI->addSubWindow(widget_6);
		widget_6->show();
		remove("./tmp/polar.jpg");

		flag = 4;
	}
}


void MainWindow::edge()
{
	if (flag != 4)
	{
		showMistake();
	}
	else
	{
		widget_7 = new WidgetPic(720, 58, nullptr);
		widget_7->setWindowTitle("5. 边缘检测图像");

		img_1.create(imgg.size(), imgg.type());
		int thre = Otsu(imgg);
		threshold(imgg, img_1, 220, 255, CV_THRESH_BINARY);

		for (int th = 0; th < 360 / ergodic; th = th++)
		{
			for (int r = roi - 1; r > 0; r--)
			{
				if (img_1.at<uchar>(r, th) == 255)
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
		ofstream outfile1("./tmp/边界数据.txt");
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

		widget_7->SetPixmap("./tmp/Edge.jpg");
		pMDI->addSubWindow(widget_7);
		widget_7->show();
		remove("./tmp/Edge.jpg");

		flag = 5;
	}
}


void MainWindow::mark()
{
	if (flag != 5)
	{
		showMistake();
	}
	else
	{
		widget_8 = new WidgetPic;
		widget_8->setWindowTitle("6. 要害点标记");

		cir(TH, n, ergodic);   //目标边缘曲率

		vector<int> cur = Max_Cur(8, TH);
		vector<row_roi> p_target = FindTarget(cur, n);
		//if (p_target.size() == 0)
		//	continue;

		// 标记机翼&起落架
		for (int m = 0; m < p_target.size(); m++)
		{

			int point_x, point_y;
			point_x = centerx + p_target[m].roi * (cos(p_target[m].angle * 3.14159 / 180)); //col
			point_y = centery - p_target[m].roi * (sin(p_target[m].angle * 3.14159 / 180));

			//point_x += 2;
			//point_y -= 4;

			line(outImage, Point(point_x - 10, point_y), Point(point_x + 10, point_y), Scalar(0, 0, 255), 1, CV_AA);
			line(outImage, Point(point_x, point_y - 10), Point(point_x, point_y + 10), Scalar(0, 0, 255), 1, CV_AA);
		}

		// 标记中心
		line(outImage, Point(centerx - 10, centery), Point(centerx + 10, centery), Scalar(255, 0, 0), 1, CV_AA);
		line(outImage, Point(centerx, centery - 10), Point(centerx, centery + 10), Scalar(255, 0, 0), 1, CV_AA);

		imwrite("./tmp/result.png", outImage);

		widget_8->SetPixmap("./tmp/result.png");
		pMDI->addSubWindow(widget_8);
		widget_8->show();
		remove("./tmp/result.png");

		flag = 6;
	}
}


MainWindow::~MainWindow()
{
}

