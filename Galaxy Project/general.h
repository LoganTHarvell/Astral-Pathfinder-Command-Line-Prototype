//
//  general.h
//  Galaxy Project
//
//  Created by Logan Harvell on 11/20/15.
//  Copyright © 2015 Logan Harvell. All rights reserved.
//


#ifndef general_h
#define general_h


#include <stdio.h>
#include <stdlib.h>

#undef TRUE
#define TRUE 1

#undef FALSE
#define FALSE 0


//function to enable debugging from terminal
void debug_in_terminal(int argc, const char *argv[]);

//function to adjust for window resize
void window_resize(void);

//function to ask yes or no question
int yes_or_no(char *question);

//function to check success of memory allocation
int check_memory(void * pointer);	//returns true if pointer is not null

//function to open files and check for success
FILE *open_file(const char *filename);

//function to print error and exit program
void exception_exit(char *error, int error_value);


#endif /* general_h */
