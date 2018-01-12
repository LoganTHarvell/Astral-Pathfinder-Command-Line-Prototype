//
//  galaxy.c
//  Galaxy Project
//
//  Created by Logan Harvell on 11/17/15.
//  Copyright © 2015 Logan Harvell. All rights reserved.
//


#include "parameters.h"
#include "galaxy.h"
#include "general.h"


struct Galaxy *create_homeworld() {
	
	//initializes galaxy homeworld to null
	struct Galaxy *homeworld = NULL;
	//allocates memory for homeworld
	homeworld = (struct Galaxy *)malloc(sizeof(struct Galaxy));
	
	//checks for success allocating memory
	if (homeworld == NULL) {
		return NULL;
	}
	
	//sets homeworld coordinates
	homeworld->planet.coordinates.x = rand()%(NUMBER_OF_PLANETS+1);
	homeworld->planet.coordinates.y = rand()%(NUMBER_OF_PLANETS+1);
	//sets homeworld resources
	homeworld->planet.fertility = STARTING_POPULATION/PEOPLE_FOOD_RQMT;
	homeworld->planet.deposits = SHIP_COST * 2 + 50;
	//initializes homeworld max population to starting population
	homeworld->planet.colony_flag = COLONIZED;
	
	//initializes next pointer to null
	homeworld->next = NULL;
	
	return homeworld;
}


int check_coordinates(struct Coordinates coordinates, struct Galaxy *galaxy) {
	
	struct Galaxy *temp = galaxy;
	
	while (temp != NULL) {
		//checks if coordinates mark existing planet
		if (temp->planet.coordinates.x == coordinates.x &&
			temp->planet.coordinates.y == coordinates.y) {
			return FALSE;
		}
		//continues to next planet of linked list
		temp = temp->next;
	}
	
	return TRUE;
}


struct Galaxy *create_planet(struct Galaxy *galaxy) {
		
	//initializes pointer to null
	struct Galaxy *next_planet = NULL;
	//allocates memory for next planet
	next_planet = (struct Galaxy *)malloc(sizeof(struct Galaxy));
	
	//checks success of memory allocations, exits if failed
	check_memory(next_planet);
	
	do {
		//set planet coordinates
		next_planet->planet.coordinates.x = rand()%(NUMBER_OF_PLANETS+1);
		next_planet->planet.coordinates.y = rand()%(NUMBER_OF_PLANETS+1);
		
	} while (!check_coordinates(next_planet->planet.coordinates, galaxy));
	
	//sets planet fertility to random value between 10 and 100
	next_planet->planet.fertility = (rand()%(MAX_FERTILITY-MIN_FERTILITY+1) + MIN_FERTILITY);
	//sets planet deposits to random value between 10 and 100
	next_planet->planet.deposits = (rand()%(DEPOSITS_RANGE+1)) + MIN_DEPOSITS;
	
	//initializes max population to 0
	next_planet->planet.colony_flag = FALSE;
	
	//initializes next pointer to null
	next_planet->next = NULL;
	
	
	return next_planet;
}


struct Galaxy *initialize_galaxy() {
	
	//seeds rand()
	srand((unsigned)time(NULL));
	
	//initializes homeworld
	struct Galaxy *homeworld = NULL;
	homeworld = create_homeworld();
	
	//checks success of memory allocation, exits if failed
	check_memory(homeworld);
	
	struct Galaxy *galaxy = homeworld;
	
	//creates linked list of 100 planets in galaxy
	int i;
	for (i=0; i<NUMBER_OF_PLANETS; i++) {
		galaxy->next = create_planet(homeworld);
		galaxy = galaxy->next;
	}
	
	
	return homeworld;
}


void free_memory(struct Galaxy *galaxy) {
	
	//advances through linked list and frees memory
	while (galaxy != NULL) {
		void *temp = galaxy;
		galaxy = galaxy->next;
		free(temp);
		
	}
	
	
}