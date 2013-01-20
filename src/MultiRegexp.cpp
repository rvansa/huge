/*
 * MultiRegexp.cpp
 *
 *  Created on: Jan 20, 2013
 *      Author: radim
 */


#include "MultiRegexp.h"

using namespace huge;

const MatchResult MatchResult::NOT_FOUND(-1, -1, -1);

MultiRegexp::MultiRegexp(const char *expressions[], size_t n) throw(RegexpCompileException) {
	size_t total_length = 0;
	size_t parentheses = 0;
	for (size_t expid = 0; expid < n; expid++) {
		const char *str = expressions[expid];
		for (; *str != 0; ++str) {
			if (*str == '(') parentheses++;
		}
		total_length += (str - expressions[expid]);
	}
	_parenthesses = parentheses + n;
	_match_table = new size_t[_parenthesses];
	_match_buffer = new regmatch_t[_parenthesses];

	size_t *match = _match_table;
	std::string buffer(total_length + 3*n, 0);
	for (size_t expid = 0; expid < n; ++expid) {
		buffer.push_back('(');
		*(match++) = expid;
		size_t length = 0;
		const char *str = expressions[expid];
		for (; *str != 0; ++str) {
			if (*str == '(') {
				*(match++) = expid;
			}
		}
		buffer.append(expressions[expid], str - expressions[expid]);
		buffer.push_back(')');
		if (expid != n - 1) {
			buffer.push_back('|');
		}
	}
	if (tre_regcomp(&_compiled, buffer.c_str(), REG_EXTENDED | REG_NEWLINE)) {
		delete[] _match_table;
		delete[] _match_buffer;
		// find which expression caused the failure;
		for (size_t expid = 0; expid < n; ++expid) {
			if (tre_regcomp(&_compiled, expressions[expid], REG_EXTENDED | REG_NEWLINE)) {
				throw RegexpCompileException(expressions[expid]);
			}
			tre_regfree(&_compiled);
		}
		throw RegexpCompileException(buffer.c_str());
	}
}

MultiRegexp::~MultiRegexp() {
	delete[] _match_table;
	delete[] _match_buffer;
	tre_regfree(&_compiled);
}

MatchResult MultiRegexp::match() {
	if (_parenthesses == 0 || _match_buffer[0].rm_so < 0)
		return MatchResult::NOT_FOUND;
	for (size_t i = 0; i < _parenthesses; ++i) {
		if (_match_buffer[i].rm_so >= 0) {
			return _match_table[i];
		}
	}
	return MatchResult::NOT_FOUND;
}

