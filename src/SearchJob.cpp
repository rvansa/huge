/*
 * SearchJob.cpp
 *
 *  Created on: Jan 20, 2013
 *      Author: radim
 */

#include "SearchJob.h"
#include "Buffer.h"

using namespace huge;

SearchJob::SearchJob() {
	// TODO Auto-generated constructor stub

}

SearchJob::~SearchJob() {

}

int SearchJob::get_next_char(unsigned char *c, unsigned int *pos_add, void *context) {
	SearchJob *self = static_cast<SearchJob *>(context);
	if (self->_read_buffer_pos >= self->_read_buffer_end) {
		/* Here is the pause point */
		if (self->should_wait()) {
			self->wait();
		}

		ReadOperation op(self->_current_file_view);
		try {
			size_t read = op.read(self->_read_buffer, READ_BUFFER_SIZE);
			if (read == 0) {
				*c = 0;
				*pos_add = 0;
				return 1;
			}
			self->_read_buffer_pos = 0;
			self->_read_buffer_end = read;
			self->_current_file_view.pos(self->_current_file_view.pos() + read);
		} catch (FileReadException &e) {
			*c = 0;
			return 1;
		}
	}
	*c = self->_read_buffer[self->_read_buffer_pos++];
	*pos_add = 1;
	return 0;
}

void SearchJob::rewind(size_t pos, void *context) {
	SearchJob *self = static_cast<SearchJob *>(context);
	self->_current_file_view.pos(self->_initial_file_view.pos() + pos);
	self->_read_buffer_pos = 0;
	self->_read_buffer_end = 0;
}

int SearchJob::compare(size_t pos1, size_t pos2, size_t len, void *context) {
	SearchJob *self = static_cast<SearchJob *>(context);
	Buffer<char, READ_BUFFER_SIZE> first(len);
	Buffer<char, READ_BUFFER_SIZE> second(len);
	FileView view1(self->_initial_file_view);
	FileView view2(self->_initial_file_view);
	view1.pos(view1.pos() + pos1);
	view2.pos(view2.pos() + pos2);
	ReadOperation op1(view1);
	ReadOperation op2(view2);
	try {
		char *first_buf = first();
		size_t read = op1.read(first_buf, len);
		if (read < len) {
			return 1;
		}
		op1.finish();
		char *second_buf = second();
		read = op2.read(second_buf, len);
		if (read < len) {
			return 1;
		}
		for (size_t i = 0; i < len; ++i) {
			if (first_buf[i] != second_buf[i]) return 1;
		}
	} catch (FileReadException &e) {
		return 1;
	}
	return 0;
}


MatchResult SearchJob::find_next(const FileView &file_view, MultiRegexp &regexp, bool report_submatch) {
	_initial_file_view = file_view;
	_current_file_view = file_view;
	_read_buffer_pos = _read_buffer_end = 0;
	tre_str_source source;
	source.get_next_char = SearchJob::get_next_char;
	source.compare = SearchJob::compare;
	source.rewind = SearchJob::rewind;
	source.context = this;
	if (tre_reguexec(regexp.compiled(), &source, regexp.match_buffer_size(), regexp.match_buffer(), 0)) {
		return MatchResult::NOT_FOUND;
	} else {
		return regexp.match(file_view.pos(), report_submatch);
	}

/*	ReadOperation readOp(file_view);
	int read;
	do {
		read = readOp.read(buffer, DATA_CHUNK);
	} while (read > 0);
*/

}
