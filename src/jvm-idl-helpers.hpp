
	#ifndef __JVM_IDL_HELPERS_H_
#define __JVM_IDL_HELPERS_H_

/*
#include <hl.h>
#include <string>

void hl_cache_string_type( vstring *str);
vstring * hl_utf8_to_hlstr( const char *str);
vstring * hl_utf8_to_hlstr( const std::string &str);

#pragma once



template<class T, class C>
class  IteratorWrapper {
	private:
		bool _initialized;
		typename C::iterator _it;
		C &_collection;
	public:	
		inline void reset() {
			_initialized = false;
		}
		inline IteratorWrapper( C&col ) : _collection(col) {
			reset();
		}
		inline bool next() {
			if (!_initialized) {
				_initialized = true;
				_it = _collection.begin();
			} else {
				_it++;
			}
			return  (_it != _collection.end());
		}
		inline T &get() {
			return *_it;
		}
        inline T *getPtr() {
			return &(*_it);
		}
};
*/
#endif
	