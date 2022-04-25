#pragma once
#pragma execution_character_set("utf-8")		// ��ֹ����

#include "MainWindow.h"
#include "function.h"
#include "MyLabel.h"
#include "UAV2022.h"
#include "WidgetPic.h"


MainWindow::MainWindow(QMainWindow *parent)
	: QMainWindow(parent)			// �Ը�����г�ʼ��
{
	ui.setupUi(this);				// ?
	this->resize(QDesktopWidget().availableGeometry(this).size() * 0.8);	// �趨�������С��ʼֵΪȫ����80%
	this->setWindowTitle("���˻�Ҫ������ϵͳ");
	//this->setWindowIcon(QIcon(":/new/prefix1/2.jpg"));

	// Central Widget
	pMDI = new QMdiArea();
	this->setCentralWidget(pMDI);	// ��������
	
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
	connect(ui.result, SIGNAL(triggered()), this, SLOT(result()));
}


void MainWindow::createDockWindows()
{
	QDockWidget *dock = new QDockWidget(tr("���ƴ���"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea |
		Qt::RightDockWidgetArea);
	WidgetControl *wc_1 = new WidgetControl();
	dock->setWidget(wc_1);		// ���ô˺���֮ǰ����Ϊ widget ��Ӳ���
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
			QMessageBox::about(this, "��ʾ", "��������ͼ��");
		}
		else
		{
			QMessageBox::about(this, "��ʾ", "���Ƚ���֡���ֲ���");
		}
		break;
	case 1:
		QMessageBox::about(this, "��ʾ", "���Ƚ��о��ο�����ĵ�궨");
		break;
	case 2:
		QMessageBox::about(this, "��ʾ", "������ʾ�����귶Χ");
		break;
	case 3:
		QMessageBox::about(this, "��ʾ", "������ʾ������ͼ��");
		break;
	case 4:
		QMessageBox::about(this, "��ʾ", "���Ƚ��б�Ե��ȡ");
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
	widget_1->setWindowTitle("�� 1 ֡ͼ��");

	QString ImagePath;
	ImagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromLocal8Bit(""), tr("Image Files (*.jpg *.png)"));	// �ļ�ѡ��Ի���
	name_1 = ImagePath.toStdString();

	if (name_1 != "")
	{
		outImage = src_1 = imread(name_1);

		widget_1->SetPixmap(ImagePath);
		pMDI->addSubWindow(widget_1);			// Ƕ�뵽MainWindow�У�
		widget_1->show();

		f_1 = 1;
	}
}


void MainWindow::frame_2()
{
	widget_2 = new WidgetPic();
	widget_2->setWindowTitle("�� 2 ֡ͼ��");

	QString ImagePath;
	ImagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), QString::fromLocal8Bit(""), tr("Image Files (*.jpg *.png)"));	// �ļ�ѡ��Ի���
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
		widget_3->setWindowTitle("1. ֡����");

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
		QMessageBox::about(this, "��ʾ", "��������ͼ��");
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
		widget_4->setWindowTitle("2. ���ο�����ĵ㶨λ");

		int width = src_1.cols;
		int height = src_1.rows;
		int x11 = 0, x22 = 0, y11 = 0, y22 = 0;		// 
		int boxcenterx = 0, boxcentery = 0;			// ���ĵ�����

		unsigned char *img0 = (unsigned char  *)malloc(width * height * sizeof(unsigned char));	// ʹָ��img0ָ��һ���ѷ�����ڴ棻sizeof()�����������ʹ�С
		unsigned char *img00 = (unsigned char  *)malloc(width * height * sizeof(unsigned char));
		Mat diff_temp = diff_result.clone();

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				img00[i*width + j] = diff_temp.at<uchar>(i, j);
			}
		vector<int> Single_RowPos, Single_ColPos;		// �����������ֱ��¼�С���ͶӰʱ���س��ֵķ�Χ
		FindSingleTarget(img00, height, width, Single_RowPos, Single_ColPos);  //centerx��col��  �� centery��row��

		// ��Ŀ��߽綨λ
		x11 = Single_ColPos.front();	// Single_ColPos.front()��¼�˵�һ�γ������ص�����ţ������˻���������صĺ�����
		x22 = Single_ColPos.back();		// ����Ͼ䣬�����˻����ұ����صĺ�����
		y11 = Single_RowPos.front();
		y22 = Single_RowPos.back();

		// ��Ե�Ŀ�����ĵ����΢��
		centerx = (x11 + x22) / 2 /*- 4*/;
		centery = (y11 + y22) / 2;

		boxcenterx = centerx;
		boxcentery = centery;

		cvtColor(src_1, greyFrame, CV_BGR2GRAY);
		cvtColor(greyFrame, image_2, CV_GRAY2BGR);

		// ������ĵ�
		for (int i = -2; i <= 2; i++)
			for (int j = -2; j <= 2; j++)
			{
				image_2.at<Vec3b>(centery + i, centerx + j)[0] = 255;
				image_2.at<Vec3b>(centery + i, centerx + j)[1] = 0;
				image_2.at<Vec3b>(centery + i, centerx + j)[2] = 0;
			}

		boxwidth = abs(x11 - x22);	//abs(x11 - x22) + 70;//70//126;//
		boxheight = abs(y11 - y22); //abs(y11 - y22) + 70;//70//121;//

		// ���ƾ��ο�
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
		widget_5->setWindowTitle("3. ������ת����Χ");

		t = new int*[360 / ergodic];
		roi = floor(sqrt(boxwidth * boxwidth + boxheight * boxheight)) / 2;
		int max = 0;
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0, temp = 0;
		double the = 0, x0 = 0, y0 = 0, x = 0, y = 0;

		for (int i = 0; i < 360 / ergodic; ++i)
		{
			t[i] = new int[roi];
		}

		// ����th��360����������������ת����Χ
		for (int th = 0; th < 360; th = th + ergodic)
		{
			for (int r = 1; r < roi; r++)  //��th�Ƕ����Ծ���r����
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

				/************2021/07/15 ���������ת������*********************/

				if (r == roi - 1)
				{
					image_2.at<Vec3b>(y1 + 1, x1 + 1)[0] = 0;
					image_2.at<Vec3b>(y1 + 1, x1 + 1)[1] = 0;
					image_2.at<Vec3b>(y1 + 1, x1 + 1)[2] = 255;
				}
				/************2021/07/15 ���������ת������*********************/

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
		widget_6->setWindowTitle("4. ������ͼ��");

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
		widget_7->setWindowTitle("5. ��Ե���ͼ��");

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

		Mat img22;	// �������Ե��ȡ
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

		Mat img2;	// ��Ͻ��
		img2.create(imgg.size(), imgg.type());

		// ���Ƕȶ�Ӧ�߽��ֱ������
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

		ofstream outfile("./tmp/�߽�y����ȡ��.txt");
		for (int i = 0; i < y_pixel.size(); i++)
		{
			outfile << i << "      " << 480 - y_pixel[i] << endl;
		}
		// ��¼���Ƕȶ�Ӧ�߽�����
		ofstream outfile1("./tmp/�߽�����.txt");
		for (int i = 0; i < 360 / ergodic; i++)
		{
			outfile1 << i + 20 << "	" << m[i + 20] << endl;
		}

		MedFilterImage(ergodic, m, n);		// ��Ե��Ϣ��ֵ�˲�

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
		widget_8->setWindowTitle("6. Ҫ������");

		cir(TH, n, ergodic);   //Ŀ���Ե����

		vector<int> cur = Max_Cur(8, TH);
		vector<row_roi> p_target = FindTarget(cur, n);
		//if (p_target.size() == 0)
		//	continue;

		// ��ǻ���&�����
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

		// �������
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


void MainWindow::result()
{
	if (f_1 && f_2)
	{
		//...diff
		widget_3 = new WidgetPic();
		widget_3->setWindowTitle("1. ֡����");

		Diff2frame(src_1, src_2, diff_result);
		imwrite("./tmp/tmp.jpg", diff_result);

		widget_3->SetPixmap("./tmp/tmp.jpg");
		pMDI->addSubWindow(widget_3);
		//widget_3->show();
		remove("./tmp/tmp.jpg");
		flag = 1;

		//...locate
		widget_4 = new WidgetPic();
		widget_4->setWindowTitle("2. ���ο�����ĵ㶨λ");

		int width = src_1.cols;
		int height = src_1.rows;
		int x11 = 0, x22 = 0, y11 = 0, y22 = 0;		// 
		int boxcenterx = 0, boxcentery = 0;			// ���ĵ�����

		unsigned char *img0 = (unsigned char  *)malloc(width * height * sizeof(unsigned char));	// ʹָ��img0ָ��һ���ѷ�����ڴ棻sizeof()�����������ʹ�С
		unsigned char *img00 = (unsigned char  *)malloc(width * height * sizeof(unsigned char));
		Mat diff_temp = diff_result.clone();

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				img00[i*width + j] = diff_temp.at<uchar>(i, j);
			}
		vector<int> Single_RowPos, Single_ColPos;		// �����������ֱ��¼�С���ͶӰʱ���س��ֵķ�Χ
		FindSingleTarget(img00, height, width, Single_RowPos, Single_ColPos);  //centerx��col��  �� centery��row��

		// ��Ŀ��߽綨λ
		x11 = Single_ColPos.front();	// Single_ColPos.front()��¼�˵�һ�γ������ص�����ţ������˻���������صĺ�����
		x22 = Single_ColPos.back();		// ����Ͼ䣬�����˻����ұ����صĺ�����
		y11 = Single_RowPos.front();
		y22 = Single_RowPos.back();

		// ��Ե�Ŀ�����ĵ����΢��
		centerx = (x11 + x22) / 2 /*- 4*/;
		centery = (y11 + y22) / 2;

		boxcenterx = centerx;
		boxcentery = centery;

		cvtColor(src_1, greyFrame, CV_BGR2GRAY);
		cvtColor(greyFrame, image_2, CV_GRAY2BGR);

		// ������ĵ�
		for (int i = -2; i <= 2; i++)
			for (int j = -2; j <= 2; j++)
			{
				image_2.at<Vec3b>(centery + i, centerx + j)[0] = 255;
				image_2.at<Vec3b>(centery + i, centerx + j)[1] = 0;
				image_2.at<Vec3b>(centery + i, centerx + j)[2] = 0;
			}

		boxwidth = abs(x11 - x22);	//abs(x11 - x22) + 70;//70//126;//
		boxheight = abs(y11 - y22); //abs(y11 - y22) + 70;//70//121;//

		// ���ƾ��ο�
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
		//widget_4->show();
		remove("./tmp/locate.jpg");

		// polar_range()
		widget_5 = new WidgetPic;
		widget_5->setWindowTitle("3. ������ת����Χ");

		t = new int*[360 / ergodic];
		roi = floor(sqrt(boxwidth * boxwidth + boxheight * boxheight)) / 2;
		int max = 0;
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0, temp = 0;
		double the = 0, x0 = 0, y0 = 0, x = 0, y = 0;

		for (int i = 0; i < 360 / ergodic; ++i)
		{
			t[i] = new int[roi];
		}

		// ����th��360����������������ת����Χ
		for (int th = 0; th < 360; th = th + ergodic)
		{
			for (int r = 1; r < roi; r++)  //��th�Ƕ����Ծ���r����
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

				/************2021/07/15 ���������ת������*********************/

				if (r == roi - 1)
				{
					image_2.at<Vec3b>(y1 + 1, x1 + 1)[0] = 0;
					image_2.at<Vec3b>(y1 + 1, x1 + 1)[1] = 0;
					image_2.at<Vec3b>(y1 + 1, x1 + 1)[2] = 255;
				}
				/************2021/07/15 ���������ת������*********************/

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
		//widget_5->show();
		remove("./tmp/PolarRange.jpg");
		flag = 3;

		//...polar()
		widget_6 = new WidgetPic(720, 58, nullptr);
		widget_6->setWindowTitle("4. ������ͼ��");

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
		//widget_6->show();
		remove("./tmp/polar.jpg");
		flag = 4;

		//...edge()
		widget_7 = new WidgetPic(720, 58, nullptr);
		widget_7->setWindowTitle("5. ��Ե���ͼ��");

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

		Mat img22;	// �������Ե��ȡ
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

		Mat img2;	// ��Ͻ��
		img2.create(imgg.size(), imgg.type());

		// ���Ƕȶ�Ӧ�߽��ֱ������
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

		ofstream outfile("./tmp/�߽�y����ȡ��.txt");
		for (int i = 0; i < y_pixel.size(); i++)
		{
			outfile << i << "      " << 480 - y_pixel[i] << endl;
		}
		// ��¼���Ƕȶ�Ӧ�߽�����
		ofstream outfile1("./tmp/�߽�����.txt");
		for (int i = 0; i < 360 / ergodic; i++)
		{
			outfile1 << i + 20 << "	" << m[i + 20] << endl;
		}

		MedFilterImage(ergodic, m, n);		// ��Ե��Ϣ��ֵ�˲�

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
		//widget_7->show();
		remove("./tmp/Edge.jpg");
		flag = 5;

		//...mark
		widget_8 = new WidgetPic;
		widget_8->setWindowTitle("6. Ҫ������");

		cir(TH, n, ergodic);   //Ŀ���Ե����

		vector<int> cur = Max_Cur(8, TH);
		vector<row_roi> p_target = FindTarget(cur, n);
		//if (p_target.size() == 0)
		//	continue;

		// ��ǻ���&�����
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

		// �������
		line(outImage, Point(centerx - 10, centery), Point(centerx + 10, centery), Scalar(255, 0, 0), 1, CV_AA);
		line(outImage, Point(centerx, centery - 10), Point(centerx, centery + 10), Scalar(255, 0, 0), 1, CV_AA);

		imwrite("./tmp/result.png", outImage);

		widget_8->SetPixmap("./tmp/result.png");
		pMDI->addSubWindow(widget_8);
		widget_8->show();
		remove("./tmp/result.png");
		flag = 6;
	}
	else
	{
		QMessageBox::about(this, "��ʾ", "��������ͼ��");
	}
}


MainWindow::~MainWindow()
{
}