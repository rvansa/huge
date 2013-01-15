/*
 * Command.h
 *
 *  Created on: Jan 12, 2013
 *      Author: rvansa
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include "classes.h"
#include "File.h"
#include "Display.h"
#include "Huge.h"

namespace huge {

	class Command {
	public:
		virtual ~Command();
		virtual bool execute(Huge &huge) = 0;
		static Command *read_command();
	};

	class NoopCommand: public Command {
	public:
		virtual bool execute(Huge &huge) { return true; }
	};

	class QuitCommand: public Command {
	public:
		virtual bool execute(Huge &huge) { return false; }
	};

	class LineIndent: public Command {
	public:
		bool execute(Huge &huge) {
			huge.formatter().toggle_line_indent();
			return true;
		}
	};

	class PosChangeLines: public Command {
	public:
		PosChangeLines(int lines): _lines(lines) {}

		bool execute(Huge &huge) {
			huge.formatter().pos_change_lines(_lines, huge.display().size(), huge.file_view());
			return true;
		}
	private:
		int _lines;
	};

	class WordWrap: public Command {
	public:
		bool execute(Huge &huge) {
			huge.formatter().toggle_word_wrap();
			return true;
		}
	};

}

#endif /* COMMAND_H_ */
