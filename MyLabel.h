#pragma once

#include <QLabel>
#include <QDebug.h>
#include <QApplication>
#include <QMenu>
#include <QAction.h>

class MyLabel : public QLabel
{
	Q_OBJECT

public:
	MyLabel(QWidget *parent = Q_NULLPTR);	// 怎么理解参数？
	~MyLabel();

	QPixmap pixmap;
	void showImage(QString ImagePath);

public:
	// 右键菜单
	QMenu *RightMenu;
	QAction *Open1;

public slots:
	void Label_RightMenu();

private:
	float ratio = 1.0;
	int pixW, pixH;		// 图片原始宽高
	int NowW, NowH;		// 图片当前宽高

	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void wheelEvent(QWheelEvent *event);
};