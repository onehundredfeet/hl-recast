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

#ifndef _TRI_MESH_PARTITION_H_
#define _TRI_MESH_PARTITION_H_
#pragma once
struct TriMeshPartitionNode
{
	float bmin[2];
	float bmax[2];
	int i;
	int n;
};

struct TriMeshPartition
{
	inline TriMeshPartition() : nodes(0), nnodes(0), tris(0), ntris(0), maxTrisPerChunk(0) {};
	inline ~TriMeshPartition() { delete [] nodes; delete [] tris; }

	TriMeshPartitionNode* nodes;
	int nnodes;
	int* tris;
	unsigned char *areas;
	int ntris;
	int maxTrisPerChunk;

	/// Creates partitioned triangle mesh (AABB tree),
	/// where each node contains at max trisPerChunk triangles.
	bool partition(const float* verts, const int* tris, int ntris, int trisPerChunk);

	/// Returns the chunk indices which overlap the input rectable.
	int getChunksOverlappingRect(float bmin[2], float bmax[2], int* ids, const int maxIds);

	/// Returns the chunk indices which overlap the input segment.
	int getChunksOverlappingSegment(float p[2], float q[2], int* ids, const int maxIds);


private:
	// Explicitly disabled copy constructor and copy assignment operator.
	TriMeshPartition(const TriMeshPartition&);
	TriMeshPartition& operator=(const TriMeshPartition&);
};

#include "TriMeshPartition.cpp"
#endif // CHUNKYTRIMESH_H
