/*snake_hame.cpp -------
*
* Filename: snake_game.cpp
* Description:
* Author: Adeel Bhutta
* Maintainer:
* Created: Sat Sep 12 13:16:12 2022
* Last-Updated: September 12 16:51 2022
*
*/

/* Commentary:
*
*
*
*/

/* Change log:
*
*
*/

/*Copyright (c) 2022 Adeel Bhutta
*
* All rights reserved.
*
* Additional copyrights may follow
*/
#include "game.hpp"
#include <cstdlib>
#include <cstdio>
#include <ncurses.h>

int main(void){
    printf("thuoeou");
    game();
    printf("tuenao");
    return(0);
}
// sets welcome screen
void setWelcomeScreen()
{

	initscr(); 
	noecho();
	
	
}

// gets the users input
int Input()
{
	int input = getch();
	refresh();
	endwin();
	clear();
	return input;	
}

int ready() 
{
    setWelcomeScreen();
	 move(1, 10);
            printw("Welcome to the snake game!");
            move (2, 10);
            printw("Rules:");
            move(3,10);
            printw("Use the arrow keys to move your snake to collect as many points as possible!");
            move(4, 10);
            printw("You gain points and size by running over the food, which can either make you");
            move(5,10);
            printw("grow or shrink. Vowels makes you grow, while consonants makes you shrink.");
            move(7,10);
            printw("Controls:");
            move(8,10);
            printw("Press [Q] or [q] at anytime to quit the game");
            move(9,10);
            printw("Press [P] or [p] to pause the game");
            move(10,10);
            printw("Avoid the obstacles to stay alive!");
            move(12, 10);
            printw("WARNING: STARTING GAME STARTS IT IMMEDIATLY"); 
            move(14, 30);
            printw("There are three level, [1] is easy, [2] is medium, and [3] is hard"); 
            move(15, 30);
            printw("Press what level you wish to play: "); 
            move(16, 30);
            printw("WARNING: STARTING GAME STARTS IT IMMEDIATLY"); 
	return Input();
}