#pragma once
#include<vector>
#include<iostream>
#include<string>
#include"Train.h"

using namespace std;

enum Wday{
	SUNDAY,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
};

class Ctimetable
{
public:
	string num;		//�����\�ԍ�
	string station;		//�w��
	string route;		//�H����
	string direction;	//����

	vector <Ctrain> WeekdayDiagram;		//�����_�C��
	vector <Ctrain> SaturdayDiagram;	//�y�j���_�C��
	vector <Ctrain> HolidayDiagram;		//�x���_�C��

	vector <string> destinationInfo;	//�s����f�[�^
	vector <string> typeInfo;			//��ʃf�[�^

	Ctrain GetNextTrain(int wday, int hour, int minute);
	Ctrain GetNextTrain(int wday, Ctrain train_before);
	Ctrain SearchNextTrain(vector <Ctrain> diagram, int hour, int minute);
	Ctrain SearchNextTrain(vector <Ctrain> diagram, Ctrain train);

	Ctimetable(void);	//�R���X�g���N�^
	~Ctimetable(void);	//�f�X�g���N�^
};

