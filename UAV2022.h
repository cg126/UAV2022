#pragma once

#include <QtWidgets/QWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QMainWindow>
#include "ui_UAV2022.h"

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

private:
	//��ض���
	int action;
	//ͼƬԭʼ���
	int pixW;
	int pixH;
	float ratio;		// ����

	//ͼƬ��Ŀǰ���
	int NowW;
	int NowH;

	QRect Paint;		//�滭����
	QLabel label;

	QPoint offset;
	QPoint Alloffset;
	//bool event(QEvent *event);
	//void wheelEvent(QWheelEvent *event);     //��껬���¼�
public slots:
	void LoadImage();
	void LoadImage_2();

	//void ImageEnlarge();
	//void ImageShrink();


private:
    Ui::UAV2022Class ui;
	void paintEvent(QPaintEvent *event);
};
