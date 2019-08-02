#pragma once

#include "srgui_global.h"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qpushbutton.h>
#include "ScreenRecord.h"
#include <QtCore/qprocess.h>

class SRGUI_EXPORT SRGui : public QWidget
{
	Q_OBJECT

public:
	explicit SRGui(QWidget * parent = 0);
	~SRGui();

private:
	QPushButton *m_btn;
	QPushButton *startBtn;
	QPushButton *finishBtn;
	QPushButton *closeBtn;
	QProcess *process;
	void iniUI(); //UI�������ʼ��
	void iniSignalSlots(); //��ʼ���ź���ۺ���

private slots:
	void onBtnClick();
	void clickstartBtn();
	void clickfinishBtn();
	void exitSR();
};
