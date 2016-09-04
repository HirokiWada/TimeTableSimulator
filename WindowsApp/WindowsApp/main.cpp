#include "ReadFile.h"
#include "Timetable.h"
#include "Train.h"
#include <sstream>
#include "Display.h"
#include "Switch.h"
#include <time.h>
#include "Dir.h"

bool FlgDisplayTimetable = false;
bool FlgDisplayStation = false;
bool FlgWatchSw = false;
bool FlgSwW = false;
bool FlgSwY = false;

int state;			//Display状態

enum State {
	STATE_DISPLAY,	//時刻表表示状態
	STATE_SELECT,	//駅選択画面表示状態
};

//10sおきに実行される関数
//時刻表表示を更新するフラグを立てる
DWORD WINAPI cbDispayTimetable(LPVOID lpParam)
{
	while(1) {
		if (state == STATE_DISPLAY) {
			FlgDisplayTimetable = true;
		}
		//10秒おきに更新
		Sleep(10000);
	}
}

//100msおきに実行される関数
//スイッチ状態監視フラグを立てる
DWORD WINAPI cbWatchSw(LPVOID lpParam)
{
	while(1) {
		FlgWatchSw = true;
		//100msおきに監視
		Sleep(100);
	}
}

//時刻表表示データをディスプレイに送付する関数（時刻表表示状態）
//（データ作成部と送信部を二つに分けたほうが良いか）
void DispayTimetable(CDisplay LEDMatrix, Ctimetable timetable)
{
	CreadFile readFile;

	readFile.setFilename(timetable);
	readFile.storeTimetable(&timetable);

	time_t now = time(NULL);
	struct tm *ts;
	ts = localtime(&now);

	//↓不要
	int mon = ts ->tm_mon + 1;	//月
	int day = ts ->tm_mday;		//日
	int hour = ts ->tm_hour;	//時
	int minute = ts ->tm_min;	//分
	int wday = ts ->tm_wday;	//曜日

	LEDMatrix.ClearStringData();

	//↓時刻構造体を渡す方が良いか
	Ctrain train_1st = timetable.GetNextTrain(wday, hour, minute);
	Ctrain train_2nd = timetable.GetNextTrain(wday, train_1st);

	//ディスプレイに情報送信
	LEDMatrix.SetTimetable(timetable);
	LEDMatrix.SetTrain(train_1st, train_2nd);

	LEDMatrix.SetType();
	LEDMatrix.SetTime();
	LEDMatrix.SetDestination();

	//↓時刻構造体を渡す方が良いか
	LEDMatrix.SetDateAndTime(mon, day, hour, minute);
	LEDMatrix.RefreshDisplay();
}

//駅情報をディスプレイに送信する関数（駅選択画面表示状態）
void DispayStation(CDisplay LEDMatrix, Ctimetable timetable)
{
	LEDMatrix.SetTimetable(timetable);
	LEDMatrix.ClearStringData();
	LEDMatrix.SetStation();
	LEDMatrix.RefreshDisplay();
}

//スイッチ状態を監視する関数
void WatchSw(CSwitch Sw1, CSwitch Sw2)
{
	Sw1.RefreshState();
	Sw2.RefreshState();

	if (Sw1.GetState() == ON) FlgSwW = true;
	if (Sw2.GetState() == ON) FlgSwY = true;
}

//dataフォルダ内の時刻表情報をtimetable vectorに格納する関数
//選択できる駅が決まる。
vector <Ctimetable> StoreTimeTable()
{
	vector <Ctimetable> Timetable;
	Ctimetable EmptyTimetable;
	Dir FnameList;
	// ファイル入力（相対座標にしたほうがよいか）
	vector<string> backfilelist = FnameList.read("C:\\Users\\h_wada\\Documents\\Visual Studio 2010\\Projects\\TimeTableSimulator_0806\\TimeTableSimulator\\data\\");
	for(int i = 0; i < backfilelist.size(); i++){
		Timetable.push_back(EmptyTimetable);
		Timetable[i].num = FnameList.GetStationNum(backfilelist[i]);
		Timetable[i].station = FnameList.GetStationName(backfilelist[i]);
		Timetable[i].route = FnameList.GetRouteName(backfilelist[i]);
		Timetable[i].direction = FnameList.GetDirectionName(backfilelist[i]);
	};
	return Timetable;
}

//選択している時刻表を次のものに進める。
//（リングバッファ?にしておけばこのような処理は不要か）
void TurnTimetable(int *index, int max)
{
	*index += 1;
	if (*index >= max) {
		*index = 0;
	}
}

//main関数
//ディスプレイ表示とスイッチ監視を別スレッドにして処理しようとしたがうまく動かなかった。
//シリアル通信時にMutexを取得するようにしたが、片方のスレッドしか動かない。
//結局フラグ処理で両者がシリアル通信時に競合しないようにした。
int main() {
	//LED定期表示スレッド作成
	DWORD ID_DisplayTimetable;
	HANDLE Thread_DisplayTimetable = CreateThread(NULL, 0, cbDispayTimetable, NULL, CREATE_SUSPENDED, &ID_DisplayTimetable);
	SetThreadPriority(Thread_DisplayTimetable, THREAD_PRIORITY_BELOW_NORMAL);
	ResumeThread(Thread_DisplayTimetable);

	//スイッチ状態監視スレッド作成
	DWORD ID_WatchSw;
	HANDLE Thread_WatchSw = CreateThread(NULL, 0, cbWatchSw, NULL, CREATE_SUSPENDED, &ID_WatchSw);
	SetThreadPriority(Thread_WatchSw, THREAD_PRIORITY_NORMAL);
	ResumeThread(Thread_WatchSw);

	state = STATE_DISPLAY;
	int index_timetable = 0;

	CDisplay LEDMatrix;

	CSwitch SwW('W');
	CSwitch SwY('Y');

	////時刻表情報格納
	vector <Ctimetable> timetable = StoreTimeTable();

	while(1) {
		if (FlgDisplayTimetable) {
			DispayTimetable(LEDMatrix, timetable[index_timetable]);
			FlgDisplayTimetable = false;
		}

		if (FlgDisplayStation) {
			DispayStation(LEDMatrix, timetable[index_timetable]);
			FlgDisplayStation = false;
		}

		if (FlgWatchSw) {
			WatchSw(SwW, SwY);
			FlgWatchSw = false;
		}

		//ボタン押下時の処理
		//白ボタン押下(DISPLAY状態)：DISPLAY状態⇒SELECT状態
		if ((state == STATE_DISPLAY) && FlgSwW) {
			state = STATE_SELECT;
			FlgSwW = false;
			FlgDisplayStation = true;	
		}
		//白ボタン押下(SELECT状態)：駅選択
		if ((state == STATE_SELECT) && FlgSwW) {
			//表示駅のIndexを進める
			TurnTimetable(&index_timetable, timetable.size());
			printf("index:%d\n", index_timetable);
			FlgSwW = false;
			FlgDisplayStation = true;
		}
		//黄ボタン押下(SELECT状態)：SELECT状態⇒DISPLAY状態
		if ((state == STATE_SELECT) && FlgSwY) {
			state = STATE_DISPLAY;
			FlgDisplayTimetable = true;	//すぐに表示する（10秒待たない）
			FlgSwY = false;
		}
	}
}
