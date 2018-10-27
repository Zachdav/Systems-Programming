#include <cstdlib>
#include <ncurses.h>

int main() {
	initscr();                 
    	printw("Hello World !!!");
      	refresh();
      	getch();
      	endwin();
      	return EXIT_SUCCESS;
} // main
