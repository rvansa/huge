/*
 * Command.cpp
 *
 *  Created on: Jan 12, 2013
 *      Author: rvansa
 */

#include "Command.h"
#include <ncurses.h>

using namespace huge;

Command *Command::read_command() {
	unsigned int c = getch();
	switch (c) {
	case 'q':
		return new QuitCommand();
	case 't':
		return new LineIndent();
	case 'w':
		return new WordWrap();
	case KEY_DOWN:
		return new PosChangeLines(1);
	case KEY_UP:
		return new PosChangeLines(-1);
	}
	return new NoopCommand();
}

Command::~Command() {
}

