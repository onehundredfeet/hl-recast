#ifndef __H_RECAST_H_
#define __H_RECAST_H_

// #include <DebugDraw.h>
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
#include <vector>


#include "fastlz/fastlz.h"
#include "TriMeshBuilder.h"
#include "TriMeshPartition.h"

#include "hl-idl-helpers.hpp"
#include "NavVerts.h"
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

inline int getNodeTriIndex(TriMeshPartition *_this, int nodeIdx) {
    return _this->nodes[nodeIdx].i;
}

inline int getNodeTriCount(TriMeshPartition *_this, int nodeIdx) {
    return _this->nodes[nodeIdx].n;
}

inline int *getTriVertIndices(TriMeshPartition *_this, int triIndex) {
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

/*
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
*/


static unsigned short MESH_NULL_IDX = 0xffff;

class NavBuffer {
    unsigned char *_ptr;
    int _size;

   public:
    inline unsigned char *ptr() { return _ptr; }
    inline int size() { return _size; }
    NavBuffer(unsigned char *ptr, int size) : _ptr(ptr), _size(size) {}
    ~NavBuffer() {
        if (_ptr != nullptr)
            dtFree(_ptr);
    }
};

class NavMesh : public dtNavMesh {
   public:
    void create(_h_float3 *origin, float tileWidth, float tileHeight, int maxTiles, int maxPolys) {
        dtNavMeshParams params;
        params.orig[0] = origin->x;
        params.orig[1] = origin->y;
        params.orig[2] = origin->z;
        params.tileWidth = tileWidth;
        params.tileHeight = tileHeight;
        params.maxTiles = maxTiles;
        params.maxPolys = maxPolys;

        init(&params);
    }
    dtStatus load(NavBuffer *data, int flags) {
        return init(data->ptr(), data->size(), flags);
    }
    dtTileRef addTile(NavBuffer *tileData, int flags) {
        dtTileRef outRef = 0;
        dtNavMesh::addTile(tileData->ptr(), tileData->size(), flags | DT_COMPRESSEDTILE_FREE_DATA, 0, &outRef);
        return outRef;
    }
    void replaceTile(NavBuffer *tileData, dtTileRef lastRef, int flags) {
        dtNavMesh::addTile(tileData->ptr(), tileData->size(), flags, lastRef | DT_COMPRESSEDTILE_FREE_DATA, nullptr);
    }
    dtStatus removeTile(dtTileRef ref) {
        return DT_SUCCESS;
    }

    dtNavMeshQuery *createQuery(int maxNodes) {
        auto *query = new dtNavMeshQuery();
        query->init(this, maxNodes);
        return query;
    }
};

class NavTileConverter {
    dtNavMeshCreateParams _params;
    std::vector<unsigned short int> _qverts;
    std::vector<unsigned short int> _polyVerts;
    std::vector<unsigned short int> _polyFlags;

    std::vector<unsigned char> _polyAreas;
    // temp
    std::unordered_map<iVert, int, ivert_hash_fn> _vertMap;
    std::vector<int> _vertIndex;

   public:
    NavTileConverter() {
        reset();
    }
    void reset() {
        _params.buildBvTree = false;

        _params.detailMeshes = nullptr;
        _params.detailVerts = nullptr;
        _params.detailVertsCount = 0;
        _params.detailTris = nullptr;
        _params.detailTriCount = 0;

        _params.walkableHeight = 1.;  ///< The agent height. [Unit: wu]
        _params.walkableRadius = 1.;  ///< The agent radius. [Unit: wu]
        _params.walkableClimb = 1.;   ///< The agent maximum t

        _params.tileX = 0;
        _params.tileY = 0;
        _params.tileLayer = 0;
        _params.nvp = 3;

        _params.offMeshConVerts = nullptr;
        _params.offMeshConAreas = nullptr;
        _params.offMeshConDir = nullptr;
        _params.offMeshConRad = nullptr;
        _params.offMeshConUserID = nullptr;
        _params.offMeshConCount = 0;

        _params.userId = 0;
        _params.cs = 1.0;
        _params.ch = 1.0;
    }
    void buildF(float *verts, int totalVerts, int polyCount, int *vertCounts, int *polyType, int *polyFlags) {
        _vertMap.clear();
        _vertIndex.resize(totalVerts);
        _qverts.clear();
        _qverts.reserve(totalVerts);
        auto min_x = _params.bmin[0];
        auto min_y = _params.bmin[1];
        auto min_z = _params.bmin[2];

        double quantFactor = 1.0 / _params.cs;

        // collapse duplicates
        int writeHead = 0;
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
            auto vidx = writeHead++;
            _vertIndex[i] = vidx;
            _vertMap[v] = vidx;
            _qverts.push_back(v.x);
            _qverts.push_back(v.y);
            _qverts.push_back(v.z);
        }

        if (totalVerts > 0)
            _params.verts = &_qverts[0];
        else
            _params.verts = nullptr;

        _params.vertCount = (int)_vertMap.size();
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
        _polyFlags.resize(polyCount);
        _polyAreas.resize(polyCount);
        _polyVerts.clear();
        _polyVerts.reserve(polyCount * nvp * 2);

        for (int p = 0; p < polyCount; p++) {
            auto vertCount = vertCounts[p];
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

        _params.polys = &_polyVerts[0];
        _params.polyAreas = &_polyAreas[0];
        _params.polyFlags = &_polyFlags[0];
    }
    void buildD(double *verts, int totalVerts, int *vertCounts, int polyCount) {
    }
    void setUnits(double gridSize, double cellHeight) {
        _params.cs = gridSize;
        _params.ch = cellHeight;
    }
    void setLocation(int x, int y, int layer, _h_float3 *min, _h_float3 *max) {
        _params.tileX = x;
        _params.tileY = y;
        _params.tileLayer = layer;

        _params.bmin[0] = min->x;
        _params.bmin[1] = min->y;
        _params.bmin[2] = min->z;

        _params.bmax[0] = max->x;
        _params.bmax[1] = max->y;
        _params.bmax[2] = max->z;
    }

    void setWalkability(float walkableHeight, float walkableRadius, float walkableClimb) {
        _params.walkableHeight = walkableHeight;  ///< The agent height. [Unit: wu]
        _params.walkableRadius = walkableRadius;  ///< The agent radius. [Unit: wu]
        _params.walkableClimb = walkableClimb;    ///< The agent maximum traversable ledge. (Up/Down) [Unit: wu]
    }
    NavBuffer *getTileData() {
        _params.buildBvTree = false;

        _params.detailMeshes = nullptr;
        _params.detailVerts = nullptr;
        _params.detailVertsCount = 0;
        _params.detailTris = nullptr;
        _params.detailTriCount = 0;

        unsigned char *data = nullptr;
        int dataSize = 0;
        if (dtCreateNavMeshData(&_params, &data, &dataSize)) {
            return new NavBuffer(data, dataSize);
        }

        return nullptr;
    }
};

#include "NavWorld.h"
#include "PerformanceTimer.h"
/*
class InputGeom
{

	bool loadMesh(class rcContext* ctx, const std::string& filepath);
	bool loadGeomSet(class rcContext* ctx, const std::string& filepath);
public:
	InputGeom();
	~InputGeom();
	
	
	bool load(class rcContext* ctx, const std::string& filepath);
	bool saveGeomSet(const BuildSettings* settings);
	
	/// Method to return static mesh data.
	const TriMeshBuilder* getMesh() const { return m_mesh; }
	const float* getMeshBoundsMin() const { return m_meshBMin; }
	const float* getMeshBoundsMax() const { return m_meshBMax; }
	const float* getNavMeshBoundsMin() const { return m_hasBuildSettings ? m_buildSettings.navMeshBMin : m_meshBMin; }
	const float* getNavMeshBoundsMax() const { return m_hasBuildSettings ? m_buildSettings.navMeshBMax : m_meshBMax; }
	const TriMeshPartition* getChunkyMesh() const { return m_chunkyMesh; }
	const BuildSettings* getBuildSettings() const { return m_hasBuildSettings ? &m_buildSettings : 0; }
	bool raycastMesh(float* src, float* dst, float& tmin);

	/// @name Off-Mesh connections.
	///@{
	int getOffMeshConnectionCount() const { return m_offMeshConCount; }
	const float* getOffMeshConnectionVerts() const { return m_offMeshConVerts; }
	const float* getOffMeshConnectionRads() const { return m_offMeshConRads; }
	const unsigned char* getOffMeshConnectionDirs() const { return m_offMeshConDirs; }
	const unsigned char* getOffMeshConnectionAreas() const { return m_offMeshConAreas; }
	const unsigned short* getOffMeshConnectionFlags() const { return m_offMeshConFlags; }
	const unsigned int* getOffMeshConnectionId() const { return m_offMeshConId; }
	void addOffMeshConnection(const float* spos, const float* epos, const float rad,
							  unsigned char bidir, unsigned char area, unsigned short flags);
	void deleteOffMeshConnection(int i);
	void drawOffMeshConnections(struct duDebugDraw* dd, bool hilight = false);
	///@}

	/// @name Box Volumes.
	///@{
	int getConvexVolumeCount() const { return m_volumeCount; }
	const ConvexVolume* getConvexVolumes() const { return m_volumes; }
	void addConvexVolume(const float* verts, const int nverts,
						 const float minh, const float maxh, unsigned char area);
	void deleteConvexVolume(int i);
	void drawConvexVolumes(struct duDebugDraw* dd, bool hilight = false);
	///@}
	
private:
	// Explicitly disabled copy constructor and copy assignment operator.
	InputGeom(const InputGeom&);
	InputGeom& operator=(const InputGeom&);
};
*/

/*
	TriMeshPartition* m_chunkyMesh;
	TriMeshBuilder* m_mesh;
	float m_meshBMin[3], m_meshBMax[3];
	BuildSettings m_buildSettings;
	bool m_hasBuildSettings;
	
	/// @name Off-Mesh connections.
	///@{
	static const int MAX_OFFMESH_CONNECTIONS = 256;
	float m_offMeshConVerts[MAX_OFFMESH_CONNECTIONS*3*2];
	float m_offMeshConRads[MAX_OFFMESH_CONNECTIONS];
	unsigned char m_offMeshConDirs[MAX_OFFMESH_CONNECTIONS];
	unsigned char m_offMeshConAreas[MAX_OFFMESH_CONNECTIONS];
	unsigned short m_offMeshConFlags[MAX_OFFMESH_CONNECTIONS];
	unsigned int m_offMeshConId[MAX_OFFMESH_CONNECTIONS];
	int m_offMeshConCount;
	///@}

	/// @name Convex Volumes.
	///@{
	static const int MAX_VOLUMES = 256;
	ConvexVolume m_volumes[MAX_VOLUMES];
	int m_volumeCount;
	///@}
	
*/

/*
void dtMeshCapture::captureNavMesh(const dtNavMesh &nm, unsigned short flags) {
    duDebugDrawNavMeshPolysWithFlags(this, nm, flags, 0xffffffff);
}

void dtMeshCapture::captureHeightField(const rcHeightfield &hf) {
    duDebugDrawHeightfieldSolid(this, hf);
}

void dtMeshCapture::captureCompactHeightField(const rcCompactHeightfield &chf) {
    duDebugDrawCompactHeightfieldSolid(this, chf);
}

void dtMeshCapture::captureHeighfieldLayerSet(const rcHeightfieldLayerSet &hfls) {
    duDebugDrawHeightfieldLayers(this, hfls);
}

/// Begin drawing primitives.
///  @param prim [in] primitive type to draw, one of rcDebugDrawPrimitives.
///  @param size [in] size of a primitive, applies to point size and line width
///  only.
void dtMeshCapture::begin(duDebugDrawPrimitives prim, float size) {
    _currentMode = prim;
    _prim = 0;
    _lineWidth = size;
}

/// Submit a vertex
///  @param pos [in] position of the verts.
///  @param color [in] color of the verts.
void dtMeshCapture::vertex(const float *pos, unsigned int color) {
    _lastVerts[_prim++] = float3(pos[0], pos[1], pos[2]);
    finishPrim();
}

/// Submit a vertex
///  @param x,y,z [in] position of the verts.
///  @param color [in] color of the verts.
void dtMeshCapture::vertex(const float x, const float y, const float z, unsigned int color) {
    _lastVerts[_prim++] = float3(x, y, z);
    finishPrim();
}

/// Submit a vertex
///  @param pos [in] position of the verts.
///  @param color [in] color of the verts.
void dtMeshCapture::vertex(const float *pos, unsigned int color, const float *uv) {
    _lastVerts[_prim++] = float3(pos[0], pos[1], pos[2]);
    finishPrim();
}

/// Submit a vertex
///  @param x,y,z [in] position of the verts.
///  @param color [in] color of the verts.
void dtMeshCapture::vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v) {
    _lastVerts[_prim++] = float3(x, y, z);
    finishPrim();
}

/// End drawing primitives.
void dtMeshCapture::end() {}
*/

#endif
