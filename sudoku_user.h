/*! \file sudoku_user.h */
#ifndef SUDOOKU_USER_H
#define SUDOOKU_USER_H

/*! \def PLACEMENTS
 * \brief Places 10 numbers
 *
 * When the sudoku grid is generated, the program places 10 numbers
 * in randomly chosen squares
 * */

/*! \def EASY 
 * \brief difficulty
 * */

/*! \def NORMAL 
 * \brief difficulty
 * */

/*! \def HARD 
 * \brief difficulty
 * */

#define PLACEMENTS 10 

// difficulty levels
#define EASY 2
#define NORMAL 20
#define HARD 40

extern int SudokuGrid[][9];

bool U_user_play(int n);
void U_user_generate(int n);
void U_remove_squares(int n);

#endif
