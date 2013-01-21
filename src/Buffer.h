#include <memory.h>

namespace huge {

template<typename T, size_t StackSize>
class Buffer {
public:
	Buffer(size_t actual_size): _size(actual_size), _buffer(_stack_buffer) {
		if (actual_size > StackSize) {
			_buffer = new T[actual_size];
		}
	}
	Buffer(const Buffer<T, StackSize> &other): _size(other._size) {
		copy_in(other);
	}
	~Buffer() {
		if (_buffer != _stack_buffer) delete[] _buffer;
	}

	Buffer<T, StackSize> &operator=(const Buffer<T, StackSize> &other) {
		_size = other._size;
		copy_in(other);
		return *this;
	}

	T *operator()() {
		return _buffer;
	}
private:
	void copy_in(const Buffer<T, StackSize> &other) {
		if (other._buffer == other._stack_buffer) {
			_buffer = _stack_buffer;
		} else {
			_buffer = new T[other._size];
		}
		for (size_t i = 0; i < other._size; ++i) {
			_buffer[i] = other._buffer[i];
		}
	}

	size_t _size;
	T *_buffer;
	T _stack_buffer[StackSize];
};

}
