#pragma once

#include <QWidget>
#include "ui_WidgetMenu.h"

class WidgetMenu : public QWidget
{
	Q_OBJECT

public:
	WidgetMenu(QWidget *parent = Q_NULLPTR);
	~WidgetMenu();

private:
	Ui::WidgetMenu ui;
};
