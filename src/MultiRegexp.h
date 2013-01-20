/*
 * MultiRegexp.h
 *
 *  Created on: Jan 20, 2013
 *      Author: radim
 */

#ifndef MULTIREGEXP_H_
#define MULTIREGEXP_H_

#include <string>
#include "tre.h"
#include "Exception.h"

namespace huge {

class RegexpCompileException: public Exception {
public:
	RegexpCompileException(const char *expression):
		Exception(std::string("Regexp compilation failed for '").append(expression).append("'")) {}
};

class MatchResult {
friend class SearchJob;
public:
	size_t which() const { return _which; }
	off_t start_pos() const { return _start_pos; }
	off_t end_pos() const { return _end_pos; }
	bool found() const { return _start_pos >= 0; }
private:
	MatchResult(size_t w, off_t s, off_t e): _which(w), _start_pos(s), _end_pos(e) {}

	const size_t _which;
	const off_t _start_pos;
	const off_t _end_pos;
	static const MatchResult NOT_FOUND;
};


class MultiRegexp {
public:
	MultiRegexp(const char *expressions[], size_t n) throw(RegexpCompileException);
	inline const regex_t *compiled() const { return &_compiled; }
	inline regmatch_t *match_buffer() const { return _match_buffer; }
	inline size_t match_buffer_size() const { return _parenthesses; }
	MatchResult match();
	virtual ~MultiRegexp();
private:
	MultiRegexp(const MultiRegexp &) {}
	MultiRegexp &operator=(const MultiRegexp &) {}

	regex_t _compiled;
	regmatch_t * const _match_buffer = NULL;
	const size_t * const _match_table = NULL;
	const size_t _parenthesses = 0;
};

} /* namespace huge */
#endif /* MULTIREGEXP_H_ */
