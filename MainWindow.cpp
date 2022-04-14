#pragma once
#pragma execution_character_set("utf-8")		// 防止乱码

#include "MainWindow.h"
#include "function.h"
#include "MyLabel.h"
#include "UAV2022.h"
#include "WidgetPic.h"


MainWindow::MainWindow(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);		// ?

	this->setWindowTitle("无人机要害点检测系统");
	//this->setWindowIcon(QIcon(":/new/prefix1/2.jpg"));
	connect(ui.old, SIGNAL(triggered()), this, SLOT(on_old_triggered));
	/*QLabel lab;
	lab.setAlignment(Qt::AlignCenter);
	lab.setGeometry(30, 50, 480, 640);
	lab.setText("hello");*/

	//中间实验数据显示区域
	pMDI = new QMdiArea();
	this->setCentralWidget(pMDI); //核心部件
	

	//picWidget = new WidgetPic();

	//DI->addSubWindow(m_picWidget);
}


void MainWindow::on_old_triggered()
{
	UAV2022 *m_UAVwidget;
	m_UAVwidget = new UAV2022();
	m_UAVwidget->show();
	//pMDI->addSubWindow(m_UAVwidget);
}


//void MainWindow::read()
//{
//	picWidget = new WidgetPic();
//
//	
//	
//	
//	
//	
//	
//	
//	
//	pMDI->addSubWindow(m_UAVwidget);
//}




MainWindow::~MainWindow()
{

}
