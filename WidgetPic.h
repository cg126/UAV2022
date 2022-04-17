#pragma once

#include <QWidget>
#include <QLabel>
#include <qmessagebox.h>
#include "ui_WidgetPic.h"
#include <qmdisubwindow.h>

class WidgetPic : public QMdiSubWindow
{
	Q_OBJECT

public:
	WidgetPic(QMdiSubWindow *parent = Q_NULLPTR);
	WidgetPic(int width, int height, QMdiSubWindow *parent);
	~WidgetPic();

public:
	QPixmap pixmap;
	Mat Image;
	void SetPixmap(QString ImagePath);

private:
	Ui::WidgetPic ui;
};
