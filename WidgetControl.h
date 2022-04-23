#pragma once

#include <QWidget>
#include "ui_WidgetControl.h"

class WidgetControl : public QWidget
{
	Q_OBJECT

public:
	WidgetControl(QWidget *parent = Q_NULLPTR);
	~WidgetControl();

private:
	Ui::WidgetControl ui;
};