//
//  menu.h
//  Galaxy Project
//
//  Created by Logan Harvell on 11/17/15.
//  Copyright © 2015 Logan Harvell. All rights reserved.
//


#ifndef menu_h
#define menu_h

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>


//struct for player colonies' info
struct Status {
	
	int year;
	int num_colonies;
	int population_size;
	int infrastructure;
	int ships_available;
	int traveling_ships;
	int landing_ships;
	int fertility;
	int deposits;
	int food_produced;
	int food_available;
	int minerals_produced;
	
};

//struct for player input info
struct Answers {
	
	int farmers;
	int miners;
	int outbound_ships;
	int minerals_ships;
	int minerals_infrastructure;
	
};


//create struct for highscore list
struct Highscores {
	
	char name[MAX_NAME_LENGTH+1];
	int score;
	
};


//function to initialize status
struct Status initialize_status(void);	//returns status initialized with start data

//function to ouput status updates
void display_status(struct Status const info);

//function to get outbound ships quantity
int get_outbound_ships(int ships_available, int *food_produced, int *available_population);	//returns quantitity of ships sent

//function to get farmer quantity from user input
int get_farmers(int *available_population);	//returns quantity of farmers

//function to get miner quantity from user input
int get_miners(int *availble_population);	//returns quantity of miners

//function to get mineral quantity for ships from user input
int get_ship_minerals(int minerals_produced);	//returns quantity of minerals

//function to get mineral quantity for ships from user input
int get_infrastructure_minerals(int minerals_produced, int minerals_ships);	//returns quantity of minerals

//function to get user input and adjust question answers
struct Answers user_input(struct Status *info);	//returns player answers

//function to initialize highscores array with highscores from file
void initialize_highscores(struct Highscores highscores[]);

//function to request player name
char *get_name(void);

//function to update highscores
void update_highscores(int score, struct Highscores highscores[]);

//funtion to display highscores upon user request
void display_highscores(struct Highscores highscores[]);


#endif /* menu_h */
