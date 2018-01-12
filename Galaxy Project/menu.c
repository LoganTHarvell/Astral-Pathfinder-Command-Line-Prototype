//
//  menu.c
//  Galaxy Project
//
//  Created by Logan Harvell on 11/17/15.
//  Copyright © 2015 Logan Harvell. All rights reserved.
//


#include "parameters.h"
#include "menu.h"
#include "general.h"


//function to initialize status
struct Status initialize_status() {
	
	struct Status status;
	
	status.year = 0;
	status.num_colonies = 1;
	status.population_size = STARTING_POPULATION;
	status.infrastructure = STARTING_POPULATION;
	status.ships_available = 0;
	status.traveling_ships = 0;
	status.landing_ships = 0;
	status.fertility = STARTING_POPULATION/PEOPLE_FOOD_RQMT;
	status.deposits = SHIP_COST * 2 + 50;
	status.food_produced = status.fertility;
	status.food_available = status.food_produced;
	status.minerals_produced = 0;
	
	return status;
}


//function to ouput status updates
void display_status(struct Status const info) {
	
	//prints all information
	printw("\n\n");
	printw("-----------------------");
	printw("Current State of Your Planets");
	printw("-----------------------");
	printw("\n\n");
	
	printw("In year %d...\n\n", info.year);
	printw("Planets colonized: %d\n", info.num_colonies);
	printw("Current population: %d\n", info.population_size);
	printw("Current infrastructre will support %d people.\n", info.infrastructure);
	printw("Ships available: %d\n", info.ships_available);
	printw("Ships in transit: %d\n", info.traveling_ships);
	printw("Max food production (1 unit of food feeds %d people): %d units\n", PEOPLE_FOOD_RQMT, info.fertility);
	printw("Mineral deposits available: %d units\n", info.deposits);
	printw("Food produced: %d units\n", info.food_produced);
	printw("Refined minerals available: %d units\n", info.minerals_produced);
	
	printw("\n");
	printw("------------------------------------");
	printw("END");
	printw("------------------------------------");
	printw("\n\n\n");
	
}


//function to get outbound ships quantity
int get_outbound_ships(int ships_available, int *food_available, int *available_population) {
	
	//verifies ships are available
	if (0 == ships_available) {
		printw("You have no available ships to send.\n");
		return 0;
	}
	//verifies people are available to send in ships
	else if (SHIP_PERSONNEL_RQMT > *available_population) {
		printw("You do not have enough people available to send a ship.\n");
		return 0;
	}
	//verifies enough food is available
	else if (SHIP_FOOD_RQMT > *food_available) {
		printw("You do not have enough food to send a ship.\n");
		return 0;
	}
	
	int outbound_ships = 0;
	while (TRUE) {
		
		//prompts user
		printw("How many ships do you want to send?\n");
		printw("Requirements per ship: %d people, %d units of food\n", SHIP_PERSONNEL_RQMT, SHIP_FOOD_RQMT);
		//gets and verifies valid user input
		if (scanw("%d", &outbound_ships) == 1) {
			//checks if user input is within quantity possible range
			if (0 <= outbound_ships && outbound_ships <= ships_available) {
				if ((outbound_ships*SHIP_FOOD_RQMT) <= *food_available) {
					break;
				}
				//prints error message if ship food requirements are not met
				else {
					printw("You do not have enough food for that many ships.\n");
				}
			}
			//prints error message if quantity entered is invalid
			else {
				printw("Quantity entered is invalid.\n");
			}
		}
		else {
			//prints error message if input is invalid
			printw("Invalid input.\n");
		}
	}
	
	//adjusts variables for outboound ships
	*available_population -= (outbound_ships * SHIP_PERSONNEL_RQMT);
	
	return outbound_ships;
}


//function to get farmer quantity from user input
int get_farmers(int *available_population) {
	
	//verifies minerals are available
	if (0 == *available_population) {
		printw("You have no people available to farm.\n");
		return 0;
	}
	
	int farmers = 0;
	while (TRUE) {
	
		//prompts user
		printw("How many people do you want to farm? (%d farmers produce 1 unit of food)\n", FARMING_COST);
		//gets and verifies valid user input
		if (scanw("%d", &farmers) == 1) {
			
			//checks if user input is within quantity possible range
			if (0 <= farmers && farmers <= *available_population) {
				break;
			}
			//prints error message if quantity entered is invalid
			else {
				printw("Quantity entered is invalid.\n");
			}
		}
		else {
			//prints error message if input is invalid
			printw("Invalid input.\n");
		}
	}
	
	//adjust availble population
	*available_population -= farmers;
	
	return farmers;
}


//function to get miner quantity from user input
int get_miners(int *available_population) {
	
	//verifies people are available
	if (0 == *available_population) {
		printw("You have no people available to mine.\n");
		return 0;
	}
	
	int miners = 0;
	while (TRUE) {
		
		//prompts user
		printw("How many people do you want to mine? (%d miners produce 1 unit of minerals)\n", MINING_COST);
		//gets and verifies valid user input
		if (scanw("%d", &miners) == 1) {
			//checks if user input is within quantity possible range
			if (0 <= miners && miners <= *available_population) {
				break;
			}
			//prints error message if quantity entered is invalid
			else {
				printw("Quantity entered is invalid.\n");
			}
		}
		else {
			//prints error message if input is invalid
			printw("Invalid input.\n");
		}
	}
	
	//adjusts available population
	*available_population -= miners;
	
	return miners;
}


//function to get mineral quantity for ships from user input
int get_ship_minerals(int minerals_produced) {
	
	//verifies minerals are available
	if (0 == minerals_produced) {
		printw("You have no minerals available to allocate for ships.\n");
		return 0;
	}
	
	int minerals_ships = 0;
	while (TRUE) {
		
		//prompts user
		printw("How many minerals do you want to use for ships? (%d minerals per ship)\n", SHIP_COST);
		//gets and verifies valid user input
		if (scanw("%d", &minerals_ships) == 1) {
			//checks if user input is within quantity possible range
			if (0 <= minerals_ships && minerals_ships <= minerals_produced) {
				break;
			}
			//prints error message if quantity entered is invalid
			else {
				printw("Quantity entered is invalid.\n");
			}
		}
		else {
			//prints error message if input is invalid
			printw("Invalid input.\n");
		}
	}
	
	return minerals_ships;
}


//function to get mineral quantity for ships from user input
int get_infrastructure_minerals(int minerals_produced, int minerals_ships) {
	
	//verifies minerals are available
	if (0 == (minerals_produced - minerals_ships)) {
		printw("You have no minerals available to allocate for infrastructure.\n");
		return 0;
	}
	
	int minerals_infrastructure = 0;
	while (TRUE) {
		
		//prompts user
		printw("How many minerals do you want to use for infrastructure?\n");
		//gets and verifies valid user input
		if (scanw("%d", &minerals_infrastructure) == 1) {
			//checks if user input is within quantity possible range
			if (0 <= minerals_infrastructure && minerals_infrastructure <= (minerals_produced-minerals_ships)) {
				break;
			}
			//prints error message if quantity entered is invalid
			else {
				printw("Quantity entered is invalid.\n");
			}
		}
		else {
			//prints error message if input is invalid
			printw("Invalid input.\n");
		}
	}
	
	return minerals_infrastructure;
}


//function to get user input and adjust question answers
struct Answers user_input(struct Status *info) {
	
	struct Answers answers;
	
	printw("This year...\n");
	
	do {

		//sets available population to population size
		int available_population = info->population_size;
		
		//initialize answers with user input
		answers.outbound_ships = get_outbound_ships(info->ships_available, &info->food_produced, &available_population);
		answers.farmers = get_farmers(&available_population);
		answers.miners = get_miners(&available_population);
		answers.minerals_ships = get_ship_minerals(info->minerals_produced);
		answers.minerals_infrastructure = get_infrastructure_minerals(info->minerals_produced, answers.minerals_ships);
		
	} while (yes_or_no("Do you wish to change your answers?"));
	
	return answers;
}


//function to initialize highscores array with highscores from file
void initialize_highscores(struct Highscores highscores[]) {
	
	int i;
	for (i=0; i<MAX_NUM_SCORES; i++) {
		highscores[i].name[0] = (char)NULL;
		highscores[i].score = 0;
	}

	FILE *fptr = open_file("highscores.txt");

	i = 0;
	while (i<MAX_NUM_SCORES && fscanf(fptr, "%"MAX_NAME_LENGTH_STR"s%d", highscores[i].name, &highscores[i].score) != EOF) i++;
	
	fclose(fptr);
	
}


//function to request player name
char *get_name() {
	
	static char name[MAX_NAME_LENGTH+1];
	
	//prompts user for name
	printw("Enter player name (exclude spaces).\n");
	scanw("%"MAX_NAME_LENGTH_STR"s", name);
	
	return name;
}


//function to update and save highscores
void update_highscores(int score, struct Highscores highscores[]) {
	
	int index = 0;

	//updates array of highscores
	while (MAX_NUM_SCORES > index && highscores[index].name[0] && highscores[index].score >= score) {
		index++;
	}

	//inserts new highscore entry
	if (MAX_NUM_SCORES > index) {
		
		//adjusts list for new entry
		int i = MAX_NUM_SCORES-2;
		while (i>=index) {
			strcpy(highscores[i+1].name, highscores[i].name);
			highscores[i+1].score = highscores[i].score;
			i--;
		}
	
		strcpy(highscores[index].name, get_name());
		highscores[index].score = score;
	
		//writes highscores to file
    FILE *fptr = open_file(HIGHSCORE_FILENAME);
		
		i = 0;
		while (MAX_NUM_SCORES > i && highscores[i].name[0]) {
			fprintf(fptr, "%s %d\n", highscores[i].name, highscores[i].score);
			i++;
		}
	
		fclose(fptr);
	}
	
}

//funtion to display highscores upon user request
void display_highscores(struct Highscores highscores[]) {
	
	printw("\n\nThe Current Highscores are...\n");
	int i = 0;
	while (MAX_NUM_SCORES > i && highscores[i].name[0]) {
			printw("%d. %s %d\n", i+1, highscores[i].name, highscores[i].score);
		i++;
	}
	
}


