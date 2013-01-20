/*
 * SearchJob.cpp
 *
 *  Created on: Jan 20, 2013
 *      Author: radim
 */

#include "SearchJob.h"

using namespace huge;

#define DATA_CHUNK;

SearchJob::SearchJob() {
	// TODO Auto-generated constructor stub

}

SearchJob::~SearchJob() {
	// TODO Auto-generated destructor stub
}

MatchResult SearchJob::find_next(const FileView &file_view, MultiRegexp &regexp) {
	_initial_file_view(file_view);
	tre_str_source source;
	source.get_next_char = SearchJob::get_next_char;
	source.compare = SearchJob::compare;
	source.rewind = SearchJob::rewind;
	source.context = this;
	if (tre_reguexec(regexp.compiled(), &source, regexp.match_buffer_size(), regexp.match_buffer(), 0)) {
		return MatchResult::NOT_FOUND;
	} else {
		return regexp.match();
		return SearchResult(which, regexp.match_buffer()[])
	}

/*	ReadOperation readOp(file_view);
	int read;
	do {
		read = readOp.read(buffer, DATA_CHUNK);
	} while (read > 0);
*/

}
