#ifndef __HL_RECAST_H_
#define __HL_RECAST_H_

#include <DetourTileCacheBuilder.h>
#include "fastlz/fastlz.h"
struct FastLZCompressor : public dtTileCacheCompressor
{
	virtual int maxCompressedSize(const int bufferSize)
	{
		return (int)(bufferSize* 1.05f);
	}
	
	virtual dtStatus compress(const unsigned char* buffer, const int bufferSize,
							  unsigned char* compressed, const int /*maxCompressedSize*/, int* compressedSize)
	{
		*compressedSize = fastlz_compress((const void *const)buffer, bufferSize, compressed);
		return DT_SUCCESS;
	}
	
	virtual dtStatus decompress(const unsigned char* compressed, const int compressedSize,
								unsigned char* buffer, const int maxBufferSize, int* bufferSize)
	{
		*bufferSize = fastlz_decompress(compressed, compressedSize, buffer, maxBufferSize);
		return *bufferSize < 0 ? DT_FAILURE : DT_SUCCESS;
	}
};


struct LinearAllocator : public dtTileCacheAlloc
{
	unsigned char* buffer;
	size_t capacity;
	size_t top;
	size_t high;
	
	LinearAllocator(const size_t cap) : buffer(0), capacity(0), top(0), high(0)
	{
		resize(cap);
	}
	
	~LinearAllocator()
	{
		dtFree(buffer);
	}

	void resize(const size_t cap)
	{
		if (buffer) dtFree(buffer);
		buffer = (unsigned char*)dtAlloc(cap, DT_ALLOC_PERM);
		capacity = cap;
	}
	
	virtual void reset()
	{
		high = dtMax(high, top);
		top = 0;
	}
	
	virtual void* alloc(const size_t size)
	{
		if (!buffer)
			return 0;
		if (top+size > capacity)
			return 0;
		unsigned char* mem = &buffer[top];
		top += size;
		return mem;
	}
	
	virtual void free(void* /*ptr*/)
	{
		// Empty
	}
};

struct RemapProcessor : public dtTileCacheMeshProcess{

	inline RemapProcessor() 
	{
	}

	virtual void process(struct dtNavMeshCreateParams* params,unsigned char* polyAreas, unsigned short* polyFlags)
	{
        /*
		// Update poly flags from areas.
		for (int i = 0; i < params->polyCount; ++i)
		{
			if (polyAreas[i] == DT_TILECACHE_WALKABLE_AREA)
				polyAreas[i] = SAMPLE_POLYAREA_GROUND;

			if (polyAreas[i] == SAMPLE_POLYAREA_GROUND ||
				polyAreas[i] == SAMPLE_POLYAREA_GRASS ||
				polyAreas[i] == SAMPLE_POLYAREA_ROAD)
			{
				polyFlags[i] = SAMPLE_POLYFLAGS_WALK;
			}
			else if (polyAreas[i] == SAMPLE_POLYAREA_WATER)
			{
				polyFlags[i] = SAMPLE_POLYFLAGS_SWIM;
			}
			else if (polyAreas[i] == SAMPLE_POLYAREA_DOOR)
			{
				polyFlags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
			}
		}
        */
	}
};

struct TileCacheData
{
	unsigned char* data;
	int dataSize;
};

inline static void rcConfigCopy( rcConfig *b, rcConfig * a) {
	memcpy(b, a, sizeof(rcConfig));
}


#endif