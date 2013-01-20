/*
 * Formatter.h
 *
 *  Created on: Jan 13, 2013
 *      Author: rvansa
 */

#ifndef FORMATTER_H_
#define FORMATTER_H_

#include <unistd.h>
#include "classes.h"

namespace huge {

	struct Character {
		char c;
		off_t origin;

		Character(): c(0), origin(0) {}
		Character(char c, off_t origin): c(c), origin(origin) {}
	};


	class CharacterBuffer {
	public:
		virtual ~CharacterBuffer() {}
		virtual void put(int row, int col, char c, off_t origin) = 0;
		virtual void put_newline(off_t origin) { (void) origin; }
	};

	class BasicCharacterBuffer: public CharacterBuffer {
	public:
		BasicCharacterBuffer(): _buffer(0), _width(0), _capacity(0) {}
		~BasicCharacterBuffer() { if (_buffer != 0) delete _buffer; }
		void assure_capacity(int rows, int cols);
		void put(int row, int col, char c, off_t origin);
		Character get(int row, int col);
	private:
		Character *_buffer;
		int _width;
		size_t _capacity;
	};

	class InfoCharacterBuffer: public CharacterBuffer {
	public:
		void put(int row, int col, char c, off_t origin);
		void put_newline(off_t origin);
		inline off_t last_origin() { return _last; }
	private:
		off_t _last;
	};

	class DisplaySize; // in Display.h

	class Formatter {
	public:
		Formatter();
		virtual ~Formatter();

		bool read(const FileView &file_view, CharacterBuffer &buffer, int rows, int cols) const;
		bool pos_change_lines(int lines, const DisplaySize &display_size, FileView &file_view) const;

		inline void toggle_line_indent() { line_indent = !line_indent; }
		inline void toggle_word_wrap() { word_wrap = !word_wrap; }

	private:
		int line_indent_chars = 4;
		bool line_indent = false;
		bool word_wrap = false;

		inline int get_line_indent() const { return line_indent ? line_indent_chars : 0; }
	};

}

#endif /* FORMATTER_H_ */
