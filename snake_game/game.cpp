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
#include <curses.h>
#include <cstdio>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include "snake.hpp"
#include "food.hpp"
#include "game_window.hpp"
#include "key.hpp"
#include "game.hpp"
using namespace std;

// Initialize global Variables
int score = 0;
int gameSpeed = 10;
Direction snake_direction = RIGHT;
bool quit = false;
Snake* lastSegment;
int lives = 3;

// Obstacle Struct
struct Obstacle {
    int x;
    int y;
    int size;

    Obstacle(int x_, int y_, int size_) : x(x_), y(y_), size(size_) {}
};

enum DifficultyLevel {
    EASY,
    HARD,
    ADVANCED
};

struct GameParameters {
    int snakeSpeed;
    int numFoodItems;
    int numObstacles;
    int numLives;
};



GameParameters setEasyParameters() {
    return { 1, 5, 3, 3 }; // Example values, adjust as needed
}

// Function to set game parameters for hard difficulty
GameParameters setHardParameters() {
    return { 2, 10, 5, 3 }; // Example values, adjust as needed
}

// Function to set game parameters for advanced difficulty
GameParameters setAdvancedParameters() {
    return { 3, 15, 7, 3 }; // Example values, adjust as needed
}

vector<Obstacle> generate_obstacles(int width, int height, int snake_x, int snake_y, DifficultyLevel difficulty) {
    vector<Obstacle> obstacles;
    srand(time(nullptr)); // Seed the random number generator

    int numObstacles = 0;

    switch (difficulty) {
        case EASY:
            numObstacles = 3; // Example value, adjust as needed
            break;
        case HARD:
            numObstacles = 5; // Example value, adjust as needed
            break;
        case ADVANCED:
            numObstacles = 7; // Example value, adjust as needed
            break;
    }

    for (int i = 0; i < numObstacles; ++i) {
        int obstacle_x = rand() % (width - 2) + 1; // Generate random x coordinate within the game window (excluding borders)
        int obstacle_y = rand() % (height - 2) + 1; // Generate random y coordinate within the game window (excluding borders)
        
        // Ensure obstacle doesn't start on the snake's position
        while ((obstacle_x == snake_x && obstacle_y == snake_y) || 
               (obstacle_x == snake_x + 1 && obstacle_y == snake_y) || 
               (obstacle_x == snake_x && obstacle_y == snake_y + 1) || 
               (obstacle_x == snake_x + 1 && obstacle_y == snake_y + 1)) {
            obstacle_x = rand() % (width - 2) + 1;
            obstacle_y = rand() % (height - 2) + 1;
        }

        int size = rand() % 4 + 2; // Random size between 2 and 5

        // Ensure obstacle doesn't extend beyond game window boundaries
        if (obstacle_x + size > width - 1) {
            obstacle_x = width - size - 1;
        }
        if (obstacle_y + size > height - 1) {
            obstacle_y = height - size - 1;
        }

        obstacles.push_back({obstacle_x, obstacle_y, size});
    }

    return obstacles;
}

void decrement_lives() {
    lives--;
}

void print_obstacles(const vector<Obstacle>& obstacles) {
    for (const auto& obstacle : obstacles) {
        for (int i = 0; i < obstacle.size; ++i) {
            for (int j = 0; j < obstacle.size; ++j) {
                mvprintw(obstacle.y + i, obstacle.x + j * 2, "* "); // Multiply j by 2 to create a grid
            }
        }
    }
}

void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = rand() % (width-2) + x_offset + 1;
    *food_y = rand() % (height-2) + y_offset + 1;
}


bool check_boundary_collision(int x, int y, int width, int height) {
    return x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1;
}

bool check_obstacle_collision(int x, int y, const vector<Obstacle>& obstacles) {
    // Check if the coordinates (x, y) overlap with any obstacle in the vector
    for (const auto& obstacle : obstacles) {
        if (x >= obstacle.x && x < obstacle.x + obstacle.size &&
            y >= obstacle.y && y < obstacle.y + obstacle.size) {
            return true; // Collision detected
        }
    }
    return false; // No collision
}

bool snake_length_less_than_one(Snake* snake) {
    // Check if snake's length is less than 1 (only the head is left)
    return snake->next == nullptr;
}

bool check_game_over(Snake* snake, int width, int height, const vector<Obstacle>& obstacles) {
    // Check if snake hits any of the 4 edges / walls
    if (check_boundary_collision(snake->x, snake->y, width, height)) {
        return true;
    }

    // Check if snake collides with itself
    if (eat_itself(snake)) {
        return true;
    }

    // Check if snake's length is less than 1
    if (snake_length_less_than_one(snake)) {
        return true;
    }

    // Check if snake touches any obstacle
    if (check_obstacle_collision(snake->x,snake->y, obstacles)) {
        return true;
    }

    return false;
}

struct HighScoreEntry {
    std::string playerName;
    int score;

    HighScoreEntry(const std::string& name, int s) : playerName(name), score(s) {}
};

// Compare function to sort high score entries by score in descending order
bool compareByScore(const HighScoreEntry& a, const HighScoreEntry& b) {
    return a.score > b.score;
}

// Function to save the best 10 scores to a file
void saveBestScores(const std::vector<HighScoreEntry>& bestScores) {
    std::ofstream outputFile("saves/save_best_10.game");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }

    for (const auto& entry : bestScores) {
        outputFile << entry.playerName << " " << entry.score << std::endl;
    }

    outputFile.close();
}

// Function to update the list of best scores with a new score
void updateBestScores(std::vector<HighScoreEntry>& bestScores, const std::string& playerName, int newScore) {
    bestScores.emplace_back(playerName, newScore);
    std::sort(bestScores.begin(), bestScores.end(), compareByScore); // Sort the best scores
    if (bestScores.size() > 10) {
        bestScores.pop_back(); // Keep only the top 10 scores
    }
}


void show_game_over_screen(int score) {
    // Clear the screen
    clear();

    // Print the game over message and score
    mvprintw(10, 10, "Game Over!");
    mvprintw(12, 10, "Score: %d", score);
    mvprintw(14, 10, "Press any key to exit...");

    // Refresh the screen
    refresh();

    // Wait for user input before exiting
    getch();
}

int get_speed() {
    return gameSpeed;   
}

void update_score(int points) {
    score += points;
}

void print_score() {
    mvprintw(5, 10, "Score: %d", score); // Print the score at position (0, 0)
    refresh(); 
}

void update_speed() {
    if (score % 100 == 0 && score != 0) {
        gameSpeed *= 1.5;
    }
}

Snake* find_last_segment(Snake* snake) {
    if (snake->next == nullptr) {
        return snake;
    } else {
        return find_last_segment(snake->next);
    }
}

void add_tail_segment(Snake* snake, int new_tail_x, int new_tail_y) {
    // Find the last segment of the snake
    Snake* last_segment = snake;
    while (last_segment->next != nullptr) {
        last_segment = last_segment->next;
    }

    // Determine the position of the new tail segment based on the direction of the snake
    int tail_x = last_segment->x;
    int tail_y = last_segment->y;

    switch (snake_direction) {
        case UP:
            tail_y += 1; // Move the tail segment one cell down
            break;
        case DOWN:
            tail_y -= 1; // Move the tail segment one cell up
            break;
        case LEFT:
            tail_x += 1; // Move the tail segment one cell to the right
            break;
        case RIGHT:
            tail_x -= 1; // Move the tail segment one cell to the left
            break;
    }

    // Create a new tail segment at the updated position
    Snake* new_segment = create_tail(tail_x, tail_y);

    // Link the new tail segment to the last segment
    last_segment->next = new_segment;

    // Ensure that the new tail segment becomes the last segment
    new_segment->next = nullptr;
}

void save_points_to_file(int score) {
    std::ofstream outputFile("snake_scores.txt", std::ios::app); // Open the file in append mode
    if (outputFile.is_open()) {
        outputFile << "Final Score: " << score << std::endl;
        outputFile.close();
    }
}

void show_welcome_screen() {
    clear();
    mvprintw(10, 10, "Welcome to Snake Game!");
    mvprintw(12, 10, "Choose Difficulty Level:");
    mvprintw(14, 10, "1. Easy");
    mvprintw(16, 10, "2. Hard");
    mvprintw(18, 10, "3. Advanced");
    refresh();
    
    int choice;
    while (true) {
        choice = getch() - '0';
        if (choice >= 1 && choice <= 3) {
            break;
        }
    }

    DifficultyLevel difficulty;
    switch (choice) {
        case 1:
            difficulty = EASY;
            break;
        case 2:
            difficulty = HARD;
            break;
        case 3:
            difficulty = ADVANCED;
            break;
    }

    // Adjust game parameters based on difficulty level
    GameParameters params;
    switch (difficulty) {
        case EASY:
            params = setEasyParameters();
            break;
        case HARD:
            params = setHardParameters();
            break;
        case ADVANCED:
            params = setAdvancedParameters();
            break;
    }

    // Use params to set game parameters
    int snakeSpeed = params.snakeSpeed;
    int numFoodItems = params.numFoodItems;
    int numObstacles = params.numObstacles;
    int numLives = params.numLives;
}

void game(){
    enum State state = INIT; // Set the initial state
    static int x_max, y_max; //Max screen size variables
    static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
    gamewindow_t *window; // Name of the board
    Snake *snake; // The snake
    Food *foods,*new_food; // List of foods (Not an array)
    vector<HighScoreEntry> bestScores;
    std::string playerName;
    int newScore = 1000;
    std::cout << "Enter your name: ";
    std::cin >> playerName;


    const int height = 30; 
    const int width = 70;
    char ch;

    struct timespec timeret;
    timeret.tv_sec = 0;
    timeret.tv_nsec = 999999999/4;

    vector<Obstacle> obstacles;

    // Generate obstacles
    obstacles = generate_obstacles(width, height, x_offset, y_offset, EASY);

    while(state != EXIT){
        switch(state){
        case INIT:
            initscr();
            clear();
            start_color();
            nodelay(stdscr, TRUE); //Dont wait for char
            noecho(); // Don't echo input chars
            getmaxyx(stdscr, y_max, x_max);
            cbreak();
            keypad(stdscr, TRUE); // making keys work
            curs_set(0); // hide cursor
            timeout(100);

            // Setting height and width of the board
            x_offset = (x_max / 2) - (width / 2);
            y_offset = (y_max / 2) - (height / 2);
            
            // Print Welcome Screen
            show_welcome_screen();

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
        
            ch = getch();
            
            /* Write your code here */
            switch (ch) {
                case 'p':
                    mvprintw(0, 0, "Game paused. Press 'p' to resume...");
                    refresh();
                    while ((ch = getch()) != 'p') {}
                    mvprintw(0, 0, "                          "); // Clear pause message
                    break;
                case 'P':
                    mvprintw(0, 0, "Game paused. Press 'p' to resume...");
                    refresh();
                    while ((ch = getch()) != 'p') {}
                    mvprintw(0, 0, "                          "); // Clear pause message
                    break;
                case 'q':
                    endwin();
                    exit(0);
                    break;
                case 'Q':
                    endwin();
                    exit(0);
                    break;
                case 'w':
                    if (snake_direction != DOWN) {
                        snake_direction = UP;
                    }
                    break;
                case 's':
                    if (snake_direction != UP) {
                        snake_direction = DOWN;
                    }
                    break;
                case 'a':
                    if (snake_direction != RIGHT) {
                        snake_direction = LEFT;
                    }
                    break;
                case 'd':
                    if (snake_direction != LEFT) {
                        snake_direction = RIGHT;
                    }
                    break;
            }

            move_snake(snake, snake_direction);

            // Check for food collision
            if (food_exists(foods, snake->x, snake->y)) {
                enum Type foodType = food_type(foods, snake->x, snake->y);
                switch (foodType) {
                    case Increase:
                        update_score(20);
                        update_speed();
                        break;
                    case Decrease:
                        update_score(-10);
                        update_speed();
                        break;
                    default:
                        break;
                }
                foods = remove_eaten_food(foods, snake->x, snake->y);

                generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease; // Randomly deciding type of food
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);
            }

            // Check for game over scenarios
            if (check_game_over(snake, width, height, obstacles)) {
                if (lives > 0) {
                    lives--;
                    state = INIT;
                } else {
                    state = DEAD;
                    break;
                } 
            }

			// Draw everything on the screen
            clear();
            print_score();
            mvprintw(7, 10, "Lives: %d", lives);
            draw_Gamewindow(window);
            draw_snake(snake);
            draw_food(foods);
            print_obstacles(obstacles);
            break;

        case DEAD:
            save_points_to_file(score);

            show_game_over_screen(score);
            break;
        }
        refresh();
        nanosleep(&timeret, NULL);
    }
}
