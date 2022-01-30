#include <iostream>
#include <stdio.h>
#include "Bricks.h"

#define BRICK_ROW_MAX 3
#define BRICK_PER_ROW_MAX 21
#define BRICK_WIDTH 10
#define BRICK_HEIGHT 1
#define BRICK_COUNT_MAX    BRICK_ROW_MAX * BRICK_PER_ROW_MAX
#define BRICK_ONE_BEGIN_X 3
#define BRICK_ONE_BEGIN_Y 5

using namespace std;

struct brick
{
	unsigned int startx;
	unsigned int starty;
	bool live;
};

brick Bricks[BRICK_COUNT_MAX];


#define clear()       
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

static void draw_brick(bool live, unsigned int x, unsigned int y, unsigned int h, unsigned int w);
static void print_live_bricks(void);

unsigned int points = 0;

void Bricks_init(void)
{

	gotoxy(220, 3);
	cout << "Points: " << points;

	//setting each brick's x and y position	
	for (unsigned int i = 0; i < BRICK_ROW_MAX; i++)
	{
		//set row's first brick manually
		Bricks[i * BRICK_PER_ROW_MAX ].startx = BRICK_ONE_BEGIN_X;
		Bricks[i * BRICK_PER_ROW_MAX ].starty = (BRICK_ONE_BEGIN_Y + 1)+ (i * (BRICK_HEIGHT + 1));
		Bricks[i * BRICK_PER_ROW_MAX].live = true;

		for (unsigned int j = 1; j < BRICK_PER_ROW_MAX; j++)
		{
			Bricks[i * BRICK_PER_ROW_MAX + j].startx = Bricks[i * BRICK_PER_ROW_MAX + j - 1].startx + BRICK_WIDTH + 1;
			Bricks[i * BRICK_PER_ROW_MAX + j].starty = Bricks[i * BRICK_PER_ROW_MAX + j - 1].starty;
			Bricks[i * BRICK_PER_ROW_MAX + j].live = true;
		}
	}

	//print the bricks
	for (unsigned int i = 0; i < (BRICK_PER_ROW_MAX * BRICK_ROW_MAX); i++)
	{
		draw_brick(true, Bricks[i].startx, Bricks[i].starty, BRICK_HEIGHT, BRICK_WIDTH);
	}	
}

bool is_brick_hit(unsigned int* futurex, int* futurey)
{
	bool hit = false;

	for (unsigned int i = BRICK_COUNT_MAX; i > 0; i--)
	{
		//only live bricks
		if (Bricks[i - 1].live)
		{
			if (*futurey <= 6)
			{
				i=i;
			}
			//is this brick in the path of being hit
			if (((*futurex >= Bricks[i - 1].startx) && (*futurex <= (Bricks[i - 1].startx + BRICK_WIDTH))) &&
				((*futurey <= (Bricks[i - 1].starty + BRICK_HEIGHT))))
			{	
				
				Bricks[i - 1].live = false;

				points++;

				*futurey = Bricks[i - 1].starty + BRICK_HEIGHT;
				hit = true;

				gotoxy(220, 3);
				cout << "Points: " << points;
				break;
			}
		}		
	}

	if (hit == true)
	{
		print_live_bricks();
	}
	
	return hit;	
}

static void draw_brick(bool live, unsigned int x, unsigned int y, unsigned int h, unsigned int w)
{
	unsigned char ch1 = '_';
	unsigned char ch2 = '|';

	if (live)
	{
		ch1 = '_';
		ch2 = '|';
	}
	else
	{
		ch1 = ' ';
		ch2 = ' ';
	}

	//go to location
	gotoxy(x + 1,y);
	
	for (unsigned int cols = 0; cols < w - 1; cols++)
	{
		cout << ch1;
	}
	
	for (unsigned int rows = 0; rows < h; rows++)
	{
		gotoxy(x, y + rows + 1);
		cout << ch2;

		gotoxy(x + w, y + rows + 1);
		cout << ch2;
	}
	
	gotoxy(x + 1, y + h);

	for (unsigned int cols = 0; cols < w-1; cols++)
	{
		cout << ch1;
	}
}

static void print_live_bricks(void)
{
	for (unsigned int i = 0; i < BRICK_COUNT_MAX; i++)
	{	
		draw_brick(Bricks[i].live, Bricks[i].startx, Bricks[i].starty, BRICK_HEIGHT, BRICK_WIDTH);		
	}
}
