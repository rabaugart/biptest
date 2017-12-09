/*
 * Console.cpp
 *
 *  Created on: 09.12.2017
 *      Author: rainer
 */

#include <ncurses.h>

#include <rashm/Console.h>

namespace rashm {

struct Console::ConsoleImpl {
	ConsoleImpl() {
		initscr();			/* Start curses mode 		  */
		//printw("Hello World !!!");	/* Print Hello World		  */
		refresh();			/* Print it on to the real screen */
		cbreak();
		noecho();
		curs_set(0);
	}

	~ConsoleImpl() {
		endwin();
	}

	bool key() {
		timeout(500);
		return getch() != ERR;			/* Wait for user input */
	}

	void show( int row, int col, std::string const & text ) {
		mvprintw( row, col, text.c_str() );
		refresh();
	}
};

Console::Console() : impl(std::make_unique<ConsoleImpl>()) {
}

Console::~Console() {
}

void Console::show( int row, int col, std::string const & text ) {
	impl->show( row, col, text );
}

bool Console::key() {
	return impl->key();
}

} /* namespace rashm */
