//============================================================================
// Name        : Snake.cpp
// Author      : Flizzet
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y;						// Player position
int tailX[100], tailY[100];		// Tail arrays
int nTail;						// Length of the tail
int fruitX, fruitY;				// Position of fruit (score)
int score;						// Players score
/* Players position enumerator */
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;					// Players position


int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

void Setup() {
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}

void Draw() {
	/* Clear screen */
	system("clear");
	/* Draw background border along width */
	for (int i = 0; i < width + 2; i++) {
		cout << "#";
	}
	/* End line */
	cout << endl;

	/* Print map */
	// Scale height
	for (int i = 0; i < height; i++) {
		// Scale width
		for (int j = 0; j < width; j++) {
			/* Check if it's a first field */
			if (j == 0) {
				/* Draw a wall */
				cout << "#";
			}

			/* Draw player */
			if (i == y && j == x) {
				cout << "O";
			} else if (i == fruitY && j == fruitX) {
				cout << "F";
			} else {
				/* Draw tail
				 * Also the most for loops in for loops I've ever done #include sauce.h */
				bool print = false;
				for (int k = 0; k < nTail; k++) {
					if (tailX[k] == j && tailY[k] == i) {
						cout << "o";
						print = true;
					}

				}
				if (!print) {
					cout << " ";
				}
			}

			/* Check if it's a last field */
			if (j == width - 1) {
				/* Draw a wall */
				cout << "#";
			}
		}
		cout << endl;
	}

	/* Draw background border along height */
	for (int i = 0; i < height + 2; i++) {
		cout << "#";
	}
	/* End line */
	cout << endl;

	/* Draw score */
	cout << "Score: " << score << endl;
}

void Input() {
	/* Set direction based on key hit */
	if (kbhit()) {
		switch (getchar()) {
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'x':
			gameOver = true;
			break;
		}
	}
}

void Logic() {
	/* Previous tail positions */
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	/* Move the tail */
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++) {
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;

		/* The person in this tutorial I'm watching doesn't explain what the fuck any of this means so idk just roll with it buddy */
		prevX = prev2X;
		prevY = prev2Y;
	}

	/* Move based on direction */
	switch(dir) {
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	default:
		break;
	}

	/* Collision with walls */
	if (x > width || x < 0 || y > height || y < 0) {
		gameOver = true;
	}
	/* Collision with tail */
	for (int i = 0; i < nTail; i++) {
		if (tailX[i] == x && tailY[i] == y) {
			gameOver = true;
		}
	}

	/* Collision with fruit */
	if (x == fruitX && y == fruitY) {
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}
 }

int main() {
	Setup();
	while(!gameOver) {
		Draw();
		Input();
		Logic();
		usleep(105000);
	}
	return 0;
}
