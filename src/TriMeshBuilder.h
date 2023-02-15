#ifndef __TRI_MESH_BUILDER_H_
#define __TRI_MESH_BUILDER_H_

#include <vector>
#pragma once

class TriMeshBuilder
{
public:
	TriMeshBuilder();
	~TriMeshBuilder();

	const float* getVerts() const { return &_verts[0]; }
	const float* getNormals() const { return &_normals[0]; }
	const int* getTris() const { return &_tris[0]; }
	int getVertCount() const { return _vertCount; }
	int getTriCount() const { return _triCount; }

    void reserve(int verts, int tris);
	
    void addVertex(float x, float y, float z);
    
    void addTriangle(int a, int b, int c);
    void setNormal(int tidx, float x, float y, float z);
    void setFlags(int tidx, unsigned short flags);
	
    // bulk
    void setVerts(float *verts, int count);
    void setTris(int *tris, int count);

    bool loadObj( const char* filename );
private:
	// Explicitly disabled copy constructor and copy assignment operator.
	TriMeshBuilder(const TriMeshBuilder&);
	TriMeshBuilder& operator=(const TriMeshBuilder&);

	float _scale;	
	std::vector<float> _verts;
	std::vector<int> _tris;
	std::vector<float> _normals;
	std::vector<unsigned short> _flags;
	int _vertCount;
	int _triCount;
};
#include "TriMeshBuilder.cpp"
#endif