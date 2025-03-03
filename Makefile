objects = sudoku.o sudoku_solve.o sudoku_ncurses.o sudoku_user.o

SudokuSolver : $(objects) sudoku_solve.h sudoku_ncurses.h sudoku_user.h
	gcc -g -o SudokuSolver $(objects) -lncurses -lm
sudoku_solve.o : sudoku_solve.c sudoku_solve.h sudoku_user.h
	gcc -g -c sudoku_solve.c
sudoku_ncurses.o : sudoku_ncurses.c sudoku_ncurses.h
	gcc -g -c sudoku_ncurses.c 
sudoku_user.o: sudoku_user.c sudoku_user.h
	gcc -g -c sudoku_user.c
clean : 
	rm SudokuSolver $(objects)
