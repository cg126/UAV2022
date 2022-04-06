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
	MyLabel(QWidget *parent = Q_NULLPTR);	// ��ô��������
	~MyLabel();

	
	void showImage_ZI(QString ImagePath);

public:
	// �Ҽ��˵�
	QMenu *RightMenu;
	QAction *Open1;
	QPixmap pixmap;

	enum Type
	{
		None = 0,
		Shrink,
		Amplification,
		Move
	};

protected:

	
	void paintEvent(QPaintEvent *event) override;
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void wheelEvent(QWheelEvent *event) override;	// ����
	void mousePressEvent(QMouseEvent * event);		// ��갴��
	void mouseMoveEvent(QMouseEvent * event);		// ����ɿ�

public slots:
	void Label_RightMenu();							// �Ҽ��˵�
	void showImage(QString ImagePath);
	void LoadImage();

private:
	int action;				// �����ź�
	float ratio = 1.0;
	int pixW, pixH;			// ͼƬԭʼ���
	int NowW, NowH;			// ͼƬ��ǰ���

	int XPtInterval = 0;    //ƽ��X���ֵ
	int YPtInterval = 0;    //ƽ��Y���ֵ
	QPoint OldPos;
	bool Pressed = false;
};


