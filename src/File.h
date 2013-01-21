/*
 * File.h
 *
 *  Created on: Jan 12, 2013
 *      Author: rvansa
 */

#ifndef FILE_H_
#define FILE_H_

#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include "Exception.h"

namespace huge {

	class ReadOperation;

	class FileReadException: public Exception {
	public:
		FileReadException(const std::string &filename): Exception(std::string("Problems reading file ") + filename) {}
	};

	class FileOpenException: public Exception {
	public:
		FileOpenException(const std::string &filename): Exception(std::string("Problems opening file ") + filename) {}
	};

	class File {
	friend class ReadOperation;
	friend class ReadBackOperation;
	public:
		File(const char *filename) throw(FileOpenException);
		virtual ~File();
		inline bool is_opened(void) const { return _descriptor >= 0; }
		inline const char *filename(void) const { return _filename; }
		void lock(void);
		void unlock(void);
	protected:
		size_t read_data(char *buffer, size_t max) throw(FileReadException);
	private:
		const char *_filename;
		pthread_mutex_t _lock;
		int _descriptor;
	};

	class FileView {
	public:
		FileView(): _file(0), _pos(0) {}
		FileView(File &file): _file(&file), _pos(0) {}
		inline File &file() const { return *_file; }
		inline off_t pos() const { return _pos; }
		inline FileView &pos(off_t pos) { _pos = pos; return *this; }
	private:
		File *_file;
		off_t _pos;
	};

	/* Guardian for the file lock */
	class ReadOperation {
	public:
		ReadOperation(const FileView &fv): _file(fv.file()), _start_pos(fv.pos()), _started(false), _finished(false) {}
		~ReadOperation() {
			finish();
		}
		void start() throw(FileReadException);
		inline void finish() {
			if (!_started || _finished) return;
			_file.unlock();
			_finished = true;
		}
		inline size_t read(char *buffer, size_t max) throw(FileReadException)
		{
			if (!_started) start();
			return _file.read_data(buffer, max);
		}
	private:
		File &_file;
		off_t _start_pos;
		bool _started;
		bool _finished;
	};

	class ReadBackOperation {
	public:
		ReadBackOperation(const FileView &fv);
		~ReadBackOperation() {
			_file_view.file().unlock();
		}
		size_t read(char *buffer, size_t max) throw(FileReadException);
	private:
		const FileView &_file_view;
		off_t _pos;
	};
}

#endif /* FILE_H_ */
