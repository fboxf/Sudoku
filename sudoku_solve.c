//https://stackoverflow.com/questions/32343262/sudoku-checker-accessing-3x3-subgrid-in-c
//https://iq.opengenus.org/backtracking-sudoku/
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include "sudoku_solve.h"
#include "sudoku_user.h"
#include "sudoku_ncurses.h"

int SudokuGrid[BOARD_SIZE][BOARD_SIZE];

/** \brief Return true if count for each row is 45
 *
 * The sum of any row should equal 45. Rather than use resources to
 * resolve a puzzle thats already known, I decided to add up each row
 * \returns bool*/
bool S_check_usergrid(int grid[][BOARD_SIZE]){
	int count;

	for(size_t row = 0; row < BOARD_SIZE; row++){
		count = 0;
		for (size_t col = 0; col < BOARD_SIZE; col++){
			count += grid[row][col];
		}
		if (count == 45){
			continue;
		} else {
			return false;
		}
	}
	return true;
}

/** \brief Initialise all grid elements to zero
 * 
 * This is used on first start up and also if a potential solution is not
 * possible before trying another
 * @param[in,out] grid 2D array of size BOARD_SIZE */
void S_init(int grid[][BOARD_SIZE]){
	for(size_t row = 0; row < BOARD_SIZE; row++){
		for (size_t col = 0; col < BOARD_SIZE; col++){
			grid[row][col] = 0;
		}
	}
}

/** \brief Produce a random number, in a random place on the board
 *
 * @param[in,out] *r Pointer to integer representing a row 
 * @param[in,out] *r Pointer to integer representing a col 
 * @param[in,out] *r Pointer to integer representing a number */
void S_generate(int *r, int *c, int *n){
	*r = rand() % BOARD_SIZE;
	*c = rand() % BOARD_SIZE;
	*n = rand() % (9 - 1 + 1) + 1;
}

/** \brief Solve a sudoku puzzle, if blank, generate one
 *
 * Uses a backtracing algorithm to place a value 1 through 9
 * into an empty space and then move on to another empty square. If
 * an empty square has no solutions, the algorithm resets that square
 * and 'backtracks' to try another option.
 * @param[in,out] a 2D array */
bool S_solve(int a[][BOARD_SIZE]){
	int row, col;
	
	S_getEmpty(&row, &col); // this needs to be counted
							// doesn't matter which ones are removed at the end

	if (S_checkForFull(a)){
		return true;
	} 

	for (size_t i = 1; i <= 9; i++){
		if (S_canPutOnRow(row,i) && 
				S_canPutOnCol(col,i) &&
					S_canPutInSubGrid(row, col, i)){
			SudokuGrid[row][col] = i;
			if (S_solve(a)){					
				return true;
			}
		} 
		SudokuGrid[row][col] = 0;
	}
	return false;
}

/** \brief find an empty grid square
 * @param[in,out] *r pointer to an empty row 
 * @param[in,out] *c pointer to an empty col */
bool S_getEmpty(int *r, int *c){
	for (size_t i = 0; i < BOARD_SIZE; i++){
		for (size_t j = 0; j < BOARD_SIZE; j++){
			if (SudokuGrid[i][j] == 0){
				*r = i;
				*c = j;
				return true;
			} 
		}
	}
	return false;
}

/** \brief Check if grid is full
 * @param[in] grid[][9] A sudoku array */
bool S_checkForFull(int grid[][BOARD_SIZE]){
	for (size_t i = 0; i < BOARD_SIZE; i++){
		for (size_t j = 0; j < BOARD_SIZE; j++){
			if (grid[i][j] == 0){
				return false;
			}
		}
	}
	return true;
}

/** \brief Function to check if a number can be placed on row 
 *
 * For each iteration, i is checked to see if it contains the number
 * to check, n. If it's found, return false.
 * @param[in] row Proposed row of varaible n
 * @param[in] n Integer number from 1 to 9 to check 
 * \returns bool True if n can be placed */
bool S_canPutOnRow(int row, int n){
	for (size_t i = 0; i < BOARD_SIZE; i++){
		if (SudokuGrid[row][i] == n){
			return false;
		}
	}
	return true;
}

/** \brief Function to check if a number can be placed on col
 *
 * @param[in] col Proposed col of varaible n
 * @param[in] n Integer number from 1 to 9 to check 
 * \returns bool True if n can be placed */
bool S_canPutOnCol(int col, int n){
	for (size_t i = 0; i < BOARD_SIZE; i++){
		if (SudokuGrid[i][col] == n){
			return false;
		}
	}
	return true;
}

/** \brief Check if number exists in subgrid
 *
 * Provides bounds to check if the number has already appeared */
bool S_canPutInSubGrid(int row, int col, int n){
	int len = BOARD_SIZE;
	int index = row * len + col;
	int x = sqrt(len);
	int box_index = (index % len) / x + x * (index / (len * x));

	int row_start;
	int col_start;
	
	switch (box_index){
		case 0: row_start = 0;
				col_start = 0;
				break;
		case 1: row_start = 0;
				col_start = 3;
				break;
		case 2: row_start = 0;
				col_start = 6;
				break;
		case 3: row_start = 3;
				col_start = 0;
				break;
		case 4: row_start = 3;
				col_start = 3;
				break;
		case 5: row_start = 3;
				col_start = 6;
				break;
		case 6: row_start = 6;
				col_start = 0;
				break;
		case 7: row_start = 6;
				col_start = 3;
				break;
		case 8: row_start = 6;
				col_start = 6;
				break;
	}

	for (size_t i = 0; i < 3 ; i++){
		for (size_t j = 0; j < 3; j++){
			if (SudokuGrid[row_start + i][col_start + j] == n){
				return false;
			}
		}
	}

	return true;
}

/** \brief DEBUG - print grids to console 
 * */
void S_printGrid(void){
	for (size_t i = 0; i < BOARD_SIZE; i++){
		for (size_t j = 0; j < BOARD_SIZE; j++){
			printf("%d ", SudokuGrid[i][j]);
		}
		printf("\n");
	}
}
