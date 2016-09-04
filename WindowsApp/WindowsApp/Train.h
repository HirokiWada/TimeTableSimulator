#pragma once
class Ctrain
{
public:
	int destination;	//行き先
	int type;			//種別
	int hour;			//時
	int minutes;		//分

	Ctrain(void);		//コンストラクタ
	~Ctrain(void);		//デストラクタ
};

