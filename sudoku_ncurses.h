/*! \file sudou_ncurses.h */
#ifndef SUDOKU_NCURSES_H
#define SUDOKU_NCURSES_H

extern int SudokuGrid[][9];

WINDOW* S_create_screen(void);
int S_create_color(void);
void S_start(void);
void N_screen_Title(void);	
int N_screen_Menu(void);
int N_play_options(void);
void N_drawBoard(void);
void N_blank_row(int n, WINDOW *s);
void N_blank_col(int n, WINDOW *s);

void N_moveCursor(void);
void N_typeNumbers(int ch, int row, int col);
#endif
