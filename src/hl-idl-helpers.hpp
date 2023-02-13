
	#ifndef __HL_IDL_HELPERS_H_
#define __HL_IDL_HELPERS_H_

#include <hl.h>
#include <string>

void hl_cache_string_type( vstring *str);
vstring * hl_utf8_to_hlstr( const char *str);
vstring * hl_utf8_to_hlstr( const std::string &str);

#pragma once


// Float vector
struct _h_float2 {
	float x;
	float y;
};

struct _h_float3 {
	float x;
	float y;
	float z;
};

struct _h_float4 {
	float x;
	float y;
	float z;
	float w;
};

// int vector
struct _h_int2 {
	int x;
	int y;
};

struct _h_int3 {
	int x;
	int y;
	int z;
};

struct _h_int4 {
	int x;
	int y;
	int z;
	int w;
};

// double vector
struct _h_double2 {
	double x;
	double y;
};

struct _h_double3 {
	double x;
	double y;
	double z;
};

struct _h_double4 {
	double x;
	double y;
	double z;
	double w;
};


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

#endif
	