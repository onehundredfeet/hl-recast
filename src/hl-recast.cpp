#include "hl-recast.h"
#include <DetourDebugDraw.h>
#include <RecastDebugDraw.h>

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
