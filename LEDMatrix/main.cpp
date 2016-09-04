#include "Display.h"

/*include必要無いファイルは記述しないべき？
#include "mbed.h"
#include "font.h"
#include <string>
*/

CDisplay mydisplay(p21, p22, p23, p24, p25, p26, p6, p5, p15, p16, p17);    //Displayインスタンス
CStringData StrData;        //送信文字列データ
InterruptIn Sw_W(p11);      //白色ボタン
InterruptIn Sw_Y(p12);      //黄色ボタン

Serial pc(USBTX, USBRX);    //PCとのシリアル通信
string msgbuf;              //通信バッファ（受信）    

bool isReceivedCommand = false; //シリアル受信フラグ
bool isReady = false;           //レディー状態フラグ
bool isPushedSwW = false;       //白色ボタン押下フラグ
bool isPushedSwY = false;       //黄色ボタン押下フラグ
bool isChattSwW = false;        //白色ボタンチャタリング中フラグ
bool isChattSwY = false;        //黄色ボタンチャタリング中フラグ

Timer ChattTimerSwW;            //チャタリング防止用タイマー(白色ボタン用)
Timer ChattTimerSwY;            //チャタリング防止用タイマー(黄色ボタン用)
const int ChatteringTime = 200; //チャタリング判定時間[ms]

Timer PowerOffTimer;                //オートパワーオフ用タイマー
const int PowerOffTime = 10 * 60;   //パワーオフ時間[s]

/*
シリアル受信割り込み時の時のコールバック関数
シリアル受信バッファの文字をmbedの受信バッファにコピーする。
*/
void isrRx()
{
    char ch;
    ch = pc.getc(); //1文字取得
    
    //改行コードCR(0x0d)+LF(0x0a)
    //0x0dを受信後にコマンド処理する。
    if (ch == 0x0D) {
        ch = pc.getc();         //0x0aをシリアル受信バッファから取り除く
        isReceivedCommand = true;
    }
    else {
        msgbuf.push_back(ch);   //取得文字をmbed内の受信バッファに詰める
    }
}

/*
引数の開始位置を取得する
*/
int GetArgPos(string str)
{
    if (str.find(',') != string::npos) {
        return msgbuf.find(',') + 1;
    }
    else return 0;  //','が無い場合は0を返す。
}

/*
string型の引数を取得する
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
    
    //↑で動く理由が良く分かっていない。
    //「retrun buf;」とすると表示されない。
    //bufに終端'\0'が無いから？
}

/*
int型の引数を取得する
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
Color型の引数を取得する
テンプレートを使用すると一関数にできる？
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
白スイッチの状態をPCに送信する。
PCからは100msおきに問い合わせがくる。
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
    //チャタリング処理用
    //200ms以上経過したらチャタリング中フラグを降ろす。
    if (ChattTimerSwW.read_ms() > ChatteringTime) {
        ChattTimerSwW.stop();
        ChattTimerSwW.reset();
        isChattSwW = false;
    }
}

/*
黄スイッチの状態をPCに送信する
スイッチクラス作った方が良いが時間切れ。
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
コマンドに応じた処理を行う。
関数テーブルにしようかとも思ったがやめた。（メリットは？？）
*/
void AnalysisCommand()
{
    //文字データ消去
    if (msgbuf.find("CLEAR") == 0) {
        mydisplay.ClearCharData();
    }
    //文字データ設定
    else if (msgbuf.find("SET_STRING") == 0) {
        string arg = AnalysisStrArg();
        StrData.SetString(arg);
    }
    //文字位置設定（水平方向）
    else if (msgbuf.find("SET_POS_H") == 0) {
        int arg = AnalysisIntArg();
        StrData.SetPos_H(arg);
    }
    //文字位置設定（垂直方向）
    else if (msgbuf.find("SET_POS_V") == 0) {
        int arg = AnalysisIntArg();
        StrData.SetPos_V(arg);
    }
    //文字色設定
    else if (msgbuf.find("SET_COLOR") == 0) {
        Color arg = AnalysisColorArg();
        StrData.SetColor(arg);
    }
    //文字データをLEDマトリクスの形式に変換
    else if (msgbuf.find("MAKE_DATA") == 0) {
        mydisplay.MakeStringData(StrData);
    }
    //LEDマトリクスにデータ送信（表示更新）
    else if (msgbuf.find("REFRESH") == 0) {
        mydisplay.SendData();
    }
    //白スイッチの状態を送信
    else if (msgbuf.find("ASK_STATE_SW_W") == 0) {
        SendStateSwW();
    }
    //黄スイッチの状態を送信
    else if (msgbuf.find("ASK_STATE_SW_Y") == 0) {
        SendStateSwY();
    }
    //未登録のコマンドの場合
    else {
        pc.puts(msgbuf.c_str());
        pc.puts(" unknown command\n");
    }
    //mbed内の受信バッファを消去
    msgbuf.clear();
}

/*
電源ON後に表示する文字データを作成
*/
void SetInitialStrData()
{
    StrData.SetString("Start");
    StrData.SetPos_H(0);
    StrData.SetPos_V(0);
    StrData.SetColor(GREEN);
}

/*
白スイッチ押下時のコールバック関数
*/
void SwWIrq()
{
    //チャタリング中フラグがあがっているときは処理しない
    if (!isChattSwW) {
        //スイッチ押されたフラグ、チャタリング中フラグを上げる。
        isPushedSwW = true;
        isChattSwW = true;
        
        //チャタリング防止用タイマースタート
        ChattTimerSwW.start();
        
        //パワーオフタイマーをリスタート
        PowerOffTimer.reset();
        PowerOffTimer.start();
    }
}

/*
黄スイッチ押下時のコールバック関数
*/
void SwYIrq()
{
    //チャタリング中フラグがあがっているときは処理しない
   if (!isChattSwY) {
        //スイッチ押されたフラグ、チャタリング中フラグを上げる。
        isPushedSwY = true;
        isChattSwY = true;
        
        //チャタリング防止用タイマースタート
        ChattTimerSwY.start();
        
        //パワーオフタイマーをリスタート
        PowerOffTimer.reset();
        PowerOffTimer.start();
    }
}

/*
スイッチの初期設定
*/
void InitSw()
{
    Sw_W.mode(PullUp);  //入力モードをプルアップに設定
    Sw_Y.mode(PullUp);  //入力モードをプルアップに設定
    Sw_W.fall(SwWIrq);  //立下り検知時のコールバック関数を登録
    Sw_Y.fall(SwYIrq);  //立下り検知時のコールバック関数を登録
}

/*
コマンド処理が完了したことをPCに送信
*/
void SendReady()
{
    pc.puts("READY\n");
}

/*
LEDを全消灯する。
*/
void PowerOff()
{
    mydisplay.ClearCharData();
    mydisplay.SendData();
}

/*
main関数
基本的にはフラグをポーリングして、行う処理を変えるという作り。
フラグは割り込み（シリアル受信、外部割込み、タイマー）によって上げ下げしている。

OSを用いて複数タスクで処理することで処理速度の向上が見込める？

現状コマンド処理中に次のコマンドを受付けない。
どう動作するかはよく分からない。シリアル受信バッファにはたまったまま？
（今はSendReadyをするまでPCの方も待ち状態としているので、そういったことは起きないはず。）
*/
int main() {
    //LEDマトリクス、スイッチの初期化
    mydisplay.InitDisplay(4);
    InitSw();

    //シリアル受信時のコールバック関数登録
    pc.attach(isrRx, Serial::RxIrq);
    
    //オートパワーオフタイマーを開始
    PowerOffTimer.start();
    
    //最初にディスプレイに表示
    SetInitialStrData();
    mydisplay.MakeStringData(StrData);
    mydisplay.SendData();
    
    //最初にPCに表示
    pc.printf("Hello World\n");

    while(1) {
        //コマンドを受け付けたら処理を行う。
        if (isReceivedCommand) {
            AnalysisCommand();
            isReceivedCommand = false;
            SendReady();
        }
        //パワーオフタイマーが規定時間以上になった際にLEDを全消灯する。
        if (PowerOffTimer.read() > PowerOffTime) {
            PowerOff();
            PowerOffTimer.stop();
        }
    }
}