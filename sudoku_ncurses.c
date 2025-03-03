#include <ncurses.h>
#include <stdbool.h>
#include "sudoku_ncurses.h"
#include "sudoku_solve.h"
#include "sudoku_user.h"

#define DEBUG

/** \brief Draw a second screen
 *
 * Create a different window for the sudoku board. More of an experiment
 * than actually required
 * \returns s_screen A pointer to a variable of type WINDOW */
WINDOW* S_create_screen(void){
	WINDOW *s_screen; 

	s_screen = newwin(0,0,0,0);

	if(s_screen == NULL){
		endwin();
	} else {
		return s_screen;
	}
}

/** \brief Colour options */
int S_create_color(void){
	start_color();
	
	init_pair(1, COLOR_RED, COLOR_WHITE); // stdscr
	init_pair(2, COLOR_BLUE, COLOR_WHITE); // sudoku screen

	bkgd(COLOR_PAIR(1));
}

/** \brief Calls appropriate functions for the program.
 *
 * Receives integer n representing menu choice and calls 
 * relevant functions related to user choice.
 * @param[in] n Integer
 **/
void S_start(void){
	S_create_color();

	// menu options
	int Selection_Menu = 0;
	int Selection_Menu_Sub = 0;

	N_screen_Title();

	while(Selection_Menu != 1){
		Selection_Menu = N_screen_Menu(); // Main menu
		
		// MAIN MENU - PLAY
		if (Selection_Menu == 0){
			// OPEN SUBMENU
			Selection_Menu_Sub = N_play_options();
			U_user_play(Selection_Menu_Sub);
		}
		clear();
	}
}

/** \brief Function to display title */
void N_screen_Title(void){
	int MenuChoice = 0;
	
	move(0,1);
	attron(A_BOLD);
	attron(A_UNDERLINE);
	addstr("Sudoku\n");
	move(2,1);
	attroff(A_BOLD);
	attroff(A_UNDERLINE);

	addstr("Please choose an option:\n\n");
}

/** \brief Function to display menu 
 * 	1. 	A random board is generated in full (to check it works) and
 * 		and then numbers are removed before user is given control 
 * 	2.	The player can enter some details onto the board and then the 
 * 		computer solves the puzzle (different colour/bold for solutions) */
int N_screen_Menu(void){
	int menu_selection = 0, ch;
	char *menu_Options[] = {
		" 1 Play Sudoku\n",
		" 2 Exit\n",
	};

	attroff(A_REVERSE);
	N_screen_Title();

	move(4,0);

	for (size_t i = 0; i < 2; i++){
		if (menu_selection == i){
			attron(A_REVERSE);
		} else {
			attroff(A_REVERSE);
		}
		printw("%s", menu_Options[i]);
	}

	// hide the cursor, no echo and no delay
	curs_set(0);
	noecho();
	nodelay(stdscr, true);

	while ((ch = getch()) != '\n'){

		switch(ch){
			case 'w': 
				if (menu_selection == 0){
					menu_selection = 1;
				} else {
					menu_selection--;
				}
				break;
			case 's':
				if (menu_selection == 1){
					menu_selection = 0;
				} else {
					menu_selection++;
				}
				break;
			default:
				break;
		}

		move(4,0);

		for (size_t i = 0; i < 2; i++){
			if (menu_selection == i){
				attron(A_REVERSE);
			} else {
				attroff(A_REVERSE);
			}
			printw("%s", menu_Options[i]);
		}
	}

	clear();
	refresh();

	return menu_selection;
}

/** \brief SUbmenu */
int N_play_options(void){
	int play_menu_selection = 1, ch;
	char *play_menu_Options[] = {
		" 1 Easy\n",
		" 2 Normal\n",
		" 3 Hard\n",
		" 4 Back\n"
	};

	attroff(A_REVERSE);
	N_screen_Title();

	for (size_t i = 0; i < 4; i++){
		if (play_menu_selection = i){
			attron(A_REVERSE);
		} else {
			attroff(A_REVERSE);
		}
		printw("%s", play_menu_Options[i]);
	}

	while ((ch = getch()) != '\n'){

		switch(ch){
			case 'w': 
				if (play_menu_selection == 0){
					play_menu_selection = 3;
				} else {
					play_menu_selection--;
				}
				break;
			case 's':
				if (play_menu_selection == 3){
					play_menu_selection = 0;
				} else {
					play_menu_selection++;
				}
				break;
			default:
				break;
		}

		move(4,0);

		for (size_t i = 0; i < 4; i++){
			if (play_menu_selection == i){
				attron(A_REVERSE);
			} else {
				attroff(A_REVERSE);
			}
			printw("%s", play_menu_Options[i]);
		}
	}

	clear();
	refresh();

	return play_menu_selection;
}

/** \brief Draw board to ncurses window
 *
 * Maybe I should centre this and for every (i + 1) % 3 draw hline
 * and (j + 1) % 3 draw vline.
 **/
void N_drawBoard(void){

	WINDOW *s = S_create_screen();
	wbkgd(s,COLOR_PAIR(2));

	for (size_t row = 0; row < BOARD_SIZE; row++){
		N_blank_row(row, s);
		for (size_t col = 0; col < BOARD_SIZE; col++){
			N_blank_col(col, s);
			if (SudokuGrid[row][col] == 0){
				wprintw(s,"  ");
			} else {
			wprintw(s,"%d ", SudokuGrid[row][col]);
			}
		}
		wprintw(s,"\n");
	}

	wrefresh(s); // updates current screen >> standard screen
}

/** insert a blank row when drawing sudoku
 * @param[in] n Row number */
void N_blank_row(int n, WINDOW *s){
	if (n == 0 || n == 3 || n == 6) {
		wprintw(s,"\n");
	}
		wrefresh(s);
}

/** Insert a blank column when drawing grid
 * @param[in] n Col number*/
void N_blank_col(int n, WINDOW *s){
	if (n == 0 || n == 3 || n == 6){
		wprintw(s," ");
	}
		wrefresh(s);
}

/** \brief Traverse along a sudoku board and overwrite numbers if they 
 * are zero */
void N_moveCursor(void){
	int row, col, value;
	row = 0; col = 0;

	curs_set(1);
	refresh();

	int ch, y_loc, x_loc; // current character
	move(1,1);

	getyx(stdscr, y_loc, x_loc);
	
	while ((ch = getch()) != '\n'){
		switch(ch){
			case 'w': // move up
					if (y_loc == 1){
						break;
					} else if (y_loc == 5 || y_loc == 9){
						move(y_loc -= 2, x_loc);
						row--;
						break;
					} else {
						move(y_loc -= 1, x_loc);
						row--;
						break;
					}
			case 's':
					if (y_loc == 11){
						break;
					} else if (y_loc == 3 || y_loc == 7){
						move(y_loc += 2, x_loc);
						row++;
						break;
					} else {
						move(y_loc += 1, x_loc);
						row++;
						break;
					}
			case 'a':
					if (x_loc == 1){
						break;
					} else if (x_loc == 8 || x_loc == 15){
						move(y_loc, x_loc -= 3);
						col--;
						break;
					} else {
						move(y_loc, x_loc -= 2);
						col--;
						break;
					}
			case 'd':
					if (x_loc == 19){
						break;
					} else if (x_loc == 5 || x_loc == 12){
						move(y_loc, x_loc += 3);
						col++;
						break;
					} else {
						move(y_loc, x_loc += 2);
						col++;
						break;
					}
		}

		N_typeNumbers(ch, row, col);
	}
	
	if(ch == '\n'){
		move (13,0); // move to bottom
		//printw("Row: %d, Col: %d Value: %d\n", row,col, SudokuGrid[row][col]);
		
		// Put a version of solve sudoku here
		if (S_check_usergrid(SudokuGrid)){
			printw("Well done! You correctly solved the sudoku!\n");
		} else {
			printw("Unfortunately that isn't the correct solution\n");
		}

		refresh();
		
		nodelay(stdscr, false); // allow getch() to pause execution
		getch();
		clear;
	}
}

/** \brief Allow numbers to be typed over the sudoku board
 *
 * @param[in] ch Character
 * @param[in] row Row
 * @param[in] col Col */
void N_typeNumbers(int ch, int row, int col){
	int curs_row,curs_col;

	getyx(stdscr,curs_row,curs_col);

	echo();
	int count = 0;
	switch(ch){
		case '0':
			SudokuGrid[row][col] = 0;
			addch('0' | A_REVERSE);
			break;
		case '1':
			SudokuGrid[row][col] = 1;
			addch('1' | A_REVERSE);
			break;
		case '2':
			SudokuGrid[row][col] = 2;
			addch('2' | A_REVERSE);
			break;
		case '3':
			SudokuGrid[row][col] = 3;
			addch('3' | A_REVERSE);
			break;
		case '4':
			SudokuGrid[row][col] = 4;
			addch('4' | A_REVERSE);
			break;
		case '5':
			SudokuGrid[row][col] = 5;
			addch('5' | A_REVERSE);
			break;
		case '6':
			SudokuGrid[row][col] = 6;
			addch('6' | A_REVERSE);
			break;
		case '7':
			SudokuGrid[row][col] = 7;
			addch('7' | A_REVERSE);
			break;
		case '8':
			SudokuGrid[row][col] = 8;
			addch('8' | A_REVERSE);
			break;
		case '9':
			SudokuGrid[row][col] = 9;
			addch('9' | A_REVERSE);
			break;
		default:
			break;
	}
	
	move(curs_row, curs_col);
	refresh();
	noecho();
}
