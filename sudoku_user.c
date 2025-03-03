/*! \file sudoku_user.c */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "sudoku_user.h"
#include "sudoku_solve.h" // had to include stdbool as THIS references bool
#include "sudoku_ncurses.h"

/** \brief Calls a number of function to solve a randomly generated 
 * sudoku puzzle 
 *
 * Initialise a SudokuGrid and generate solvable puzzle. Remove squares
 * then hand over control to user
 * @param[in] n Difficulty level selected by user input */
bool U_user_play(int n){
	int squaresToRemove = 0;

	switch(n){
		case 0: squaresToRemove = EASY;
				break;
		case 1: squaresToRemove = NORMAL;
				break;
		case 2: squaresToRemove = HARD;
				break;
		case 3:
				return false;
	}

	S_init(SudokuGrid);
	U_user_generate(PLACEMENTS);

	if(S_solve(SudokuGrid)){
		U_remove_squares(squaresToRemove); // eventually a user choice
#ifdef DEBUG
		S_printGrid();
#endif
		N_drawBoard();
		N_moveCursor();
	} else {
		puts("Not solvable");
	}

	return true;
}

/** \brief Generate a sudoku board for the user
 *
 * Takes as an argument n number of squares to fill on the board. 
 * For each number, check if it's possible to place. Increment counter if possible
 * @param[in] n Number of squares to fill.*/
void U_user_generate(int n){
	int count, row, col, number;
	count = 0;

	while (count < n){
		S_generate(&row, &col, &number);
		if (SudokuGrid[row][col] != 0){
			continue;
		} else if( 
					S_canPutOnRow(row,number) && 
					S_canPutOnCol(col,number) && 
					S_canPutInSubGrid(row, col, number)){
			SudokuGrid[row][col] = number;
#ifdef DEBUG
			printf("Coord - Row: %d Col: %d Value: %d\n", row, col, number);
#endif
			count++;
		} else {
			continue;
		}
	}
}

/** \brief Removes a designated number of squares from a solvable
 * board
 *
 * @param[in] n Number of squares to remove */
void U_remove_squares(int n){
	int rand_row, rand_col;
	for (size_t i = 0; i <= n; i++){
		rand_row = rand() % BOARD_SIZE; rand_col = rand() % BOARD_SIZE;
		SudokuGrid[rand_row][rand_col] = 0;
	}
}
