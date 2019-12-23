#include "Game.h"
#include <string>
#include <curses.h>

int main(int argc, char* argv[]) {
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(FALSE);
	nodelay(stdscr, TRUE);

	auto g = Game();
	g.Start();

	getch();
	endwin();
}