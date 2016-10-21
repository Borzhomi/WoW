#include "Game.h"

WoW::WoW()
{
	Field *player = nullptr;
	Field *computer = nullptr;
	II *IskIn = nullptr;
}
void WoW::menu()
{
	while (true)
	{
		system("cls");
		cout << "МЕНЮ"
			<< "\n1. Играть"
			<< "\n2. Выход" << endl;
		switch (_getch())
		{
		case '1':
			run();
			break;
		case '2':
			exit(1);
		}
	}
}
void WoW::run()
{
	srand(time(NULL));
	authorisation();
	IskIn = new II;
	int xstep, ystep;
	Status st; //Для ИИ
	while (true)
	{
		while (true) //Ход игрока
		{
			system("cls");
			cout << "\n";
			player->paint(1);
			cout << "\n\n";
			computer->paint(0);
			cout << "Ваш ход" << endl;
			cout << "Введите координаты для выстрела: "; 
			xstep = _getche();
			if (xstep >= 'A' && xstep <= 'Z') xstep -= 65;
			else if (xstep >= 'a' && xstep <= 'z') xstep -= 97;
			cin >> ystep;
			if (!computer->check(ystep, xstep, st)) break;
			if (computer->loss())
			{
				cout << "ВЫ ВЫИГРАЛИ!!!" << endl;
				player->win();
				save();
				system("pause");
				return;
			}
		}
		while (true) //Ход компьютера
		{
			system("cls");
			cout << "\n";
			player->paint(1);
			cout << "\n\n";
			computer->paint(0);
			cout << "Ход компьютера" << endl;
			IskIn->step(ystep, xstep);
			cout << "Координаты выстрела: ";
			cout << char(xstep + 65) << ystep << endl;
			if (!player->check(ystep, xstep, st))
			{
				IskIn->priority(ystep, xstep, st);
				break;
			}
			if (player->loss())
			{
				cout << "ВЫ ПРОИГРАЛИ!!!" << endl;
				computer->win();
				save();
				system("pause");
				return;
			}
			IskIn->priority(ystep, xstep, st);
		}
	}
}
void WoW::authorisation()
{
	APlayer tmp;
	ifstream ifile("save.dat", ios::binary);
	ifile.seekg(0, ios::end);
	if (ifile.tellg() < sizeof(tmp))
	{
		char name[LEN];
		system("cls");
		cout << "Введите имя: ";
		cin >> name;
		player = new Field(name, 0);
		computer = new Field("Computer", 0);
	}
	else
	{
		ifile.seekg(0);
		ifile.read((char*)&tmp, sizeof(tmp));
		player = new Field(tmp.name, tmp.win);
		ifile.read((char*)&tmp, sizeof(tmp));
		computer = new Field(tmp.name, tmp.win);
	}
	ifile.close();
}
void WoW::save()
{
	APlayer tmp;
	ofstream ofile("save.dat", ios::trunc | ios::binary);
	tmp = player->putplayer();
	ofile.write((char*)&tmp, sizeof(tmp));
	tmp = computer->putplayer();
	ofile.write((char*)&tmp, sizeof(tmp));
	ofile.close();
}
WoW::~WoW()
{
	delete player;
	delete computer;
	delete IskIn;
}

/////////////////////////////////////////////////////////////////////////////

Field::Field(char* name, int win)
{
	for (int i = 0; i < YFIELD; i++)
	{
		for (int j = 0; j < XFIELD; j++)
		{
			field[i][j] = ' ';
		}
	}
	int i = 0;
	for (int j = 0; j < AFTERDECK1; j++)
	{
		ship[i] = new Ship1(field);
		i++;
	}
	for (int j = 0; j < AFTERDECK2; j++)
	{
		ship[i] = new Ship2(field);
		i++;
	}
	for (int j = 0; j < AFTERDECK3; j++)
	{
		ship[i] = new Ship3(field);
		i++;
	}
	for (int j = 0; j < AFTERDECK4; j++)
	{
		ship[i] = new Ship4(field);
		i++;
	}
	strcpy_s(player.name, LEN, name);
	player.win = win;
}
void Field::paint(int init)
{
	cout << "Игрок " << player.name << "\tПобед " << player.win << "\n\n";
	for (int i = 0; i <= YFIELD; i++)
	{
		for (int j = 0; j <= XFIELD; j++)
		{
			if (i == YFIELD && j != XFIELD) cout << char(j + 65);
			else if (j == XFIELD && i != YFIELD) cout << i;
			else if (i != YFIELD && j != XFIELD)
			{
				if (init) cout << field[i][j];
				else
				{
					if (field[i][j] == 1) cout << ' ';
					else cout << field[i][j];
				}
			}
		}
		cout << endl;
	}
}
bool Field::check(int y, int x, Status& st)
{
	if (field[y][x] != 1)
	{
		field[y][x] = '-';
		cout << "Промах...\n";
		st = live;
		system("pause");
		return false;
	}
	else
	{
		field[y][x] = '*';
		for (int i = 0; i < AFTERDECK1 + AFTERDECK2 + AFTERDECK3 + AFTERDECK4; i++)
		{
			if (ship[i]->editstatus(y, x))
			{
				if (ship[i]->putstatus() == die) st = die;
				else st = wound;
				break;
			}
		}
		system("pause");
		return true;
	}
}
APlayer Field::putplayer()
{
	return player;
}
bool Field::loss()
{
	for (int i = 0; i < AFTERDECK1 + AFTERDECK2 + AFTERDECK3 + AFTERDECK4; i++)
	{
		if (ship[i]->putstatus() == live) return false;
	}
    return true;
}
void Field::win()
{
	player.win++;
}
Field::~Field()
{
	for (int i = 0; i < AFTERDECK1 + AFTERDECK2 + AFTERDECK3 + AFTERDECK4; i++)
	{
		delete ship[i];
	}
}

//////////////////////////////////////////////////////////////////////////////

Ship1::Ship1(char field[][XFIELD])
{
	while (true)
	{
		ship.y = rand() % YFIELD;
		ship.x = rand() % XFIELD;
		if (ship.y) if (field[ship.y - 1][ship.x] != ' ') continue;
		if (ship.x) if (field[ship.y][ship.x - 1] != ' ') continue;
		if (ship.y < YFIELD - 1) if (field[ship.y + 1][ship.x] != ' ') continue;
		if (ship.x < XFIELD - 1) if (field[ship.y][ship.x + 1] != ' ') continue;
		if (field[ship.y][ship.x] != ' ') continue;
		field[ship.y][ship.x] = 1;
		ship.status = live;
		status = live;
		break;
	}
} 
bool Ship1::editstatus(int y, int x)
{
	if (ship.y == y && ship.x == x)
	{
		ship.status = die;
		status = die;
		cout << "Убит!!!" << endl;
		return true;
	}
	else return false;
}
Status Ship1::putstatus()
{
	return status;
}
Ship2::Ship2(char field[][XFIELD])
{
	int i = 0;
	while (i < 2)
	{
		if (!i)
		{
			ship[i].y = rand() % YFIELD;
			ship[i].x = rand() % XFIELD;
			if (ship[i].y) if (field[ship[i].y - 1][ship[i].x] != ' ') continue;
			if (ship[i].x) if (field[ship[i].y][ship[i].x - 1] != ' ') continue;
			if (ship[i].y < YFIELD - 1) if (field[ship[i].y + 1][ship[i].x] != ' ') continue;
			if (ship[i].x < XFIELD - 1) if (field[ship[i].y][ship[i].x + 1] != ' ') continue;
			if (field[ship[i].y][ship[i].x] != ' ') continue;
		}
		else
		{
			int tmp = 1;
			switch (tmp)
			{
			case 1:
				ship[i].y = ship[i - 1].y - 1;
				ship[i].x = ship[i - 1].x;
				if (!ship[i].y && !ship[i].x)
				{
					if (field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (!ship[i].y && ship[i].x && ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (!ship[i].y && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y > 0 && !ship[i].x)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y > 0 && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y > 0)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
			case 2:
				ship[i].y = ship[i - 1].y;
				ship[i].x = ship[i - 1].x + 1;
				if (!ship[i].y && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y && ship[i].x == XFIELD - 1 && ship[i].y < YFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (!ship[i].y && ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
			case 3:
				ship[i].y = ship[i - 1].y + 1;
				ship[i].x = ship[i - 1].x;
				if (ship[i].y == YFIELD - 1 && !ship[i].x)
				{
					if (field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x > 0 && ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y < YFIELD - 1 && !ship[i].x)
				{
					if (field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y < YFIELD - 1 && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y < YFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
			case 4:
				ship[i].y = ship[i - 1].y;
				ship[i].x = ship[i - 1].x - 1;
				if (!ship[i].y && !ship[i].x)
				{
					if (field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y && !ship[i].x && ship[i].y < YFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && !ship[i].x)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (!ship[i].y && ship[i].x > 0)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x > 0)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].x > 0)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				i = 0;
				continue;
			}
		}
		field[ship[i].y][ship[i].x] = 1;
		ship[i].status = live;
		i++;
	}
	status = live;
}
bool Ship2::editstatus(int y, int x)
{
	for (int i = 0; i < 2; i++)
	{
		if (ship[i].y == y && ship[i].x == x)
		{
			ship[i].status = die;
			if (ship[0].status == die && ship[1].status == die)
				cout << "Убит!!!\n";
			else cout << "Ранен!\n";
			return true;
		}
	}
	return false;
}
Status Ship2::putstatus()
{
	return status;
}
Ship3::Ship3(char field[][XFIELD])
{
	int i = 0;
	while (i < 3)
	{
		if (!i)
		{
			ship[i].y = rand() % YFIELD;
			ship[i].x = rand() % XFIELD;
			if (ship[i].y) if (field[ship[i].y - 1][ship[i].x] != ' ') continue;
			if (ship[i].x) if (field[ship[i].y][ship[i].x - 1] != ' ') continue;
			if (ship[i].y < YFIELD - 1) if (field[ship[i].y + 1][ship[i].x] != ' ') continue;
			if (ship[i].x < XFIELD - 1) if (field[ship[i].y][ship[i].x + 1] != ' ') continue;
			if (field[ship[i].y][ship[i].x] != ' ') continue;
		}
		else
		{
			int tmp = 1;
			switch (tmp)
			{
			case 1:
				ship[i].y = ship[i - 1].y - 1;
				ship[i].x = ship[i - 1].x;
				if (!ship[i].y && !ship[i].x)
				{
					if (field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (!ship[i].y && ship[i].x && ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (!ship[i].y && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y > 0 && !ship[i].x)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y > 0 && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y > 0)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
			case 2:
				ship[i].y = ship[i - 1].y;
				ship[i].x = ship[i - 1].x + 1;
				if (!ship[i].y && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y && ship[i].x == XFIELD - 1 && ship[i].y < YFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (!ship[i].y && ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
			case 3:
				ship[i].y = ship[i - 1].y + 1;
				ship[i].x = ship[i - 1].x;
				if (ship[i].y == YFIELD - 1 && !ship[i].x)
				{
					if (field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x > 0 && ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y < YFIELD - 1 && !ship[i].x)
				{
					if (field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y < YFIELD - 1 && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y < YFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
			case 4:
				ship[i].y = ship[i - 1].y;
				ship[i].x = ship[i - 1].x - 1;
				if (!ship[i].y && !ship[i].x)
				{
					if (field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y && !ship[i].x && ship[i].y < YFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && !ship[i].x)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (!ship[i].y && ship[i].x > 0)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x > 0)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].x > 0)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				i = 0;
				continue;
			}
		}
		field[ship[i].y][ship[i].x] = 1;
		ship[i].status = live;
		i++;
	}
	status = live;
}
bool Ship3::editstatus(int y, int x)
{
	for (int i = 0; i < 3; i++)
	{
		if (ship[i].y == y && ship[i].x == x)
		{
			ship[i].status = die;
			if (ship[0].status == die && ship[1].status == die && ship[2].status == die)
				cout << "Убит!!!\n";
			else cout << "Ранен!\n";
			return true;
		}
	}
	return false;
}
Status Ship3::putstatus()
{
	return status;
}
Ship4::Ship4(char field[][XFIELD])
{
	int i = 0;
	while (i < 4)
	{
		if (!i)
		{
			ship[i].y = rand() % YFIELD;
			ship[i].x = rand() % XFIELD;
			if (ship[i].y) if (field[ship[i].y - 1][ship[i].x] != ' ') continue;
			if (ship[i].x) if (field[ship[i].y][ship[i].x - 1] != ' ') continue;
			if (ship[i].y < YFIELD - 1) if (field[ship[i].y + 1][ship[i].x] != ' ') continue;
			if (ship[i].x < XFIELD - 1) if (field[ship[i].y][ship[i].x + 1] != ' ') continue;
			if (field[ship[i].y][ship[i].x] != ' ') continue;
		}
		else
		{
			int tmp = 1;
			switch (tmp)
			{
			case 1:
				ship[i].y = ship[i - 1].y - 1;
				ship[i].x = ship[i - 1].x;
				if (!ship[i].y && !ship[i].x)
				{
					if (field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (!ship[i].y && ship[i].x && ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (!ship[i].y && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y > 0 && !ship[i].x)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y > 0 && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y > 0)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
			case 2:
				ship[i].y = ship[i - 1].y;
				ship[i].x = ship[i - 1].x + 1;
				if (!ship[i].y && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y && ship[i].x == XFIELD - 1 && ship[i].y < YFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (!ship[i].y && ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
			case 3:
				ship[i].y = ship[i - 1].y + 1;
				ship[i].x = ship[i - 1].x;
				if (ship[i].y == YFIELD - 1 && !ship[i].x)
				{
					if (field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x > 0 && ship[i].x < XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y < YFIELD - 1 && !ship[i].x)
				{
					if (field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y < YFIELD - 1 && ship[i].x == XFIELD - 1)
				{
					if (field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y < YFIELD - 1)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x + 1] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
			case 4:
				ship[i].y = ship[i - 1].y;
				ship[i].x = ship[i - 1].x - 1;
				if (!ship[i].y && !ship[i].x)
				{
					if (field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y && !ship[i].x && ship[i].y < YFIELD - 1)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && !ship[i].x)
				{
					if (field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (!ship[i].y && ship[i].x > 0)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].y == YFIELD - 1 && ship[i].x > 0)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				else if (ship[i].x > 0)
				{
					if (field[ship[i].y][ship[i].x - 1] == ' ' && field[ship[i].y - 1][ship[i].x] == ' ' && field[ship[i].y + 1][ship[i].x] == ' ' && field[ship[i].y][ship[i].x] == ' ') break;
				}
				i = 0;
				continue;
			}
		}
		field[ship[i].y][ship[i].x] = 1;
		ship[i].status = live;
		i++;
	}
	status = live;
}
bool Ship4::editstatus(int y, int x)
{
	for (int i = 0; i < 4; i++)
	{
		if (ship[i].y == y && ship[i].x == x)
		{
			ship[i].status = die;
			if (ship[0].status == die && ship[1].status == die && ship[2].status == die && ship[3].status == die)
				cout << "Убит!!!\n";
			else cout << "Ранен!\n";
			return true;
		}
	}
	return false;
}
Status Ship4::putstatus()
{
	return status;
}