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
	MatchResult find_next(const FileView &file_view, MultiRegexp &regexp);
private:
	static int get_next_char(char *c, unsigned int *pos_add, void *context);
	static void rewind(size_t pos, void *context);
	static void compare(size_t pos1, size_t pos2, size_t len, void *context);

	FileView *_initial_file_view = NULL;
	FileView *_current_file_view = NULL;
};

} /* namespace huge */
#endif /* SEARCHJOB_H_ */
