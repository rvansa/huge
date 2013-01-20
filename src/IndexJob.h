/*
 * IndexJob.h
 *
 *  Created on: Jan 19, 2013
 *      Author: radim
 */

#ifndef INDEXJOB_H_
#define INDEXJOB_H_

class IndexJob {
public:
	IndexJob();
	virtual ~IndexJob();
	virtual void run();
};

bool p(const char *str, int n, void *dest);

#endif /* INDEXJOB_H_ */
