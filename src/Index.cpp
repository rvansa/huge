/*
 * Index.cpp
 *
 *  Created on: Jan 23, 2013
 *      Author: radim
 */

#include "Index.h"

namespace huge {

Index::Index(): _line_to_pos(100000), _pos_to_line(100000) {
	_line_to_pos.put(42, 0);
	_line_to_pos.get(42);
}

Index::~Index() {
	// TODO Auto-generated destructor stub
}

} /* namespace huge */
