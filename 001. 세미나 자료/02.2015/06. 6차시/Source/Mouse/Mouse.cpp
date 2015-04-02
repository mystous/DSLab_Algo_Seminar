#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>


#define MAZE_SIZE  19
#define ROBOT       2

void gotoxy(int x, int y)
{ 
	COORD Cur ;
	Cur.X = x ;
	Cur.Y = y ;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur); 
}

void delay(int n) //딜레이
{
	unsigned o = GetTickCount();
	while( (GetTickCount()-o) < n );
}

//맵
int maze[MAZE_SIZE][MAZE_SIZE] =
{ { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
{ 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
{ 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1 },
{ 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1 },
{ 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1 },
{ 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
{ 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1 },
{ 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 },
{ 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 },
{ 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 },
{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 },
{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1 },
{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1 },
{ 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1 },
{ 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1 },
{ 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
{ 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1 },
{ 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };

int sx = MAZE_SIZE - 1, sy = MAZE_SIZE - 2;
int rec[MAZE_SIZE*MAZE_SIZE];  /* record robot's path */

#define UP     1
#define RIGHT  2
#define DOWN   4
#define LEFT   8

//벽 종류 선택
int get_shape(int m[][MAZE_SIZE], int x, int y)
{
	static int shape[] = {  32, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64 };
	int s = 0;
	if (m[y][x])
	{
		if (y > 0  &&  m[y-1][x]) 
			s = UP;
		if (y < MAZE_SIZE - 2  &&  m[y+1][x]) 
			s = DOWN;
		if (x > 0  &&  m[y][x-1]) 
			s = LEFT;
		if (x < MAZE_SIZE - 2  &&  m[y][x+1]) 
			s = RIGHT;
	}    
	return shape[s];
}
//벽 프린터
void draw_maze(int m[][MAZE_SIZE])
{
	int i, j;
	for (j = 0; j < MAZE_SIZE; j++)
	{
		for (i = 0; i < MAZE_SIZE; i++)
		{
			gotoxy(i + 1, j + 1);
			_putch(get_shape(m, i, j)); 
		}
	}
}
void record(int x, int y)
{
	static int index = 0;
	rec[index++] = x;
	rec[index++] = y;
}
void forward(int *x, int *y, int dir)
{
	gotoxy(*x + 1, *y + 1);
	_putch(' ');
	*x = (dir == LEFT)  ? --(*x) :
		(dir == RIGHT) ? ++(*x) : *x;
	*y = (dir == UP)    ? --(*y) :
		(dir == DOWN)  ? ++(*y) : *y;
	record(*x, *y);
	gotoxy(*x + 1, *y + 1);
	_putch(42);
}
void right(int *dir)
{
	*dir <<= 1;
	*dir = (*dir > LEFT) ? UP : *dir;
}
void left(int *dir)
{
	*dir >>= 1;
	*dir = (*dir == 0) ? LEFT : *dir;
}
int still_in_maze(int x, int y)
{
	if (x > 0  &&  x < MAZE_SIZE - 1  &&  y > 0  &&  y < MAZE_SIZE - 1)
		return 1;
	else
		return 0;
}
int wall_ahead(int m[][MAZE_SIZE], int x, int y, int dir)
{
	x = (dir == LEFT)  ? --x : (dir == RIGHT) ? ++x : x;
	y = (dir == UP)    ? --y : (dir == DOWN)  ? ++y : y;
	return m[y][x];
}
void right_hand(int m[][MAZE_SIZE], int x, int y, int dir)
{
	gotoxy(x + 1, y + 1);
	_putch(ROBOT);
	record(x, y);
	forward(&x, &y, dir);
	while (still_in_maze(x, y))
	{
		delay(10);
		right(&dir);
		while (wall_ahead(m, x, y, dir))
			left(&dir);
		forward(&x, &y, dir);
	}
	record(-1, -1);  /* end of path */
}
int del_path(int i, int j)
{
	int k = i;
	while (rec[j] >= 0)
		rec[i++] = rec[j++];
	rec[i] = -1;
	return k;   /* k is j position after deleting */
}
void shortest_path(void)
{
	int i = 0;
	int x, y;
	int j;
	int x1, y1;
	while (rec[i] >= 0)
	{
		x = rec[i];
		y = rec[i + 1];
		j = i + 2;
		while (rec[j] >=0)
		{
			x1 = rec[j];
			y1 = rec[j + 1];
			if (x == x1  &&  y == y1)
				j = del_path(i, j);
			j++;
			j++;
		}
		i++;
		i++;
	}
	i = 0;
	while (rec[i] >= 0)
	{
		x = rec[i++];
		y = rec[i++];
		gotoxy(x + 1, y + 1);
		_putch(ROBOT);
		delay(10);
		gotoxy(x + 1, y + 1);
		_putch(' ');
	}
}

int main(void)
{
	/*
	rec = (int*)malloc(MAZE_SIZE * MAZE_SIZE);
	if (rec == NULL)
	{
		cputs("\r\n Memory allocation error ! ");
		exit(1);
	}
	*/

	system("cls");
	draw_maze(maze);
	gotoxy(40, 5);
	_cputs("Simulation of Micro Mouse");
	gotoxy(40, 10);
	_cputs(" Press any key to start...");
	right_hand(maze, sx, sy, LEFT);
	gotoxy(40, 10);
	_cputs("  Press any key to see shortest path...");
	shortest_path();
	gotoxy(40, 10);
	_cputs("  Press any key to end program...      ");
}