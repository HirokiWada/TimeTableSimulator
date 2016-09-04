#include "Switch.h"

void CSwitch::RefreshState(void)
{
	CSerialCom SC;
	string RsvMsg;
	SC.CmdAskStateSw(name, &RsvMsg);

	if (RsvMsg.find("ON") == 0) {
		state = ON;
	}
	else {
		state = OFF;
	}
}

void CSwitch::SetName(char SwName)
{
	name = SwName;
}

CSwitch::CSwitch(char SwName)
{
	name = SwName;
	state = OFF;
}


CSwitch::~CSwitch(void)
{
}
