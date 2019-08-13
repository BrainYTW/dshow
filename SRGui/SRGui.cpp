#pragma execution_character_set("utf-8")

#include "SRGui.h"
#include <qglobal.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qmessagebox.h>

void SRGui::iniUI()
{
	player = new QMediaPlayer;

	videoWidget = new QVideoWidget;
	QHBoxLayout *HLay1 = new QHBoxLayout; //ˮƽ����
	HLay1->addWidget(videoWidget);

	player->setVideoOutput(videoWidget);
	previewBtn = new QPushButton(tr("Ԥ��"));

	startBtn = new QPushButton(tr("��ʼ¼��"));
	finishBtn = new QPushButton(tr("���"));
	closeBtn = new QPushButton(tr("�˳�"));
	QHBoxLayout *HLay2 = new QHBoxLayout; //ˮƽ����
	HLay2->addStretch();
	HLay2->addWidget(startBtn);
	HLay2->addWidget(finishBtn);
	HLay2->addWidget(previewBtn);
	HLay2->addWidget(closeBtn);
	HLay2->addStretch();

	QVBoxLayout *Lay = new QVBoxLayout; //��ֱ����
	Lay->addLayout(HLay1);
	Lay->addLayout(HLay2);

	setLayout(Lay);
}

void SRGui::iniSignalSlots()
{
	QObject::connect(startBtn, SIGNAL(clicked()), this, SLOT(clickstartBtn()));
	QObject::connect(previewBtn, SIGNAL(clicked()), this, SLOT(preview()));
	QObject::connect(finishBtn, SIGNAL(clicked()), this, SLOT(clickfinishBtn()));
	QObject::connect(closeBtn, SIGNAL(clicked()), this, SLOT(exitSR()));//�ر�¼��exe
	QObject::connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));//�ر�������
}

SRGui::SRGui(QWidget * parent) : QWidget(parent) ,  
  m_playerState(QMediaPlayer::StoppedState)      //���캯��
{
	//this->resize(200, 200);
	this->resize(600, 500);
	process = new QProcess();
	process->start("C:/Users/qf/Desktop/screen-recording/x64/Debug/dshow_test_new.exe");
	iniUI();
	iniSignalSlots();
}

SRGui::~SRGui()
{

}

void SRGui::preview()
{
	player->setMedia(QUrl::fromLocalFile("C:\\Users\\qf\\Desktop\\test\\Example.avi"));
	m_playerState = QMediaPlayer::PlayingState;
	player->play();
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
