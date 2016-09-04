#pragma once

#include "Timetable.h"
#include <string>

const int FNAMESIZE = 128;

class CreadFile
{
public:
	string filename;		//ƒtƒ@ƒCƒ‹–¼
	void storeDistinatinoAndTypeInfo(Ctimetable *timetable);
	void storeTrainInfo(Ctimetable *timetable);
	void storeTimetable(Ctimetable *timetable);
	void delBeforeColon(string *str);
	void CreadFile::skipSpace(string *str);
	void setFilename(Ctimetable timetable);
	int CreadFile::getHour(string buf);
	int CreadFile::getDestination(string *str);
	int CreadFile::getType(string *str);
	int CreadFile::getMinute(string *str);
	bool CreadFile::isLimitedExpress(Ctimetable *timetable, int type);
	void CreadFile::storeDiagram(string str, Ctimetable *timetable, string day);

	CreadFile(void);
	~CreadFile(void);
};

