#include "ReadFile.h"
#include <string>
#include <fstream>
#define EXCEPTLIMITEDEXPRESS

using namespace std;
const int BUFSIZE = 128;
const int OFFSETASCIITODEC = 48;


void CreadFile::delBeforeColon(string *str)
{
	int pos_colon = str -> find(":");
	str -> erase(0, pos_colon + 1);
}

void CreadFile::skipSpace(string *str)
{
	while(str ->at(0) == ' '){
		str->erase(0, 1);
	}
}

void CreadFile::setFilename(Ctimetable timetable)
{
	//filename = "data/" + timetable.station + " " + timetable.route + " " + timetable.direction + ".txt";
	filename = "data/" + timetable.num + "_" + timetable.station + "_" + timetable.route + "_" + timetable.direction + ".txt";
}

void CreadFile::storeDistinatinoAndTypeInfo(Ctimetable *timetable)
{
	ifstream ifs(filename.c_str());
	string buf;
	
	while(getline(ifs, buf)){
		//空行の場合はスキップ
		if(buf.size() == 0) continue;
		//英大文字の場合目的地データ格納
		if(('A' <= buf[0]) && (buf[0] <= 'Z')){
			delBeforeColon(&buf);
			timetable -> destinationInfo.push_back(buf);
		}
		//英小文字の場合目的地データ格納
		else if(('a' <= buf[0]) && (buf[0] <= 'z')){
			delBeforeColon(&buf);
			timetable -> typeInfo.push_back(buf);
		}
	}
}

int CreadFile::getHour(string str)
{
	int hour = 0;
	int pos_colon = str.find(":");

	for(int i=0; i < pos_colon; i++){
		hour *= 10;
		hour += (str[i] - OFFSETASCIITODEC);
	}
	return hour;
}

int CreadFile::getDestination(string *str)
{
	int res = 0;
	if(('A' <= str->at(0) && (str->at(0) <= 'Z'))){
		res = str->at(0) - 64;	//A=1となるようにオフセット
		str->erase(0, 1);
	}
	return res;
}

int CreadFile::getType(string *str)
{
	int res = 0;
	if(('a' <= str->at(0) && (str->at(0) <= 'z'))){
		res = str->at(0) - 96;	//a=1となるようにオフセット
		str->erase(0, 1);
	}
	return res;
}

int CreadFile::getMinute(string *str)
{
	int minute = 0;

	for(int i=0; i < 2; i++){
		minute *= 10;
		minute += (str->at(i) - OFFSETASCIITODEC);
	}
	str->erase(0, 2);
	return minute;
}

bool CreadFile::isLimitedExpress(Ctimetable *timetable, int num)
{
	string type;
	type = timetable ->typeInfo.at(num);
	//if((type == "普通") || (type == "快速") || (type == "新快速")){
	if((type == "L") || (type == "R") || (type == "S")){
		return false;
	}
	else return true;
}

void CreadFile::storeDiagram(string str, Ctimetable *timetable, string day)
{
	int hour = getHour(str);
	delBeforeColon(&str);

	while(str.size() > 0){
		//空白は飛ばす
		skipSpace(&str);

		Ctrain buf;
		buf.hour = hour;
		buf.destination = getDestination(&str);
		buf.type = getType(&str);
		buf.minutes = getMinute(&str);

#ifdef EXCEPTLIMITEDEXPRESS
		if(isLimitedExpress(timetable, buf.type)) continue;
#endif
		if(day == "[MON]") timetable ->WeekdayDiagram.push_back(buf);
		else if(day == "[SAT]") timetable ->SaturdayDiagram.push_back(buf);
		else if(day == "[SUN]") timetable ->HolidayDiagram.push_back(buf);
	}
}

void CreadFile::storeTrainInfo(Ctimetable *timetable)
{
	ifstream ifs(filename.c_str());
	string buf;

	while(getline(ifs, buf)){
		//空行の場合はスキップ
		if(buf.size() == 0) continue;
		//[MON]
		if(buf.find("[MON]") == 0){
			//コメント行スキップ
			getline(ifs, buf);
			while(getline(ifs, buf)){
				//空行まで読んだら抜ける
				if(buf.size() == 0) break;
				storeDiagram(buf, timetable, "[MON]");
			}
		}
		if(buf.find("[SAT]") == 0){
			//コメント行スキップ
			getline(ifs, buf);
			while(getline(ifs, buf)){
				//空行まで読んだら抜ける
				if(buf.size() == 0) break;
				storeDiagram(buf, timetable, "[SAT]");
			}
		}
		if(buf.find("[SUN]") == 0){
			//コメント行スキップ
			getline(ifs, buf);
			while(getline(ifs, buf)){
				//空行まで読んだら抜ける
				if(buf.size() == 0) break;
				storeDiagram(buf, timetable, "[SUN]");
			}
		}
	}
}

void CreadFile::storeTimetable(Ctimetable *timetable)
{
	storeDistinatinoAndTypeInfo(timetable);
	storeTrainInfo(timetable);
}


CreadFile::CreadFile(void)
{
}


CreadFile::~CreadFile(void)
{
}
