/*
 * MultiRegexp.h
 *
 *  Created on: Jan 20, 2013
 *      Author: radim
 */

#ifndef MULTIREGEXP_H_
#define MULTIREGEXP_H_

#include <string>
#include <memory.h>
#include "tre.h"
#include "Exception.h"

namespace huge {

class RegexpCompileException: public Exception {
public:
	RegexpCompileException(const char *expression):
		Exception(std::string("Regexp compilation failed for '").append(expression).append("'")) {}
};

class MatchResult {
friend class MultiRegexp;
public:
	static const MatchResult NOT_FOUND;

	size_t which() const { return _which; }
	off_t start_pos() const { return _start_pos; }
	off_t end_pos() const { return _end_pos; }
	bool found() const { return _start_pos >= 0; }

	MatchResult(const MatchResult &other): _which(other._which),
		_start_pos(other._start_pos), _end_pos(other._end_pos), _num_submatch(other._num_submatch) {
		copy_submatches(other);
	}
	MatchResult &operator=(const MatchResult &other) {
		_which = other._which;
		_start_pos = other._start_pos;
		_end_pos = other._end_pos;
		_num_submatch = other._num_submatch;
		copy_submatches(other);
		return *this;
	}
	~MatchResult() {
		if (_submatch != NULL && _submatch != _submatch_buffer) delete[] _submatch;
	}

private:
	const static size_t SUBMATCH_BUFFER_SIZE = 16;

	MatchResult(size_t w, off_t s, off_t e, size_t submatches = 0): _which(w), _start_pos(s), _end_pos(e), _submatch(NULL), _num_submatch(0) {
		if (submatches == 0) _submatch = NULL;
		else if (submatches < SUBMATCH_BUFFER_SIZE / 2) _submatch = _submatch_buffer;
		else _submatch = new off_t[2 * submatches];
	}

	void add_submatch(off_t start, off_t end) {
		_submatch[_num_submatch << 1] = start;
		_submatch[(_num_submatch << 1) + 1] = end;
		_num_submatch++;
	}

	void copy_submatches(const MatchResult &other) {
		// TODO: use Buffer
		if (other._submatch == NULL) {
			_submatch = NULL;
		} else {
			if (other._submatch == other._submatch_buffer) {
				_submatch = _submatch_buffer;
			} else {
				_submatch = new off_t[other._num_submatch];
			}
			memcpy(_submatch, other._submatch, other._num_submatch * 2 * sizeof(off_t));
		}
	}

	size_t _which;
	off_t _start_pos;
	off_t _end_pos;
	off_t _submatch_buffer[SUBMATCH_BUFFER_SIZE];
	off_t *_submatch;
	size_t _num_submatch;
};


class MultiRegexp {
public:
	MultiRegexp(const char *expressions[], size_t n) throw(RegexpCompileException);
	inline const regex_t *compiled() const { return &_compiled; }
	inline regmatch_t *match_buffer() const { return _match_buffer; }
	inline size_t match_buffer_size() const { return _parenthesses; }
	MatchResult match(off_t begin_offset, bool report_submatch);
	virtual ~MultiRegexp();
private:
	MultiRegexp(const MultiRegexp &) {}
	MultiRegexp &operator=(const MultiRegexp &) { return *this; }

	regex_t _compiled;
	regmatch_t *_match_buffer = NULL;
	size_t *_match_table = NULL;
	size_t _parenthesses = 0;
};

} /* namespace huge */
#endif /* MULTIREGEXP_H_ */
