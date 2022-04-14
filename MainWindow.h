#pragma once

#include <QtWidgets/QWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QMainWindow>
#include "function.h"
#include "ui_UAV2022.h"
#include "UAV2022.h"
#include "ui_MainWindow.h"
#include "WidgetPic.h"
#include <qmdiarea.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QMainWindow *parent = Q_NULLPTR);
	~MainWindow();

	//�м���������
	QMdiArea *pMDI;

	WidgetPic *m_picWidget;

public slots:
	void on_old_triggered();

private:
	Ui:: MainWindow ui;
};