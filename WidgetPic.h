#pragma once

#include <QWidget>
#include <QLabel>
#include "ui_WidgetPic.h"

class WidgetPic : public QWidget
{
	Q_OBJECT

public:
	WidgetPic(QWidget *parent = Q_NULLPTR);
	~WidgetPic();

private:
	Ui::WidgetPic ui;
};
