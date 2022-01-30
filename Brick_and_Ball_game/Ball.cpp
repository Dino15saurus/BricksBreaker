#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include "Slider.h"
#include "Ball.h"
#include "Bricks.h"

using namespace std;
 
#define DEBUG_PRINT false

#define BALL_X_MAX 236
#define BALL_X_MIN 2
#define BALL_Y_MAX 63
#define BALL_Y_MIN 6
#define BALL_START_X BALL_X_MAX
#define BALL_START_Y BALL_Y_MIN

#define BALL_BOUNCE_LOWER_BOUNDARY false

#define BALL_PARABOLA_FLIGHT true
#define BALL_STRAIGHT_LINE_FLIGHT false

#if(BALL_STRAIGHT_LINE_FLIGHT == true)
#define BALL_PATH_SLOPE 2
#define BALL_PATH_STEP 1
#define BALL_TIME_SLICE 1 //2secs
#endif

#if(BALL_PARABOLA_FLIGHT == true)
#define BALL_PATH_STEP 1
#define PARABOLA_VERTEX_H_SHIFT 42
#define PARABOLA_A_VALUE 0.15//in equation y = a(x-h)^2 + k
#endif

#define clear()       
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))


unsigned int* Ballx;
unsigned int* Bally;

int vertexh;
unsigned int vertexk;

unsigned int counter = 0;
int life_count = 3;

bool print_star = false;

enum Ball_states_t
{
	TOP_RIGHT = 0,
	TOP_LEFT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT
};

Ball_states_t Ball_state;

static unsigned int straight_line_nexty(unsigned int currentx, unsigned int currenty, unsigned int futurex, int slope);
static unsigned int parabola_nexty(float a, unsigned int vertexh, unsigned int vertexk, unsigned int futurex, unsigned int currenty);

static unsigned int straight_line_nexty(unsigned int currentx, unsigned int currenty, unsigned int futurex, int slope)
{
	unsigned int futurey = (slope * (futurex - currentx)) + currenty;
	return futurey;
}

static unsigned int parabola_nexty(float a, unsigned int vertexh, unsigned int vertexk, unsigned int futurex, unsigned int currenty)
{
	// y = a(x-h)^2 + k
	unsigned int futurey;
	unsigned int temp = (unsigned int)((a * ((futurex - vertexh) * (futurex - vertexh))) + vertexk);
	
	if (abs(int(currenty - temp)) > 3)
	{
		if (temp > currenty)
		{
			futurey = currenty + 0.25 * (abs(int(currenty - temp)));
		}
		else
		{
			futurey = currenty - 0.25 * (abs(int(currenty - temp)));
		}
	}
	else
	{
		futurey = temp;
	}

	if (futurey < vertexk)
	{
		futurey = vertexk;
	}

	if ((futurex > (BALL_X_MAX+5)) || (futurey > (BALL_Y_MAX+5)))
	{
		cout << " Error";
	}

	return futurey;
}


void Ball_init(unsigned int* bx, unsigned int* by)
{
	//Initializing the ball's starting point
	*bx = BALL_START_X;
	*by = BALL_START_Y;

	Ballx = bx;
	Bally = by;
	Ball_state = BOTTOM_LEFT;

	vertexh = BALL_X_MAX;
	vertexk = BALL_Y_MIN;

	gotoxy(*bx, *by);
	printf("O");

	gotoxy(5, 3);
	cout << "Lives: " << life_count;
		
	//hiding the cursor here
	gotoxy(238, 78);
}

bool Ball(void)
{	
	bool isAlive = true;

	if (++counter > 0x1FFF)
	{
		counter = 0;

		switch (Ball_state)
		{
		case TOP_RIGHT:
		{
			unsigned int x = *Ballx;
			unsigned int y = *Bally;
			unsigned int x1 = x + BALL_PATH_STEP;
			

#if(BALL_STRAIGHT_LINE_FLIGHT == true)
			int y1 = straight_line_nexty(x, y, x1, (-1 * BALL_PATH_SLOPE));
#elif(BALL_PARABOLA_FLIGHT == true)
			int y1 = parabola_nexty(PARABOLA_A_VALUE, vertexh, vertexk, x1, y);
#endif
			gotoxy(x, y);
			cout << " ";

			//ball is hitting top boundary
			if (is_brick_hit(&x1, &y1))
			{				
#if(DEBUG_PRINT == true)
				gotoxy(5, 4);
				cout << "BALL_STATE: " << Ball_state;
				gotoxy(30, 4);
				cout << "OLD_VERTEX: " << vertexh << " " << vertexk;
				gotoxy(55, 4);
				cout << "NEW_VERTEX: " << x1 << " " << y1 << "    ";
#endif

				vertexh = x1;
				vertexk = y1;

				Ball_state = BOTTOM_RIGHT;
				print_star = true;

				if (print_star == true)
				{
					Beep(200, 25);
				}
			}
			//ball is hitting right boundary
			else if (x1 >= BALL_X_MAX)
			{

#if(DEBUG_PRINT == true)
				gotoxy(5, 4);
				cout << "BALL_STATE: " << Ball_state;
				gotoxy(30, 4);
				cout << "OLD_VERTEX: " << vertexh << " " << vertexk << "    ";
#endif
				vertexh = BALL_X_MAX - abs(vertexh - BALL_X_MAX);				
				vertexk = vertexk;

#if(DEBUG_PRINT == true)
				gotoxy(55, 4);
				cout << "NEW_VERTEX: " << vertexh << " " << vertexk << "   ";
#endif

				Ball_state = TOP_LEFT;

				//reset the position
				x1 = BALL_X_MAX;
				print_star = true;
			}
			else if (y1 <= BALL_Y_MIN)
			{
				//ball is hitting top boundary
				vertexh = x1;
				vertexk = BALL_Y_MIN;

				Ball_state = BOTTOM_RIGHT;

				y1 = BALL_Y_MIN;
				
			}
			
			//draw ball
			gotoxy(x1, y1);
			cout << 'O';
					

			*Ballx = x1;
			*Bally = y1;
			
		}
		break;

		case TOP_LEFT:
		{	
			print_star = false;
			unsigned int x = *Ballx;
			unsigned int y = *Bally;
			unsigned int x1 = x - BALL_PATH_STEP;
#if(BALL_STRAIGHT_LINE_FLIGHT == true)
			int y1 = straight_line_nexty(x, y, x1, (1 * BALL_PATH_SLOPE));
#elif(BALL_PARABOLA_FLIGHT == true)
			int y1 = parabola_nexty(PARABOLA_A_VALUE, vertexh, vertexk, x1, y);
#endif
			gotoxy(x, y);
			cout << " ";			

			if (is_brick_hit(&x1, &y1))
			{				
#if(DEBUG_PRINT == true)
				gotoxy(5, 4);
				cout << "BALL_STATE: " << Ball_state;
				gotoxy(30, 4);
				cout << "OLD_VERTEX: " << vertexh << " " << vertexk << "   ";
				gotoxy(55, 4);
				cout << "NEW_VERTEX: " << x1 << " " << y1 << "    ";
#endif
				vertexh = x1;
				vertexk = y1;

				print_star = true;
				if (print_star == true)
				{
					Beep(200, 25);
				}


				Ball_state = BOTTOM_LEFT;
			}
			else if (x1 <= BALL_X_MIN)
			{
#if(DEBUG_PRINT == true)
				gotoxy(5, 4);
				cout << "BALL_STATE: " << Ball_state;
				gotoxy(30, 4);
				cout << "OLD_VERTEX: " << vertexh << " " << vertexk << "   ";
#endif
				//ball is hitting left boundary
				vertexh = BALL_X_MIN - (vertexh - BALL_X_MIN);
				vertexk = vertexk;

#if(DEBUG_PRINT == true)
				gotoxy(55, 4);
				cout << "NEW_VERTEX: " << vertexh << " " << vertexk << "   ";
#endif

				Ball_state = TOP_RIGHT;

				//reset the position
				x1 = BALL_X_MIN;
			}			
			else if (y1 <= BALL_Y_MIN)
			{
				vertexh = x1;
				vertexk = BALL_Y_MIN ;

				//ball is hitting top boundary				
				Ball_state = BOTTOM_LEFT;

				y1 = BALL_Y_MIN;
			}		

			//draw ball
			gotoxy(x1, y1);
			cout << 'O';

			if (print_star == true)
			{
				Beep(200, 25);
			}

			*Ballx = x1;
			*Bally = y1;
			
		}
		break;

		case BOTTOM_RIGHT:
		{			
			unsigned int x = *Ballx;
			unsigned int y = *Bally;
			unsigned int x1 = x + BALL_PATH_STEP;
#if(BALL_STRAIGHT_LINE_FLIGHT == true)
			int y1 = straight_line_nexty(x, y, x1, (1 * BALL_PATH_SLOPE));
#elif(BALL_PARABOLA_FLIGHT == true)
			int y1 = parabola_nexty(PARABOLA_A_VALUE, vertexh, vertexk, x1, y);
#endif
			gotoxy(x, y);
			cout << " ";

			//is it going to hit the slider		
			unsigned int sliderx;
			unsigned int slidery;
			unsigned int slider_w;

			slider_getCoordinates(&sliderx, &slidery, &slider_w);
			//if slider is hit
			if ((y1 >= slidery) && ((sliderx <= x1) && (x1 <= (sliderx + slider_w))))
			{
#if(DEBUG_PRINT == true)
				gotoxy(5, 4);
				cout << "BALL_STATE: " << Ball_state;
				gotoxy(30, 4);
				cout << "OLD_VERTEX: " << vertexh << " " << vertexk << "    ";
#endif
				vertexh = vertexh + PARABOLA_VERTEX_H_SHIFT; 
				vertexk = BALL_Y_MIN;

#if(DEBUG_PRINT == true)
				gotoxy(55, 4);
				cout << "NEW_VERTEX: " << vertexh << " " << vertexk << "   ";
#endif

				Ball_state = TOP_RIGHT;
				*Ballx = x1;
				*Bally = slidery;
			}
			//is it beyond the lower boundary
			else if (y1 >= BALL_Y_MAX - 1)
			{
#if (BALL_BOUNCE_LOWER_BOUNDARY	== true)
				
				//bounce off lower boundary
				Ball_state = TOP_RIGHT;
				*Ballx = x1;
				*Bally = BALL_Y_MAX;

#if(DEBUG_PRINT == true)
				gotoxy(5, 4);
				cout << "BALL_STATE: " << Ball_state;
				gotoxy(30, 4);
				cout << "OLD_VERTEX: " << vertexh << " " << vertexk << "    ";
#endif
				vertexh = vertexh + PARABOLA_VERTEX_H_SHIFT;				
				vertexk = BALL_Y_MIN;

#if(DEBUG_PRINT == true)
				gotoxy(55, 4);
				cout << "NEW_VERTEX: " << vertexh << " " << vertexk << "   ";
#endif
				
#else				
				//ball is dead reinitialize the ball back to starting point
				gotoxy(5, 3);
				cout << "Lives: " << life_count;

				life_count--;

				Ball_init(Ballx, Bally);
				Ball_state = BOTTOM_LEFT;

				gotoxy(5, 3);
				cout << "Lives: " << life_count;

				if (life_count == 0)
				{
					gotoxy(5, 3);
					cout << "YOU HAVE RAN OUT OF LIVES";
					isAlive = false;
					break;
				}				
#endif
			}
			//ball is hitting right boundary
			else if (x1 >= BALL_X_MAX)
			{	

#if(DEBUG_PRINT == true)
				gotoxy(5, 4);
				cout << "BALL_STATE: " << Ball_state;
				gotoxy(30, 4);
				cout << "OLD_VERTEX: " << vertexh << " " << vertexk << "    ";
#endif
				vertexh = BALL_X_MAX + abs(vertexh - BALL_X_MAX);
				vertexk = vertexk;

#if(DEBUG_PRINT == true)
				gotoxy(55, 4);
				cout << "NEW_VERTEX: " << vertexh << " " << vertexk << "   ";
#endif

				Ball_state = BOTTOM_LEFT;

				//reset the position
				*Ballx = BALL_X_MAX;
				*Bally = y1;
			}
			else
			{
				*Ballx = x1;
				*Bally = y1;
			}
			
			//draw ball
			gotoxy(*Ballx, *Bally);
			cout << 'O';
			
		}
		break;

		case BOTTOM_LEFT:
		{			
			unsigned int x = *Ballx;
			unsigned int y = *Bally;
			unsigned int x1 = x - BALL_PATH_STEP;
#if(BALL_STRAIGHT_LINE_FLIGHT == true)
			int y1 = straight_line_nexty(x, y, x1, (-1 * BALL_PATH_SLOPE));
#elif(BALL_PARABOLA_FLIGHT == true)
			int y1 = parabola_nexty(PARABOLA_A_VALUE, vertexh, vertexk, x1,y);
#endif
		
			gotoxy(x, y);
			cout << " "; 			
			
			//is it going to hit the slider		
			unsigned int sliderx;
			unsigned int slidery;
			unsigned int slider_w;

			slider_getCoordinates(&sliderx, &slidery, &slider_w);
			//if slider is hit
			if ((y1 >= slidery) && ((sliderx <= x1) && (x1 <= (sliderx + slider_w))))
			{
#if(DEBUG_PRINT == true)
				gotoxy(5, 4);
				cout << "BALL_STATE: " << Ball_state;
				gotoxy(30, 4);
				cout << "OLD_VERTEX: " << vertexh << " " << vertexk << "    ";
#endif
				vertexh = vertexh - PARABOLA_VERTEX_H_SHIFT;//2*(abs(int(x1 - vertexh)));				
				vertexk = BALL_Y_MIN;

#if(DEBUG_PRINT == true)
				gotoxy(55, 4);
				cout << "NEW_VERTEX: " << vertexh << " " << vertexk << "   ";
#endif

				Ball_state = TOP_LEFT;
				*Ballx = x1;
				*Bally = slidery;
			}
			//is it beyond the lower boundary
			else if (y1 >= BALL_Y_MAX - 1)
			{
#if (BALL_BOUNCE_LOWER_BOUNDARY	== true)
				{

#if(DEBUG_PRINT == true)
					gotoxy(5, 4);
					cout << "BALL_STATE: " << Ball_state;
					gotoxy(30, 4);
					cout << "OLD_VERTEX: " << vertexh << " " << vertexk << "    ";
#endif
					vertexh = vertexh - PARABOLA_VERTEX_H_SHIFT;			
					vertexk = BALL_Y_MIN;

#if(DEBUG_PRINT == true)
					gotoxy(55, 4);
					cout << "NEW_VERTEX: " << vertexh << " " << vertexk << "   ";
#endif

					//bounce off lower boundary
					Ball_state = TOP_LEFT;
					*Ballx = x1;
					*Bally = BALL_Y_MAX;
				}
#else
				{
					//ball is dead reinitialize the ball back to starting point
					gotoxy(5, 3);
					cout << "Lives: " << life_count;

					life_count--;

					Ball_init(Ballx, Bally);
					Ball_state = BOTTOM_LEFT;

					gotoxy(5, 3);
					cout << "Lives: " << life_count;

					if (life_count == 0)
					{
						gotoxy(5, 3);
						cout << "YOU HAVE RAN OUT OF LIVES";
						isAlive = false;

						break;
					}

				}
#endif
			}
			//ball is hitting left boundary
			else if (x1 <= BALL_X_MIN)
			{

#if(DEBUG_PRINT == true)
				gotoxy(5, 4);
				cout << "BALL_STATE: " << Ball_state;
				gotoxy(30, 4);
				cout << "OLD_VERTEX: " << vertexh << " " << vertexk << "   ";
#endif
				//ball is hitting left boundary
				vertexh = BALL_X_MIN - (vertexh - BALL_X_MIN);
				vertexk = vertexk;

#if(DEBUG_PRINT == true)
				gotoxy(55, 4);
				cout << "NEW_VERTEX: " << vertexh << " " << vertexk << "   ";
#endif

				Ball_state = BOTTOM_RIGHT;

				//reset the position
				*Ballx = BALL_X_MIN;
				*Bally = y1;
			}
			//normal path
			else
			{
				*Ballx = x1;
				*Bally = y1;
			}
			//draw ball
			gotoxy(*Ballx, *Bally);
			cout << 'O';
			
		}
		break;

		default:
		{
			Ball_state = TOP_RIGHT;
		}
		break;

		}
	} 

	//return isAlive to main
	return isAlive;

}//end of ball funct

