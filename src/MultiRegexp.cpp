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
	_parenthesses = parentheses + n + 1;
	_match_table = new size_t[_parenthesses];
	_match_buffer = new regmatch_t[_parenthesses];

	size_t *match = _match_table;
	*(match++) = -1; // first match is for the whole result position
	std::string buffer;
	buffer.reserve(total_length + 3*n);
	for (size_t expid = 0; expid < n; ++expid) {
		buffer.push_back('(');
		*(match++) = expid;
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

MatchResult MultiRegexp::match(off_t begin_offset, bool report_submatch) {
	if (_parenthesses == 0 || _match_buffer[0].rm_so < 0)
		return MatchResult::NOT_FOUND;
	for (size_t i = 1; i < _parenthesses; ++i) {
		if (_match_buffer[i].rm_so >= 0) {
			if (report_submatch) {
				size_t j = i + 1;
				for (; j < _parenthesses; ++j) {
					if (_match_buffer[i].rm_so < 0) break;
				}
				MatchResult result(_match_table[i], begin_offset + _match_buffer[i].rm_so, begin_offset + _match_buffer[i].rm_eo, j - i);
				for (j = i + 1; j < _parenthesses; ++j) {
					result.add_submatch(begin_offset + _match_buffer[j].rm_so, begin_offset + _match_buffer[j].rm_eo);
				}
				return result;
			} else {
				return MatchResult(_match_table[i], begin_offset + _match_buffer[i].rm_so, begin_offset + _match_buffer[i].rm_eo);
			}
		}
	}
	return MatchResult::NOT_FOUND;
}

