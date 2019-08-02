#pragma execution_character_set("utf-8")

#include "SRGui.h"
#include <qglobal.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qmessagebox.h>

void SRGui::iniUI()
{
	m_btn = new QPushButton(tr("testBtn"));
	QHBoxLayout *HLay1 = new QHBoxLayout;
	HLay1->addWidget(m_btn);

	startBtn = new QPushButton(tr("开始录制"));
	finishBtn = new QPushButton(tr("完成"));
	closeBtn = new QPushButton(tr("退出"));
	QHBoxLayout *HLay2 = new QHBoxLayout;
	HLay2->addStretch();
	HLay2->addWidget(startBtn);
	HLay2->addWidget(finishBtn);
	HLay2->addWidget(closeBtn);
	HLay2->addStretch();

	QVBoxLayout *Lay = new QVBoxLayout;
	Lay->addLayout(HLay1);
	Lay->addLayout(HLay2);

	setLayout(Lay);
}

void SRGui::iniSignalSlots()
{
	QObject::connect(startBtn, SIGNAL(clicked()), this, SLOT(clickstartBtn()));
	QObject::connect(finishBtn, SIGNAL(clicked()), this, SLOT(clickfinishBtn()));
	QObject::connect(closeBtn, SIGNAL(clicked()), this, SLOT(exitSR()));//关闭录屏exe
	QObject::connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));//关闭主界面
	QObject::connect(m_btn, SIGNAL(clicked()), this, SLOT(onBtnClick()));
	//QObject::connect(process, SIGNAL(started()), SLOT(start()));
}

SRGui::SRGui(QWidget * parent) : QWidget(parent)  //构造函数
{
	//this->resize(200, 200);
	//QVBoxLayout *lay = new QVBoxLayout(); //垂直布局
	//m_btn = new QPushButton();
	//QObject::connect(m_btn, SIGNAL(clicked()), this, SLOT(onBtnClick()));
	//lay->addWidget(m_btn);
	//this->setLayout(lay);
	this->resize(600, 500);
	process = new QProcess();
	process->start("C:/Users/qf/Desktop/screen-recording/x64/Debug/dshow_test_new.exe");
	iniUI();
	iniSignalSlots();
}

SRGui::~SRGui()
{

}

void SRGui::onBtnClick()
{
	QMessageBox *test = new QMessageBox(QMessageBox::Icon::Information, QString("test"), QString("ninja"));
	test->show();
}

void SRGui::clickstartBtn()
{
	//SCREENRECORD *s = new SCREENRECORD;
	//s->StartRecord();
	//QString path("C:/Users/qf/Desktop/screen-recording/x64/Debug/dshow_test_new.exe");
	process->write("s");
}

void SRGui::clickfinishBtn()
{
	process->write("f");
}

void SRGui::exitSR()
{
	process->write("q");
}
