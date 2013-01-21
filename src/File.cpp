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

size_t File::read_data(char *buffer, size_t max) throw(FileReadException) {
	ssize_t rd;
	size_t total_read = 0;
	do {
		rd = ::read(_descriptor, buffer, max);
		if (rd < 0) throw FileReadException(_filename);
		buffer += rd;
		max -= rd;
		total_read += rd;
	} while (rd > 0);
	return total_read;
}

void ReadOperation::start() throw (FileReadException) {
	_file.lock();
	_started = true;
	if (::lseek(_file._descriptor, _start_pos, SEEK_SET) < 0) throw FileReadException(_file.filename());
}

ReadBackOperation::ReadBackOperation(const FileView &fv): _file_view(fv), _pos(_file_view.pos()) {
	_file_view.file().lock();
}

size_t ReadBackOperation::read(char *buffer, size_t max) throw(FileReadException) {
	if (max > (size_t) _pos) max = _pos;
	_pos = _pos - max;
	if (lseek(_file_view.file()._descriptor, _pos, SEEK_SET)) {
		throw FileReadException(_file_view.file().filename());
	}
	return _file_view.file().read_data(buffer, max);
}
