#include "Display.h"
#include "font.h"

/*
Displayを初期化
（7関数あるのでまとめた）
*/
void CDisplay::InitDisplay(int displaynum, int charnum_V, int charnum_H)
{
    InitPort();
    SetDisplayNum(displaynum);
    SetDisplayDotNum();
    SetCharDotNum(charnum_V, charnum_H);
    SetCharNum();
    InitDisplayData();
    ClearCharData();
}

/*
Display枚数を設定
*/
void CDisplay::SetDisplayNum(int num)
{
    DisplayNum = num;
}

/*
Displayのドット数を設定
*/
void CDisplay::SetDisplayDotNum(void)
{
    DisplayDotNum_V = LEDNum_V;
    DisplayDotNum_H = DisplayNum * LEDNum_H;
    DisplayDotNum_Total = DisplayDotNum_V * DisplayDotNum_H;
}

/*
1文字のドット数を設定
*/
void CDisplay::SetCharDotNum(int num_V, int num_H)
{
    CharDotNum_V = num_V;
    CharDotNum_H = num_H;
}

/*
表示できる文字数を設定
*/
void CDisplay::SetCharNum(void)
{
    CharNum_V = DisplayDotNum_V / CharDotNum_V;
    CharNum_H = DisplayDotNum_H / CharDotNum_H;
}

/*
表示データの配列を初期化
*/
void CDisplay::InitDisplayData(void)
{
    //既に確保済みの場合は解放
    if (pDataRed != NULL) delete pDataRed;
    if (pDataGreen != NULL) delete pDataRed;

    pDataRed = new bool[DisplayDotNum_Total];
    pDataGreen = new bool[DisplayDotNum_Total];
}

/*
ポートの設定
*/
void CDisplay::InitPort(void)
{
    _RAMMode = LOW;
    _RAM = RAMA;
    _Addr = LOW;
    _DataGreen = LOW;
    _DataRed = LOW;
    _CLK = LOW;
    _WriteEnable = LOW;
    _AddrLatch = LOW; 
}

/*
使用するRAMを選択（自動切換えのため不使用）
*/
void CDisplay::SelectRAM(DisplayRAM SelectedRAM)
{
    _RAM = SelectedRAM;
}

/*
横一列分のデータをDisplayに送信
*/
void CDisplay::SendLineData(int DotRow)
{
    int DataPos = DotRow * DisplayDotNum_H;

    //水平方向に1文字ずつ送信
    for (volatile int i = 0; i < DisplayDotNum_H; i++) {
        //送信前準備
        _CLK = LOW;
        _DataRed = OFF;
        _DataGreen = OFF;
        
        //表示データの配列がONの時は出力ON
        if (pDataRed[DataPos] == ON) _DataRed = ON;
        if (pDataGreen[DataPos] == ON) _DataGreen = ON;
        
        //クロック立ち上げてデータ取り込み後、データ位置を進める。
        _CLK = HIGH;
        DataPos++;
    }
    //1行分表示データ送信後に行アドレスを設定し書き込み。
    _Addr = DotRow;
    _AddrLatch = HIGH;
    _WriteEnable = HIGH;
    _AddrLatch = LOW;
    _WriteEnable = LOW;
}

/*
全データをDisplayに送信
*/
void CDisplay::SendData(void)
{
    //一列ずつ送信
    for (volatile int RowPos = 0; RowPos < DisplayDotNum_V; RowPos++) {
        SendLineData(RowPos);
    }
    InitPort();
}

/*
表示データを全消去
*/
void CDisplay::ClearCharData(void)
{
    for (int i = 0; i < DisplayDotNum_Total; i++) {
        pDataRed[i] = OFF;
        pDataGreen[i] = OFF;
    }
}

/*
文字データを作成（1文字分）
表示データの配列に格納する。
*/
void CDisplay::MakeCharData(const char *FontData, Color color, int pos_H, int pos_V)
{
    //ガード条件（ディスプレイ範囲外の時）
    if (isCharInDisplay(pos_H, pos_V) == false) return;

    int CharLeftTopPos = CharDotNum_H * pos_H + CharDotNum_V * DisplayDotNum_H * pos_V;
    
    for (volatile int i = 0; i < CharDotNum_H; i++) {
        int FontDataPos = 1;
        int DisplayDataPos = CharLeftTopPos + i;

        for (volatile int j = 0; j < CharDotNum_V; j++) {
            if ((FontData[i] & FontDataPos) != 0) {
                switch(color) {
                    case RED:
                        pDataRed[DisplayDataPos] = ON;
                        break;
                    case GREEN:
                        pDataGreen[DisplayDataPos] = ON;
                        break;
                    case ORANGE:
                        pDataRed[DisplayDataPos] = ON;
                        pDataGreen[DisplayDataPos] = ON;
                        break;
                }
            }
            FontDataPos <<= 1;
            DisplayDataPos += DisplayDotNum_H;
        }
    }
}

/*
文字データを作成（文字列）
表示データの配列に格納する。
*/
void CDisplay::MakeStringData(CStringData StrData)
{
    string str = StrData.GetString();
    int pos_H_begin = StrData.GetPos_H();
    int pos_V = StrData.GetPos_V();
    Color color = StrData.GetColor();
    
    int pos_H_end = pos_H_begin + str.length();
    int str_pos = 0;
    
    for (int pos_H = pos_H_begin; pos_H < pos_H_end; pos_H++) {
        //ガード条件（ディスプレイ範囲外の時）
        if (isCharInDisplay(pos_H, pos_V) == false) return;

        //1文字分のデータ作成
        const char *FontData = Font[str[str_pos++] - 0x20];
        MakeCharData(FontData, color, pos_H, pos_V);
    }
}

/*
表示データをコピーする。
*/
void CDisplay::CopyDotData(bool *tgt, const bool *src)
{
    for (int i = 0; i < DisplayDotNum_Total; i++) {
        tgt[i] = src[i];
    }
}

/*
表示データを左にずらす。
*/
void CDisplay::ShiftDataLeft(int ShiftNum)
{
    bool *pTempDataRed = new bool[DisplayDotNum_Total];
    bool *pTempDataGreen = new bool[DisplayDotNum_Total];
    int DataPos = 0;

    CopyDotData(pTempDataRed, pDataRed);
    CopyDotData(pTempDataGreen, pDataGreen);

    for (int i = 0; i < ShiftNum; i++) {
        for (int pos_V = 0; pos_V < DisplayDotNum_V; pos_V++) {
            for (int pos_H = 0; pos_H < DisplayDotNum_H; pos_H++) {
                if (pos_H == (DisplayDotNum_H - 1)) {
                    pDataRed[DataPos] = pTempDataRed[DataPos - (DisplayDotNum_H -1)];
                    pDataGreen[DataPos] = pTempDataGreen[DataPos - (DisplayDotNum_H -1)];
                }
                else {
                    pDataRed[DataPos] = pTempDataRed[DataPos + 1];
                    pDataGreen[DataPos] = pTempDataGreen[DataPos + 1];
                }
                DataPos++;
            }
        }
    }
    delete pTempDataRed;
    delete pTempDataGreen;
}

/*
文字位置がDisplay内かを判定する。
*/
bool CDisplay::isCharInDisplay(int pos_H, int pos_V)
{
    if ((pos_H >= CharNum_H) || (pos_V >= CharNum_V)) {
        return false;
    }
    else {
        return true;
    }
}

/*
コンストラクタ
*/
CDisplay::CDisplay(PinName RAMMode, PinName RAM, PinName Addr0, PinName Addr1, PinName Addr2, PinName Addr3,
                   PinName DataRed, PinName DataGreen, PinName CLK, PinName WriteEnable, PinName AddrLatch)
                   : _RAMMode(RAMMode),_RAM(RAM), _Addr(Addr0, Addr1, Addr2, Addr3),
                   _DataRed(DataRed), _DataGreen(DataGreen), _CLK(CLK), _WriteEnable(WriteEnable), _AddrLatch(AddrLatch),
                   DisplayNum(0), DisplayDotNum_V(0), DisplayDotNum_H(0),  DisplayDotNum_Total(0), CharDotNum_V(0), CharDotNum_H(0),
                   CharNum_V(0), CharNum_H(0), pDataRed(NULL), pDataGreen(NULL)
{
    ;
}

/*
デストラクタ
*/    
CDisplay::~CDisplay()
{
    delete pDataRed;
    delete pDataGreen;
}
