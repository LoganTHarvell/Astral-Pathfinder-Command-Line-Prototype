//
//  gameplay.h
//  Galaxy Project
//
//  Created by Logan Harvell on 11/17/15.
//  Copyright © 2015 Logan Harvell. All rights reserved.
//


#ifndef gameplay_h
#define gameplay_h


#include <stdlib.h>

enum {
	
	FERTILITY,
	DEPOSITS
	
};


//functions to calculate occurrence of random events
int alien_attack(void);		//returns true if aliens attack
int blight(void);			//returns true if planet has blight
int explosion(void);		//returns true if planet's mine explodes
int epidemic(void);			//returns true if planet has an epidemic
int ship_lost(void);		//returns true if ship is lost

//function to convert outbound ships into ships in transit
int send_ships(int *outbound_ships, struct Status *status);	//returns ships in transit

//function to convert landing ships into discovered planets
void land_ships(struct Galaxy *galaxy, int *traveling_ships, int *landing_ships, int *population_size);

//functions to calculate production
int ships_production(int minerals_ships, int *minerals_produced);	//returns quantity of ships produced
int infrastructure_developed(int minerals_infrastructure, int *minerals_produced);	//returns added max population
int minerals_production(int miners, int deposits, struct Galaxy *galaxy);	//returns quantity of minerals produced
int food_production(int farmers, int fertility, struct Galaxy *galaxy);	//returns quantity of food produced

//function to calculate deaths from epidemic
void epidemic_deaths(struct Answers *answers, struct Status *status, struct Galaxy *galaxy);

//functions to calculate population changes
int births(int food_available, int population_size);	//returns number of births
int deaths(int infrastructure, int food_available, int population_size);	//returns number of deaths

//function to update people
void update_population(int infrastructure, int food_available, int *population_size);

//function to convert discovered planets into colonized planets
int colonize_planets(struct Galaxy *galaxy);	//returns the number of colonies

//function to calculate colonized planets total fertility and deposits
int *potential_production(struct Galaxy *galaxy);	//returns array with potential production of industries


//function to update galaxy
void update_galaxy(struct Galaxy *galaxy, struct Status *status, struct Answers *const answers);

//function to test for endgame conditions
int end_of_game(struct Status *const status);	//returns false if endgame conditions are met

//function to update scores
int *update_high_scores(int *high_scores);	//returns a pointer to highscores array


#endif /* gameplay_h */
