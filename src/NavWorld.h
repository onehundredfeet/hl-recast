#ifndef __RECAST_NAV_WORLD_H_
#define __RECAST_NAV_WORLD_H_

#pragma once

#include <DetourAlloc.h>
#include <DetourCommon.h>
#include <DetourDebugDraw.h>
#include <DetourNavMesh.h>
#include <DetourNavMeshBuilder.h>
#include <DetourTileCache.h>
#include <DetourTileCacheBuilder.h>
#include <Recast.h>
#include <RecastAlloc.h>
#include <RecastDebugDraw.h>
#include <stdio.h>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "NavVerts.h"
#include "PerfTimer.h"
#include "TriMeshBuilder.h"
#include "TriMeshPartition.h"
#include "fastlz/fastlz.h"
#include "hl-idl-helpers.hpp"

struct FastLZCompressor : public dtTileCacheCompressor {
    inline dtTileCacheCompressor *asSuper() {
        return this;
    }
    virtual int maxCompressedSize(const int bufferSize) {
        return (int)(bufferSize * 1.05f);
    }

    virtual dtStatus compress(const unsigned char *buffer, const int bufferSize,
                              unsigned char *compressed, const int /*maxCompressedSize*/, int *compressedSize) {
        *compressedSize = fastlz_compress((const void *const)buffer, bufferSize, compressed);
        return DT_SUCCESS;
    }

    virtual dtStatus decompress(const unsigned char *compressed, const int compressedSize,
                                unsigned char *buffer, const int maxBufferSize, int *bufferSize) {
        *bufferSize = fastlz_decompress(compressed, compressedSize, buffer, maxBufferSize);
        return *bufferSize < 0 ? DT_FAILURE : DT_SUCCESS;
    }
};

struct LinearAllocator : public dtTileCacheAlloc {
    unsigned char *buffer;
    size_t capacity;
    size_t top;
    size_t high;

    inline dtTileCacheAlloc *asSuper() {
        return this;
    }

    LinearAllocator(const size_t cap) : buffer(0), capacity(0), top(0), high(0) {
        //        printf("Linear alloc %lu\n", cap);
        resize(cap);
        //      printf("Done Linear alloc %lu\n", cap);
    }

    virtual ~LinearAllocator() {
        dtFree(buffer);
    }

    void resize(const size_t cap) {
        if (buffer) dtFree(buffer);
        buffer = (unsigned char *)dtAlloc(cap, DT_ALLOC_PERM);
        capacity = cap;
    }

    virtual void reset() {
        high = dtMax(high, top);
        top = 0;
    }

    virtual void *alloc(const size_t size) {
        if (!buffer)
            return 0;
        if (top + size > capacity)
            return 0;
        unsigned char *mem = &buffer[top];
        top += size;
        return mem;
    }

    virtual void free(void * /*ptr*/) {
        // Empty
    }
};

struct RemapProcessor : public dtTileCacheMeshProcess {
   private:
    std::map<int, int> _areaMap;
    std::map<int, int> _flagMap;
    int _flagDefault;

   public:
    inline RemapProcessor(int flagDefault = 0xffff) {
        _flagDefault = flagDefault;
    }

    virtual ~RemapProcessor() {
    }

    inline dtTileCacheMeshProcess *asSuper() {
        return this;
    }

    virtual void process(struct dtNavMeshCreateParams *params, unsigned char *polyAreas, unsigned short *polyFlags) {
        for (int i = 0; i < params->polyCount; ++i) {
            auto x = _areaMap.find(polyAreas[i]);

            if (x != _areaMap.end()) {
                polyAreas[i] = x->second;
            }

            auto y = _flagMap.find(polyAreas[i]);

            if (y != _flagMap.end()) {
                polyFlags[i] = y->second;
            } else {
                polyFlags[i] = _flagDefault;
            }
        }
    }

    void mapArea(int dtType, int areaType) {
        _areaMap[dtType] = areaType;
    }

    void mapFlags(int dtType, int flags) {
        _flagMap[dtType] = flags;
    }
};

struct TileCacheData {
    unsigned char *data;
    int dataSize;
};

static const int MAX_CONVEXVOL_PTS = 12;
struct ConvexVolume {
    float verts[MAX_CONVEXVOL_PTS * 3];
    float hmin, hmax;
    int nverts;
    int area;
};

struct AgentParameters {
    float walkableSlopeAngle = 45.0f;
    float walkableClimb = 0.9f;
    float walkableRadius = 0.6f;
    float walkableHeight = 2.0f;
};

class NavWorld;
 class SourceTriChunk {
        friend NavWorld;
        // NavWorld *_world;
        TriMeshPartition _partition;
        TriMeshBuilder _mesh;
        bool _enabled = true;
        int _maxTrisPerPartitionChunk;

       public:
        SourceTriChunk(int maxTrisPerPartitionChunk) : _maxTrisPerPartitionChunk(maxTrisPerPartitionChunk) {
        }

       ~SourceTriChunk() {
        }
        bool finalize();
        bool isEnabled() { return _enabled; }
        bool setEnabled(bool v) { return _enabled = v; }
        TriMeshBuilder &mesh() { return _mesh; }
        TriMeshBuilder *meshPtr() { return &_mesh; }
        TriMeshPartition &partition() { return _partition; }
        TriMeshPartition *partitionPtr() { return &_partition; }
        int getVertCount() { return _mesh.getVertCount(); }
        int getTriCount() { return _mesh.getTriCount(); }
        const float *getVerts() { return _mesh.getVerts(); }
        const int *getTris() { return _mesh.getTris(); }
        int maxTrisPerChunk() { return _partition.maxTrisPerChunk; }
    };

////////////////////////////////////////////////////////////////////////
// NAV WORLD
//////////////////////////////////////////////////////////////////////////
// Derived from sample tile mesh
class NavWorld {
   public:
    // Exposed types
   
    class TileBuilder {
        rcContext _context;
        friend NavWorld;
        NavWorld *_world;
        rcHeightfield _solid;
        rcCompactHeightfield _chf;
        LinearAllocator _talloc;
        FastLZCompressor _tcomp;
        RemapProcessor _tmproc;
        rcHeightfieldLayerSet _lset;

        // rcConfig _cfg;
        float _bmin[3];
        float _bmax[3];
        int _totalSourceChunks = 0;

        int _x;
        int _y;
        int _borderSize = 0;
        int _ntiles;

        std::vector<int> _chunkIds;
        // Allocate array that can hold triangle flags.
        std::vector<unsigned char> _triAreas;

        static const int MAX_CHUNKS = 512;

        std::vector<TileCacheData> _tileCacheData;

        void bind(int x, int y);
        int numCachedTiles() const { return _tileCacheData.size(); }
        const TileCacheData &getCachedTileData(int i) const { return _tileCacheData[i]; }
        void clearCachedTiles() {
            _tileCacheData.clear();
        }
        void dispose() {
            _chf.dispose();
            _solid.dispose();
            _lset.dispose();
        }

       public:
        int x() const { return _x; }
        int y() const { return _y; }

        // Not sure how much to put in here
        TileBuilder() : _talloc(32 * 1024) {
            _chunkIds.resize(MAX_CHUNKS);
        }
        ~TileBuilder() {
            dispose();
        }
        void retire() {
            _world->retire(this);
        }
        void reset();
        void queueForSwap() {
            //            _world->queueForSwap(this);
        }
        int numSourceChunks() { return _totalSourceChunks; }
        bool isEmpty() {
            return _tileCacheData.size() == 0;
        }

        bool buildTileColumnCacheData();

        bool insertIntoCache();
        bool inflate();
    };
    class QueryWorker {
        const int DEFAULT_MAX_NODES = 512;

       public:
        float getAreaCost(int i);
        void setAreaCost(int i, float cost);
        short getIncludeFlags();
        void setIncludeFlags(short flags);
        short getExcludeFlags();
        void setExcludeFlags(short flags);

        void setQueryArea(h_float3 center, h_float3 halfExtents);

        dtStatus findNearestPoly();
        dtPolyRef nearestPoly();
        void getNearestPoint(h_float3 point);
    	dtStatus findEndPoints(h_float3 start, h_float3 end, h_float3 halfExtents);
        bool centerOverNearestPoly();

        void setCurrentAsStart();
        void setCurrentAsEnd();

        void setStartPoint(dtPolyRef ref, h_float3 point);
        void setEndPoint(dtPolyRef ref, h_float3 point);

        void setMaximumPathLength(int maxNodes);  // needs to include the straightened path if you want it

        dtStatus findPath();
        int pathLength();
        void getPathNodes(dtPolyRef *nodes);
        int getPathNode(int i);

        dtStatus straightenPath();
        int straightPathLength();
        void getStraightPathPositions(h_float3_array nodes);
        void getStraightPathFlags(unsigned char *flags);
        void getStraightPathRefs(dtPolyRef *refs);

        void getStraightPathPosition(int i, h_float3 nodes);
        unsigned char getStraightPathNodeFlags(int i);
        dtPolyRef getStraightPathNodePoly(int i);

        void retire();
        void reset();

       private:
        QueryWorker(NavWorld *);
        dtQueryFilter _filter;
        dtNavMeshQuery _query;
        friend NavWorld;
        NavWorld *_world;

        _h_float3 _center;
        _h_float3 _halfExtents;
        _h_float3 _nearestPoint;
        _h_float3 _startPoint;
        _h_float3 _endPoint;
        dtPolyRef _nearestPoly;
        dtPolyRef _startPoly;
        dtPolyRef _endPoly;
        bool _centerOverNearestPoly = false;
        int _maxPathLength;
        int _pathLength;
        int _straightPathLength;
        std::vector<dtPolyRef> _path;
        std::vector<dtPolyRef> _straightPath;
        std::vector<_h_float3> _straightPos;
        std::vector<unsigned char> _straightFlags;
    };

    // exposed functions
    int maxTrisPerChunk() {
        int maxTris = 0;
        for (auto chunk : _chunks) {
            maxTris = std::max(maxTris, chunk->maxTrisPerChunk());
        }
        return maxTris;
    }

    static const int DEFAULT_TEMP_MEMORY = 64 * 1024;
    NavWorld() : _talloc(DEFAULT_TEMP_MEMORY) {
        _maxTrisPerPartitionChunk = DEFAULT_TRIS_PER_PARTITION_CHUNK;
    }

   private:
    dtNavMesh _navMesh;
    dtTileCache _tileCache;

    LinearAllocator _talloc;
    FastLZCompressor _tcomp;
    RemapProcessor _tmproc;

    float _tileSize;
    float _cellSize;
    float _cellHeight;
    int _tileCountWidth;
    int _tileCountHeight;
    int _tileSizeInCells;
    static const int DEFAULT_TRIS_PER_PARTITION_CHUNK = 64;
    int _maxTrisPerPartitionChunk;

    AgentParameters _agentParams;

    bool _filterLowHangingObstacles;
    bool _filterLedgeSpans;
    bool _filterWalkableLowHeightSpans;

    float _origin[3];
    float _bmin[3];
    float _bmax[3];

    float _lastBuiltTileBmin[3];
    float _lastBuiltTileBmax[3];

    std::vector<SourceTriChunk *> _chunks;
    std::vector<SourceTriChunk *> _ownedChunks;
    std::set<TileBuilder *> _activeBuilders;
    std::vector<TileBuilder *> _dormantBuilders;
    std::vector<ConvexVolume> _convexVolumes;
    std::set<QueryWorker *> _activeQueryWorkers;
    std::vector<QueryWorker *> _dormantQueryWorkers;

    void retire(TileBuilder *builder);

   public:
    SourceTriChunk *addChunk() {
        auto layer = new SourceTriChunk(_maxTrisPerPartitionChunk);
        _chunks.push_back(layer);
        _ownedChunks.push_back(layer);
        return layer;
    }

    void attachChunk(SourceTriChunk *chunk) {
        _chunks.push_back(chunk);
    }

    void getTileRegion(h_float2 bmin, h_float2 bmax, h_int2 tmin, h_int2 tmax);
    TileBuilder *getTileBuilder(int x, int y);
    QueryWorker *getQueryWorker();

    static NavWorld *create(h_float3 origin, h_float3 dim, int tileSizeInCells, float cellSize, float cellHeight, int maxTiles, int maxPolys, int maxObstacles, AgentParameters *);
};

#include "NavWorld.cpp"
#endif
