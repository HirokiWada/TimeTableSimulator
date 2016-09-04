#pragma once

#include <tchar.h>
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

enum Color{	//文字色
	RED,
	GREEN,
	ORANGE,
};

const int WaitTime = 1;		//通信待ち時間[ms] 20ms以上

const int ReceiveBufSize = 1024;	//受信バッファサイズ（バイト単位）
const int SendBufSize = 1024;		//送信バッファサイズ（バイト単位）

class CSerialCom
{
public:
	HANDLE comPort;			//COMポート
	stringstream CmdBuf;	//送信コマンドバッファ

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

