#include "NavWorld.h"
#pragma once

NavWorld *NavWorld::create(_h_float3 *origin, _h_float3 *dim, int tileSizeInCells, float cellSize, float cellHeight, int maxTiles, int maxPolys) {
    auto *x = new NavWorld();
    x->_tileSize = tileSizeInCells * cellSize;
    x->_cellSize = cellSize;
    x->_cellHeight = cellHeight;
    x->_tileSizeInCells = tileSizeInCells;

    x->_origin[0] = origin->x;
    x->_origin[1] = origin->y;
    x->_origin[2] = origin->z;

    x->_bmin[0] = origin->x;
    x->_bmin[1] = origin->y;
    x->_bmin[2] = origin->z;

    x->_bmax[0] = origin->x + dim->x;
    x->_bmax[1] = origin->y + dim->y;
    x->_bmax[2] = origin->z + dim->z;

    dtNavMeshParams params;
    params.orig[0] = origin->x;
    params.orig[1] = origin->y;
    params.orig[2] = origin->z;
    params.tileWidth = x->_tileSize;
    params.tileHeight = x->_tileSize;
    params.maxTiles = maxTiles;
    params.maxPolys = maxPolys;
    x->_navMesh.init(&params);
    return x;
}

void NavWorld::TileBuilder::bind(int x, int y) {
    _x = x;
    _y = y;
    auto borderSize = ceil(_world->_walkableRadius / _world->_cellSize);

    _tileCacheData.clear();

    if (borderSize != _borderSize) {
        _borderSize = borderSize;
        dispose();
        auto width = _world->_tileSizeInCells + _borderSize * 2;
        auto height = _world->_tileSizeInCells + _borderSize * 2;

        if (!rcCreateHeightfield(&_context, _solid, width, height, _bmin, _bmax, _world->_cellSize, _world->_cellHeight)) {
            _context.log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
        }
    }

    _bmin[0] = _world->_bmin[0] + x * _world->_tileSize;
    _bmin[1] = _world->_bmin[1];
    _bmin[2] = _world->_bmin[2] + y * _world->_tileSize;
    _bmax[0] = _world->_bmin[0] + (x + 1) * _world->_tileSize;
    _bmax[1] = _world->_bmax[1];
    _bmax[2] = _world->_bmin[2] + (y + 1) * _world->_tileSize;
    _bmin[0] -= borderSize * _world->_cellSize;
    _bmin[2] -= borderSize * _world->_cellSize;
    _bmax[0] += borderSize * _world->_cellSize;
    _bmax[2] += borderSize * _world->_cellSize;

    _triAreas.resize(_world->maxTrisPerChunk());

    // TODO CALCULATE MIN AND MAX
    rcVcopy(_solid.bmin, _bmax);
    rcVcopy(_solid.bmax, _bmin);
}

bool NavWorld::TileBuilder::buildTileColumnCacheData() {
    const int tx = _x;
    const int ty = _y;

    //    FastLZCompressor comp;
    //  RasterizationContext rc;
    // Tile bounds.
    const float tcs = _world->_tileSize;

    int totalChunks = 0;

    for (int i = 0; i < _world->_chunks.size(); ++i) {
        auto &chunk = *_world->_chunks[i];
        const float *verts = chunk.getVerts();
        const int nverts = chunk.getVertCount();
        const TriMeshPartition &partition = chunk.partition();

        float tbmin[2], tbmax[2];
        tbmin[0] = _bmin[0];
        tbmin[1] = _bmin[2];
        tbmax[0] = _bmax[0];
        tbmax[1] = _bmax[2];

        // If you have multiple meshes you need to process, allocate
        // and array which can hold the max number of triangles you need to process.
        const int ncid = chunk.partition().getChunksOverlappingRect(tbmin, tbmax, &_chunkIds[0], _chunkIds.size());
        totalChunks += ncid;
        if (!ncid) {
            continue;
        }

        // Rasterize chunks
        for (int c = 0; c < ncid; ++c) {
            const TriMeshPartitionNode &node = chunk.partition().nodes[_chunkIds[c]];
            const int *tris = &chunk.partition().tris[node.i * 3];
            const int ntris = node.n;

            memset(&_triAreas[0], 0, ntris * sizeof(unsigned char));
            rcMarkWalkableTriangles(&_context, _world->_walkableSlopeAngle,
                                    verts, nverts, tris, ntris, &_triAreas[0]);

            if (!rcRasterizeTriangles(&_context, verts, nverts, tris, &_triAreas[0], ntris, _solid, _world->_walkableClimb))
                return false;
        }
    }

    if (totalChunks == 0) {
        return true;
    }
    // Once all geometry is rasterized, we do initial pass of filtering to
    // remove unwanted overhangs caused by the conservative rasterization
    // as well as filter spans where the character cannot possibly stand.
    if (_world->_filterLowHangingObstacles)
        rcFilterLowHangingWalkableObstacles(&_context, _world->_walkableClimb, _solid);
    if (_world->_filterLedgeSpans)
        rcFilterLedgeSpans(&_context, _world->_walkableHeight, _world->_walkableClimb, _solid);
    if (_world->_filterWalkableLowHeightSpans)
        rcFilterWalkableLowHeightSpans(&_context, _world->_walkableHeight, _solid);

    if (!rcBuildCompactHeightfield(&_context, _world->_walkableHeight, _world->_walkableClimb, _solid, _chf)) {
        _context.log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
        return false;
    }

    // Erode the walkable area by agent radius.
    if (!rcErodeWalkableArea(&_context, _world->_walkableRadius, _chf)) {
        _context.log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
        return false;
    }

    // (Optional) Mark areas.
    for (int i = 0; i < _world->_convexVolumes.size(); ++i) {
        const ConvexVolume &v = _world->_convexVolumes[i];
        rcMarkConvexPolyArea(&_context, v.verts, v.nverts,
                             v.hmin, v.hmax,
                             (unsigned char)v.area, _chf);
    }

    if (!rcBuildHeightfieldLayers(&_context, _chf, _borderSize, _world->_walkableHeight, _lset)) {
        _context.log(RC_LOG_ERROR, "buildNavigation: Could not build heighfield layers.");
        return 0;
    }
    
    for (int i = 0; i < _lset.nlayers; ++i) {
        _tileCacheData.push_back({});
        TileCacheData &tile = _tileCacheData.back();
        const rcHeightfieldLayer *layer = &_lset.layers[i];

        // Store header
        dtTileCacheLayerHeader header;
        header.magic = DT_TILECACHE_MAGIC;
        header.version = DT_TILECACHE_VERSION;

        // Tile layer location in the navmesh.
        header.tx = tx;
        header.ty = ty;
        header.tlayer = i;
        dtVcopy(header.bmin, layer->bmin);
        dtVcopy(header.bmax, layer->bmax);

        // Tile info.
        header.width = (unsigned char)layer->width;
        header.height = (unsigned char)layer->height;
        header.minx = (unsigned char)layer->minx;
        header.maxx = (unsigned char)layer->maxx;
        header.miny = (unsigned char)layer->miny;
        header.maxy = (unsigned char)layer->maxy;
        header.hmin = (unsigned short)layer->hmin;
        header.hmax = (unsigned short)layer->hmax;

        dtStatus status = dtBuildTileCacheLayer(&_tcomp, &header, layer->heights, layer->areas, layer->cons,
                                                &tile.data, &tile.dataSize);
        if (dtStatusFailed(status)) {
            return false;
        }
    }


    return true;
}
 /*
bool NavWorld::TileBuilder::build(int &dataSize) {
   

    _tileMemUsage = 0;
    _tileBuildTime = 0;

    cleanup();

    const float *verts = _geom->getMesh()->getVerts();
    const int nverts = _geom->getMesh()->getVertCount();
    const int ntris = _geom->getMesh()->getTriCount();
    const TriMeshPartition *chunkyMesh = _geom->getChunkyMesh();

    // Init build configuration from GUI
    memset(&_cfg, 0, sizeof(_cfg));
    _cfg.cs = _cellSize;
    _cfg.ch = _cellHeight;
    _cfg.walkableSlopeAngle = _agentMaxSlope;
    _cfg.walkableHeight = (int)ceilf(_agentHeight / _cfg.ch);
    _cfg.walkableClimb = (int)floorf(_agentMaxClimb / _cfg.ch);
    _cfg.walkableRadius = (int)ceilf(_agentRadius / _cfg.cs);
    _cfg.maxEdgeLen = (int)(_edgeMaxLen / _cellSize);
    _cfg.maxSimplificationError = _edgeMaxError;
    _cfg.minRegionArea = (int)rcSqr(_regionMinSize);      // Note: area = size*size
    _cfg.mergeRegionArea = (int)rcSqr(_regionMergeSize);  // Note: area = size*size
    _cfg.maxVertsPerPoly = (int)_vertsPerPoly;
    _cfg.tileSize = (int)_tileSize;
    _cfg.borderSize = _cfg.walkableRadius + 3;  // Reserve enough padding.
    _cfg.width = _cfg.tileSize + _cfg.borderSize * 2;
    _cfg.height = _cfg.tileSize + _cfg.borderSize * 2;
    _cfg.detailSampleDist = _detailSampleDist < 0.9f ? 0 : _cellSize * _detailSampleDist;
    _cfg.detailSampleMaxError = _cellHeight * _detailSampleMaxError;

    // Expand the heighfield bounding box by border size to find the extents of geometry we need to build this tile.
    //
    // This is done in order to make sure that the navmesh tiles connect correctly at the borders,
    // and the obstacles close to the border work correctly with the dilation process.
    // No polygons (or contours) will be created on the border area.
    //
    // IMPORTANT!
    //
    //   :''''''''':
    //   : +-----+ :
    //   : |     | :
    //   : |     |<--- tile to build
    //   : |     | :
    //   : +-----+ :<-- geometry needed
    //   :.........:
    //
    // You should use this bounding box to query your input geometry.
    //
    // For example if you build a navmesh for terrain, and want the navmesh tiles to match the terrain tile size
    // you will need to pass in data from neighbour terrain tiles too! In a simple case, just pass in all the 8 neighbours,
    // or use the bounding box below to only pass in a sliver of each of the 8 neighbours.
    rcVcopy(_cfg.bmin, bmin);
    rcVcopy(m_cfg.bmax, bmax);
    _cfg.bmin[0] -= _cfg.borderSize * _cfg.cs;
    _cfg.bmin[2] -= _cfg.borderSize * _cfg.cs;
    _cfg.bmax[0] += _cfg.borderSize * _cfg.cs;
    _cfg.bmax[2] += _cfg.borderSize * _cfg.cs;

    // Reset build times gathering.
    _ctx->resetTimers();

    // Start the build process.
    _ctx->startTimer(RC_TIMER_TOTAL);

    _ctx->log(RC_LOG_PROGRESS, "Building navigation:");
    _ctx->log(RC_LOG_PROGRESS, " - %d x %d cells", _cfg.width, _cfg.height);
    _ctx->log(RC_LOG_PROGRESS, " - %.1fK verts, %.1fK tris", nverts / 1000.0f, ntris / 1000.0f);

    // Allocate voxel heightfield where we rasterize our input data to.
    _solid = rcAllocHeightfield();
    if (!_solid) {
        _ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
        return 0;
    }
    if (!rcCreateHeightfield(_ctx, *_solid, _cfg.width, _cfg.height, _cfg.bmin, _cfg.bmax, _cfg.cs, _cfg.ch)) {
        _ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
        return 0;
    }

    // Allocate array that can hold triangle flags.
    // If you have multiple meshes you need to process, allocate
    // and array which can hold the max number of triangles you need to process.
    _triareas = new unsigned char[chunkyMesh->maxTrisPerChunk];
    if (!_triareas) {
        _ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory '_triareas' (%d).", chunkyMesh->maxTrisPerChunk);
        return 0;
    }

    float tbmin[2], tbmax[2];
    tbmin[0] = _cfg.bmin[0];
    tbmin[1] = _cfg.bmin[2];
    tbmax[0] = _cfg.bmax[0];
    tbmax[1] = _cfg.bmax[2];
    int cid[512];  // TODO: Make grow when returning too many items.
    const int ncid = rcGetChunksOverlappingRect(chunkyMesh, tbmin, tbmax, cid, 512);
    if (!ncid)
        return 0;

    _tileTriCount = 0;

    for (int i = 0; i < ncid; ++i) {
        const TriMeshPartitionNode &node = chunkyMesh->nodes[cid[i]];
        const int *ctris = &chunkyMesh->tris[node.i * 3];
        const int nctris = node.n;

        _tileTriCount += nctris;

        memset(_triareas, 0, nctris * sizeof(unsigned char));
        rcMarkWalkableTriangles(_ctx, _cfg.walkableSlopeAngle,
                                verts, nverts, ctris, nctris, _triareas);

        if (!rcRasterizeTriangles(_ctx, verts, nverts, ctris, _triareas, nctris, *_solid, _cfg.walkableClimb))
            return 0;
    }

    if (!_keepInterResults) {
        delete[] _triareas;
        _triareas = 0;
    }

    // Once all geometry is rasterized, we do initial pass of filtering to
    // remove unwanted overhangs caused by the conservative rasterization
    // as well as filter spans where the character cannot possibly stand.
    if (_filterLowHangingObstacles)
        rcFilterLowHangingWalkableObstacles(_ctx, _cfg.walkableClimb, *_solid);
    if (_filterLedgeSpans)
        rcFilterLedgeSpans(_ctx, _cfg.walkableHeight, _cfg.walkableClimb, *_solid);
    if (_filterWalkableLowHeightSpans)
        rcFilterWalkableLowHeightSpans(_ctx, _cfg.walkableHeight, *_solid);

    // Compact the heightfield so that it is faster to handle from now on.
    // This will result more cache coherent data as well as the neighbours
    // between walkable cells will be calculated.
    _chf = rcAllocCompactHeightfield();
    if (!_chf) {
        _ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
        return 0;
    }
    if (!rcBuildCompactHeightfield(_ctx, _cfg.walkableHeight, _cfg.walkableClimb, *_solid, *_chf)) {
        _ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
        return 0;
    }

    if (!_keepInterResults) {
        rcFreeHeightField(_solid);
        _solid = 0;
    }

    // Erode the walkable area by agent radius.
    if (!rcErodeWalkableArea(_ctx, _cfg.walkableRadius, *_chf)) {
        _ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
        return 0;
    }

    // (Optional) Mark areas.
    const ConvexVolume *vols = _geom->getConvexVolumes();
    for (int i = 0; i < _geom->getConvexVolumeCount(); ++i)
        rcMarkConvexPolyArea(_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *_chf);

    // Partition the heightfield so that we can use simple algorithm later to triangulate the walkable areas.
    // There are 3 martitioning methods, each with some pros and cons:
    // 1) Watershed partitioning
    //   - the classic Recast partitioning
    //   - creates the nicest tessellation
    //   - usually slowest
    //   - partitions the heightfield into nice regions without holes or overlaps
    //   - the are some corner cases where this method creates produces holes and overlaps
    //      - holes may appear when a small obstacles is close to large open area (triangulation can handle this)
    //      - overlaps may occur if you have narrow spiral corridors (i.e stairs), this make triangulation to fail
    //   * generally the best choice if you precompute the nacmesh, use this if you have large open areas
    // 2) Monotone partioning
    //   - fastest
    //   - partitions the heightfield into regions without holes and overlaps (guaranteed)
    //   - creates long thin polygons, which sometimes causes paths with detours
    //   * use this if you want fast navmesh generation
    // 3) Layer partitoining
    //   - quite fast
    //   - partitions the heighfield into non-overlapping regions
    //   - relies on the triangulation code to cope with holes (thus slower than monotone partitioning)
    //   - produces better triangles than monotone partitioning
    //   - does not have the corner cases of watershed partitioning
    //   - can be slow and create a bit ugly tessellation (still better than monotone)
    //     if you have large open areas with small obstacles (not a problem if you use tiles)
    //   * good choice to use for tiled navmesh with medium and small sized tiles

    if (_partitionType == SAMPLE_PARTITION_WATERSHED) {
        // Prepare for region partitioning, by calculating distance field along the walkable surface.
        if (!rcBuildDistanceField(_ctx, *_chf)) {
            _ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build distance field.");
            return 0;
        }

        // Partition the walkable surface into simple regions without holes.
        if (!rcBuildRegions(_ctx, *_chf, _cfg.borderSize, _cfg.minRegionArea, _cfg.mergeRegionArea)) {
            _ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build watershed regions.");
            return 0;
        }
    } else if (_partitionType == SAMPLE_PARTITION_MONOTONE) {
        // Partition the walkable surface into simple regions without holes.
        // Monotone partitioning does not need distancefield.
        if (!rcBuildRegionsMonotone(_ctx, *_chf, _cfg.borderSize, _cfg.minRegionArea, _cfg.mergeRegionArea)) {
            _ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build monotone regions.");
            return 0;
        }
    } else  // SAMPLE_PARTITION_LAYERS
    {
        // Partition the walkable surface into simple regions without holes.
        if (!rcBuildLayerRegions(_ctx, *_chf, _cfg.borderSize, _cfg.minRegionArea)) {
            _ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build layer regions.");
            return 0;
        }
    }

    // Create contours.
    _cset = rcAllocContourSet();
    if (!_cset) {
        _ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'cset'.");
        return 0;
    }
    if (!rcBuildContours(_ctx, *_chf, _cfg.maxSimplificationError, _cfg.maxEdgeLen, *_cset)) {
        _ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create contours.");
        return 0;
    }

    if (_cset->nconts == 0) {
        return 0;
    }

    // Build polygon navmesh from the contours.
    _pmesh = rcAllocPolyMesh();
    if (!_pmesh) {
        _ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmesh'.");
        return 0;
    }
    if (!rcBuildPolyMesh(_ctx, *_cset, _cfg.maxVertsPerPoly, *_pmesh)) {
        _ctx->log(RC_LOG_ERROR, "buildNavigation: Could not triangulate contours.");
        return 0;
    }

    // Build detail mesh.
    _dmesh = rcAllocPolyMeshDetail();
    if (!_dmesh) {
        _ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'dmesh'.");
        return 0;
    }

    if (!rcBuildPolyMeshDetail(_ctx, *_pmesh, *_chf,
                               _cfg.detailSampleDist, _cfg.detailSampleMaxError,
                               *_dmesh)) {
        _ctx->log(RC_LOG_ERROR, "buildNavigation: Could build polymesh detail.");
        return 0;
    }

    if (!_keepInterResults) {
        rcFreeCompactHeightfield(_chf);
        _chf = 0;
        rcFreeContourSet(_cset);
        _cset = 0;
    }

    unsigned char *navData = 0;
    int navDataSize = 0;
    if (_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON) {
        if (_pmesh->nverts >= 0xffff) {
            // The vertex indices are ushorts, and cannot point to more than 0xffff vertices.
            _ctx->log(RC_LOG_ERROR, "Too many vertices per tile %d (max: %d).", _pmesh->nverts, 0xffff);
            return 0;
        }

        // Update poly flags from areas.
        for (int i = 0; i < _pmesh->npolys; ++i) {
            if (_pmesh->areas[i] == RC_WALKABLE_AREA)
                _pmesh->areas[i] = SAMPLE_POLYAREA_GROUND;

            if (_pmesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
                _pmesh->areas[i] == SAMPLE_POLYAREA_GRASS ||
                _pmesh->areas[i] == SAMPLE_POLYAREA_ROAD) {
                _pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
            } else if (_pmesh->areas[i] == SAMPLE_POLYAREA_WATER) {
                _pmesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
            } else if (_pmesh->areas[i] == SAMPLE_POLYAREA_DOOR) {
                _pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
            }
        }

        dtNavMeshCreateParams params;
        memset(&params, 0, sizeof(params));
        params.verts = _pmesh->verts;
        params.vertCount = _pmesh->nverts;
        params.polys = _pmesh->polys;
        params.polyAreas = _pmesh->areas;
        params.polyFlags = _pmesh->flags;
        params.polyCount = _pmesh->npolys;
        params.nvp = _pmesh->nvp;
        params.detailMeshes = _dmesh->meshes;
        params.detailVerts = _dmesh->verts;
        params.detailVertsCount = _dmesh->nverts;
        params.detailTris = _dmesh->tris;
        params.detailTriCount = _dmesh->ntris;
        params.offMeshConVerts = _geom->getOffMeshConnectionVerts();
        params.offMeshConRad = _geom->getOffMeshConnectionRads();
        params.offMeshConDir = _geom->getOffMeshConnectionDirs();
        params.offMeshConAreas = _geom->getOffMeshConnectionAreas();
        params.offMeshConFlags = _geom->getOffMeshConnectionFlags();
        params.offMeshConUserID = _geom->getOffMeshConnectionId();
        params.offMeshConCount = _geom->getOffMeshConnectionCount();
        params.walkableHeight = _agentHeight;
        params.walkableRadius = _agentRadius;
        params.walkableClimb = _agentMaxClimb;
        params.tileX = tx;
        params.tileY = ty;
        params.tileLayer = 0;
        rcVcopy(params.bmin, _pmesh->bmin);
        rcVcopy(params.bmax, _pmesh->bmax);
        params.cs = _cfg.cs;
        params.ch = _cfg.ch;
        params.buildBvTree = true;

        if (!dtCreateNavMeshData(&params, &navData, &navDataSize)) {
            _ctx->log(RC_LOG_ERROR, "Could not build Detour navmesh.");
            return 0;
        }
    }
    _tileMemUsage = navDataSize / 1024.0f;

    _ctx->stopTimer(RC_TIMER_TOTAL);

    // Show performance stats.
    duLogBuildTimes(*_ctx, _ctx->getAccumulatedTime(RC_TIMER_TOTAL));
    _ctx->log(RC_LOG_PROGRESS, ">> Polymesh: %d vertices  %d polygons", _pmesh->nverts, _pmesh->npolys);

    _tileBuildTime = _ctx->getAccumulatedTime(RC_TIMER_TOTAL) / 1000.0f;

    dataSize = navDataSize;
    return navData;
   
    return false;
}
 */