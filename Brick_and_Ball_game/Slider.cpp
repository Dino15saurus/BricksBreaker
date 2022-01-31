#include <iostream>
#include <stdio.h>
#include <conio.h>
#include "Slider.h"

using namespace std;

#define SLIDER_WIDTH 20
#define SLIDER_HEIGHT 1
#define SLIDER_START_X 200
#define SLIDER_START_Y 60
#define SLIDER_X_MAX 235 - SLIDER_WIDTH
#define SLIDER_X_MIN 5
#define SLIDER_LEFT_KEY 0xE04B
#define SLIDER_RIGHT_KEY 0xE04D
#define SLIDER_SLIDE_STEP_SIZE 5

#define clear()       
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define hide_cursor() //printf("\e[?25l");
#define show_cursor() printf("\e[?25h");

unsigned int sliderx, slidery;

static void slider_move(unsigned int oldx, unsigned int oldy, unsigned int newx, unsigned int newy);
static void print_or_erase_slider(bool print_or_erase, unsigned int x, unsigned int y);

static void print_or_erase_slider(bool print_or_erase, unsigned int x, unsigned int y)
{
	//go to location
	gotoxy(x + 1,y);
	char ch;

	if (print_or_erase)
	{
		ch = '_';
	}
	else
	{
		ch = ' ';
	}

	for (unsigned int cols = 0; cols < SLIDER_WIDTH - 1; cols++)
	{
		cout << ch;
	}

	if (print_or_erase)
	{
		ch = '|';
	}
	else
	{
		ch = ' ';
	}

	for (unsigned int rows = 0; rows < SLIDER_HEIGHT; rows++)
	{
		gotoxy(x, y + rows + 1);
		cout << ch;

		gotoxy(x + SLIDER_WIDTH, y + rows + 1);
		cout << ch;
	}

	if (print_or_erase)
	{
		ch = '_';
	}
	else
	{
		ch = ' ';
	}

	gotoxy(x + 1, y + SLIDER_HEIGHT);

	for (unsigned int cols = 0; cols < SLIDER_WIDTH - 1; cols++)
	{
		cout << ch;
	}
}

static void slider_move(unsigned int oldx, unsigned int oldy, unsigned int newx, unsigned int newy)
{
	//first erase slider at current location
	print_or_erase_slider(false,oldx,oldy);

	//paint at the new location
	print_or_erase_slider(true, newx, newy);
}

void slider_init(void)
{	
	sliderx = SLIDER_START_X;
	slidery = SLIDER_START_Y;
	
	print_or_erase_slider(true, sliderx, slidery);
}

void slider(void)
{
 	if (_kbhit())
	{
		hide_cursor();
		unsigned int kbkey = (_getch() << 8);
		kbkey |= (_getch());
		
		switch (kbkey)
		{
			case SLIDER_LEFT_KEY:
			{
				if (sliderx > SLIDER_X_MIN)
				{
					print_or_erase_slider(false, sliderx, slidery);

					if ((sliderx - SLIDER_SLIDE_STEP_SIZE) < SLIDER_X_MIN)
					{
						sliderx = SLIDER_X_MIN;
					}
					else
					{
						sliderx = sliderx - SLIDER_SLIDE_STEP_SIZE;
					}

					print_or_erase_slider(true, sliderx, slidery);
				}
			}
			break;

			case SLIDER_RIGHT_KEY:
			{
				if (sliderx < SLIDER_X_MAX)
				{
					print_or_erase_slider(false, sliderx, slidery);

					if ((sliderx + SLIDER_SLIDE_STEP_SIZE) > SLIDER_X_MAX)
					{
						sliderx = SLIDER_X_MAX;
					}
					else
					{
						sliderx = sliderx + SLIDER_SLIDE_STEP_SIZE;
					}

					print_or_erase_slider(true, sliderx, slidery);
				}								
			}
			break;
		}

	}
}

void slider_getCoordinates(unsigned int *x, unsigned int *y, unsigned int *w)
{
	*x = sliderx;
	*y = slidery;
	*w = SLIDER_WIDTH;
}




