#pragma once

#include "Timetable.h"
#include "SerialCom.h"

//表示位置(水平位置)
const int PosType = 0;			//電車種別
const int PosTime = 2;			//電車時刻
const int PosDistination = 8;	//電車行き先
const int PosDateAndTime = 20;	//現在日時

extern CSerialCom SC;

class CDisplay
{
public:
	Ctimetable timetable;	//時刻表データ
	Ctrain train_1st;		//上段表示列車データ
	Ctrain train_2nd;		//下段表示列車データ
	
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

