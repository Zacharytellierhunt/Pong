#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>
using namespace std;


const enum enumDirection {
	STOP = 0, 
	LEFT = 1, 
	UPLEFT = 2, 
	DOWNLEFT = 3, 
	RIGHT = 4, 
	UPRIGHT = 5, 
	DOWNRIGHT = 6
};



class cBall {

private:
	int x, y;
	int originalX, originalY;
	enumDirection direction;

public:
	cBall(int posX, int posY) {

		originalX = posX;
		originalY = posY;
		direction = STOP;

		x = posX;
		y = posY;

	}

	void Reset() {

		direction = STOP;

		x = originalX;
		y = originalY;

	}

	void changeDirection(enumDirection d) {

		direction = d;

	}

	void randomDirection() {

		direction = (enumDirection)((rand() % 6) + 1);

	}

	inline int getX() { return x; }
	inline int getY() { return y; }
	inline enumDirection getDirection() { return direction; }

	void Move() {

		switch (direction) {

		case STOP:
			break;

		case LEFT:
			x--;
			break;

		case RIGHT:
			x++;
			break;

		case UPLEFT:
			x--;
			y--;
			break;

		case DOWNLEFT:
			x--;
			y++;
			break;

		case UPRIGHT:
			x++;
			y--;
			break;

		case DOWNRIGHT:
			x++;
			y++;
			break;

		default:
			break;

		}

	}

	friend ostream& operator << (ostream& stream, cBall ball) {

		stream << "Ball [" << ball.x << "," << ball.y << "][" << ball.direction << "]";
		return stream;

	}

};

class cPaddle {

private:

	int x, y;
	int originalX, originalY;

public:
	cPaddle() {
	
		x = 0;
		y = 0;
		originalX = 0;
		originalY = 0;
	
	}

	cPaddle(int posX, int posY) : cPaddle() {
	
		originalX = posX;
		originalY = posY;
		x = posX;
		y = posY;
	
	}

	inline void Reset() { x = originalX; y = originalY; }
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline void moveUp() { y--; }
	inline void moveDown() { y++; }

	//TODO: Optimize this, it can be turned into its own method with overloads because this same code
	//block appears in multiple places.
	friend ostream& operator << (ostream& stream, cPaddle Paddle) {

		stream << "Paddle [" << Paddle.x << "," << Paddle.y << "]";
		return stream;

	}
};

class cGameManager {

private:

	int width;
	int height;
	int scoreP1;
	int scoreP2;
	char up1, down1;
	char up2, down2;
	bool quit;

	//pointers, watch to not create a memory leak
	cBall* ball;
	cPaddle* player1;
	cPaddle* player2;

public:

	cGameManager(int w, int h) {
	
		srand(time(NULL));
		quit = false;
		width = w; height = h;
		ball = new cBall(w / 2, h / 2);

		//Player 1

		up1 = 'w'; down1 = 's';
		scoreP1 = 0;
		player1 = new cPaddle(1, h / 2 - 3);

		//Player 2
		
		up2 = 'o'; down2 = 'l';
		scoreP2 = 0;
		player2 = new cPaddle(w - 2, h / 2 - 3);
	
	}
	~cGameManager() {
	
		//destructor; frees memory to avoid memory leaks
		delete ball, player1, player2;
	
	}

	void ScoreUp(cPaddle* player) {
	
		if (player == player1)
			scoreP1++;
		else if (player == player2)
			scoreP2++;

		ball->Reset();
		player1->Reset();
		player2->Reset();
	
	}

	//draws all characters to screen
	void Draw() {
	
		clearScreen();

		for (int i = 0; i < width + 2; i++)
			//top wall 
			cout << "\xDC";
		cout << endl;

		//chars and spacing
		for (int i = 0; i < height; i++) {
			//TODO: see if i and j can be call x and y
			for (int j = 0; j < width; j++) {
			
				int ballx = ball->getX();
				int bally = ball->getY(); 

				int player1x = player1->getX();
				int player1y = player1->getY();
				
				int player2x = player2->getX();
				int player2y = player2->getY();
			
				//left wall 
				if (j == 0)
					cout << "\xDB";

				//ball
				if (ballx == j && bally == i)
					cout << "\xFE";

				//player1 paddle
				else if (player1x == j && player1y == i)
					cout << "\xDE";//player1, left top
				else if (player1x == j && player1y + 1 == i)
					cout << "\xDE";//player1, left middletop
				else if (player1x == j && player1y + 2 == i)
					cout << "\xDE";//player1, left middlebottom
				else if (player1x == j && player1y + 3 == i)
					cout << "\xDE";//player1, left bottom

				//player 2 paddle
				else if (player2x == j && player2y == i)
					cout << "\xDD";//player2, right top
				else if (player2x == j && player2y + 1 == i)
					cout << "\xDD";//player2, right middletop
				else if (player2x == j && player2y + 2 == i)
					cout << "\xDD";//player2, right middlebottom
				else if (player2x == j && player2y + 3 == i)
					cout << "\xDD";//player2, right bottom

				//empty space
				else
					cout << " ";


				//right wall
				if (j == width - 1)
					cout << "\xDB";

			}
			cout << endl;


		}
		for (int i = 0; i < width + 2; i++)
			//bottom wall
			cout << "\xDF";

		cout << endl;

		cout << "Score 1:" << scoreP1 << endl << "Score 2:" << scoreP2 << endl;

	}
	// Use Direct implementation of "cls" to clear screen without flickering
	void clearScreen()
	{
		HANDLE hOut;
		COORD Position;

		hOut = GetStdHandle(STD_OUTPUT_HANDLE);

		Position.X = 0;
		Position.Y = 0;
		SetConsoleCursorPosition(hOut, Position);
	}

	void Input() {
	
		ball->Move();

		int ballx = ball->getX();
		int bally = ball->getY();

		int player1x = player1->getX();
		int player1y = player1->getY();

		int player2x = player2->getX();
		int player2y = player2->getY();

		//handles keyboard inputs
		if (_kbhit()) {
		
			char current = _getch();

			//Player 1
			//moves player1 paddle up if y is less than 0
			if (current == up1)
				if (player1y > 0)
					player1->moveUp();
			//moves player1 paddle down if y does not exceed map height +4
			if (current == down1)
				if (player1y +4 < height)
					player1->moveDown();


			//Player 2
			//moves player2 paddle up if y is less than 0
			if (current == up2)
				if (player2y > 0)
					player2->moveUp();
			//moves player1 paddle down if y does not exceed map height +4
			if (current == down2)
				if (player2y + 4 < height)
					player2->moveDown();


			//checks if ball is stopped moves ball in random direction
			if (ball->getDirection() == STOP)
				ball->randomDirection();

			//quits game when q is pressed
			if (current == 'q')
				quit = true;

		}
	}

	void Logic() {
		
		//TODO: these variable show up in mutiple places create a function to improve readability
		int ballx = ball->getX(); 
		int bally = ball->getY();

		int player1x = player1->getX();
		int player1y = player1->getY();

		int player2x = player2->getX();
		int player2y = player2->getY();
	

		//Player 1, left BOUNCE
		for (int i = 0; i < 4; i++)
			if (ballx == player1x + 1)
				if (bally == player1y + i)
					ball->changeDirection((enumDirection)((rand() % 3) + 4));
		//Player 2, right BOUNCE
		for (int i = 0; i < 4; i++)
			if (ballx == player2x - 1)
				if (bally == player2y + i)
					ball->changeDirection((enumDirection)((rand() % 3) + 1));

		//top wall BOUNCE
		if (bally == height - 1) {
			ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
			cout << "heyo";
		}
		//bottom wall BOUNCE
		if (bally == 0) {
			ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
			cout << "dude";
		}
		
		//left wall, player 1 net
		if (ballx == 0)
			ScoreUp(player2);
		//right wall, player 2 net
		if (ballx == width - 1)
			ScoreUp(player1);
	}

	void Run() {
	
		while (!quit) {
		
			Draw();
			Input();
			Logic();
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
			Sleep(50);

		}
	}

};

int main() {

	cGameManager game(40, 20);
	game.Run();


	return 0;
	 
}