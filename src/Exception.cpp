/*
 * Exception.cpp
 *
 *  Created on: Jan 13, 2013
 *      Author: rvansa
 */

#include <malloc.h>
#include "Exception.h"

using namespace huge;

Exception::Exception(const std::string &msg): _message(msg) {
#ifdef USE_BACKTRACE
		void *stacktrace[MAX_STACK_SIZE];
		_symbol_count = backtrace(stacktrace, MAX_STACK_SIZE);
		_symbols = backtrace_symbols(stacktrace, _symbol_count);
#endif
}

Exception::~Exception() {
#ifdef USE_BACKTRACE
	free(_symbols);
#endif
}

void Exception::print() const {
	printf("Exception: %s\n", _message.c_str());
#ifdef USE_BACKTRACE
	for (int i = 0; i < _symbol_count; ++i) {
		printf("at %s\n", _symbols[i]);
	}
#endif
}
