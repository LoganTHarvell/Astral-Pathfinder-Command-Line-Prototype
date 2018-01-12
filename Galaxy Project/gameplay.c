//
//  gameplay.c
//  Galaxy Project
//
//  Created by Logan Harvell on 11/17/15.
//  Copyright © 2015 Logan Harvell. All rights reserved.
//


#include "parameters.h"
#include "galaxy.h"
#include "menu.h"
#include "gameplay.h"
#include "general.h"


//functions to deterimine random events
int alien_attack() {
	float x = (rand()/(float)RAND_MAX);
	return ALIEN_ATTACK_CHANCE >= x;
}
int blight() {
	float x = (rand()/(float)RAND_MAX);
	return BLIGHT_CHANCE >= x;
}
int explosion() {
	float x = (rand()/(float)RAND_MAX);
	return EXPLOSION_CHANCE >= x;
}
int epidemic() {
	float x = (rand()/(float)RAND_MAX);
	return EPIDEMIC_CHANCE >= x;
}
int ship_lost() {
	float x = (rand()/(float)RAND_MAX);
	return SHIP_LOST_CHANCE >= x;
}


//function to convert outbound ships into traveling ships
int send_ships(int *outbound_ships, struct Status *status) {
	
	//calculates traveling ships and people
	int traveling_ships = *outbound_ships;
	int travelers = traveling_ships * SHIP_PERSONNEL_RQMT;
	
	//adjusts population size for travelers
	status->population_size -= travelers;
	status->food_available -= (traveling_ships * SHIP_FOOD_RQMT);
	status->ships_available -= traveling_ships;
	//resets outbound ships to 0
	*outbound_ships = 0;
	
	return traveling_ships;
}


//function to converts landing ships into discovered planets
void land_ships(struct Galaxy *galaxy, int *traveling_ships, int *landing_ships, int *population_size) {
	
	struct Galaxy *temp = galaxy;
	
	
	
	//calculates newly discovered planets
	while (0 < (*landing_ships) && temp != NULL) {
		
		//converts landing ships into discovered planets
		if (temp->planet.colony_flag == UNDISCOVERED) {
			//adjust ship variable if event occurs
			if (ship_lost()) {
				printw("Contact with a ship was lost!\n");
			}
			else {
        //adjusts population size with people on ships
        *population_size += (*landing_ships)*SHIP_PERSONNEL_RQMT;
        
        temp->planet.colony_flag = DISCOVERED;
			}
      // updates number of landing ships
      (*landing_ships)--;
		}
    
		//continues to next planet
		temp = temp->next;
	}
	
	//adjusts traveling ships and landed ships
	*landing_ships = *traveling_ships;
	traveling_ships = 0;
	
}


//function to calculate ships production
int ships_production(int minerals_ships, int *minerals_produced) {
	
	int ships = 0;
	
	//subtracts minerals for ships from mineral reserves
	*minerals_produced -= minerals_ships;
	//counts ships built with provided minerals
	ships = minerals_ships/SHIP_COST;
	
	return ships;
}


//function to update max population/infrastructure
int infrastructure_developed(int minerals_infrastructure, int *minerals_produced) {
	
	int infrastructure_increases = 0;

	//calculates infrastructure increases
	infrastructure_increases = minerals_infrastructure/INFRASTRUCTURE_COST;
	
	//subtracts minerals for ships from mineral reserves
	*minerals_produced -= minerals_infrastructure;
	
	return infrastructure_increases;
}


//function to calculate minerals production
int minerals_production(int miners, int deposits, struct Galaxy *galaxy) {
	
	int production = 0;
	
	//checks if deposits or miners are equal to zero
	if (deposits && miners) {
	
		struct Galaxy *temp = galaxy;
		//travels through galaxy linked list
		while (temp != NULL) {
			//calculates production of planet
			if (temp->planet.colony_flag == COLONIZED) {

				//checks for mine explosion
				if (explosion() && temp->planet.deposits) {
					printw("The planet located at (%d, %d) suffered an explosion in the mines.\n",
						   temp->planet.coordinates.x, temp->planet.coordinates.y);
				}
				else {
					//verifies production does not exceed planet limits
					if (temp->planet.deposits < (miners * (temp->planet.deposits/(float)deposits))/(float)MINING_COST) {
						production += temp->planet.deposits;
						temp->planet.deposits = 0;
					}
					else {
						production += (miners * (temp->planet.deposits/(float)deposits))/(float)MINING_COST;
						temp->planet.deposits -= (miners * (temp->planet.deposits/(float)deposits))/(float)MINING_COST;
					}
				}
			}
			temp = temp->next;
		}
		
	}
	
	return production;
}


//function to calculate food production
int food_production(int farmers, int fertility, struct Galaxy *galaxy) {
	
	int production = 0;
	
	//checks if farmers are equal to zero
	if (farmers) {
	
		struct Galaxy *temp = galaxy;
		
		int i = 0;
		//travels through galaxy linked list
		while (temp != NULL) {
			//calculates production of planet
			if (temp->planet.colony_flag == COLONIZED) {
				//checks for blight
				if (i++ && blight() && temp->planet.fertility) {
					printw("The planet located at (%d, %d) suffered a blight.\n",
						   temp->planet.coordinates.x, temp->planet.coordinates.y);
				}
				else {
					//verifies production does not exceed planet fertility
					if (temp->planet.fertility < (farmers * (temp->planet.fertility/(float)fertility))/(float)FARMING_COST) {
						production += temp->planet.fertility;
					}
					else {
						production += (farmers * (temp->planet.fertility/(float)fertility))/(float)FARMING_COST;
					}
				}
			}
			temp = temp->next;
		}
	
	}
	return production;
}


//function to calculate deaths from epidemic
void epidemic_deaths(struct Answers *answers, struct Status *status, struct Galaxy *galaxy) {
	
	struct Galaxy *temp = galaxy;
	
	//travels through galaxy linked list
	while (temp != NULL) {
		//calculates production of planet
		if (temp->planet.colony_flag == COLONIZED) {
			//checks for epidemic
			if (epidemic()) {
				if (answers->miners && temp->planet.deposits && rand() > (RAND_MAX/2)) {
					printw("The miners on the planets at (%d, %d) suffered an epidemic.\n",
						   temp->planet.coordinates.x, temp->planet.coordinates.y);
					status->population_size -= answers->miners * (temp->planet.deposits/(float)status->deposits) * EPIDEMIC_DEATH_RATE;
				}
				else if (answers->farmers && temp->planet.fertility) {
					printw("The farmers on the planet at (%d, %d) suffered an epidemic.\n",
						   temp->planet.coordinates.x, temp->planet.coordinates.y);
					status->population_size -= answers->farmers * (temp->planet.fertility/(float)status->fertility) * EPIDEMIC_DEATH_RATE;
				}
			}
		}
		temp = temp->next;
	}
	
	
	
}


//functions to calculate births
int births(int food_available, int population_size) {
	
	//calculates surplus food
	int surplus;
	if (0 > (surplus = ((food_available*PEOPLE_FOOD_RQMT)-population_size))) {
		surplus = 0;
	}
	
	//initializes births to half of population size
	int births = 0;
	
	//creates a birth multiplier between a min and max
	float multiplier = (rand()/(RAND_MAX/BIRTH_MULTIPLIER_RANGE)) + MIN_BIRTH_MULTIPLIER;
	
	births = (population_size/2) * (multiplier * (surplus/(population_size/PEOPLE_FOOD_RQMT)+1));
	
	return births;
}


//function to calculate deaths
int deaths(int infrastructure, int food_available, int population_size) {
	
	//initializes deaths to population size
	int deaths = 0;
	
	//creates a death multiplier between a min and max
	float multiplier = (rand()/(RAND_MAX/DEATH_MULTIPLIER_RANGE)) + MIN_DEATH_MULTIPLIER;
	
	deaths = (population_size*multiplier);
	
	//checks if people starved
	if ((population_size-deaths) > (food_available * PEOPLE_FOOD_RQMT)) {
		deaths += ((population_size-deaths) - (food_available * PEOPLE_FOOD_RQMT));
	}
	//checks if population exceeds infrastructure limit
	if ((population_size-deaths) > infrastructure) {
		deaths += ((population_size-deaths) - infrastructure);
	}
	
	return deaths;
}


//function to update people
void update_population(int infrastructure, int food_available, int *population_size) {
	
	//adds births, subtracts deaths from population
	*population_size += births(food_available, *population_size);
	*population_size -= deaths(infrastructure, food_available, *population_size);
	
}


//function to convert discovered planets into colonized planets
int colonize_planets(struct Galaxy *galaxy) {
	
	struct Galaxy *temp = galaxy;
	
	//counts number of colonized planets
	int num_colonies = 0;
	
	//travels through galaxy, updates discovered planets into colonized planets
	while (temp != NULL) {
		if (temp->planet.colony_flag == DISCOVERED) {
			temp->planet.colony_flag = COLONIZED;
			num_colonies++;
		}
		else if (temp->planet.colony_flag == COLONIZED) {
			if (alien_attack()) {
				temp->planet.colony_flag = UNDISCOVERED;
				printw("The planet located at (%d, %d) has been wiped out by aliens!\n",
					   temp->planet.coordinates.x, temp->planet.coordinates.y);
				num_colonies--;
			}
			num_colonies++;
		}
		temp = temp->next;
	}
	
	return num_colonies;
}


//function to calculate colonized planets resources
int *potential_production(struct Galaxy *galaxy) {
	
	struct Galaxy *temp = galaxy;
	
	//declares and initializes potential production to 0
	static int potential[RESOURCES];
	potential[FERTILITY] = 0;
	potential[DEPOSITS] = 0;
	
	//travels through galaxy linked list
	while (temp != NULL) {
		//sums colonized planet potential production
		if (temp->planet.colony_flag == COLONIZED) {
			potential[FERTILITY] += temp->planet.fertility;
			potential[DEPOSITS] += temp->planet.deposits;
		}
		temp = temp->next;
	}
	
	return potential;
}


//updates galaxy with changes and updates status with current information
void update_galaxy(struct Galaxy *galaxy, struct Status *status, struct Answers *const answers) {
	
	//converts outbound ships into traveling ships, adjusts ship variables
	status->traveling_ships = send_ships(&answers->outbound_ships, status);
	
	//converts landing ships into discovered planets, adjusts ship variables
	land_ships(galaxy, &status->traveling_ships, &status->landing_ships, &status->population_size);
	
	//calculate ship production
	status->ships_available += ships_production(answers->minerals_ships, &status->minerals_produced);
	//calculate infrastructure increase
	status->infrastructure += infrastructure_developed(answers->minerals_infrastructure, &status->minerals_produced);
	//calculate minerals production
	status->minerals_produced += minerals_production(answers->miners, status->deposits, galaxy);
	//calculate food production
	status->food_produced = 0;
	status->food_produced = food_production(answers->farmers, status->fertility, galaxy);
	
	//calculates deaths due to epidemic
	epidemic_deaths(answers, status, galaxy);
	
	//calculate population change
	if (status->population_size) {
		update_population(status->infrastructure, status->food_available, &status->population_size);
	}
	
	//updates food available
	status->food_available = status->food_produced;
	
	//updates discovered planets into colonized planets
	status->num_colonies = colonize_planets(galaxy);
	
	//calculate resources
	int *potential = potential_production(galaxy);
	status->fertility = potential[FERTILITY];
	status->deposits = potential[DEPOSITS];
	
	//adds a year
	status->year++;
	
}


//function to test for endgame conditions
int end_of_game(struct Status *const status) {
	
	//checks population size lose condition
	if (0 == status->population_size) {
		printw("All of your people have died. Do Better.\n");
	}
	//checks colonized planets win condition
	else if (NUMBER_OF_PLANETS == status->num_colonies) {
		printw("You have colonized the entire galaxy! Congratulations!\n");
	}
	//checks depleted resources endgame condition
	else if (0 == status->minerals_produced + status->deposits + status->ships_available + status->traveling_ships) {
		printw("You are unable to further colonize the galaxy.");
	}
	else return FALSE;
	
	printw("\n\nGame over.\n\n");
	
	return TRUE;
}
