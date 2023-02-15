#include "TriMeshBuilder.h"
#pragma once

TriMeshBuilder::TriMeshBuilder() {
    
}
TriMeshBuilder::~TriMeshBuilder() {

}
void TriMeshBuilder::reserve(int verts, int tris) {
    this->_verts.reserve(verts * 3);
    this->_normals.reserve(verts * 3);
    this->_tris.reserve(tris * 3);
    this->_flags.reserve(verts);

}

void TriMeshBuilder::setVerts(float *verts, int count) {
    this->_verts.reserve(count * 3);
    this->_normals.reserve(count * 3);

    for (int i = 0; i < count; i++) {
        this->_verts.push_back(verts[i * 3]);
        this->_verts.push_back(verts[i * 3 + 1]);
        this->_verts.push_back(verts[i * 3 + 2]);

        this->_normals.push_back(0.0f);
        this->_normals.push_back(1.0f);
        this->_normals.push_back(0.0f);
    }
}

void TriMeshBuilder::setTris(int *tris, int count) {
    this->_tris.reserve(count * 3);
    this->_flags.reserve(count);

    for (int i = 0; i < count; i++) {
        this->_tris.push_back(tris[i * 3]);
        this->_tris.push_back(tris[i * 3 + 1]);
        this->_tris.push_back(tris[i * 3 + 2]);

        this->_flags.push_back(0);
    }
}

void TriMeshBuilder::addVertex(float x, float y, float z) {
    _verts.push_back(x);
    _verts.push_back(y);
    _verts.push_back(z);

    _normals.push_back(0.0f);
    _normals.push_back(1.0f);
    _normals.push_back(0.0f);
}
void TriMeshBuilder::setNormal(int idx, float x, float y, float z) {
    _normals[idx] = x;
    _normals[idx + 1] = y;
    _normals[idx + 2] = z;
}

void TriMeshBuilder::addTriangle(int a, int b, int c) {
    _tris.push_back(a);
    _tris.push_back(b);
    _tris.push_back(c);

    _flags.push_back(0);
}

void TriMeshBuilder::setFlags(int idx, unsigned short flags) {
    _flags[idx] = flags;
}