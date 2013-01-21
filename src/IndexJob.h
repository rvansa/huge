/*
 * IndexJob.h
 *
 *  Created on: Jan 19, 2013
 *      Author: radim
 */

#ifndef INDEXJOB_H_
#define INDEXJOB_H_

#include "SearchJob.h"

namespace huge {

class IndexJob: public SearchJob {
public:
	IndexJob(File &file);
	virtual ~IndexJob();
	virtual void run();
private:
	File &_file;
};

}

#endif /* INDEXJOB_H_ */
