#include "Timetable.h"

Ctrain Ctimetable::SearchNextTrain(vector <Ctrain> diagram, int hh, int mm)
{
	int minute_comp = hh * 60 + mm;		//分換算して比較
	if (hh < 2) {
		minute_comp += (hh + 24) * 60;	//0時以降対応
	}

	for (int i = 0; i < diagram.size(); i++) {
		int minute = diagram[i].hour * 60 + diagram[i].minutes;
		if (diagram[i].hour < 2) {
			minute += ((diagram[i].hour) + 24) * 60;	//0時以降対応
		}
		if (minute > minute_comp) {
			return diagram[i];
		}
	}
	//該当する時刻が無い場合最初の時刻（始発）を返す。
	return diagram[0];
}

Ctrain Ctimetable::SearchNextTrain(vector <Ctrain> diagram, Ctrain train_before)
{
	int minute_comp = train_before.hour * 60 + train_before.minutes;		//分換算して比較
	if (train_before.hour < 2) {
		minute_comp += ((train_before.hour) + 24) * 60;	//0時以降対応
	}
	
	for (int i = 0; i < diagram.size(); i++) {
		int minute = diagram[i].hour * 60 + diagram[i].minutes;
		if (diagram[i].hour < 2) {
			minute += ((diagram[i].hour) + 24) * 60;	//0時以降対応
		}
		if (minute > minute_comp) {
			return diagram[i];	
		}
	}
	//該当する時刻が無い場合最初の時刻（始発）を返す。
	return diagram[0];
}

Ctrain Ctimetable::GetNextTrain(int wday, int hour, int minute)
{
	switch (wday){
	case SUNDAY:	//休日ダイヤ
		return SearchNextTrain(HolidayDiagram, hour, minute);
		break;
	case SATURDAY:	//土曜ダイヤ
		return SearchNextTrain(SaturdayDiagram, hour, minute);
		break;
	default:		//平日ダイヤ
		return SearchNextTrain(WeekdayDiagram, hour, minute);
		break;
	}
}

Ctrain Ctimetable::GetNextTrain(int wday, Ctrain train_before)
{
	switch (wday){
	case SUNDAY:	//休日ダイヤ
		return SearchNextTrain(HolidayDiagram, train_before);
		break;
	case SATURDAY:	//土曜ダイヤ
		return SearchNextTrain(SaturdayDiagram, train_before);
		break;
	default:		//平日ダイヤ
		return SearchNextTrain(WeekdayDiagram, train_before);
		break;
	}
}

Ctimetable::Ctimetable(void)
{
	station = "";					//駅名
	route = "";						//路線名
	direction = "";					//方面
	destinationInfo.push_back("");	//行き先
	typeInfo.push_back("L");		//種別
}


Ctimetable::~Ctimetable(void)
{
}
