#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>  // for rand()
#include <ctime>    // for time()

using namespace std;

// Create gameOver variable.
bool gameOver;
// Setting the size of the game screen.
const int height = 20;
const int width = 40;

// Variables to store the coordinates of the snake's head
// and the fruit, as well as the scores.
int x, y, fruitX, fruitY, score, highScore;

// Arrays storing the coordinates of each segment of the tail
// and a variable tracking the length of the tail.
int tailX[100],tailY[100],nTail;

// Define an enumeration to hold possible directions of movement.
enum eDirection {STOP = 0, UP, DOWN, LEFT, RIGHT};
// Variable to store the direction of the snake.
eDirection dir;

// Function to determine if the fruit is on the snake or not.
bool IsFruitOnSnake() {
	// Check if the coordinates of the fruit match any segment of the tail.
    for (int i = 0; i < nTail; i++) {
        if (fruitX == tailX[i] && fruitY == tailY[i]) {
            return true;
        }
    }
    // Check if the coordinates of the fruit match the coordinates of the
    // snake's head.
    if (fruitX == x && fruitY == y) {
    	return true;
    }
    else {
    	return false;
    }
}

// Function to place the fruit on the game screen.
void PlaceFruit() {

	// Continues looping until a valid position for the fruit is found.
	do {
		// Generates random coordinates for the fruit with the game boundaries.
		fruitX = rand()% (width - 2) + 1;
		fruitY = rand()% (height - 2) + 1;
	} while ((fruitX == x && fruitY == y) || IsFruitOnSnake());
}

// Function to initialize the game state at the start of each round.
void Setup(){
	gameOver = false;
	// Makes it so the snake is not moving in any direction.
	dir=STOP;
	// Generates the initial position of the snake's head.
	x=width/2;
	y=height/2;
	// Sets the beginning length of the tail to 0.
    nTail = 0;

    // Generates the initial tail positions.
    for (int i = 0; i < 100; i++) {
        tailX[i] = 0;
        tailY[i] = 0;
    }

    // Places the initial fruit on the game grid.
	PlaceFruit();
}
// Function to draw the game grid and display the snake, fruit, and scores.
void Draw(){

	// Sets the cursor position to the top-left corner of the game screen.
	// This ensures that each update of the game will start drawing at the
	// beginning of the console window.
	COORD coord = {1, 1};
	coord.X = 0;
	coord.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

	// Draws the top border of the game grid.
	for(int i = 0;i < width; i++){
		cout << "-";
	} cout << endl;

	// Draws the game grid, snake, and fruit.
	for(int i = 0;i < height; i++){
		for(int j = 0; j < width; j++){
			// Draws the walls.
			if(j == 0 || j == width - 1){
				cout << "|";
			// Draws the head of the snake.
			}else if(i == y && j == x){
				cout << "O";
			// Draws the fruit.
			}else if(i == fruitY && j == fruitX){
				cout << "*";
			}else {
				bool print = false;

				// Check if the current position corresponds to any segment of
				// the snake's tail.
				for(int k = 0; k < nTail; k++){
					if(i == tailY[k] && j == tailX[k]){
						// Prints a tail segment if the position does correspond.
						cout << "o";
						print = true;
					}
				}
				// Display an empty space if there is nothing at this position.
				if(!print){cout<<" ";}
			}
		} cout<<endl;
	}

	// Draws the bottom border of the game grid.
	for(int i=0;i<width;i++){
		cout<<"-";
	}cout<<endl;

	// Inserts a blank line.
	cout<<endl;

	// Displays the current score and the high score.
	cout<<"Score: "<<score<<endl;
	cout<<"High Score: "<<highScore<<endl;
}

// Function to handle user input for controlling the snake.
void Input(){

	// Checks arrow keys for direction input.
	 if (GetAsyncKeyState(VK_UP) & 0x8000) {
		dir = UP;
	} else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		dir = DOWN;
	} else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		dir = LEFT;
	} else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		dir = RIGHT;
	}

	// Checks keyboard (WASD) for direction input.
	if(_kbhit()){
		switch(_getch()){
		case 'w':
			dir=UP;
			break;
		case 'a':
			dir=LEFT;
			break;
		case 's':
			dir=DOWN;
			break;
		case 'd':
			dir=RIGHT;
			break;
		default:
			break;
		}
	}
}

// Function to update game logic, including snake movement, collisions, and
// fruit consumption.
void Logic(){

	// Save the current position of the snake's head
	int prev2X, prev2Y;
	int prevX = tailX[0];
	int prevY = tailY[0];

	// Update the position of the snake's head based on the current direction.
	tailX[0]= x;
	tailY[0]= y;

	// Update the positions of the snake's body segements.
	for(int i=1;i<nTail;i++){
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	// Move the snake's head based on the current direction.
	switch(dir){
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	default:
		break;
	}

	// Checks for collisions with the game boundaries.
	if(x < 0 || x > width || y < 0 || y > height){
		gameOver = true;
	}

	// Checks for collision with the snake's body.
	for(int i = 0; i < nTail; i++){
		if(x == tailX[i] && y == tailY[i]){
			gameOver = true;
		}
	}

	// Checks if the snake has eaten the fruit and updates the score.
	if(x==fruitX && y==fruitY){
		score+=10;

		// Loop to find a new random position for the fruit that does not
		// interfere with the snake.
		do {
			fruitX = rand() % (width - 2) + 1;
			fruitY = rand() % (height - 2) + 1;
		} while (IsFruitOnSnake());

		// Increase the length of the snake
		nTail++;
	}
}

// Function to clear the screen.
void clearScreen() {
    system("cls");
}

// Main function containing the game loop.
int main(){

	// Aids the rand() function in generating random positions for the fruit.
	srand(static_cast<unsigned>(time(nullptr)));

	// Initializes the game state.
	Setup();

	// Stores user input on whether or not they want to play again.
	string input = "";

	// Variable to control the game loop.
	bool running = true;


	// Continues looping as long as user wants to play the game.
	while(running){

		// Continues looping until the round is over.
		while(!gameOver){

			// Draws the game elements.
			Draw();

			// Handles user input.
			Input();

			// Updates game logic.
			Logic();

			// Adjusts sleep duration based on the snake's direction.
			if (dir == UP || dir == DOWN) {
				Sleep(80);
			} else {
				Sleep(40);
			}
		}

		// Prompts user to play again or exit the game.
		cout << "\nPlay again? (y/n) ";
		cin >> input;

		// Processes user input for play again.
		if (input == "y") {
			// Updates the high score if the current score is higher.
			if (score > highScore) {
				highScore = score;
			}

			// Resets the score and clears the screen for a new game.
			score = 0;
			clearScreen();
			// Restarts the game.
			Setup();
			gameOver = false;
		}
		// Exits the game loop if the user chooses not to play again.
		else if (input == "n") {
			running = false;
		}
	}

	return 0;
}
