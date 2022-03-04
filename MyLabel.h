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
	MyLabel(QWidget *paren = Q_NULLPTR);	// 怎么理解参数？
	~MyLabel();

public:
	// 右键菜单
	QMenu *RightMenu;
	QAction *Open1;

public slots:
	void Label_RightMenu();

private:
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
};