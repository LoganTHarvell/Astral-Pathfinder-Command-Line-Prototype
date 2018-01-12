//
//  general.c
//  Galaxy Project
//
//  Created by Logan Harvell on 11/20/15.
//  Copyright © 2015 Logan Harvell. All rights reserved.
//


#include "general.h"


#include <ncurses.h>
#include <string.h>


void debug_in_terminal(int argc, const char * argv[]) {
	
	//enables debugging in terminal
	for (int argi = 1; argi < argc; argi++)
	{
		if (strcmp(argv[argi], "--debug-in-terminal") == 0)
		{
			printf("Debugging in terminal enabled\n");
			getchar(); // Without this call debugging will be skipped
			break;
		}
	}
}


void window_resize() {
	
	endwin();
	refresh();
	getch();

	
}


int yes_or_no(char *question) {
	
	//initalizes answer to null
	char answer = 0;
	
	while (TRUE) {
		
		//prompt user with passed in question
		printw("\n%s y/n\n", question);
		scanw("%c", &answer);
		
		//prints error message when answer is invalid
		if (answer != 'y' && answer != 'n') {
			printw("Invalid answer.\n");
		}
		else {
			break;
		}
		
	}
	
	return answer == 'y' ? TRUE : FALSE;
}


int check_memory(void * pointer) {
	
	//ends program if pointer is null
	if (pointer == NULL) {
		exception_exit("Error allocating memory.", EXIT_FAILURE);
	}
	
	return TRUE;
}


FILE *open_file(const char *filename) {
	
	FILE *file_ptr;
	if ((file_ptr = fopen(filename, "r+"))) return file_ptr;
	if ((file_ptr = fopen(filename, "w+"))) return file_ptr;
	
	//exits if file cannot be opened
	exception_exit("Error opening highscores file.", EXIT_FAILURE);

	return NULL;
}


void exception_exit(char *error, int error_value) {
	
	printw("\n\nERROR: %s\n\n", error);
	printw("Exiting program.\n\n");

	refresh();
	exit(error_value);
	
}