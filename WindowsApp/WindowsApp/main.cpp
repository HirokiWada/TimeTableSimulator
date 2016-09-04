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

int state;			//Display���

enum State {
	STATE_DISPLAY,	//�����\�\�����
	STATE_SELECT,	//�w�I����ʕ\�����
};

//10s�����Ɏ��s�����֐�
//�����\�\�����X�V����t���O�𗧂Ă�
DWORD WINAPI cbDispayTimetable(LPVOID lpParam)
{
	while(1) {
		if (state == STATE_DISPLAY) {
			FlgDisplayTimetable = true;
		}
		//10�b�����ɍX�V
		Sleep(10000);
	}
}

//100ms�����Ɏ��s�����֐�
//�X�C�b�`��ԊĎ��t���O�𗧂Ă�
DWORD WINAPI cbWatchSw(LPVOID lpParam)
{
	while(1) {
		FlgWatchSw = true;
		//100ms�����ɊĎ�
		Sleep(100);
	}
}

//�����\�\���f�[�^���f�B�X�v���C�ɑ��t����֐��i�����\�\����ԁj
//�i�f�[�^�쐬���Ƒ��M�����ɕ������ق����ǂ����j
void DispayTimetable(CDisplay LEDMatrix, Ctimetable timetable)
{
	CreadFile readFile;

	readFile.setFilename(timetable);
	readFile.storeTimetable(&timetable);

	time_t now = time(NULL);
	struct tm *ts;
	ts = localtime(&now);

	//���s�v
	int mon = ts ->tm_mon + 1;	//��
	int day = ts ->tm_mday;		//��
	int hour = ts ->tm_hour;	//��
	int minute = ts ->tm_min;	//��
	int wday = ts ->tm_wday;	//�j��

	LEDMatrix.ClearStringData();

	//�������\���̂�n�������ǂ���
	Ctrain train_1st = timetable.GetNextTrain(wday, hour, minute);
	Ctrain train_2nd = timetable.GetNextTrain(wday, train_1st);

	//�f�B�X�v���C�ɏ�񑗐M
	LEDMatrix.SetTimetable(timetable);
	LEDMatrix.SetTrain(train_1st, train_2nd);

	LEDMatrix.SetType();
	LEDMatrix.SetTime();
	LEDMatrix.SetDestination();

	//�������\���̂�n�������ǂ���
	LEDMatrix.SetDateAndTime(mon, day, hour, minute);
	LEDMatrix.RefreshDisplay();
}

//�w�����f�B�X�v���C�ɑ��M����֐��i�w�I����ʕ\����ԁj
void DispayStation(CDisplay LEDMatrix, Ctimetable timetable)
{
	LEDMatrix.SetTimetable(timetable);
	LEDMatrix.ClearStringData();
	LEDMatrix.SetStation();
	LEDMatrix.RefreshDisplay();
}

//�X�C�b�`��Ԃ��Ď�����֐�
void WatchSw(CSwitch Sw1, CSwitch Sw2)
{
	Sw1.RefreshState();
	Sw2.RefreshState();

	if (Sw1.GetState() == ON) FlgSwW = true;
	if (Sw2.GetState() == ON) FlgSwY = true;
}

//data�t�H���_���̎����\����timetable vector�Ɋi�[����֐�
//�I���ł���w�����܂�B
vector <Ctimetable> StoreTimeTable()
{
	vector <Ctimetable> Timetable;
	Ctimetable EmptyTimetable;
	Dir FnameList;
	// �t�@�C�����́i���΍��W�ɂ����ق����悢���j
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

//�I�����Ă��鎞���\�����̂��̂ɐi�߂�B
//�i�����O�o�b�t�@?�ɂ��Ă����΂��̂悤�ȏ����͕s�v���j
void TurnTimetable(int *index, int max)
{
	*index += 1;
	if (*index >= max) {
		*index = 0;
	}
}

//main�֐�
//�f�B�X�v���C�\���ƃX�C�b�`�Ď���ʃX���b�h�ɂ��ď������悤�Ƃ��������܂������Ȃ������B
//�V���A���ʐM����Mutex���擾����悤�ɂ������A�Е��̃X���b�h���������Ȃ��B
//���ǃt���O�����ŗ��҂��V���A���ʐM���ɋ������Ȃ��悤�ɂ����B
int main() {
	//LED����\���X���b�h�쐬
	DWORD ID_DisplayTimetable;
	HANDLE Thread_DisplayTimetable = CreateThread(NULL, 0, cbDispayTimetable, NULL, CREATE_SUSPENDED, &ID_DisplayTimetable);
	SetThreadPriority(Thread_DisplayTimetable, THREAD_PRIORITY_BELOW_NORMAL);
	ResumeThread(Thread_DisplayTimetable);

	//�X�C�b�`��ԊĎ��X���b�h�쐬
	DWORD ID_WatchSw;
	HANDLE Thread_WatchSw = CreateThread(NULL, 0, cbWatchSw, NULL, CREATE_SUSPENDED, &ID_WatchSw);
	SetThreadPriority(Thread_WatchSw, THREAD_PRIORITY_NORMAL);
	ResumeThread(Thread_WatchSw);

	state = STATE_DISPLAY;
	int index_timetable = 0;

	CDisplay LEDMatrix;

	CSwitch SwW('W');
	CSwitch SwY('Y');

	////�����\���i�[
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

		//�{�^���������̏���
		//���{�^������(DISPLAY���)�FDISPLAY��ԁ�SELECT���
		if ((state == STATE_DISPLAY) && FlgSwW) {
			state = STATE_SELECT;
			FlgSwW = false;
			FlgDisplayStation = true;	
		}
		//���{�^������(SELECT���)�F�w�I��
		if ((state == STATE_SELECT) && FlgSwW) {
			//�\���w��Index��i�߂�
			TurnTimetable(&index_timetable, timetable.size());
			printf("index:%d\n", index_timetable);
			FlgSwW = false;
			FlgDisplayStation = true;
		}
		//���{�^������(SELECT���)�FSELECT��ԁ�DISPLAY���
		if ((state == STATE_SELECT) && FlgSwY) {
			state = STATE_DISPLAY;
			FlgDisplayTimetable = true;	//�����ɕ\������i10�b�҂��Ȃ��j
			FlgSwY = false;
		}
	}
}
