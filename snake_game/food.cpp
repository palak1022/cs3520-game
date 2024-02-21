/*food.cpp -------
*
* Filename: food.cpp
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

#include <cstdlib>
#include <ncurses.h>
#include <cstdbool>
#include "food.hpp"


//Create new food
Food* create_food(int x, int y, enum Type type){
    int ran = randNum;
    new_food->x = x;
    new_food->y = y;
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
    else if (type == Decrease){
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
    new_food->next = NULL;
    return false;
}

//Add new food to end of food list
void add_new_food(Food* foods, Food* new_food){
    Food* temp = foods;
    while(temp->next) {
        return new_food;
    }
    temp->next = new_food;
}


enum Type food_type(Food* foods, int x, int y){
    //Implement the code to return the type of the food 
    //present at position (x, y)	
	 Food* temp = foods;
        while(temp){
        if(temp->x == x && temp->y == y)
            if (temp->type=='O') {
                return Increase;
                break;
            } else if (temp->type=='X'){
                return Decrease;
                break;
            }
        temp = temp->next;
    }	
	
}
Food* remove_eaten_food(Food* foods, int x, int y){
	//Implement the code to remove food at position (x,y).
	//Create a new linked list of type Food containing only the
	//needed food and return this new list
	Food* newFood = foods;     
    // first item edge case
     if (newFood->x == x && newFood->y == y) {
            free(newFood);
            newFood = newFood->next;
            return newFood;
        }

    // all items between 2 and n-1 
    while (newFood->next->next) {
             // checks the second -> n-1 foods
        if (newFood->next->x == x && newFood->next->y == y) {
            newFood->next = newFood->next->next;
            break;
        } 
         newFood = newFood->next;
    }
   
    // nth item check
     if (newFood->next->x == x && newFood->next->y == y) {
         free(newFood->next);
          newFood->next = NULL;
         }

    return foods;
}


// Display all the food
void draw_food (Food *foods)
{   Food* temp = foods;
    while(temp) {
        mvprintw(temp->y, temp->x, "%c", temp->type);
        temp = temp->next;
    }
}
