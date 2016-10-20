#include "Game.h"

II::II()
{
	for (int i = 0; i < YFIELD; i++)
	{
		for (int j = 0; j < XFIELD; j++)
		{
			field[i][j] = ' ';
		}
	}
}
void II::step(int& y, int& x)
{
	for (int i = 0; i < YFIELD; i++)
	{
		for (int j = 0; j < XFIELD; j++)
		{
			if (field[i][j] == '!')
			{
				y = i;
				x = j;
				return;
			}
		}
	}
	while (true)
	{
		y = rand() % YFIELD;
		x = rand() % XFIELD;
		if (field[y][x] == ' ') return;
	}
}
void II::priority(int y, int x, Status st)
{
	if (st == live)
		field[y][x] = '-';
	else if (st == wound)
	{
		field[y][x] = '*';
		if (y) if (field[y - 1][x] == ' ') field[y - 1][x] = '!';
		if (x) if (field[y][x - 1] == ' ') field[y][x - 1] = '!';
		if (y < YFIELD - 1) if (field[y + 1][x] == ' ') field[y + 1][x] = '!';
		if (x < XFIELD - 1) if (field[y][x + 1] == ' ') field[y][x + 1] = '!';
	}
	else
	{
		while(true)
		{
			if (y) if (field[y - 1][x] == ' ') field[y - 1][x] = '-';
			if (x) if (field[y][x - 1] == ' ') field[y][x - 1] = '-';
			if (y < YFIELD - 1) if (field[y + 1][x] == ' ') field[y + 1][x] = '-';
			if (x < XFIELD - 1) if (field[y][x + 1] == ' ') field[y][x + 1] = '-';
			field[y][x] == '-';
			if (y) if (field[y - 1][x] == '*')
			{
				y--;
				continue;
			}
			if (x) if (field[y][x - 1] == '*')
			{
				x--;
				continue;
			}
			if (y < YFIELD - 1) if (field[y + 1][x] == '*')
			{
				y++;
				continue;
			}
			if (x < XFIELD - 1) if (field[y][x + 1] == '*')
			{
				x++;
				continue;
			}
			break;
		}
	}
}