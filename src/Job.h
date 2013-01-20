/*
 * Job.h
 *
 *  Created on: Jan 20, 2013
 *      Author: radim
 */

#ifndef JOB_H_
#define JOB_H_

#include <pthread.h>

namespace huge {

class Job {
public:
	Job();
	virtual ~Job();
	virtual void run() = 0;
	void pause() { /* TODO */ }
	void resume() { /* TODO */ }
protected:
	bool should_wait() { /* TODO */ return false; }
	void wait() { /* TODO */ }
private:
	pthread_mutex_t _lock;
	pthread_cond_t _condvar;
};

}

#endif /* JOB_H_ */
