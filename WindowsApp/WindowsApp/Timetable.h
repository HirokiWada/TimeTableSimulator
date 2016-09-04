#pragma once
#include<vector>
#include<iostream>
#include<string>
#include"Train.h"

using namespace std;

enum Wday{
	SUNDAY,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
};

class Ctimetable
{
public:
	string num;		//時刻表番号
	string station;		//駅名
	string route;		//路線名
	string direction;	//方面

	vector <Ctrain> WeekdayDiagram;		//平日ダイヤ
	vector <Ctrain> SaturdayDiagram;	//土曜日ダイヤ
	vector <Ctrain> HolidayDiagram;		//休日ダイヤ

	vector <string> destinationInfo;	//行き先データ
	vector <string> typeInfo;			//種別データ

	Ctrain GetNextTrain(int wday, int hour, int minute);
	Ctrain GetNextTrain(int wday, Ctrain train_before);
	Ctrain SearchNextTrain(vector <Ctrain> diagram, int hour, int minute);
	Ctrain SearchNextTrain(vector <Ctrain> diagram, Ctrain train);

	Ctimetable(void);	//コンストラクタ
	~Ctimetable(void);	//デストラクタ
};

