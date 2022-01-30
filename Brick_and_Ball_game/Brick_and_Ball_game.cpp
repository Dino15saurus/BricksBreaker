#include <iostream>
#include <stdio.h>
#include "Ball.h"
#include "Bricks.h"
#include "Slider.h"


using namespace std;

#define DEBUG_PRINTS true

#define clear()       
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define hide_cursor() printf("\e[?25l");
#define show_cursor() printf("\e[?25h");

void printScreen(void);

unsigned int ballx, bally;

bool Bricks_hit, Slider_hit, Ball_dropped;

int main()
{
    cout << "Maximize the screen and press enter to continue....";
    cout << "Test";
    char enter = getchar();
    system("CLS");
    //hide_cursor();


    //Init variables
    Bricks_hit = false;
    Slider_hit = false;
    Ball_dropped = false;
    bool isAlive = true;

    //paint the screen
    printScreen();

    //Slider function
    slider_init();
    Bricks_init();
    Ball_init(&ballx, &bally);

    //launch Bricks task, Slider task, and Ball task.   
    while (isAlive)
    {        
        slider();
        isAlive = Ball();       
    }

    return 0;
}

void printScreen(void)
{
#define MAX_COLUMNS 237
#define MAX_ROWS   64

     //clear the screen
    //system("CLS");
    clear();

    gotoxy(0, 0);
    //Draw Upper border.
    for (unsigned int j = 0; j  < MAX_COLUMNS; j++)
    {
        cout << "_";
    }
           
    gotoxy(0, 2);
    cout << "|";

    //Draw Title        
    gotoxy(MAX_COLUMNS/2-11, 3);
    cout << "Bricks N Balls ver 1.0 ";
       
    gotoxy(MAX_COLUMNS, 2);
    cout << "|";

    gotoxy(0, 3);
    cout << "|";

    gotoxy(MAX_COLUMNS, 3);
    cout << "|";
    
    gotoxy(0, 4);
    cout << "|";
    gotoxy(MAX_COLUMNS, 4);
    cout << "|";
    gotoxy(0, 5);
    cout << "|";

    for (unsigned int j = 0; j < MAX_COLUMNS-2; j++)
    {
        cout << "_";
    }
    cout << "|\n";
        
    for (unsigned int i = 6; i <= MAX_ROWS; i++)
    {
        gotoxy(0, i);
        cout << "|";

        gotoxy(MAX_COLUMNS, i);
        cout << "|";
    }

    for (unsigned int j = 2; j < MAX_COLUMNS; j++)
    {
        gotoxy(j, MAX_ROWS);
        cout << "_";
    }
    
}


//