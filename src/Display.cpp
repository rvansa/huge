/*
 * Display.cpp
 *
 *  Created on: Jan 12, 2013
 *      Author: rvansa
 */

#include "Display.h"
#include <ncurses.h>
#include <errno.h>
#include <string.h>

using namespace huge;

Display::Display() {
	::initscr();
	::cbreak();
	::noecho();
	::keypad(stdscr, true);
}

Display::~Display() {
	::endwin();
}

/* Forces refresh after function ends */
class RefreshAfter {
public:
	RefreshAfter() {
	}
	~RefreshAfter() {
		::refresh();
	}
};

DisplaySize Display::size() const {
	int rows, cols;
	getmaxyx(stdscr, rows, cols);
	return DisplaySize(rows, cols);
}

void Display::refresh(const Formatter &formatter, const FileView &file_view) {
	RefreshAfter refresher;
	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	_buffer.assure_capacity(rows - 1, cols);

	try {
		formatter.read(file_view, _buffer, rows - 1, cols);
	} catch (FileReadException &e) {
		print_error("Error reading file %s", file_view.file());
	}
	::mvprintw(rows - 1, 0, "-------------------------------------------");
	::move(0, 0);
	for (int row = 0; row < rows - 1; ++row) {
		for (int col = 0; col < cols; ++col) {
			char c = _buffer.get(row, col).c;
			if (c != 0) {
				addch(c);
			} else {
				addch(' ');
			}
		}
	}

}

void Display::print_error(const char *fmt, const File &file) const {
	int error = errno;
	::clear();
	::mvprintw(0, 0, fmt, file.filename());
	::mvprintw(0, 1, "Error %d: %s", error, strerror(error));
}
