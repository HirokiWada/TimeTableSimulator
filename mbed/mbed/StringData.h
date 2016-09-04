#pragma once

#include <string>
#include "mbed.h"

enum Color{
    RED,
    GREEN,
    ORANGE,
};

class CStringData
{
private:
    string str;     //表示文字列
    int pos_H;      //表示位置（横）
    int pos_V;      //表示位置（縦）
    Color color;    //表示色
    
public:
    void SetString(string _str);
    void SetPos_H(int _pos_H);
    void SetPos_V(int _pos_V);
    void SetColor(Color _color);

    string GetString() {return str;}
    int GetPos_H() {return pos_H;}
    int GetPos_V() {return pos_V;}
    Color GetColor() {return color;}
};