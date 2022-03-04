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

class UAV2022 : public QWidget		// UAV2022继承QWidget
{
    Q_OBJECT		// 

public:
    UAV2022(QWidget *parent = Q_NULLPTR);	// 声明构造函数

public:
	QPixmap Soure_pixmap;	// QPixmap定义的对象可存储图像
	QPixmap *Temp_pixmap;
	enum Type {
		None = 0,
		Shrink,         //缩小
		Amplification,   //放大
		Move
	};

private:
	//相关动作
	int action;
	//图片原始宽高
	int pixW;
	int pixH;
	float ratio;		// 比例

	//图片的目前宽高
	int NowW;
	int NowH;

	QRect Paint;		//绘画区域
	QLabel label;

	QPoint offset;
	QPoint Alloffset;
	//bool event(QEvent *event);
	//void wheelEvent(QWheelEvent *event);     //鼠标滑轮事件

public slots:
	void LoadImage();
	void LoadImage_2();
	void FirstLocate();

	//void ImageEnlarge();
	//void ImageShrink();


private:
    Ui::UAV2022Class ui;
	void paintEvent(QPaintEvent *event);
};
