#pragma once

#include <tchar.h>
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

enum Color{	//�����F
	RED,
	GREEN,
	ORANGE,
};

const int WaitTime = 1;		//�ʐM�҂�����[ms] 20ms�ȏ�

const int ReceiveBufSize = 1024;	//��M�o�b�t�@�T�C�Y�i�o�C�g�P�ʁj
const int SendBufSize = 1024;		//���M�o�b�t�@�T�C�Y�i�o�C�g�P�ʁj

class CSerialCom
{
public:
	HANDLE comPort;			//COM�|�[�g
	stringstream CmdBuf;	//���M�R�}���h�o�b�t�@

	CSerialCom(void);
	~CSerialCom(void);
	void ClearCmdBuf();
	void CmdClear();
	void CmdSetString(string str);
	void CmdSetPos_H(int pos_H);
	void CmdSetPos_V(int pos_V);
	void CmdSetColor(Color color);
	void CmdMakeData();
	void CmdRefresh();
	void CmdAskStateSw(char SwName, string *RsvMsg);

	void OpenPort();
	void ClosePort();
	void InitPort();

	string SendRS232C(string msg);
	void ReceiveRS232C();
};

