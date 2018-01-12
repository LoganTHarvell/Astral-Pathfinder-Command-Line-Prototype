//
//  galaxy.h
//  Galaxy Project
//
//  Created by Logan Harvell on 11/17/15.
//  Copyright © 2015 Logan Harvell. All rights reserved.
//


#ifndef galaxy_h
#define galaxy_h


#include <time.h>


//enumerates planet stages
enum {
	
	UNDISCOVERED,
	DISCOVERED,
	COLONIZED
	
};


//struct for planet coordinates
struct Coordinates {
	
	int x, y;
	
};

//struct for planet information
struct Planet {
	
	struct Coordinates coordinates;
	int fertility;
	int deposits;
	int colony_flag;
	
};

//struct for galaxy linked list
struct Galaxy {
	
	struct Planet planet;
	struct Galaxy *next;
	
};

//function to initialize homeworld
struct Galaxy *create_homeworld(void);	//returns pointer to homeworld planet

//function to check planet coordinates for preexisting planet
int check_coordinates(struct Coordinates coordinates, struct Galaxy *homeworld); //returns true if planet doesn't exist

//function to create a planet
struct Galaxy *create_planet(struct Galaxy *homeworld);	//returns pointer to a planet

//function to create linked list of planets in galaxy
struct Galaxy *initialize_galaxy(void);	//returns linked list of planets

//function to free allocated memory
void free_memory(struct Galaxy *galaxy);


#endif /* galaxy_h */
