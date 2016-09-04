#include "SerialCom.h"

void CSerialCom::OpenPort()
{
	comPort = CreateFile(_T("COM7"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL); // �V���A���|�[�g���J��

	DCB dcb; // �V���A���|�[�g�̍\����񂪓���\����
	GetCommState(comPort, &dcb); // ���݂̐ݒ�l��ǂݍ���

	dcb.BaudRate = 9600; // ���x
	dcb.ByteSize = 8; // �f�[�^��
	dcb.Parity = NOPARITY; // �p���e�B
	dcb.StopBits = ONESTOPBIT; // �X�g�b�v�r�b�g��
	dcb.fOutxCtsFlow = FALSE; // ���M��CTS�t���[
	dcb.fRtsControl = RTS_CONTROL_DISABLE; // RTS�t���[

	SetCommState(comPort, &dcb); // �ύX�����ݒ�l����������
}

void CSerialCom::InitPort()
{
	OpenPort();
	SetupComm(comPort, ReceiveBufSize, SendBufSize);
}

void CSerialCom::ClosePort()
{
	CloseHandle(comPort); // �V���A���|�[�g�����
}

//�V���A���ʐM���̊֐�
string CSerialCom::SendRS232C(string msg)
{
	//Mutex�̊l��
	//WaitForSingleObject(SCMutex, 100);

	//�|�[�g�̐ݒ�
	InitPort();

	//�|�[�g�����M�i�����҂��j
	DWORD numberOfGet = 0;			//��M������
	char RsvBuf[ReceiveBufSize];	//��M�o�b�t�@

	DWORD errors;
	COMSTAT comStat;
	ClearCommError(comPort, &errors, &comStat);
	PurgeComm(comPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	int lengthOfRecieved = ReceiveBufSize;

	//�|�[�g�֑��M
	DWORD numberOfPut;
	msg += "\r\n";
	WriteFile(comPort, msg.c_str(), msg.length(), &numberOfPut, NULL);
	
	//�����҂�
	//��蓦���Ɩ������[�v�Ɋׂ�̂ŃC�}�C�`
	while (numberOfGet == 0) {
		ReadFile(comPort, RsvBuf, lengthOfRecieved, &numberOfGet, NULL); // �|�[�g�����M
		Sleep(1);
	}

	ClosePort();

	//Mutex�̉��
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
//	SCMutex = CreateMutex(NULL, TRUE, NULL);	//�r���pMutex
}


CSerialCom::~CSerialCom(void)
{
}
