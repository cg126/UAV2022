#pragma once

#include <QtWidgets/QWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDesktopWidget>
#include <QMainWindow>
#include "function.h"
#include "ui_UAV2022.h"
#include "UAV2022.h"
#include "ui_MainWindow.h"
#include "WidgetPic.h"
#include <qmdiarea.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QMainWindow *parent = Q_NULLPTR);
	~MainWindow();

	//中间试验区域
	QMdiArea *pMDI;
	WidgetPic *widget_1, *widget_2, *widget_3, *widget_4, *widget_5, *widget_6, *widget_7, *widget_8;

public slots:
	void on_old_triggered();
	void frame_1();
	void frame_2();
	void diff();
	void locate();
	void polar_range();
	void polar();
	void edge();
	void mark();

private:
	Ui:: MainWindow ui;

	String name_1, name_2;
	Mat src_1, src_2;
	Mat diff_result;
	Mat greyFrame;
	int boxwidth = 0, boxheight = 0;

	//相关动作
	int action;
	//图片原始宽高
	int pixW;
	int pixH;
	//图片的目前宽高
	int NowW;
	int NowH;

	int m[401] = { 0 };
	int n[401] = { 0 };
	double TH[401] = { 0 };
	double TH_MID[401] = { 0 };
	double TH_MIDD[401] = { 0 };

	// 矩形框中心点
	int centerx = 0, centery = 0;
	int ergodic = 1, roi = 0;
	int **t;

	Mat image_2;
	Mat imgg;	// 存储极坐标图像
	Mat img_1;	// 存储极坐标图像二值化结果
	Mat outImage;

	QRect Paint;		//绘画区域
	QLabel label;

	QPoint offset;
	QPoint Alloffset;
	//bool event(QEvent *event);
	//void wheelEvent(QWheelEvent *event);     //鼠标滑轮事件
};