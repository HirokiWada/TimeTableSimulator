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
    string str;     //�\��������
    int pos_H;      //�\���ʒu�i���j
    int pos_V;      //�\���ʒu�i�c�j
    Color color;    //�\���F
    
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