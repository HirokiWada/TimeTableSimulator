#pragma once

#include "StringData.h"

using namespace std;

enum Dout{
    LOW = 0,
    HIGH = 1,
};

enum DisplayRAM{
    RAMA = 1,
    RAMB = 0,
};

enum LED{
    ON = true,
    OFF = false,
};

//1����LED�}�g���N�X��LED���i�c�F16�A��32�j
const int LEDNum_V = 16;
const int LEDNum_H = 32;

class CDisplay
{
private:
    int DisplayNum;         //Display����
    int DisplayDotNum_V;    //Display�h�b�g���i�c�j
    int DisplayDotNum_H;    //Display�h�b�g���i���j
    int DisplayDotNum_Total;//Display�h�b�g���i�S���j
    int CharDotNum_V;       //�����h�b�g���i�c�j
    int CharDotNum_H;       //�����h�b�g���i���j
    int CharNum_V;          //�������i�c�j
    int CharNum_H;          //�������i���j
    
    bool *pDataRed;         //�ԕ\���f�[�^�ւ̃|�C���^
    bool *pDataGreen;       //�Ε\���f�[�^�ւ̃|�C���^
    
    DigitalOut  _RAMMode,   //�����o�b�t�@RAM�ؑփ��[�h�I��
                _RAM,       //RAM�I���i�����؊����̂��ߎg�p���Ă��Ȃ��B�j
                _DataRed,   //�ԕ\���f�[�^����
                _DataGreen, //�Ε\���f�[�^����
                _CLK,       //�f�[�^��荞�ݗp�N���b�N����
                _WriteEnable,//�����o�b�t�@RAM�ւ̏������ݐM��
                _AddrLatch; //�A�h���X���b�`�M��
    BusOut _Addr;           //�����o�b�t�@RAM�A�h���X

public:
    void InitDisplay(int displaynum = 1, int charnum_V = 8, int charnum_H = 5);
    void SetDisplayNum(int num);
    void SetDisplayDotNum(void);
    void SetCharDotNum(int num_V, int num_H);
    void SetCharNum(void);
    void InitDisplayData(void);
    void SelectRAM(DisplayRAM SelectedRAM);  
    void InitPort(void);
    void SendLineData(int DotRow);
    void SendData(void);
    void MakeCharData(const char *FontData, Color color, int pos_H, int pos_V);
    void ClearCharData(void);
    void MakeStringData(CStringData StrData);
    void CopyDotData(bool *tgt, const bool *src);
    void ShiftDataLeft(int ShiftNum);
    bool isCharInDisplay(int pos_H, int pos_V);
    int GetCharNum_H() {return CharNum_H;}

    CDisplay(PinName RAMMode, PinName RAM, PinName Addr0, PinName Addr1, PinName Addr2, PinName Addr3,
             PinName DataGreen, PinName DataRed, PinName CLK, PinName WriteEnable, PinName AddrLatch);
    ~CDisplay();
};