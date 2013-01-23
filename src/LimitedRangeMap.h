/*
 * WeakMap.h
 *
 *  Created on: Jan 23, 2013
 *      Author: radim
 */

#ifndef WEAKMAP_H_
#define WEAKMAP_H_

#include <stdlib.h>
#include <map>

namespace huge {

template<typename KeyType, typename ValueType>
class Range {
public:
	Range(void): type(3) {}
	Range(const KeyType &lk, const ValueType &lv, const KeyType &rk, const ValueType &rv):
		left_key(lk), left_value(lv), right_key(rk), right_value(rv), type(0) {}

	Range(void *unused, const KeyType &rk, const ValueType &rv):
			right_key(rk), right_value(rv), type(1) { (void) unused; }

	Range(const KeyType &lk, const ValueType &lv, void *unused):
			left_key(lk), left_value(lv), type(2) { (void) unused; }

	inline bool is_left_open() { return type & 1; }
	inline bool is_right_open() { return type & 2; }
	KeyType left_key;
	ValueType left_value;
	KeyType right_key;
	ValueType right_value;
	int type;
};

template<typename KeyType, typename ValueType>
class LimitedRangeMap {
	typedef std::map<KeyType, ValueType> InternalMap;

public:
	LimitedRangeMap(size_t max_entries): _max_entries(max_entries) {}
	virtual ~LimitedRangeMap() {}
	void put(const KeyType &k, const ValueType &v) {
		_map[k] = v;
		if (_map.size() >= _max_entries) {
			InternalMap nmap;
			bool odd_even = true;
			for (typename InternalMap::iterator it = _map.begin(); it != _map.end(); ++it) {
				if (odd_even) {
					nmap[it->first] = it->second;
				}
				odd_even = !odd_even;
			}
			_map = nmap;
		}
	}
	Range<KeyType, ValueType> get(KeyType k) const {
		typename InternalMap::const_iterator upper = _map.upper_bound(k);
		if (upper == _map.cend()) {
			if (_map.size() == 0) return Range<KeyType, ValueType>();
			else {
				typename InternalMap::const_reverse_iterator rbegin = _map.crbegin();
				return Range<KeyType, ValueType>(rbegin->first, rbegin->second, NULL);
			}
		} else {
			if (upper == _map.cbegin()) return Range<KeyType, ValueType>(NULL, upper->first, upper->second);
			else {
				typename InternalMap::const_iterator lower = upper;
				lower--;
				return Range<KeyType, ValueType>(lower->first, lower->second, upper->first, upper->second);
			}
		}
	}
private:
	size_t _max_entries;
	InternalMap _map;
};

} /* namespace huge */
#endif /* WEAKMAP_H_ */
