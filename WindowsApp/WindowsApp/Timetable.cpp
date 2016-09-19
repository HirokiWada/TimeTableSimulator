#include "Timetable.h"

Ctrain Ctimetable::SearchNextTrain(vector <Ctrain> diagram, int hh, int mm)
{
	int minute_comp = hh * 60 + mm;		//•ªŠ·Z‚µ‚Ä”äŠr
	if (hh < 2) {
		minute_comp += (hh + 24) * 60;	//0ˆÈ~‘Î‰
	}

	for (int i = 0; i < diagram.size(); i++) {
		int minute = diagram[i].hour * 60 + diagram[i].minutes;
		if (diagram[i].hour < 2) {
			minute += ((diagram[i].hour) + 24) * 60;	//0ˆÈ~‘Î‰
		}
		if (minute > minute_comp) {
			return diagram[i];
		}
	}
	//ŠY“–‚·‚é‚ª–³‚¢ê‡Å‰‚Ìin”­j‚ğ•Ô‚·B
	return diagram[0];
}

Ctrain Ctimetable::SearchNextTrain(vector <Ctrain> diagram, Ctrain train_before)
{
	int minute_comp = train_before.hour * 60 + train_before.minutes;		//•ªŠ·Z‚µ‚Ä”äŠr
	if (train_before.hour < 2) {
		minute_comp += ((train_before.hour) + 24) * 60;	//0ˆÈ~‘Î‰
	}
	
	for (int i = 0; i < diagram.size(); i++) {
		int minute = diagram[i].hour * 60 + diagram[i].minutes;
		if (diagram[i].hour < 2) {
			minute += ((diagram[i].hour) + 24) * 60;	//0ˆÈ~‘Î‰
		}
		if (minute > minute_comp) {
			return diagram[i];	
		}
	}
	//ŠY“–‚·‚é‚ª–³‚¢ê‡Å‰‚Ìin”­j‚ğ•Ô‚·B
	return diagram[0];
}

Ctrain Ctimetable::GetNextTrain(int wday, int hour, int minute)
{
	switch (wday){
	case SUNDAY:	//‹x“úƒ_ƒCƒ„
		return SearchNextTrain(HolidayDiagram, hour, minute);
		break;
	case SATURDAY:	//“y—jƒ_ƒCƒ„
		return SearchNextTrain(SaturdayDiagram, hour, minute);
		break;
	default:		//•½“úƒ_ƒCƒ„
		return SearchNextTrain(WeekdayDiagram, hour, minute);
		break;
	}
}

Ctrain Ctimetable::GetNextTrain(int wday, Ctrain train_before)
{
	switch (wday){
	case SUNDAY:	//‹x“úƒ_ƒCƒ„
		return SearchNextTrain(HolidayDiagram, train_before);
		break;
	case SATURDAY:	//“y—jƒ_ƒCƒ„
		return SearchNextTrain(SaturdayDiagram, train_before);
		break;
	default:		//•½“úƒ_ƒCƒ„
		return SearchNextTrain(WeekdayDiagram, train_before);
		break;
	}
}

Ctimetable::Ctimetable(void)
{
	station = "";					//‰w–¼
	route = "";						//˜Hü–¼
	direction = "";					//•û–Ê
	destinationInfo.push_back("");	//s‚«æ
	typeInfo.push_back("L");		//í•Ê
}


Ctimetable::~Ctimetable(void)
{
}
