#include "Display.h"
#include "Timetable.h"
#include <time.h>
#include <iomanip>

void CDisplay::SetTimetable(Ctimetable _timetable)
{
	timetable = _timetable;
}

void CDisplay::SetTrain(Ctrain _train_1st, Ctrain _train_2nd)
{
	train_1st = _train_1st;
	train_2nd = _train_2nd;
}

Color CDisplay::GetTypeColor(string type)
{
	if (type == "L") {
		return GREEN;
	} else if (type == "R") {
		return ORANGE;
	} else if (type == "S") {
		return RED;
	} else {
		return RED;
	}
}


void CDisplay::SetStation()
{
	string str_num = timetable.num;
	string str_station = timetable.station;
	string str_dir = timetable.direction;

	SetStringData((str_num + ":" + str_station), 0, 0, GREEN);
	SetStringData("for", 0, 1, ORANGE);
	SetStringData(str_dir, 4, 1, ORANGE);
}

void CDisplay::SetType()
{
	Color color_1st;
	Color color_2nd;

	string type_1st = timetable.typeInfo[train_1st.type];
	string type_2nd = timetable.typeInfo[train_2nd.type];

	color_1st = GetTypeColor(type_1st);
	color_2nd = GetTypeColor(type_2nd);

	SetStringData(type_1st, PosType, 0, color_1st);
	SetStringData(type_2nd, PosType, 1, color_2nd);
}

void CDisplay::SetTime()
{
	stringstream time_1st;
	stringstream time_2nd;

	//書式設定要
	time_1st << setw(2) << right << train_1st.hour << ':' << setw(2) << setfill('0') << train_1st.minutes;
	time_2nd << setw(2) << right <<  train_2nd.hour << ':' << setw(2) << setfill('0') << train_2nd.minutes;

	SetStringData(time_1st.str(), PosTime, 0, ORANGE);
	SetStringData(time_2nd.str(), PosTime, 1, ORANGE);
}

void CDisplay::SetDestination()
{
	string distination_1st = timetable.destinationInfo[train_1st.destination];
	string distination_2nd = timetable.destinationInfo[train_2nd.destination];

	SetStringData(distination_1st, PosDistination, 0, GREEN);
	SetStringData(distination_2nd, PosDistination, 1, GREEN);
}

void CDisplay::SetDateAndTime(int mon, int day, int hour, int minute)
{
	stringstream date;
	stringstream time;

	//書式設定要
	date << setw(2) << setfill(' ') << mon << '/' << setw(2) << left << day;
	time << setw(2) << right << hour << ':' << setw(2) << setfill('0') << minute;

	SetStringData(date.str(), PosDateAndTime, 0, ORANGE);
	SetStringData(time.str(), PosDateAndTime, 1, ORANGE);

}

void CDisplay::SetStringData(string str, int pos_H, int pos_V, Color color)
{
	//10文字以上の時は分割する。
	//通信が受け取れないため。
	int i = 0;

	while (i < str.length()) {
		string str_send = str.substr(i, 10);	//10文字抽出

		CSerialCom SC;
		SC.CmdSetString(str_send);
		SC.CmdSetPos_H(pos_H);
		SC.CmdSetPos_V(pos_V);
		SC.CmdSetColor(color);
		SC.CmdMakeData();
		
		pos_H += 10;
		i +=10;
	}
}

bool CDisplay::isReady(void)
{
	bool res = false;
	//SC.CmdAskStateReady();
	//res = SC.isMbedReady();
	return res;
}

void CDisplay::ClearStringData()
{
	CSerialCom SC;
	SC.CmdClear();
}

void CDisplay::RefreshDisplay()
{
	CSerialCom SC;
	SC.CmdRefresh();
}

CDisplay::CDisplay(void)
{
}


CDisplay::~CDisplay(void)
{
}
