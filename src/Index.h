/*
 * Index.h
 *
 *  Created on: Jan 23, 2013
 *      Author: radim
 */

#ifndef INDEX_H_
#define INDEX_H_

#include "LimitedRangeMap.h"

namespace huge {

class Index {
public:
	Index();
	virtual ~Index();

private:
	LimitedRangeMap<long, off_t> _line_to_pos;
	LimitedRangeMap<off_t, long> _pos_to_line;
};

} /* namespace huge */
#endif /* INDEX_H_ */
