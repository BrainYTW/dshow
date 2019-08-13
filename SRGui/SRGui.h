#pragma once

#include "srgui_global.h"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qpushbutton.h>
#include "ScreenRecord.h"
#include <QtCore/qprocess.h>
#include <QtMultimedia/qmediaplayer.h>
#include <QtMultimediaWidgets/qvideowidget.h>

class SRGUI_EXPORT SRGui : public QWidget
{
	Q_OBJECT

public:
	explicit SRGui(QWidget * parent = 0);
	~SRGui();

private:
	QMediaPlayer *player;
	QVideoWidget *videoWidget;
	QPushButton *previewBtn;
	QPushButton *startBtn;
	QPushButton *finishBtn;
	QPushButton *closeBtn;
	QProcess *process;
	QMediaPlayer::State  m_playerState;
	void iniUI(); //UI创建与初始化
	void iniSignalSlots(); //初始化信号与槽函数

private slots:
	void preview();
	void clickstartBtn();
	void clickfinishBtn();
	void exitSR();
};
