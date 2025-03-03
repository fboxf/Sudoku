#ifndef SUDOKU_SOLVE_H
#define SUDOKU_SOLVE_H

/*! \def BOARD_SIZE
 *
 *\brief Size of board
 */
#define BOARD_SIZE 9

bool S_check_usergrid(int grid[][BOARD_SIZE]);
void S_init(int grid[][BOARD_SIZE]);
void S_generate(int *r, int *c, int *n);
bool S_solve(int a[][BOARD_SIZE]);
bool S_getEmpty(int *r, int *c); 
bool S_checkForFull(int grid[][BOARD_SIZE]);
bool S_canPutOnRow(int row, int n);
bool S_canPutOnCol(int col, int n);
bool S_canPutInSubGrid(int row, int col, int n);
void S_printGrid();

#endif
