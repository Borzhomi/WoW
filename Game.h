#pragma once
//Размеры игрового поля
#define XFIELD                20    
#define YFIELD                15
//Колличество кораблей
#define AFTERDECK4             1
#define AFTERDECK3             2
#define AFTERDECK2             3
#define AFTERDECK1             4

#include "iostream"
#include "conio.h"
#include "time.h"

using namespace std;

enum Status{live, die, wound};
///////////////////////////////////////////////Элемент корабля///////////////////////////////////////////////
struct Elem
{
	int x;
	int y;
	Status status;
};
///////////////////////////////////////////////////Корабли///////////////////////////////////////////////////
class Ships
{
protected:
	Status status;
public:
	virtual Status putstatus() = 0;             //Получение статуса корабля
	virtual bool editstatus(int y, int x) = 0;  //Изменение статусов
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
///////////////////////////////////////////////Игровое поле///////////////////////////////////////////////
class Field
{
private:
	char field[YFIELD][XFIELD];                                       //Игровое поле
	Ships *ship[AFTERDECK1 + AFTERDECK2 + AFTERDECK3 + AFTERDECK4];   //Массив кораблей
public:
	Field();
	void paint(int);                                                  //Вывод на экран поля: 1 - игрок, 0 - компьютер
	bool check(int, int, Status&);                                    //Проверка попадания
	bool loss();                                                      //Проверка на завершение игры
	~Field();
};

////////////////////////////////////////////////ИскИн////////////////////////////////////////////////////
class II
{
private:
	char field[YFIELD][XFIELD];         //Собственное игровое поле               
public:
	II();
	void step(int&, int&);              //Вычисляет координаты для хода
	void priority(int, int, Status);    //Расставляем приоритеты на поле
};
////////////////////////////////////////////World of Warships/////////////////////////////////////////////
class WoW
{
private:
	Field *player;      //Игровое поле игрока
	Field *computer;    //Игровое поле компьютера
	II *IskIn;          //ИскИн
public:
	WoW();
	void menu();        //Главное меню
	void run();         //Запуск игры
	~WoW();
};