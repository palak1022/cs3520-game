/*game.cpp -------
*
* Filename: game.cpp
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
#include <ncurses.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "snake.hpp"
#include "food.hpp"
#include "game_window.hpp"
#include "key.hpp"
#include "game.hpp"
#include <string.h>

void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = rand() % width + x_offset;
    *food_y = rand() % height + y_offset;
}

void drawWelcomeScreen()
{
    clear();
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int centerX = maxX / 2;
    int centerY = maxY / 2;
    int startY = centerY - 2;
    mvprintw(startY, centerX - 15, "Welcome to Snake Game!");
    mvprintw(startY + 2, centerX - 15, "Use the arrow keys to move the snake around");
    mvprintw(startY + 4, centerX - 15, "+20 points for eating a 0");
    mvprintw(startY + 6, centerX - 15, "-10 points for eating an X");
    mvprintw(startY + 8, centerX - 15, "Press 'q' to quit the game");
    mvprintw(startY + 10, centerX - 15, "Press 'p' to pause the game");
    mvprintw(startY + 12, centerX - 15, "Press 's' or 'S' to start the game'");
    refresh();

    char ch;
    while ((ch = getch()) != 's' && ch != 'S');
    clear();
    refresh();
}

void game(){
    enum State state = INIT; // Set the initial state
    static int x_max, y_max; //Max screen size variables
    static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
    gamewindow_t *window; // Name of the board
    Snake *snake; // The snake
    Food *foods,*new_food; // List of foods (Not an array)

    const int height = 30; 
    const int width = 70;
    char ch;

    struct timespec timeret;
    timeret.tv_sec = 0;
    timeret.tv_nsec = 999999999/4;

    
    while(state != EXIT){
        switch(state){
        case INIT:
            initscr();
            start_color();
            nodelay(stdscr, TRUE); //Dont wait for char
            noecho(); // Don't echo input chars
            getmaxyx(stdscr, y_max, x_max);
            keypad(stdscr, TRUE); // making keys work
            curs_set(0); // hide cursor
            timeout(100);

            // Setting height and width of the board
            x_offset = (x_max / 2) - (width / 2);
            y_offset = (y_max / 2) - (height / 2);

            //Welcome Screen
            drawWelcomeScreen();
            
            //Init board
            window = init_GameWindow(x_offset, y_offset, width, height);
            draw_Gamewindow(window);

            // Init snake
            snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
            
            // Init foods
            int food_x, food_y, i;
            enum Type type;

            //Generate 20 foods
            generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
            type = (rand() > RAND_MAX/2) ? Increase : Decrease; // Randomly deciding type of food
            foods = create_food(food_x, food_y, type);
            for(i = 1; i < 20; i++){
                generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                while (food_exists(foods,food_x, food_y))
                    generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);
            }
            state = ALIVE;
            break;

        case ALIVE:
            ch = get_char();
            
            switch(ch) {
                case UP:
                    move_snake(snake, UP);
                    break;
                case DOWN:
                    move_snake(snake, DOWN);
                    break;
                case LEFT:
                    move_snake(snake, LEFT);
                    break;
                case RIGHT:
                    move_snake(snake, RIGHT);
                    break;
                case 'p':
                    break;
                case 'P':
                    break;
                case 'q':
                    state = DEAD;
                    break;
                case 'Q':
                    state = DEAD;
                    break;
            }

			// Draw everything on the screen
            clear();
            mvprintw(20,20, "Key entered: %c", ch);
            draw_Gamewindow(window);
            draw_snake(snake);
            draw_food(foods);
            break;

        case DEAD:
            endwin();
            break;
        }
        refresh();
        nanosleep(&timeret, NULL);
    }
}
