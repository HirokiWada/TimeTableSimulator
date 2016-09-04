#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <Windows.h>

using namespace std;

class Dir
{
public:
	vector<string> read(string folder);
	string GetStationNum(string filename);
	string GetStationName(string filename);
	string GetRouteName(string filename);
	string GetDirectionName(string filename);

	// コンストラクタ
	Dir(void);
	// デストラクタ
	~Dir(void);
};
