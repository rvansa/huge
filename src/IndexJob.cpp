/*
 * IndexJob.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: radim
 */

#include "IndexJob.h"
#include "MultiRegexp.h"
#include <stdio.h>

using namespace huge;

IndexJob::IndexJob(File &file): _file(file) {
	// TODO Auto-generated constructor stub

}

IndexJob::~IndexJob() {
	// TODO Auto-generated destructor stub
}



void IndexJob::run() {
	FileView file_view(_file);
	const char *expressions[] = { "\n", "true", "new" };
	MultiRegexp regexp(expressions, 3);
	size_t line = 1;
	for(;;) {
		MatchResult result = find_next(file_view, regexp, false);
		if (result.found()) {
			if (result.which() == 0) {
				line++;
			} else {
				printf("Found %s on %ld (line %ld)\n", expressions[result.which()], result.start_pos(), line);
			}
			file_view.pos(result.start_pos() + 1);
		} else {
			break;
		}
	}
	printf("Total %ld lines\n", line);
}

