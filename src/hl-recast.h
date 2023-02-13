#ifndef __HL_RECAST_H_
#define __HL_RECAST_H_

#include <DebugDraw.h>
#include <DetourCommon.h>
#include <DetourNavMeshBuilder.h>
#include <DetourTileCache.h>
#include <DetourTileCacheBuilder.h>
#include <Recast.h>
#include <RecastAlloc.h>
#include <stdio.h>

#include <map>
#include <unordered_map>
#include <vector>

#include "ChunkyTriMesh.h"
#include "PerfTimer.h"
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

inline static void rcConfigCopy(rcConfig *b, rcConfig *a) {
    memcpy(b, a, sizeof(rcConfig));
}

inline static unsigned short *rcAllocShort(int length, rcAllocHint hint) {
    return (unsigned short *)rcAlloc(length * sizeof(unsigned short), hint);
}

template <class T>
inline void rcClear(T *ptr, int length) {
    // printf("Clearing %p of %d x %d\n", ptr, (int)sizeof(T), length);
    memset(ptr, 0, sizeof(T) * length);
}

template <class T>
inline T *rcOffset(T *ptr, int length) {
    return ptr + length;
}

inline int getNodeTriIndex(rcChunkyTriMesh *_this, int nodeIdx) {
    return _this->nodes[nodeIdx].i;
}

inline int getNodeTriCount(rcChunkyTriMesh *_this, int nodeIdx) {
    return _this->nodes[nodeIdx].n;
}

inline int *getTriVertIndices(rcChunkyTriMesh *_this, int triIndex) {
    return &_this->tris[triIndex];
}

enum TileCacheLayerHeaderConstants {
    TILECACHE_MAGIC = DT_TILECACHE_MAGIC,
    TILECACHE_VERSION = DT_TILECACHE_VERSION
};

enum EStatus {
    SUCCESS = DT_SUCCESS,
    FAILURE = DT_FAILURE,
    IN_PROGRESS = DT_IN_PROGRESS
};

class PerformanceTimer {
    TimeVal _start;
    TimeVal _stop;

   public:
    PerformanceTimer() {}
    ~PerformanceTimer() {}
    void start() {
        _start = getPerfTime();
    }
    void stop() {
        _stop = getPerfTime();
    }
    double deltaSeconds() {
        int microSeconds = getPerfTimeUsec(_stop - _start);

        return (double)microSeconds / 1000000;
    }

    double deltaMilliseconds() {
        int microSeconds = getPerfTimeUsec(_stop - _start);

        return (double)microSeconds / 1000;
    }

    double deltaMicroseconds() {
        int microSeconds = getPerfTimeUsec(_stop - _start);

        return (double)microSeconds;
    }
};

inline void setToZero(int *a, float *b, double *c, bool *d) {
    *a = 0;
    *b = 0.0f;
    *c = 0.0f;
    *d = false;
}
inline void setToOne(int *a, float *b, double *c, bool *d) {
    *a = 1;
    *b = 1.0f;
    *c = 1.0f;
    *d = true;
}

class dtMeshCapture : public duDebugDraw {
   public:
    struct float3 {
        float3() {
        }
        float3(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        float x;
        float y;
        float z;

        inline float3 add(float3 v) {
            return float3(x + v.x, y + v.y, z + v.z);
        }
        inline float3 sub(float3 v) {
            return float3(x - v.x, y - v.y, z - v.z);
        }
        inline float3 scale(float s) {
            return float3(x * s, y * s, z * s);
        }
    };
    dtMeshCapture(bool isSurface) : _isSurface(isSurface) {
    }

    void captureNavMesh(const dtNavMesh &nm, unsigned short flags = 0xffff);
    void captureHeightField(const rcHeightfield &hf);
    void captureCompactHeightField(const rcCompactHeightfield &chf);
    void captureHeighfieldLayerSet(const rcHeightfieldLayerSet &hfls);

    /// Begin drawing primitives.
    ///  @param prim [in] primitive type to draw, one of rcDebugDrawPrimitives.
    ///  @param size [in] size of a primitive, applies to point size and line width only.
    virtual void begin(duDebugDrawPrimitives prim, float size = 1.0f);

    /// Submit a vertex
    ///  @param pos [in] position of the verts.
    ///  @param color [in] color of the verts.
    virtual void vertex(const float *pos, unsigned int color);

    /// Submit a vertex
    ///  @param x,y,z [in] position of the verts.
    ///  @param color [in] color of the verts.
    virtual void vertex(const float x, const float y, const float z, unsigned int color);

    /// Submit a vertex
    ///  @param pos [in] position of the verts.
    ///  @param color [in] color of the verts.
    virtual void vertex(const float *pos, unsigned int color, const float *uv);

    /// Submit a vertex
    ///  @param x,y,z [in] position of the verts.
    ///  @param color [in] color of the verts.
    virtual void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v);

    /// End drawing primitives.
    virtual void end();

    size_t numVerts() { return _verts.size(); }
    size_t numSurfacePrims() { return _verts.size() / 3; }
    bool isSurface() { return _isSurface; }
    inline float3 &getVert(int idx) {
        return _verts[idx];
    }

    inline void getVert(int idx, float *vert) {
        const auto x = _verts[idx];
        vert[0] = x.x;
        vert[1] = x.y;
        vert[2] = x.z;
    }

    virtual void depthMask(bool state) {}

    virtual void texture(bool state) {}

   private:
    float3 _lastVerts[4];
    bool _isSurface;
    std::vector<float3> _verts;
    duDebugDrawPrimitives _currentMode;
    int _prim;
    float _lineWidth;

    inline void finishPrim() {
        switch (_currentMode) {
            case duDebugDrawPrimitives::DU_DRAW_QUADS:
                if (_prim == 4) {
                    _verts.push_back(_lastVerts[0]);
                    _verts.push_back(_lastVerts[1]);
                    _verts.push_back(_lastVerts[2]);

                    _verts.push_back(_lastVerts[0]);
                    _verts.push_back(_lastVerts[2]);
                    _verts.push_back(_lastVerts[3]);

                    _prim = 0;
                }
                break;
            case duDebugDrawPrimitives::DU_DRAW_TRIS:
                if (_prim == 3) {
                    _verts.push_back(_lastVerts[0]);
                    _verts.push_back(_lastVerts[1]);
                    _verts.push_back(_lastVerts[2]);
                    _prim = 0;
                }
                break;
            case duDebugDrawPrimitives::DU_DRAW_LINES:
                if (_prim == 2) {
                    auto x = _lastVerts[0];
                    auto y = _lastVerts[0];
                    auto delta = y.sub(x);
                    float3 p;

                    float ax = fabs(delta.x);
                    float ay = fabs(delta.y);
                    float az = fabs(delta.z);

                    if (ax <= ay) {
                        if (ax <= az) {
                            p = float3(1.0f, 0.0f, 0.0f);
                        } else {
                            p = float3(0.0f, 0.0f, 1.0f);
                        }
                    } else if (ay <= az) {
                        p = float3(0.0f, 1.0f, 0.0f);
                    } else {
                        p = float3(0.0f, 0.f, 1.0f);
                    }

                    p = p.scale(_lineWidth);

                    auto a = x.add(p);
                    auto b = x.sub(p);
                    auto c = y.add(p);
                    auto d = y.sub(p);

                    _verts.push_back(a);
                    _verts.push_back(b);
                    _verts.push_back(c);

                    _verts.push_back(b);
                    _verts.push_back(c);
                    _verts.push_back(d);

                    _prim = 0;
                }
                break;

            case duDebugDrawPrimitives::DU_DRAW_POINTS:
                if (_isSurface) throw std::runtime_error("Adding points to surface capture");
                _verts.push_back(_lastVerts[0]);
                _prim = 0;
                break;
        }
    }
};

struct iVert {
    unsigned short int x, y, z;
    bool operator==(const iVert &other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};
// The specialized hash function for `unordered_map` keys
struct ivert_hash_fn {
    std::size_t operator()(const iVert &node) const {
        std::size_t h1 = std::hash<unsigned short int>()(node.x);
        std::size_t h2 = std::hash<unsigned short int>()(node.y);
        std::size_t h3 = std::hash<unsigned short int>()(node.z);

        return h1 << 32 | h2 << 16 | h3;
    }
};

static unsigned short MESH_NULL_IDX = 0xffff;

class NavTileConverter {
    dtNavMeshCreateParams _params;
    std::vector<unsigned short int> _qverts;
    std::vector<unsigned short int> _polyVerts;
    std::vector<unsigned short int> _polyVertCount;
    std::vector<unsigned short int> _polyFlags;

    std::vector<unsigned char> _polyAreas;
    // temp
    std::unordered_map<iVert, int, ivert_hash_fn> _vertMap;
    std::vector<int> _vertIndex;

   public:
    void buildF(float *verts, int totalVerts, int polyCount, int *vertCounts,  int *polyType, int *polyFlags) {
        _vertMap.clear();
        _vertIndex.resize(totalVerts);
        auto min_x = _params.bmin[0];
        auto min_y = _params.bmin[1];
        auto min_z = _params.bmin[2];

        double quantFactor = 1.0 / _params.cs;

        // collapse duplicates
        for (int i = 0; i < totalVerts; i++) {
            iVert v;
            auto idx = i * 3;
            v.x = (unsigned short int)((verts[idx] - min_x) * quantFactor) & 0xfffe;
            v.y = (unsigned short int)((verts[idx + 1] - min_y) * quantFactor) & 0xfffe;
            v.z = (unsigned short int)((verts[idx + 2] - min_z) * quantFactor) & 0xfffe;
            auto it = _vertMap.find(v);
            if (it != _vertMap.end()) {
                _vertIndex[i] = it->second;
                continue;
            }
            _vertIndex[i] = i;
            _vertMap[v] = i;
        }

        _params.vertCount = _vertMap.size();
        _params.polyCount = polyCount;

        int nvp = 3;

        for (int p = 0; p < polyCount; p++) {
            auto vertCount = vertCounts[p];
            if (vertCount > nvp) {
                nvp = vertCount;
            }
        }

        _params.nvp = nvp;

        // build compatible indices
        auto polyIdx = 0;
        _polyVertCount.resize(polyCount);
        _polyFlags.resize(polyCount);
        _polyAreas.resize(polyCount);
        _polyVerts.clear();
        _polyVerts.reserve(polyCount * nvp * 2);

        for (int p = 0; p < polyCount; p++) {
            auto vertCount = vertCounts[p];
            _polyVertCount[p] = vertCount;
            _polyFlags[p] = polyFlags[p];
            _polyAreas[p] = polyType[p];

            // store the indices in the first half
            for (int i = 0; i < vertCount; i++) {
                auto idx = _vertIndex[polyIdx + i];
                _polyVerts.push_back(idx);
            }
            // pad the remaineder
            for (int i = vertCount; i < nvp; i++) {
                _polyVerts.push_back(MESH_NULL_IDX);
            }
            // pad the second half
            for (int i = 0; i < nvp; i++) {
                _polyVerts.push_back(0);
            }
            polyIdx += vertCount;
        }

        /*
                        const unsigned short* verts;			///< The polygon mesh vertices. [(x, y, z) * #vertCount] [Unit: vx]
                int vertCount;							///< The number vertices in the polygon mesh. [Limit: >= 3]
                const unsigned short* polys;			///< The polygon data. [Size: #polyCount * 2 * #nvp]
                const unsigned short* polyFlags;		///< The user defined flags assigned to each polygon. [Size: #polyCount]
                const unsigned char* polyAreas;			///< The user defined area ids assigned to each polygon. [Size: #polyCount]
                int polyCount;							///< Number of polygons in the mesh. [Limit: >= 1]
                int nvp;								///< Number maximum number of vertices per polygon. [Limit: >= 3]

            */
    }
    void buildD(double *verts, int totalVerts, int *vertCounts, int polyCount) {
    }
    void setUnits(double gridSize, double cellHeight) {
        _params.cs = gridSize;
        _params.ch = cellHeight;
    }
    void setLocation(int x, int y, _hl_float3 *min, _hl_float3 *max) {
        _params.tileX = x;
        _params.tileY = y;
    }

    void *getTileData() {
        return nullptr;
    }
};
#endif