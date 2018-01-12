//
//  main.c
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


int main(int argc, const char * argv[]) {
	
	//enables debugging in terminal
	debug_in_terminal(argc, argv);
	 
	//handles window resize
	signal(SIGWINCH, window_resize);
	
	//initializes ncurses screen
	initscr();
	
	//game introduction
	printw("%s\n\n", TITLE);
	printw("%s", INTRO);
	
	//begins game
	int playing;
	if (yes_or_no("Would you like to start?")) {
		playing = TRUE;
	}
	else {
		playing = FALSE;
	}
	clear();
	
	while (playing) {

		//initializes galaxy
		struct Galaxy *galaxy = NULL;
		galaxy = initialize_galaxy();
	
		//initialize status and declare answers
		struct Status status = initialize_status();
		struct Answers answers;
	
		//loops while endgame is false
		while (!end_of_game(&status)) {
			
			
			//displays current status of player's colonies
			display_status(status);
			//gets user input for resources allocation
			answers = user_input(&status);
			//clears screen
			clear();
			//update galaxy
			update_galaxy(galaxy, &status, &answers);
			
		}
		
		//creates array for highscores
		struct Highscores highscores[MAX_NUM_SCORES];
		
		//initialize highscores
		initialize_highscores(highscores);
		//update highscores
		update_highscores(status.population_size, highscores);
		//display highscores
		display_highscores(highscores);
		
		//frees memory allocated for galaxy
		free_memory(galaxy);
		
		//ask user to play again
		if (!yes_or_no("Do you want to play again?")) {
			playing = FALSE;
		}
		
		//clears screen
		clear();
		
	}	
	
	//ends ncurses
	endwin();
	
	
    return 0;
}
