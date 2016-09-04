#include "Display.h"

/*include�K�v�����t�@�C���͋L�q���Ȃ��ׂ��H
#include "mbed.h"
#include "font.h"
#include <string>
*/

CDisplay mydisplay(p21, p22, p23, p24, p25, p26, p6, p5, p15, p16, p17);    //Display�C���X�^���X
CStringData StrData;        //���M������f�[�^
InterruptIn Sw_W(p11);      //���F�{�^��
InterruptIn Sw_Y(p12);      //���F�{�^��

Serial pc(USBTX, USBRX);    //PC�Ƃ̃V���A���ʐM
string msgbuf;              //�ʐM�o�b�t�@�i��M�j    

bool isReceivedCommand = false; //�V���A����M�t���O
bool isReady = false;           //���f�B�[��ԃt���O
bool isPushedSwW = false;       //���F�{�^�������t���O
bool isPushedSwY = false;       //���F�{�^�������t���O
bool isChattSwW = false;        //���F�{�^���`���^�����O���t���O
bool isChattSwY = false;        //���F�{�^���`���^�����O���t���O

Timer ChattTimerSwW;            //�`���^�����O�h�~�p�^�C�}�[(���F�{�^���p)
Timer ChattTimerSwY;            //�`���^�����O�h�~�p�^�C�}�[(���F�{�^���p)
const int ChatteringTime = 200; //�`���^�����O���莞��[ms]

Timer PowerOffTimer;                //�I�[�g�p���[�I�t�p�^�C�}�[
const int PowerOffTime = 10 * 60;   //�p���[�I�t����[s]

/*
�V���A����M���荞�ݎ��̎��̃R�[���o�b�N�֐�
�V���A����M�o�b�t�@�̕�����mbed�̎�M�o�b�t�@�ɃR�s�[����B
*/
void isrRx()
{
    char ch;
    ch = pc.getc(); //1�����擾
    
    //���s�R�[�hCR(0x0d)+LF(0x0a)
    //0x0d����M��ɃR�}���h��������B
    if (ch == 0x0D) {
        ch = pc.getc();         //0x0a���V���A����M�o�b�t�@�����菜��
        isReceivedCommand = true;
    }
    else {
        msgbuf.push_back(ch);   //�擾������mbed���̎�M�o�b�t�@�ɋl�߂�
    }
}

/*
�����̊J�n�ʒu���擾����
*/
int GetArgPos(string str)
{
    if (str.find(',') != string::npos) {
        return msgbuf.find(',') + 1;
    }
    else return 0;  //','�������ꍇ��0��Ԃ��B
}

/*
string�^�̈������擾����
*/
string AnalysisStrArg() {
    string buf, res;
    int argpos = GetArgPos(msgbuf);

    if (argpos != 0) {
        sscanf(&msgbuf[argpos], "%s", buf);
    }
    for(int i = 0; i < mydisplay.GetCharNum_H(); i++) {
        if(buf[i] == '\0') {
            break;
        }
        res.push_back(buf[i]);
    }
    return res;
    
    //���œ������R���ǂ��������Ă��Ȃ��B
    //�uretrun buf;�v�Ƃ���ƕ\������Ȃ��B
    //buf�ɏI�['\0'����������H
}

/*
int�^�̈������擾����
*/
int AnalysisIntArg() {
    int res = 0;
    int argpos = GetArgPos(msgbuf);

    if (argpos != 0) {
        sscanf(&msgbuf[argpos], "%d", &res);
    }
    return res;
}

/*
Color�^�̈������擾����
�e���v���[�g���g�p����ƈ�֐��ɂł���H
*/
Color AnalysisColorArg() 
{
    Color res = RED;
    int argpos = GetArgPos(msgbuf);

    if (argpos != 0) {
        sscanf(&msgbuf[argpos], "%d", &res);
    }
    return res;
}

/*
���X�C�b�`�̏�Ԃ�PC�ɑ��M����B
PC�����100ms�����ɖ₢���킹������B
*/
void SendStateSwW()
{
    if (isPushedSwW) {
        pc.puts("ON\n");
        isPushedSwW = false;
    }
    else {
        pc.puts("OFF\n");
    }
    //�`���^�����O�����p
    //200ms�ȏ�o�߂�����`���^�����O���t���O���~�낷�B
    if (ChattTimerSwW.read_ms() > ChatteringTime) {
        ChattTimerSwW.stop();
        ChattTimerSwW.reset();
        isChattSwW = false;
    }
}

/*
���X�C�b�`�̏�Ԃ�PC�ɑ��M����
�X�C�b�`�N���X����������ǂ������Ԑ؂�B
*/
void SendStateSwY()
{
    if (isPushedSwY) {
        pc.puts("ON\n");
        isPushedSwY = false;}
    else {
        pc.puts("OFF\n");
    }
    if (ChattTimerSwY.read_ms() > ChatteringTime) {
        ChattTimerSwY.stop();
        ChattTimerSwY.reset();
        isChattSwY = false;
    }
}

/*
�R�}���h�ɉ������������s���B
�֐��e�[�u���ɂ��悤���Ƃ��v��������߂��B�i�����b�g�́H�H�j
*/
void AnalysisCommand()
{
    //�����f�[�^����
    if (msgbuf.find("CLEAR") == 0) {
        mydisplay.ClearCharData();
    }
    //�����f�[�^�ݒ�
    else if (msgbuf.find("SET_STRING") == 0) {
        string arg = AnalysisStrArg();
        StrData.SetString(arg);
    }
    //�����ʒu�ݒ�i���������j
    else if (msgbuf.find("SET_POS_H") == 0) {
        int arg = AnalysisIntArg();
        StrData.SetPos_H(arg);
    }
    //�����ʒu�ݒ�i���������j
    else if (msgbuf.find("SET_POS_V") == 0) {
        int arg = AnalysisIntArg();
        StrData.SetPos_V(arg);
    }
    //�����F�ݒ�
    else if (msgbuf.find("SET_COLOR") == 0) {
        Color arg = AnalysisColorArg();
        StrData.SetColor(arg);
    }
    //�����f�[�^��LED�}�g���N�X�̌`���ɕϊ�
    else if (msgbuf.find("MAKE_DATA") == 0) {
        mydisplay.MakeStringData(StrData);
    }
    //LED�}�g���N�X�Ƀf�[�^���M�i�\���X�V�j
    else if (msgbuf.find("REFRESH") == 0) {
        mydisplay.SendData();
    }
    //���X�C�b�`�̏�Ԃ𑗐M
    else if (msgbuf.find("ASK_STATE_SW_W") == 0) {
        SendStateSwW();
    }
    //���X�C�b�`�̏�Ԃ𑗐M
    else if (msgbuf.find("ASK_STATE_SW_Y") == 0) {
        SendStateSwY();
    }
    //���o�^�̃R�}���h�̏ꍇ
    else {
        pc.puts(msgbuf.c_str());
        pc.puts(" unknown command\n");
    }
    //mbed���̎�M�o�b�t�@������
    msgbuf.clear();
}

/*
�d��ON��ɕ\�����镶���f�[�^���쐬
*/
void SetInitialStrData()
{
    StrData.SetString("Start");
    StrData.SetPos_H(0);
    StrData.SetPos_V(0);
    StrData.SetColor(GREEN);
}

/*
���X�C�b�`�������̃R�[���o�b�N�֐�
*/
void SwWIrq()
{
    //�`���^�����O���t���O���������Ă���Ƃ��͏������Ȃ�
    if (!isChattSwW) {
        //�X�C�b�`�����ꂽ�t���O�A�`���^�����O���t���O���グ��B
        isPushedSwW = true;
        isChattSwW = true;
        
        //�`���^�����O�h�~�p�^�C�}�[�X�^�[�g
        ChattTimerSwW.start();
        
        //�p���[�I�t�^�C�}�[�����X�^�[�g
        PowerOffTimer.reset();
        PowerOffTimer.start();
    }
}

/*
���X�C�b�`�������̃R�[���o�b�N�֐�
*/
void SwYIrq()
{
    //�`���^�����O���t���O���������Ă���Ƃ��͏������Ȃ�
   if (!isChattSwY) {
        //�X�C�b�`�����ꂽ�t���O�A�`���^�����O���t���O���グ��B
        isPushedSwY = true;
        isChattSwY = true;
        
        //�`���^�����O�h�~�p�^�C�}�[�X�^�[�g
        ChattTimerSwY.start();
        
        //�p���[�I�t�^�C�}�[�����X�^�[�g
        PowerOffTimer.reset();
        PowerOffTimer.start();
    }
}

/*
�X�C�b�`�̏����ݒ�
*/
void InitSw()
{
    Sw_W.mode(PullUp);  //���̓��[�h���v���A�b�v�ɐݒ�
    Sw_Y.mode(PullUp);  //���̓��[�h���v���A�b�v�ɐݒ�
    Sw_W.fall(SwWIrq);  //�����茟�m���̃R�[���o�b�N�֐���o�^
    Sw_Y.fall(SwYIrq);  //�����茟�m���̃R�[���o�b�N�֐���o�^
}

/*
�R�}���h�����������������Ƃ�PC�ɑ��M
*/
void SendReady()
{
    pc.puts("READY\n");
}

/*
LED��S��������B
*/
void PowerOff()
{
    mydisplay.ClearCharData();
    mydisplay.SendData();
}

/*
main�֐�
��{�I�ɂ̓t���O���|�[�����O���āA�s��������ς���Ƃ������B
�t���O�͊��荞�݁i�V���A����M�A�O�������݁A�^�C�}�[�j�ɂ���ďグ�������Ă���B

OS��p���ĕ����^�X�N�ŏ������邱�Ƃŏ������x�̌��オ�����߂�H

����R�}���h�������Ɏ��̃R�}���h����t���Ȃ��B
�ǂ����삷�邩�͂悭������Ȃ��B�V���A����M�o�b�t�@�ɂ͂��܂����܂܁H
�i����SendReady������܂�PC�̕����҂���ԂƂ��Ă���̂ŁA�������������Ƃ͋N���Ȃ��͂��B�j
*/
int main() {
    //LED�}�g���N�X�A�X�C�b�`�̏�����
    mydisplay.InitDisplay(4);
    InitSw();

    //�V���A����M���̃R�[���o�b�N�֐��o�^
    pc.attach(isrRx, Serial::RxIrq);
    
    //�I�[�g�p���[�I�t�^�C�}�[���J�n
    PowerOffTimer.start();
    
    //�ŏ��Ƀf�B�X�v���C�ɕ\��
    SetInitialStrData();
    mydisplay.MakeStringData(StrData);
    mydisplay.SendData();
    
    //�ŏ���PC�ɕ\��
    pc.printf("Hello World\n");

    while(1) {
        //�R�}���h���󂯕t�����珈�����s���B
        if (isReceivedCommand) {
            AnalysisCommand();
            isReceivedCommand = false;
            SendReady();
        }
        //�p���[�I�t�^�C�}�[���K�莞�Ԉȏ�ɂȂ����ۂ�LED��S��������B
        if (PowerOffTimer.read() > PowerOffTime) {
            PowerOff();
            PowerOffTimer.stop();
        }
    }
}