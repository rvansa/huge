/*
 * File.cpp
 *
 *  Created on: Jan 12, 2013
 *      Author: rvansa
 */

#include "File.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

using namespace huge;

File::File(const char *filename): _filename(filename) {
	_descriptor = open(filename, O_RDONLY);
}

File::~File() {
	if (is_opened()) {
		close(_descriptor);
	}
}

void File::lock() {
	_locked = true;
}

void File::unlock() {
	_locked = false;
}

int File::read_data(char *buffer, int max) {
	int rd, total_read = 0;
	do {
		rd = ::read(_descriptor, buffer, max);
		if (rd < 0) return rd;
		buffer += rd;
		max -= rd;
		total_read += rd;
	} while (rd > 0);
	return total_read;
}

ReadOperation::ReadOperation(const FileView &fv): _file(fv.file()) {
	_file.lock();
	lseek(_file._descriptor, fv.pos(), SEEK_SET);
}

ReadBackOperation::ReadBackOperation(const FileView &fv): _file_view(fv), _pos(_file_view.pos()) {
	_file_view.file().lock();
}

int ReadBackOperation::read(char *buffer, int max) {
	if (max > _pos) max = _pos;
	_pos = _pos - max;
	lseek(_file_view.file()._descriptor, _pos, SEEK_SET);
	return _file_view.file().read_data(buffer, max);
}
