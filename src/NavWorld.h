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
        resize(cap);
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
////////////////////////////////////////////////////////////////////////
// NAV WORLD
//////////////////////////////////////////////////////////////////////////
// Derived from sample tile mesh
class NavWorld {
   public:
    class SourcePolyChunk {
        friend NavWorld;
        NavWorld *_world;
        TriMeshPartition _partition;
        TriMeshBuilder _mesh;
        bool _enabled = true;

       public:
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

    int maxTrisPerChunk() {
        int maxTris = 0;
        for (auto chunk : _chunks) {
            maxTris = std::max(maxTris, chunk->maxTrisPerChunk());
        }
        return maxTris;
    }

    static const int DEFAULT_TEMP_MEMORY = 32 * 1024;
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
    /*
    float _walkableSlopeAngle;
    float _walkableClimb;
    float _walkableRadius;
    float _walkableHeight;
*/
    bool _filterLowHangingObstacles;
    bool _filterLedgeSpans;
    bool _filterWalkableLowHeightSpans;

    float _origin[3];
    float _bmin[3];
    float _bmax[3];

    float _lastBuiltTileBmin[3];
    float _lastBuiltTileBmax[3];

    std::vector<SourcePolyChunk *> _chunks;
    std::vector<TileBuilder *> _activeBuilders;
    std::vector<TileBuilder *> _dormantBuilders;
    std::vector<ConvexVolume> _convexVolumes;

    void retire(TileBuilder *builder) {
        builder->reset();
        _dormantBuilders.push_back(builder);
    }

   public:
    SourcePolyChunk *addChunk() {
        auto layer = new SourcePolyChunk();
        layer->_world = this;
        _chunks.push_back(layer);
        return layer;
    }

    void getTileRegion(_h_float2 *bmin, _h_float2 *bmax, _h_int2 *tmin, _h_int2 *tmax) {
        tmin->x = (int)floorf((bmin->x - _origin[0]) / _tileSize);
        tmin->y = (int)floorf((bmin->y - _origin[2]) / _tileSize);
        tmax->x = (int)floorf((bmax->x - _origin[0]) / _tileSize);
        tmax->y = (int)floorf((bmax->y - _origin[2]) / _tileSize);

        tmin->x = dtClamp(tmin->x, 0, dtMin(_tileCountWidth - 1, tmax->x));
        tmin->y = dtClamp(tmin->y, 0, dtMin(_tileCountHeight - 1, tmax->y));
        tmax->x = dtClamp(tmax->x, tmin->x, _tileCountWidth - 1);
        tmax->y = dtClamp(tmax->y, tmin->y, _tileCountHeight - 1);
    }

    TileBuilder *getTileBuilder(int x, int y) {
        if (_dormantBuilders.size() > 0) {
            auto builder = _dormantBuilders.back();
            _dormantBuilders.pop_back();
            _activeBuilders.push_back(builder);
            builder->bind(x, y);
            return builder;
        }
        auto builder = new TileBuilder();
        builder->_world = this;
        builder->bind(x, y);
        _activeBuilders.push_back(builder);
        return builder;
    }
    /*
void setAgentParameters(float walkableSlopeAngle,
                        float walkableClimb,
                        float walkableRadius,
                        float walkableHeight) {
    _walkableSlopeAngle = walkableSlopeAngle;
    _walkableClimb = walkableClimb;
    _walkableRadius = walkableRadius;
    _walkableHeight = walkableHeight;
}

void rebuildTile(int tx, int ty) {
    int dataSize = 0;
    unsigned char *data = buildTileMesh(tx, ty, _lastBuiltTileBmin, _lastBuiltTileBmax, dataSize);

    // Remove any previous data (navmesh owns and deletes the data).
    _navMesh.removeTile(_navMesh.getTileRefAt(tx, ty, 0), 0, 0);

    // Add tile, or leave the location empty.
    if (data) {
        // Let the navmesh own the data.
        dtStatus status = _navMesh.addTile(data, dataSize, DT_TILE_FREE_DATA, 0, 0);
        if (dtStatusFailed(status))
            dtFree(data);
    }
}
*/
    static NavWorld *create(_h_float3 *origin, _h_float3 *dim, int tileSizeInCells, float cellSize, float cellHeight, int maxTiles, int maxPolys, int maxObstacles, AgentParameters *);
};

#include "NavWorld.cpp"
#endif
