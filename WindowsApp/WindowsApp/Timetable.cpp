#include "Timetable.h"

Ctrain Ctimetable::SearchNextTrain(vector <Ctrain> diagram, int hh, int mm)
{
	int minute_comp = hh * 60 + mm;		//�����Z���Ĕ�r
	for (int i = 0; i < diagram.size(); i++) {
		int minute = diagram[i].hour * 60 + diagram[i].minutes;
		if (minute > minute_comp) {
			return diagram[i];	//0���ȍ~�̌�����Ή�
		}
	}
}

Ctrain Ctimetable::SearchNextTrain(vector <Ctrain> diagram, Ctrain train_before)
{
	int minute_comp = train_before.hour * 60 + train_before.minutes;		//�����Z���Ĕ�r
	for (int i = 0; i < diagram.size(); i++) {
		int minute = diagram[i].hour * 60 + diagram[i].minutes;
		if (minute > minute_comp) {
			return diagram[i];	//0���ȍ~�̌�����Ή�
		}
	}
}

Ctrain Ctimetable::GetNextTrain(int wday, int hour, int minute)
{
	switch (wday){
	case SUNDAY:	//�x���_�C��
		return SearchNextTrain(HolidayDiagram, hour, minute);
		break;
	case SATURDAY:	//�y�j�_�C��
		return SearchNextTrain(SaturdayDiagram, hour, minute);
		break;
	default:		//�����_�C��
		return SearchNextTrain(WeekdayDiagram, hour, minute);
		break;
	}
}

Ctrain Ctimetable::GetNextTrain(int wday, Ctrain train_before)
{
	switch (wday){
	case SUNDAY:	//�x���_�C��
		return SearchNextTrain(HolidayDiagram, train_before);
		break;
	case SATURDAY:	//�y�j�_�C��
		return SearchNextTrain(SaturdayDiagram, train_before);
		break;
	default:		//�����_�C��
		return SearchNextTrain(WeekdayDiagram, train_before);
		break;
	}
}

Ctimetable::Ctimetable(void)
{
	station = "";					//�w��
	route = "";						//�H����
	direction = "";					//����
	destinationInfo.push_back("");	//�s����
	typeInfo.push_back("L");		//���
}


Ctimetable::~Ctimetable(void)
{
}
