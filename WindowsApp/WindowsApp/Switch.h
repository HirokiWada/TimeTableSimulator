#pragma once

#include "SerialCom.h"

const bool ON = true;
const bool OFF = false;

class CSwitch
{
private:
	char name;
	bool state;

public:
	void RefreshState();
	bool GetState() {
		return state;
	}
	char GetName() {
		return name;
	}
	void SetName(char SwName);
	CSwitch(char SwName);
	~CSwitch(void);
};

