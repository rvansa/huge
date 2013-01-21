/*
 * SearchJob.h
 *
 *  Created on: Jan 20, 2013
 *      Author: radim
 */

#ifndef SEARCHJOB_H_
#define SEARCHJOB_H_

#include "Job.h"
#include "File.h"
#include "MultiRegexp.h"

namespace huge {

class SearchJob: public Job {
public:
	SearchJob();
	virtual ~SearchJob();
	MatchResult find_next(const FileView &file_view, MultiRegexp &regexp, bool report_submatch = false);
private:
	static int get_next_char(unsigned char *c, unsigned int *pos_add, void *context);
	static void rewind(size_t pos, void *context);
	static int compare(size_t pos1, size_t pos2, size_t len, void *context);

	static const int READ_BUFFER_SIZE = 1024;

	FileView _initial_file_view;
	FileView _current_file_view;
	char _read_buffer[READ_BUFFER_SIZE];
	size_t _read_buffer_pos;
	size_t _read_buffer_end;
};

} /* namespace huge */
#endif /* SEARCHJOB_H_ */
