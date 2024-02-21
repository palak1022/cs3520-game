/*food.cpp -------
*
* Filename: food.cpp
* Description: Implementation of functions related to food in a game.
* Author: Adeel Bhutta
* Created: Sat Sep 12 13:16:12 2022
* Last-Updated: September 12 16:51 2022
*
*/

/* Commentary:
* This file contains the implementation of functions related to food in a game.
*/

/* Change log:
* 
* 
*/

/* Copyright (c) 2022 Adeel Bhutta
*
* All rights reserved.
*
* Additional copyrights may follow
*/

#include <cstdlib>
#include <ncurses.h>
#include <cstdbool>
#include "food.hpp" // Include the header file that declares food-related functions and structures
using  namespace std;
// Create a new food
Food* create_food(int x, int y, enum Type type,int randNum){
    Food* new_food = (Food*)malloc(sizeof(Food)); // Allocate memory for a new Food object
    int ran = randNum; // Store the random number
    new_food->x = x; // Set the x-coordinate of the new food
    new_food->y = y; // Set the y-coordinate of the new food
    // Determine the type and subtype of the new food based on the input parameters
    if (type == Increase){
        new_food->type = 'O';
        switch(ran) {
            case (0):
            new_food->subType = 'E';
            break;
            case (1):
            new_food->subType = 'U';
            break;
            default:
            new_food->subType = 'A';
            break;
        }
    }
    else if(type == Decrease){
        new_food->type = 'X';
        switch(ran) {
            case (0):
                 new_food->subType = 'W';
                 break;
            case (1):
                 new_food->subType = 'H';
                 break;
            default:
                  new_food->subType = 'L';
                  break;
        }
    }
    new_food->next = NULL; // Set the next pointer of the new food to NULL
    return new_food; // Return a pointer to the new food
}

// Check if food exists at coordinates
bool food_exists(Food* foods, int x, int y){
    Food* temp = foods; // Temporary pointer to traverse the food list
    // Iterate through the food list
    while(temp){
        // If the current food's coordinates match the input coordinates, return true
        if(temp->x == x && temp->y == y)
            return true;
        temp = temp->next; // Move to the next food in the list
    }
    return false; // Return false if no matching food is found
}

// Add new food to end of food list
void add_new_food(Food* foods, Food* new_food){
    Food* temp = foods; // Temporary pointer to traverse the food list
    // Find the last food in the list
    while(temp->next) {
        temp = temp->next;
    }
    temp->next = new_food; // Add the new food to the end of the list
}

// Determine the type of food at specified coordinates
enum Type food_type(Food* foods, int x, int y){
    // Traverse the food list
    Food* current = foods;
    while (current != NULL) {
        // Check if the current food's coordinates match the input coordinates
        if (current->x == x && current->y == y) {
            // Return the type of the current food
            if (current->type == 'O') {
                return Increase; // Return Increase if the food type is 'O'
            } else if (current->type == 'X') {
                return Decrease; // Return Decrease if the food type is 'X'
            }
        }
        // Move to the next food in the list
        current = current->next;
    }
    // If no food is found at the specified coordinates, return an invalid type (Increase)
    return Increase;
}

// Remove eaten food from the food list
Food* remove_eaten_food(Food* foods, int x, int y){
    Food* current = foods; // Pointer to traverse the food list
    Food* previous = NULL; // Pointer to keep track of the previous food in the list
    // Iterate through the food list
    while (current != NULL) {
        // Check if the current food's coordinates match the input coordinates
        if (current->x == x && current->y == y) {
            // If the current food is the first food in the list, update the head pointer
            if (previous == NULL) {
                foods = current->next;
            } else {
                // Otherwise, update the next pointer of the previous food
                previous->next = current->next;
            }
            // Free the memory allocated for the current food
            free(current);
            // Move to the next food in the list
            current = previous ? previous->next : foods;
        } else {
            // If the current food is not removed, update the previous pointer
            previous = current;
            // Move to the next food in the list
            current = current->next;
        }
    }
    // Return the updated food list
    return foods;
}

// Display all the food
void draw_food (Food *foods)
{   
    Food* temp = foods; // Temporary pointer to traverse the food list
    // Iterate through the food list
    while(temp) {
        // Display the current food at its coordinates
        mvprintw(temp->y, temp->x, "%c", temp->type);
        // Move to the next food in the list
        temp = temp->next;
    }
}
