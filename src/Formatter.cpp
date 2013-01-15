/*
 * Formatter.cpp
 *
 *  Created on: Jan 13, 2013
 *      Author: rvansa
 */

#include <memory.h>
#include <ctype.h>

#include "Display.h"
#include "Formatter.h"
#include "File.h"

using namespace huge;

/* Max width of the display */
#define MAX_WIDTH 1024

void BasicCharacterBuffer::assure_capacity(int rows, int cols) {
	size_t capacity = rows * cols;
	if (_capacity < capacity) {
		delete[] _buffer;
		_buffer = new Character[capacity];
		_capacity = capacity;
	}
	memset(_buffer, 0, sizeof (Character) * capacity);
	_width = cols;
}

void BasicCharacterBuffer::put(int row, int col, char c, off_t origin) {
	size_t pos = row * _width + col;
	// TODO assert pos < capacity
	_buffer[pos] = Character(c, origin);
}

Character BasicCharacterBuffer::get(int row, int col) {
	size_t pos = row * _width + col;
	// TODO assert pos < capacity
	return _buffer[pos];
}

void InfoCharacterBuffer::put(int row, int col, char c, off_t origin) {
	_last = origin;
}

void InfoCharacterBuffer::put_newline(off_t origin) {
	_last = origin;
}


Formatter::Formatter() {
	// TODO Auto-generated constructor stub

}

Formatter::~Formatter() {
	// TODO Auto-generated destructor stub
}

static int find_next_word_wrap(const char * const str, const int pos, const int max) {
	const char *s = str + pos;
	const char *smax = str + max;
	while (s != smax && (isalnum(*s) || *s == '_')) ++s;
	return s - str;
}

bool Formatter::read(const FileView &file_view, CharacterBuffer &buffer, int rows, int cols) const {
	if (cols > MAX_WIDTH) cols = MAX_WIDTH;
	char row_buffer[2*MAX_WIDTH];

	off_t origin = file_view.pos();
	// we need to look one char before to see if we're starting on new line
	char prev_char = '\n';
	FileView my_view(file_view);
	if (origin > 0) {
		my_view.pos(my_view.pos() - 1);
	}
	ReadOperation op(my_view);
	if (origin > 0) {
		if (op.read(&prev_char, 1) < 0) return false;
	}
	int col = (prev_char != '\n' ? get_line_indent() : 0);

	int read = op.read(row_buffer, cols);
	if (read < 0) {
		return false;
	}
	for (int row = 0; row < rows;) {
		int next_read = op.read(row_buffer + cols, cols);
		if (read == 0) {
			break;
		} if (read < 0) {
			return false;
		}
		int next_wrap = word_wrap ? find_next_word_wrap(row_buffer, 0, read + next_read) : -1;
		int i;
		for (i = 0; i < read; ++i) {
			if (row_buffer[i] == '\r') {
				// ignore
			}
			if (row_buffer[i] == '\n')
			{
				row++;
				if (row >= rows) {
					buffer.put_newline(origin + i);
					break;
				}
				col = 0;
			} else if (col + (next_wrap - i) > cols) {
				row++;
				if (row >= rows) break;
				col = get_line_indent();
				buffer.put(row, col, row_buffer[i], origin + i);
				col++;
			} else {
				buffer.put(row, col, row_buffer[i], origin + i);
				col++;
				if (col >= cols) {
					row++;
					if (row >= rows) break;
					col = get_line_indent();
				}
			}
			if (i == next_wrap) {
				next_wrap = find_next_word_wrap(row_buffer, i + 1, read + next_read);
			}
		}
		memcpy(row_buffer, row_buffer + cols, cols);
		read = next_read;
		origin += i + 1;
	}
	return true;
}

bool Formatter::pos_change_lines(int lines, const DisplaySize &display, FileView &file_view) const {
	if (lines > 0) {
		InfoCharacterBuffer buffer;
		read(file_view, buffer, lines, display.cols);
		file_view.pos(buffer.last_origin() + 1);
	} else if (lines < 0){

	}
	return true;
}
