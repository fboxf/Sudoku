/*! \file sudoku.c */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "sudoku_ncurses.h"
#include "sudoku_solve.h"
#include "sudoku_user.h"

#define DEBUG

int main(void){
	srand(time(NULL));

	initscr(); 	// start ncurses

	S_start(); 

	endwin();	// end ncurses 

	return 0;
}
