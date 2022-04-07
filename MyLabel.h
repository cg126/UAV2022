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
	QAction *Load, *Save;
	QPixmap pixmap;
	QPixmap Source_pixmap;


	enum Type
	{
		None = 0,
		Shrink,
		Amplification,
		Move
	};

protected:

	//xk
	QString Pixmap_path;
	bool event(QEvent *event);
	QPoint offset;			// �˴��ƶ������ƫ����
	QPoint Alloffset;		// ����ԭ������ƫ����


	void paintEvent(QPaintEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void wheelEvent(QWheelEvent *event) override;	// ����
	//void mousePressEvent(QMouseEvent * event);		// ��갴��
	//void mouseMoveEvent(QMouseEvent * event);		// ����ɿ�

public slots:
	void Label_RightMenu();							// �Ҽ��˵�
	void showImage(QString ImagePath);
	void LoadImage();
	void SaveImage();

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


