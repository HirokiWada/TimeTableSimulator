#include "Display.h"
#include "font.h"

/*
Display��������
�i7�֐�����̂ł܂Ƃ߂��j
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
Display������ݒ�
*/
void CDisplay::SetDisplayNum(int num)
{
    DisplayNum = num;
}

/*
Display�̃h�b�g����ݒ�
*/
void CDisplay::SetDisplayDotNum(void)
{
    DisplayDotNum_V = LEDNum_V;
    DisplayDotNum_H = DisplayNum * LEDNum_H;
    DisplayDotNum_Total = DisplayDotNum_V * DisplayDotNum_H;
}

/*
1�����̃h�b�g����ݒ�
*/
void CDisplay::SetCharDotNum(int num_V, int num_H)
{
    CharDotNum_V = num_V;
    CharDotNum_H = num_H;
}

/*
�\���ł��镶������ݒ�
*/
void CDisplay::SetCharNum(void)
{
    CharNum_V = DisplayDotNum_V / CharDotNum_V;
    CharNum_H = DisplayDotNum_H / CharDotNum_H;
}

/*
�\���f�[�^�̔z���������
*/
void CDisplay::InitDisplayData(void)
{
    //���Ɋm�ۍς݂̏ꍇ�͉��
    if (pDataRed != NULL) delete pDataRed;
    if (pDataGreen != NULL) delete pDataRed;

    pDataRed = new bool[DisplayDotNum_Total];
    pDataGreen = new bool[DisplayDotNum_Total];
}

/*
�|�[�g�̐ݒ�
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
�g�p����RAM��I���i�����؊����̂��ߕs�g�p�j
*/
void CDisplay::SelectRAM(DisplayRAM SelectedRAM)
{
    _RAM = SelectedRAM;
}

/*
����񕪂̃f�[�^��Display�ɑ��M
*/
void CDisplay::SendLineData(int DotRow)
{
    int DataPos = DotRow * DisplayDotNum_H;

    //����������1���������M
    for (volatile int i = 0; i < DisplayDotNum_H; i++) {
        //���M�O����
        _CLK = LOW;
        _DataRed = OFF;
        _DataGreen = OFF;
        
        //�\���f�[�^�̔z��ON�̎��͏o��ON
        if (pDataRed[DataPos] == ON) _DataRed = ON;
        if (pDataGreen[DataPos] == ON) _DataGreen = ON;
        
        //�N���b�N�����グ�ăf�[�^��荞�݌�A�f�[�^�ʒu��i�߂�B
        _CLK = HIGH;
        DataPos++;
    }
    //1�s���\���f�[�^���M��ɍs�A�h���X��ݒ肵�������݁B
    _Addr = DotRow;
    _AddrLatch = HIGH;
    _WriteEnable = HIGH;
    _AddrLatch = LOW;
    _WriteEnable = LOW;
}

/*
�S�f�[�^��Display�ɑ��M
*/
void CDisplay::SendData(void)
{
    //��񂸂��M
    for (volatile int RowPos = 0; RowPos < DisplayDotNum_V; RowPos++) {
        SendLineData(RowPos);
    }
    InitPort();
}

/*
�\���f�[�^��S����
*/
void CDisplay::ClearCharData(void)
{
    for (int i = 0; i < DisplayDotNum_Total; i++) {
        pDataRed[i] = OFF;
        pDataGreen[i] = OFF;
    }
}

/*
�����f�[�^���쐬�i1�������j
�\���f�[�^�̔z��Ɋi�[����B
*/
void CDisplay::MakeCharData(const char *FontData, Color color, int pos_H, int pos_V)
{
    //�K�[�h�����i�f�B�X�v���C�͈͊O�̎��j
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
�����f�[�^���쐬�i������j
�\���f�[�^�̔z��Ɋi�[����B
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
        //�K�[�h�����i�f�B�X�v���C�͈͊O�̎��j
        if (isCharInDisplay(pos_H, pos_V) == false) return;

        //1�������̃f�[�^�쐬
        const char *FontData = Font[str[str_pos++] - 0x20];
        MakeCharData(FontData, color, pos_H, pos_V);
    }
}

/*
�\���f�[�^���R�s�[����B
*/
void CDisplay::CopyDotData(bool *tgt, const bool *src)
{
    for (int i = 0; i < DisplayDotNum_Total; i++) {
        tgt[i] = src[i];
    }
}

/*
�\���f�[�^�����ɂ��炷�B
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
�����ʒu��Display�����𔻒肷��B
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
�R���X�g���N�^
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
�f�X�g���N�^
*/    
CDisplay::~CDisplay()
{
    delete pDataRed;
    delete pDataGreen;
}
