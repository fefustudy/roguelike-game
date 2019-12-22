//#include "Actor.h"
#include "Game.h"
#include <string>
//#include <curses.h>

#include <iostream>



//using std::string;


int main(int argc, char* argv[]) {

	initscr();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(FALSE);
	nodelay(stdscr, TRUE);

	//printw("pizda"); refresh();
	
	auto g = Game();
	g.Start();


	getch();
	endwin();
}