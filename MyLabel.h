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
	MyLabel(QWidget *parent = Q_NULLPTR);	// ��ô��������
	~MyLabel();

	QPixmap pixmap;
	void showImage(QString ImagePath);
	void showImage_ZI(QString ImagePath);

	Mat image;

public:
	// �Ҽ��˵�
	QMenu *RightMenu;
	QAction *Load, *Save;

protected:

	//void Label_RightMenu();							// �Ҽ��˵�
	//void contextMenuEvent(QContextMenuEvent *event);
	void paintEvent(QEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void wheelEvent(QWheelEvent *event) override;	// ����
	void mousePressEvent(QMouseEvent * event);		// ��갴��
	void mouseMoveEvent(QMouseEvent * event);		// ����ɿ�

public slots:
	void Label_RightMenu();
	void LoadImage();
	void SaveImage();

private:
	float ratio = 1.0;
	int pixW, pixH;			// ͼƬԭʼ���
	int NowW, NowH;			// ͼƬ��ǰ���

	int XPtInterval = 0;    //ƽ��X���ֵ
	int YPtInterval = 0;    //ƽ��Y���ֵ
	QPoint OldPos;
	bool Pressed = false;
};