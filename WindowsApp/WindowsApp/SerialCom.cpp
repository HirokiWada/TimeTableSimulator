#include "SerialCom.h"

void CSerialCom::OpenPort()
{
	comPort = CreateFile(_T("COM7"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL); // シリアルポートを開く

	DCB dcb; // シリアルポートの構成情報が入る構造体
	GetCommState(comPort, &dcb); // 現在の設定値を読み込み

	dcb.BaudRate = 9600; // 速度
	dcb.ByteSize = 8; // データ長
	dcb.Parity = NOPARITY; // パリティ
	dcb.StopBits = ONESTOPBIT; // ストップビット長
	dcb.fOutxCtsFlow = FALSE; // 送信時CTSフロー
	dcb.fRtsControl = RTS_CONTROL_DISABLE; // RTSフロー

	SetCommState(comPort, &dcb); // 変更した設定値を書き込み
}

void CSerialCom::InitPort()
{
	OpenPort();
	SetupComm(comPort, ReceiveBufSize, SendBufSize);
}

void CSerialCom::ClosePort()
{
	CloseHandle(comPort); // シリアルポートを閉じる
}

//シリアル通信時の関数
string CSerialCom::SendRS232C(string msg)
{
	//Mutexの獲得
	//WaitForSingleObject(SCMutex, 100);

	//ポートの設定
	InitPort();

	//ポートから受信（応答待ち）
	DWORD numberOfGet = 0;			//受信文字数
	char RsvBuf[ReceiveBufSize];	//受信バッファ

	DWORD errors;
	COMSTAT comStat;
	ClearCommError(comPort, &errors, &comStat);
	PurgeComm(comPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	int lengthOfRecieved = ReceiveBufSize;

	//ポートへ送信
	DWORD numberOfPut;
	msg += "\r\n";
	WriteFile(comPort, msg.c_str(), msg.length(), &numberOfPut, NULL);
	
	//応答待ち
	//取り逃すと無限ループに陥るのでイマイチ
	while (numberOfGet == 0) {
		ReadFile(comPort, RsvBuf, lengthOfRecieved, &numberOfGet, NULL); // ポートから受信
		Sleep(1);
	}

	ClosePort();

	//Mutexの解放
	//ReleaseMutex(SCMutex);

	string res = RsvBuf;
	return res;
}

void CSerialCom::ClearCmdBuf(void)
{
	CmdBuf.str("");
}

void CSerialCom::CmdClear(void)
{
	ClearCmdBuf();
	CmdBuf << "CLEAR";
	SendRS232C(CmdBuf.str());
}

void CSerialCom::CmdSetString(string str)
{
	ClearCmdBuf();
	CmdBuf << "SET_STRING," << str;
	SendRS232C(CmdBuf.str());
}

void CSerialCom::CmdSetPos_H(int pos_H)
{
	ClearCmdBuf();
	CmdBuf << "SET_POS_H," << pos_H;
	SendRS232C(CmdBuf.str());
}

void CSerialCom::CmdSetPos_V(int pos_V)
{
	ClearCmdBuf();
	CmdBuf << "SET_POS_V," << pos_V;
	SendRS232C(CmdBuf.str());
}

void CSerialCom::CmdSetColor(Color color)
{
	ClearCmdBuf();
	CmdBuf << "SET_COLOR," << color;
	SendRS232C(CmdBuf.str());
}

void CSerialCom::CmdMakeData(void)
{
	ClearCmdBuf();
	CmdBuf << "MAKE_DATA";
	SendRS232C(CmdBuf.str());
}

void CSerialCom::CmdRefresh(void)
{
	ClearCmdBuf();
	CmdBuf << "REFRESH";
	SendRS232C(CmdBuf.str());
}

void CSerialCom::CmdAskStateSw(char SwName, string *RsvMsg)
{
	ClearCmdBuf();
	CmdBuf << "ASK_STATE_SW_";
	CmdBuf << SwName;
	*RsvMsg = SendRS232C(CmdBuf.str());
}

CSerialCom::CSerialCom(void)
{
//	SCMutex = CreateMutex(NULL, TRUE, NULL);	//排他用Mutex
}


CSerialCom::~CSerialCom(void)
{
}
