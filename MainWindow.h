#pragma once

#include <QtWidgets/QWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QDockWidget>
#include <QtWidgets>
#include "function.h"
#include "UAV2022.h"
#include "ui_MainWindow.h"
#include "WidgetPic.h"
#include "WidgetControl.h"
#include <qmdiarea.h>		// ?

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
	void old();
	void frame_1();
	void frame_2();
	void diff();
	void locate();
	void polar_range();
	void polar();
	void edge();
	void mark();
	void result();

private:
	Ui:: MainWindow ui;

	void createToolBar();
	void createStatusBar();
	void createDockWindows();
	void showMistake();

	String name_1, name_2;
	Mat src_1, src_2;
	Mat diff_result;
	Mat greyFrame;
	int boxwidth = 0, boxheight = 0;
	int f_1 = 0, f_2 = 0, flag = 0;

	int m[401] = { 0 };
	int n[401] = { 0 };
	double TH[401] = { 0 };
	double TH_MID[401] = { 0 };
	double TH_MIDD[401] = { 0 };

	// ���ο����ĵ�
	int centerx = 0, centery = 0;
	int ergodic = 1, roi = 0;
	int **t;

	const int L = 7;

	Mat image_2;	// ������ת����Χ��ͼ��
	Mat imgg;		// �洢������ͼ��
	Mat img_1;		// �洢������ͼ���ֵ�����
	Mat outImage;	// Ҫ������ͼ��

	QPoint offset;
	QPoint Alloffset;
};