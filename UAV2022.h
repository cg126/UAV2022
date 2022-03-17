#pragma once

#include <QtWidgets/QWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QMainWindow>
#include "ui_UAV2022.h"
#include "function.h"

class UAV2022 : public QWidget		// UAV2022�̳�QWidget
{
    Q_OBJECT		// 

public:
    UAV2022(QWidget *parent = Q_NULLPTR);	// �������캯��

public:
	QPixmap Soure_pixmap;	// QPixmap����Ķ���ɴ洢ͼ��
	QPixmap *Temp_pixmap;
	enum Type {
		None = 0,
		Shrink,         //��С
		Amplification,   //�Ŵ�
		Move
	};

	//void showImage(string path);

private:
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

	// ���ο����ĵ�
	int centerx = 0, centery = 0;
	int ergodic = 1, roi = 0;
	int **t;

	Mat image2;
	Mat imgg;	// �洢������ͼ��
	Mat img1;	// �洢������ͼ���ֵ�����

	QRect Paint;		//�滭����
	QLabel label;

	QPoint offset;
	QPoint Alloffset;
	//bool event(QEvent *event);
	//void wheelEvent(QWheelEvent *event);     //��껬���¼�

public slots:
	void LoadImage();
	void LoadImage_2();
	void Diff();
	void Locate();
	void PolarRange();
	void Polar();
	void Edge();

	//void ImageEnlarge();
	//void ImageShrink();

private:
    Ui::UAV2022Class ui;
	void paintEvent(QPaintEvent *event);
};
