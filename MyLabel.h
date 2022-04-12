#pragma once

#include <QLabel>
#include <QObject>
#include <QDebug.h>
#include <QApplication>
#include <QMenu>
#include <QAction.h>
#include <QFileDialog>
#include "function.h"

class MyLabel : public QLabel
{
	Q_OBJECT

public:
	MyLabel(QWidget *parent = Q_NULLPTR);	// 怎么理解参数？
	~MyLabel();

	QPixmap pixmap;
	void showImage(QString ImagePath);
	void showImage_ZI(QString ImagePath);

	Mat image;

public:
	// 右键菜单
	QMenu *RightMenu;
	QAction *Load, *Save;

protected:

	//void Label_RightMenu();							// 右键菜单
	//void contextMenuEvent(QContextMenuEvent *event);
	void paintEvent(QEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void wheelEvent(QWheelEvent *event) override;	// 滚轮
	//void mousePressEvent(QMouseEvent * event);		// 鼠标按下
	//void mouseMoveEvent(QMouseEvent * event);		// 鼠标松开

public slots:
	void Label_RightMenu();
	void LoadImage();
	void SaveImage();

private:
	float ratio = 1.0;
	int pixW, pixH;			// 图片原始宽高
	int NowW, NowH;			// 图片当前宽高

	int XPtInterval = 0;    //平移X轴的值
	int YPtInterval = 0;    //平移Y轴的值
	QPoint OldPos;
	bool Pressed = false;
};