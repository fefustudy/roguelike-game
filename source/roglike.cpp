#include "Game.h"
#include <string>
#include <curses.h>

int main(int argc, char* argv[]) {
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(FALSE);

	auto gs = GAME_STAT::UNDEF;

	while (true) {
		switch (gs) {
		case WIN:
			mvprintw(1, 0, " WIN ");
			break;
		case DEFEAT:
			mvprintw(1, 0, " DEFEAT ");
			break;
		}

		mvprintw(3, 0, " 1 - Start Game ");
		mvprintw(5, 0, " 2 - Exit ");

		nodelay(stdscr, FALSE);
		auto in = getch();

		if (in == '2') break;
		if (in == '1') {
			nodelay(stdscr, TRUE);
			auto g = Game();
			gs = g.Start();
		}

		clear();
	}



	getch();
	endwin();
}