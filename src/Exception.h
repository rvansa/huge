/*
 * Exception.h
 *
 *  Created on: Jan 20, 2013
 *      Author: radim
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>

#define USE_BACKTRACE

namespace huge {

#ifdef USE_BACKTRACE
#include <execinfo.h>

#define MAX_STACK_SIZE 32
#endif

class Exception {
public:
	Exception(const std::string &message);
	virtual ~Exception();
	void print() const;
private:
	std::string _message;
#ifdef USE_BACKTRACE
	char **_symbols;
	int _symbol_count;
#endif
};

} /* namespace huge */
#endif /* EXCEPTION_H_ */
