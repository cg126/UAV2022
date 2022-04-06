#pragma once

#include <QLabel>
#include <QObject>
#include <QDebug.h>
#include <QApplication>
#include <QMenu>
#include <QAction.h>
#include <QPainter>
#include <QFileDialog>
#include "function.h"

class MyLabel : public QLabel
{
	Q_OBJECT

public:
	MyLabel(QWidget *parent = Q_NULLPTR);	// 怎么理解参数？
	~MyLabel();

	
	void showImage_ZI(QString ImagePath);

public:
	// 右键菜单
	QMenu *RightMenu;
	QAction *Open1;
	QPixmap pixmap;

	enum Type
	{
		None = 0,
		ZoomOut,
		ZoomIn,
		Move
	};

protected:

	
	void paintEvent(QPaintEvent *event) override;
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void wheelEvent(QWheelEvent *event) override;	// 滚轮
	void mousePressEvent(QMouseEvent * event);		// 鼠标按下
	void mouseMoveEvent(QMouseEvent * event);		// 鼠标松开

public slots:
	void Label_RightMenu();							// 右键菜单
	void showImage(QString ImagePath);
	void LoadImage();

private:
	int action;				// 动作信号
	float ratio = 1.0;
	int pixW, pixH;			// 图片原始宽高
	int NowW, NowH;			// 图片当前宽高

	int XPtInterval = 0;    //平移X轴的值
	int YPtInterval = 0;    //平移Y轴的值
	QPoint OldPos;
	bool Pressed = false;
};


