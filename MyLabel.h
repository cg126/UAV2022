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
	MyLabel(QWidget *paren = Q_NULLPTR);	// ��ô��������
	~MyLabel();

public:
	// �Ҽ��˵�
	QMenu *RightMenu;
	QAction *Open1;

public slots:
	void Label_RightMenu();

private:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
};