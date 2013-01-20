/*
 * File.h
 *
 *  Created on: Jan 12, 2013
 *      Author: rvansa
 */

#ifndef FILE_H_
#define FILE_H_

#include <sys/types.h>

namespace huge {

	class ReadOperation;

	class File {
	friend class ReadOperation;
	friend class ReadBackOperation;
	public:
		File(const char *filename);
		virtual ~File();
		inline bool is_opened(void) const { return _descriptor >= 0; }
		inline const char *filename(void) const { return _filename; }
		void lock(void);
		void unlock(void);
	protected:
		int read_data(char *buffer, int max);
	private:
		const char *_filename;
		bool _locked = false; // TODO mutex
		int _descriptor;
	};

	class FileView {
	public:
		FileView(File &file): _file(file), _pos(0) {}
		inline File &file() const { return _file; }
		inline off_t pos() const { return _pos; }
		inline FileView &pos(off_t pos) { _pos = pos; return *this; }
	private:
		File &_file;
		off_t _pos;
	};

	/* Guardian for the file lock */
	class ReadOperation {
	public:
		ReadOperation(const FileView &fv);
		~ReadOperation() {
			_file.unlock();
		}
		inline int read(char *buffer, int max)
		{
			return _file.read_data(buffer, max);
		}
	private:
		File &_file;
	};

	class ReadBackOperation {
	public:
		ReadBackOperation(const FileView &fv);
		~ReadBackOperation() {
			_file_view.file().unlock();
		}
		int read(char *buffer, int max);
	private:
		const FileView &_file_view;
		off_t _pos;
	};
}

#endif /* FILE_H_ */
