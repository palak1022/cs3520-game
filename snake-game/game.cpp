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
using  namespace std;
void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = rand() % width + x_offset + 1;
    *food_y = rand() % height + y_offset + 1;
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
      gamewindow_t *window = init_GameWindow(0, 0, 70, 30); // Example values for the game window size
      draw_Gamewindow(window);
      // Initialize the snake
    Snake *snake = init_snake(/* provide necessary parameters */);



     // Variables for scoring and game speed
    int score = 0;
    double speed_multiplier = 1.0;
    // Displaying welcome screen
    mvprintw(1, 1, "Welcome to Snake Game!");
    mvprintw(3, 1, "Press 's' or 'S' to start the game.");
    mvprintw(5, 1, "Controls:");
    mvprintw(6, 1, "Use arrow keys to move the snake.");
    mvprintw(7, 1, "Press 'q' or 'Q' to quit the game.");
    mvprintw(9, 1, "Scoring:");
    mvprintw(10, 1, "Eating increasing food (+) adds 20 points.");
    mvprintw(11, 1, "Eating decreasing food (-) subtracts 10 points.");

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

           // Handle movement based on user input
            switch (ch)
            {
            case KEY_LEFT:
                return LEFT;
                break;
            case RIGHT:
                // Move the snake right
                break;
            case UP:
                // Move the snake up
                break;
            case DOWN:
                // Move the snake down
                break;
            }

            // Draw everything on the screen
            clear();
            draw_Gamewindow(window);
            draw_snake(snake);
            draw_food(foods);
            break;
            // Move the snake
            snake = move_snake(snake, direction);

            // Draw everything on the screen
            clear();
        
            draw_Gamewindow(window);
            draw_snake(snake);
            draw_food(foods);
            break;

                }
 // Draw everything on the screen
                clear();
                draw_Gamewindow(window);
                draw_snake(snake);
                draw_food(foods);
                mvprintw(0, 0, "Score: %d", score); // Displaying score
                refresh();
                break;
    }
    
        case DEAD:
            endwin();
            break;
        }
        refresh();
        nanosleep(&timeret, NULL);
    
 
