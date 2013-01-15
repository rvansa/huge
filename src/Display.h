/*
 * Display.h
 *
 *  Created on: Jan 12, 2013
 *      Author: rvansa
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "File.h"
#include "Formatter.h"

namespace huge {

	class DisplaySize {
	public:
		int rows;
		int cols;
		DisplaySize(int rows, int cols): rows(rows), cols(cols) {}
	};

	class Display {
	public:
		Display();
		virtual ~Display();
		DisplaySize size() const;
		void refresh(const Formatter &formatter, const FileView &file_view);
		void print_error(const char *fmt, const File &file) const;

	private:
		BasicCharacterBuffer _buffer;
	};

}

#endif /* DISPLAY_H_ */
