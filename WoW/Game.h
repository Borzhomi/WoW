#pragma once
//������� �������� ����
#define XFIELD                20    
#define YFIELD                15
//����������� ��������
#define AFTERDECK4             1
#define AFTERDECK3             2
#define AFTERDECK2             3
#define AFTERDECK1             4

#include "iostream"
#include "conio.h"
#include "time.h"
#include "fstream"

using namespace std;

enum Status{live, die, wound};
enum{LEN = 80};
/////////////////////////////////////////////////�����������/////////////////////////////////////////////////
struct APlayer
{
	char name[LEN];
	int win;
};
///////////////////////////////////////////////������� �������///////////////////////////////////////////////
struct Elem
{
	int x;
	int y;
	Status status;
};
///////////////////////////////////////////////////�������///////////////////////////////////////////////////
class Ships
{
protected:
	Status status;
public:
	virtual Status putstatus() = 0;             //��������� ������� �������
	virtual bool editstatus(int y, int x) = 0;  //��������� ��������
};
class Ship1 :public Ships
{
private:
	Elem ship;
public:
	Ship1(char field[][XFIELD]);
	bool editstatus(int, int);
	Status putstatus();
};
class Ship2 :public Ships
{
private:
	Elem ship[2];
public:
	Ship2(char field[][XFIELD]);
	bool editstatus(int, int);
	Status putstatus();
};
class Ship3 :public Ships
{
private:
	Elem ship[3];
public:
	Ship3(char field[][XFIELD]);
	bool editstatus(int, int);
	Status putstatus();
};
class Ship4 :public Ships
{
private:
	Elem ship[4];
public:
	Ship4(char field[][XFIELD]);
	bool editstatus(int, int);
	Status putstatus();
};
///////////////////////////////////////////////������� ����///////////////////////////////////////////////
class Field
{
private:
	APlayer player;
	char field[YFIELD][XFIELD];                                       //������� ����
	Ships *ship[AFTERDECK1 + AFTERDECK2 + AFTERDECK3 + AFTERDECK4];   //������ ��������
public:
	Field(char*, int);
	void paint(int);                                                  //����� �� ����� ����: 1 - �����, 0 - ���������
	bool check(int, int, Status&);                                    //�������� ���������
	APlayer putplayer();                                             //��������� ���������� �� ������
	bool loss();                                                      //�������� �� ���������� ����
	void win();                                                       //���������� �������� �����
	~Field();
};

////////////////////////////////////////////////�����////////////////////////////////////////////////////
class II
{
private:
	char field[YFIELD][XFIELD];         //����������� ������� ����               
public:
	II();
	void step(int&, int&);              //��������� ���������� ��� ����
	void priority(int, int, Status);    //����������� ���������� �� ����
};
////////////////////////////////////////////World of Warships/////////////////////////////////////////////
class WoW
{
private:
	Field *player;        //������� ���� ������
	Field *computer;      //������� ���� ����������
	II *IskIn;            //�����
public:
	WoW();
	void menu();          //������� ����
	void run();           //������ ����
	void authorisation(); //����������� ������� ������
	void save();
	~WoW();
};