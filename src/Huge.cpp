/*
 * Huge.cpp
 *
 *  Created on: Jan 13, 2013
 *      Author: rvansa
 */

#include "Huge.h"

using namespace huge;

Huge::Huge(): _display(0), _file(0), _formatter(0), _file_view(0) {
	_indexed.push_back(std::string("new"));
	_indexed.push_back(std::string("true"));
}

Huge::~Huge() {
	// TODO Auto-generated destructor stub
}

