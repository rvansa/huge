/*
 * Huge.h
 *
 *  Created on: Jan 13, 2013
 *      Author: rvansa
 */

#ifndef HUGE_H_
#define HUGE_H_

#include <vector>
#include "Display.h"
#include "File.h"
#include "Formatter.h"

namespace huge {

class Huge {
public:
	Huge();
	virtual ~Huge();
	inline Huge &display(Display *d) { _display = d; return *this; }
	inline Display &display() { return *_display; }
	inline Huge &file(File *f) {
		_file = f;
		if (_file_view != 0) delete _file_view;
		_file_view = new FileView(*f);
		return *this;
	}
	inline File &file() { return *_file; }
	inline FileView &file_view() { return *_file_view; }
	inline Huge &formatter(Formatter *f) { _formatter = f; return *this; }
	inline Formatter &formatter() { return *_formatter; }
	inline const std::vector<std::string> &indexed() { return _indexed; }
private:
	Display *_display;
	File *_file;
	Formatter *_formatter;
	FileView *_file_view;
	std::vector<std::string> _indexed;
};

}

#endif /* HUGE_H_ */
