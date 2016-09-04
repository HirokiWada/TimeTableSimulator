#pragma once

#include "Timetable.h"
#include "SerialCom.h"

//�\���ʒu(�����ʒu)
const int PosType = 0;			//�d�Ԏ��
const int PosTime = 2;			//�d�Ԏ���
const int PosDistination = 8;	//�d�ԍs����
const int PosDateAndTime = 20;	//���ݓ���

extern CSerialCom SC;

class CDisplay
{
public:
	Ctimetable timetable;	//�����\�f�[�^
	Ctrain train_1st;		//��i�\����ԃf�[�^
	Ctrain train_2nd;		//���i�\����ԃf�[�^
	
	void SetTimetable(Ctimetable _timetable);
	void SetTrain(Ctrain _train_1st, Ctrain _train_2nd);
	Color GetTypeColor(string type);
	void SetStation();
	void SetType();
	void SetTime();
	void SetDestination();
	void SetDateAndTime(int mon, int day, int hour, int minute);
	
	void SetStringData(string str, int pos_H, int pos_V, Color color);
	void ClearStringData();
	void RefreshDisplay();
	bool isReady();

	CDisplay(void);
	~CDisplay(void);
};

