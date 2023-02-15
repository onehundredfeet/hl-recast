//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

// This file is heavily based on original code by Mikko Mononen

#include "TriMeshBuilder.h"
#pragma once

TriMeshBuilder::TriMeshBuilder() {
    
}
TriMeshBuilder::~TriMeshBuilder() {

}
void TriMeshBuilder::reserve(int verts, int tris) {
    this->_verts.reserve(verts * 3);
    this->_tris.reserve(tris * 3);
    this->_normals.reserve(tris * 3);
    this->_flags.reserve(verts);

}

void TriMeshBuilder::setVerts(float *verts, int count) {
    this->_verts.reserve(count * 3);
    this->_normals.reserve(count * 3);

    for (int i = 0; i < count; i++) {
        this->_verts.push_back(verts[i * 3]);
        this->_verts.push_back(verts[i * 3 + 1]);
        this->_verts.push_back(verts[i * 3 + 2]);
    }
}

void TriMeshBuilder::setTris(int *tris, int count) {
    this->_tris.reserve(count * 3);
    this->_flags.reserve(count);

    for (int i = 0; i < count; i++) {
        this->_tris.push_back(tris[i * 3]);
        this->_tris.push_back(tris[i * 3 + 1]);
        this->_tris.push_back(tris[i * 3 + 2]);

        this->_normals.push_back(0.0f);
        this->_normals.push_back(1.0f);
        this->_normals.push_back(0.0f);


        this->_flags.push_back(0);
    }
}

void TriMeshBuilder::addVertex(float x, float y, float z) {
    _verts.push_back(x);
    _verts.push_back(y);
    _verts.push_back(z);
    _vertCount++;

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

    _normals.push_back(0.0f);
    _normals.push_back(1.0f);
    _normals.push_back(0.0f);
    
    _flags.push_back(0);
    
    _triCount++;
}

void TriMeshBuilder::setFlags(int idx, unsigned short flags) {
    _flags[idx] = flags;
}

static char* parseRow(char* buf, char* bufEnd, char* row, int len)
{
	bool start = true;
	bool done = false;
	int n = 0;
	while (!done && buf < bufEnd)
	{
		char c = *buf;
		buf++;
		// multirow
		switch (c)
		{
			case '\\':
				break;
			case '\n':
				if (start) break;
				done = true;
				break;
			case '\r':
				break;
			case '\t':
			case ' ':
				if (start) break;
				// else falls through
			default:
				start = false;
				row[n++] = c;
				if (n >= len-1)
					done = true;
				break;
		}
	}
	row[n] = '\0';
	return buf;
}

static int parseFace(char* row, int* data, int n, int vcnt)
{
	int j = 0;
	while (*row != '\0')
	{
		// Skip initial white space
		while (*row != '\0' && (*row == ' ' || *row == '\t'))
			row++;
		char* s = row;
		// Find vertex delimiter and terminated the string there for conversion.
		while (*row != '\0' && *row != ' ' && *row != '\t')
		{
			if (*row == '/') *row = '\0';
			row++;
		}
		if (*s == '\0')
			continue;
		int vi = atoi(s);
		data[j++] = vi < 0 ? vi+vcnt : vi-1;
		if (j >= n) return j;
	}
	return j;
}


bool TriMeshBuilder::loadObj( const char* filename ) {
char* buf = 0;
	FILE* fp = fopen(filename, "rb");
	if (!fp)
		return false;
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		fclose(fp);
		return false;
	}
	long bufSize = ftell(fp);
	if (bufSize < 0)
	{
		fclose(fp);
		return false;
	}
	if (fseek(fp, 0, SEEK_SET) != 0)
	{
		fclose(fp);
		return false;
	}
	buf = new char[bufSize];
	if (!buf)
	{
		fclose(fp);
		return false;
	}
	size_t readLen = fread(buf, bufSize, 1, fp);
	fclose(fp);

	if (readLen != 1)
	{
		delete[] buf;
		return false;
	}

	char* src = buf;
	char* srcEnd = buf + bufSize;
	char row[512];
	int face[32];
	float x,y,z;
	int nv;
	int vcap = 0;
	int tcap = 0;
	
	while (src < srcEnd)
	{
		// Parse one row
		row[0] = '\0';
		src = parseRow(src, srcEnd, row, sizeof(row)/sizeof(char));
		// Skip comments
		if (row[0] == '#') continue;
		if (row[0] == 'v' && row[1] != 'n' && row[1] != 't')
		{
			// Vertex pos
			sscanf(row+1, "%f %f %f", &x, &y, &z);
			addVertex(x, y, z);
		}
		if (row[0] == 'f')
		{
			// Faces
			nv = parseFace(row+1, face, 32, _verts.size());
			for (int i = 2; i < nv; ++i)
			{
				const int a = face[0];
				const int b = face[i-1];
				const int c = face[i];
				if (a < 0 || a >= _verts.size() || b < 0 || b >= _verts.size() || c < 0 || c >= _verts.size())
					continue;
				addTriangle(a, b, c);
			}
		}
	}

	delete [] buf;

	// Calculate normals.
	for (int i = 0; i < _tris.size(); i += 3)
	{
		const float* v0 = &_verts[_tris[i]*3];
		const float* v1 = &_verts[_tris[i+1]*3];
		const float* v2 = &_verts[_tris[i+2]*3];
		float e0[3], e1[3];
		for (int j = 0; j < 3; ++j)
		{
			e0[j] = v1[j] - v0[j];
			e1[j] = v2[j] - v0[j];
		}
		float* n = &_normals[i];
		n[0] = e0[1]*e1[2] - e0[2]*e1[1];
		n[1] = e0[2]*e1[0] - e0[0]*e1[2];
		n[2] = e0[0]*e1[1] - e0[1]*e1[0];
		float d = sqrtf(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
		if (d > 0)
		{
			d = 1.0f/d;
			n[0] *= d;
			n[1] *= d;
			n[2] *= d;
		}
	}
	
	return true;
}
