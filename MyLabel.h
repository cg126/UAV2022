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
	MyLabel(QWidget *parent = Q_NULLPTR);	// ��ô��������
	~MyLabel();

	QPixmap pixmap;
	void showImage(QString ImagePath);

public:
	// �Ҽ��˵�
	QMenu *RightMenu;
	QAction *Open1;

public slots:
	void Label_RightMenu();

private:
	float ratio = 1.0;
	int pixW, pixH;		// ͼƬԭʼ���
	int NowW, NowH;		// ͼƬ��ǰ���

	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void wheelEvent(QWheelEvent *event);
};