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

//1枚のLEDマトリクスのLED数（縦：16、横32）
const int LEDNum_V = 16;
const int LEDNum_H = 32;

class CDisplay
{
private:
    int DisplayNum;         //Display枚数
    int DisplayDotNum_V;    //Displayドット数（縦）
    int DisplayDotNum_H;    //Displayドット数（横）
    int DisplayDotNum_Total;//Displayドット数（全部）
    int CharDotNum_V;       //文字ドット数（縦）
    int CharDotNum_H;       //文字ドット数（横）
    int CharNum_V;          //文字数（縦）
    int CharNum_H;          //文字数（横）
    
    bool *pDataRed;         //赤表示データへのポインタ
    bool *pDataGreen;       //緑表示データへのポインタ
    
    DigitalOut  _RAMMode,   //内部バッファRAM切替モード選択
                _RAM,       //RAM選択（自動切換えのため使用していない。）
                _DataRed,   //赤表示データ入力
                _DataGreen, //緑表示データ入力
                _CLK,       //データ取り込み用クロック入力
                _WriteEnable,//内部バッファRAMへの書き込み信号
                _AddrLatch; //アドレスラッチ信号
    BusOut _Addr;           //内部バッファRAMアドレス

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