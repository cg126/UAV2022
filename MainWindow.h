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

	//�м���������
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
	int f_1 = 0, f_2 = 0, flag = 0;

	//��ض���
	int action;
	//ͼƬԭʼ���
	int pixW;
	int pixH;
	//ͼƬ��Ŀǰ���
	int NowW;
	int NowH;

	int m[401] = { 0 };
	int n[401] = { 0 };
	double TH[401] = { 0 };
	double TH_MID[401] = { 0 };
	double TH_MIDD[401] = { 0 };

	// ���ο����ĵ�
	int centerx = 0, centery = 0;
	int ergodic = 1, roi = 0;
	int **t;

	Mat image_2;
	Mat imgg;	// �洢������ͼ��
	Mat img_1;	// �洢������ͼ���ֵ�����
	Mat outImage;

	QRect Paint;		//�滭����
	QLabel label;

	QPoint offset;
	QPoint Alloffset;
	//bool event(QEvent *event);
	//void wheelEvent(QWheelEvent *event);     //��껬���¼�
};