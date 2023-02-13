package recast;

import hl.NativeArray;
import recast.tools.Calculator;
import Math.ceil;
import Math.floor;
import Math.min;

//Being explicit to demonstrate where the types came from.
import recast.Native.NavMeshParams;
import recast.Native.DetourCommon;
import recast.Native.NavMesh;
import recast.Native.ChunkyTriMesh;
import recast.Native.TileCacheParams;
import recast.Native.TileCache;
import recast.Native.TileCacheData;
import recast.Native.CompressedTileFlags;
import recast.Native.RasterContext;
import recast.Native.RasterConfig;
import recast.Native.Heightfield;
import recast.Native.HeightfieldLayerSet;
import recast.Native.Alloc;
import recast.Native.CompactHeightfield;
import recast.Native.AllocHint;
import recast.Native.TileCacheLayerHeader;
import recast.Native.TileCacheLayerHeaderConstants;
import recast.Native.TileCacheCompressor;

import hvector.Vec3;
import hvector.Vec2;

class Pipeline {
    public function new() {
        
    }
	public var tileSize(get, set):Float; // World Space

	inline function set_tileSize(x) {
		_tileCellCount = Std.int(x / _cellSize);
		_tileSizeWS = _tileCellCount * _cellSize;
		return _tileSizeWS;
	}

	inline function get_tileSize() {
		return _tileSizeWS;
	}

	public var cellSize(get, set):Float; // World Space

	inline function set_cellSize(x) {
		_cellSize = x;
		_tileCellCount = Std.int(_tileSizeWS / _cellSize);
		_tileSizeWS = _tileCellCount * _cellSize;
		return _tileSizeWS;
	}

	inline function get_cellSize() {
		return _cellSize;
	}

	var _tileCellCount = Std.int(10. / 0.3); // Std.int(_tileSizeDesired / _cellSize);
	var _tileSizeWS = Std.int(10. / 0.3) * 0.3; // _tileCellCount * _cellSize;
	var _cellSize = 0.3;

	public var cellHeight = 0.2;
	public var agentMaxSlopeDeg = 45.;
	public var agentHeight = 3.;
	public var agentRadius = 0.6;
	public var agentMaxClimb = 0.9;
	public var edgeMaxLen = 6.;
	public var edgeMaxError = 1.3;
	public var regionMinSize = 8.;
	public var regionMergeSize = 15.;
	public var detailSampleDist = 6.;
	public var detailSampleMaxError = 1.;
	public var VERTS_PER_POLY = 6;
	public var EXPECTED_LAYERS_PER_TILE = 4;
	public var LINEAR_BUFFER_SIZE = 1024 * 128;
	public var MAX_LAYERS = 32;
	public var TRIANGLES_PER_CHUNK = 1024 * 10;
	public var MAX_IDS = 512;
	public var MAX_OBSTACLES = 128;

	public var filterLowHangingObstacles = true;
	public var filterLedgeSpans = true;
	public var filterWalkableLowHeightSpans = true;

	var _cacheLayerCount = 0;
	var _cacheCompressedSize = 0;
	var _cacheRawSize = 0;

	public function createTileCache(nativeVertices:NativeArray<Single>, nativeIndices:NativeArray<Int>) : VoxelTileCache{

		var verticesCount = Std.int(nativeVertices.length / 3);
		var bounds = Calculator.getBounds(nativeVertices, verticesCount);

		var gridwh = Calculator.getGridSize(_cellSize, bounds.min, bounds.max);
		var tileCounts = Calculator.getTileCounts(_tileSizeWS, gridwh.width, gridwh.height);

        // Helper mesh
		var chunkyTriMesh = new ChunkyTriMesh();
		var trianglesCount = Std.int(nativeIndices.length / 3);
		chunkyTriMesh.build(nativeVertices, nativeIndices, trianglesCount, TRIANGLES_PER_CHUNK);

		// Initialize tile cache
		var tileCache = getTileCache({ widthCount : tileCounts.tileWidthCount, heightCount: tileCounts.tileHeightCount},  bounds);

		// Reset stats
		_cacheLayerCount = 0;
		_cacheCompressedSize = 0;
		_cacheRawSize = 0;

        //temporary tile data
		var tiles = [for (i in 0...MAX_LAYERS) new TileCacheData()];

		var nverts = Std.int(nativeVertices.length / 3);

        //temporary raster
        var rasterCfg = getRasterConfig(bounds.min, bounds.max);

        var thc = tileCounts.tileHeightCount;
        var twc = tileCounts.tileWidthCount;

        for (y in 0...thc) {
			// trace('Starting row ${y}');
			for (x in 0...twc) {
				var ntiles = rasterizeTileLayers(chunkyTriMesh, nativeVertices, nverts, x, y, rasterCfg, tiles, MAX_LAYERS, tileCache.compressor);
				for (i in 0...ntiles) {
					var tile = tiles[i];
					var refID = -1;
					var result = tileCache.cache.addTile(tile.data, tile.dataSize, DT_COMPRESSEDTILE_FREE_DATA.toValue(), refID);

					if (recast.Status.isSuccess(result)) {
						_cacheLayerCount++;
						_cacheCompressedSize += tile.dataSize;
						_cacheRawSize += Calculator.calcLayerBufferSize(tileCache.parameters.width,tileCache.parameters.height); // I think this may be a bug, wrong height & width
					} else {
						throw 'Unable to add tile ${result} ${recast.Status.isSuccess(result)} ${recast.Status.isFailure(result)} ${recast.Status.isInProgress(result)}';
					}
				}
			}
		}



        return tileCache;
	}

    public function createTileCacheNavMesh(tileCache : VoxelTileCache) {
		// create nav mesh
		var navMeshCfg = getNavMeshParameters(tileCache.bounds.min, tileCache.tileCounts.widthCount, tileCache.tileCounts.heightCount);
		var navMesh = new NavMesh();
		navMesh.setParams(navMeshCfg);

        final tw = tileCache.tileCounts.widthCount;
        final th = tileCache.tileCounts.heightCount;

        for (y in 0...th) {
			for (x in 0...tw) {
				//trace('Building nav mesh tile at ${x} ${y}');
				var res = tileCache.cache.buildNavMeshTilesAt(x, y, navMesh);
				if (!Status.isSuccess(res)) {
					throw 'Failed to build nav mesh at ${x}, ${y} ${Status.isFailure(res)} ${Status.statusString(res)}';
				}
			}
        }

		return new TiledNavMesh(navMesh, tileCache);
	}


	function rasterizeTileLayers(chunkyMesh:ChunkyTriMesh, verts:NativeArray<Single>, nverts:Int, tx:Int, ty:Int, rasterCfg:RasterConfig, tiles:Array<TileCacheData>, maxTiles:Int, compressor : TileCacheCompressor):Int {

    var rc = new RasterContext(false);

    // Tile bounds.
    var tcs = rasterCfg.tileSize * rasterCfg.cs;
    
    var tempRasterCfg = new RasterConfig();
    tempRasterCfg.copy(rasterCfg);

    var borderWS = tempRasterCfg.borderSize * tempRasterCfg.cs;

    tempRasterCfg.bmin[0] = rasterCfg.bmin[0] + tx * tcs - borderWS;
    tempRasterCfg.bmin[1] = rasterCfg.bmin[1];
    tempRasterCfg.bmin[2] = rasterCfg.bmin[2] + ty * tcs - borderWS;
    tempRasterCfg.bmax[0] = rasterCfg.bmin[0] + (tx + 1) * tcs + borderWS;
    tempRasterCfg.bmax[1] = rasterCfg.bmax[1];
    tempRasterCfg.bmax[2] = rasterCfg.bmin[2] + (ty + 1) * tcs + borderWS;

    // Allocate voxel heightfield where we rasterize our input data to.

    var solid = new Heightfield();

    rc.createHeightfield(solid, tempRasterCfg.width, tempRasterCfg.height, tempRasterCfg.bmin, tempRasterCfg.bmax, tempRasterCfg.cs, tempRasterCfg.ch);

    // Allocate array that can hold triangle flags.
    // If you have multiple meshes you need to process, allocate
    // and array which can hold the max number of triangles you need to process.
    var triareas = Alloc.allocByteArray(chunkyMesh.maxTrisPerChunk, AllocHint.RC_ALLOC_PERM); // new NativeArray<>(chunkyMesh.maxTrisPerChunk);
    // trace('Tri areas ${triareas} is ${chunkyMesh.maxTrisPerChunk}');
    var tbmin = new Vec2(tempRasterCfg.bmin[0], tempRasterCfg.bmin[2]);
    var tbmax = new Vec2(tempRasterCfg.bmax[0], tempRasterCfg.bmax[2]);

    // trace('temp array');
    var cid = new NativeArray<Int>(MAX_IDS);

    // trace('overlapping');
    var ncid = chunkyMesh.getOverlappingRect(tbmin, tbmax, cid, MAX_IDS);
    if (ncid == 0) {
        return 0; // empty
    }

    // trace('looping over ${ncid}');
    for (i in 0...ncid) {
        var nodeTriIndex = chunkyMesh.getNodeTriIndex(cid[i]);
        var tris = chunkyMesh.getTriVertIndices(nodeTriIndex * 3);
        var ntris = chunkyMesh.getNodeTriCount(cid[i]);

        Alloc.clearByteArray(triareas, ntris);

        rc.markWalkableTriangles(tempRasterCfg.walkableSlopeAngle, verts, nverts, tris, ntris, triareas);

        //			trace('Marked and now raster ${i} of ${ncid}\n');
        if (!rc.rasterizeTriangles(verts, nverts, tris, triareas, ntris, solid, tempRasterCfg.walkableClimb)) {
            trace('Failed raster ${i}');
            return 0;
        }
        //			trace('Done raster ${i}');
    }

    // Once all geometry is rasterized, we do initial pass of filtering to
    // remove unwanted overhangs caused by the conservative rasterization
    // as well as filter spans where the character cannot possibly stand. // 37ms
    if (filterLowHangingObstacles)
        rc.filterLowHangingWalkableObstacles(tempRasterCfg.walkableClimb, solid);
    if (filterLedgeSpans)
        rc.filterLedgeSpans(tempRasterCfg.walkableHeight, tempRasterCfg.walkableClimb, solid);
    if (filterWalkableLowHeightSpans)
        rc.filterWalkableLowHeightSpans(tempRasterCfg.walkableHeight, solid);


    var chf = new CompactHeightfield();
    rc.buildCompactHeightfield(tempRasterCfg.walkableHeight, tempRasterCfg.walkableClimb, solid, chf);


    // Erode the walkable area by agent radius.
    rc.erodeWalkableArea(tempRasterCfg.walkableRadius, chf);


    // Layer set?
    var lset = new HeightfieldLayerSet();

    if (!rc.buildHeightfieldLayers(chf, tempRasterCfg.borderSize, tempRasterCfg.walkableHeight, lset)) {
        throw "Could not build heightfield layers";
    }

    var ntiles = 0;
    var icount = Std.int(min(lset.nlayers, maxTiles));

    var header = new TileCacheLayerHeader();
    // Store header
    header.magic = TILECACHE_MAGIC.toValue();
    header.version = TILECACHE_VERSION.toValue();

    for (i in 0...icount) {
        var tile = tiles[ntiles++];
        var layer = lset.layers(i);

        // Tile layer location in the navmesh.
        header.tx = tx;
        header.ty = ty;
        header.tlayer = i;
        header.bmin = layer.bmin;
        header.bmax = layer.bmax;

        if (layer.width > 255) throw 'Layer width is beyond 255: ${layer.width}';
        if (layer.height > 255) throw 'Layer height is beyond 255: ${layer.height}';
        // Tile info.
        header.width = layer.width;
        header.height = layer.height;
//        trace('layer width ${layer.width} height ${ layer.height}');
  //      trace('layer width ${header.width} height ${ header.height}');
        header.minx = layer.minx;
        header.maxx = layer.maxx;
        header.miny = layer.miny;
        header.maxy = layer.maxy;
        header.hmin = layer.hmin;
        header.hmax = layer.hmax;

        var tint = 0;
        tile.data = compressor.buildTileCacheLayer(header, layer.heights, layer.areas, layer.cons, tint);
        tile.dataSize = tint;
    }


    Alloc.freeArray(triareas);
    return icount;


    return 0;
}

	function getRasterConfig(bmin, bmax) {
		//trace("Processing Recast...");
		// -------------------------------------------------------------------------------------
		// Step 1. Initialize build config.
		// -------------------------------------------------------------------------------------

		// Values are taken from cpp sample Sample_SoloMesh.cpp
		var cfg = new recast.Native.RasterConfig();

		// Voxels
		cfg.cs = _cellSize;
		cfg.ch = cellHeight;
		cfg.walkableSlopeAngle = agentMaxSlopeDeg;

		// Regions
		cfg.minRegionArea = Std.int(regionMinSize * regionMinSize); // Note: area = size*size
		cfg.mergeRegionArea = Std.int(regionMergeSize * regionMergeSize); // Note: area = size*size
		cfg.maxEdgeLen = Std.int(edgeMaxLen / _cellSize);

		// Nav Mesh
		cfg.maxSimplificationError = edgeMaxError;
		cfg.maxVertsPerPoly = VERTS_PER_POLY;

		// Tiles
		cfg.tileSize = Std.int(_tileCellCount);
		cfg.walkableRadius = Std.int(ceil(agentRadius / cfg.cs));
		cfg.borderSize = cfg.walkableRadius + 3; // Reserve enough padding.
		cfg.width = cfg.tileSize + cfg.borderSize * 2;
		cfg.height = cfg.tileSize + cfg.borderSize * 2;
		cfg.detailSampleDist = detailSampleDist < 0.9 ? 0 : _cellSize * detailSampleDist;
		cfg.detailSampleMaxError = cellHeight * detailSampleMaxError;
		cfg.walkableHeight = Std.int(ceil(agentHeight / cfg.ch)); // voxel units
		cfg.walkableClimb = Std.int(floor(agentMaxClimb / cfg.ch));

		if (cfg.width > 255)
			throw 'tile width is beyond 255: ${cfg.width}';
		if (cfg.height > 255)
			throw 'tile height is beyond 255: ${cfg.height}';

		cfg.bmin = bmin;
		cfg.bmax = bmax;

		return cfg;
	}

	function getNavMeshParameters(bmin, tw, th) {
		var params = new NavMeshParams();
		params.orig = bmin;
		params.tileWidth = _tileSizeWS;
		params.tileHeight = _tileSizeWS;

		// Max tiles and max polys affect how the tile IDs are caculated.
		// There are 22 bits available for identifying a tile and a polygon.
		var tileBits = Std.int(min(DetourCommon.ilog2(DetourCommon.nextPow2(tw * th * EXPECTED_LAYERS_PER_TILE)), 14));
		if (tileBits > 14)
			tileBits = 14;
		var polyBits = 22 - tileBits;

		params.maxTiles = 1 << tileBits;
		params.maxPolys = 1 << polyBits;

		return params;
	}

	function getTileCache(tilecounts : {widthCount : Int, heightCount : Int}, bounds : {min : Vec3, max : Vec3}) {
        var vtc = new VoxelTileCache(LINEAR_BUFFER_SIZE);

        vtc.bounds = bounds;
        vtc.tileCounts = tilecounts;
        final tw = tilecounts.widthCount;
        final th = tilecounts.heightCount;

		// Tile cache params.
		var tcparams = vtc.parameters;
		tcparams.orig = bounds.min;
		tcparams.cs = _cellSize;
		tcparams.ch = cellHeight;
		tcparams.width = _tileCellCount; // voxel units
		tcparams.height = _tileCellCount; // voxel units
		tcparams.walkableHeight = agentHeight;
		tcparams.walkableRadius = agentRadius;
		tcparams.walkableClimb = agentMaxClimb;
		tcparams.maxSimplificationError = edgeMaxError;
		tcparams.maxTiles = tw * th * EXPECTED_LAYERS_PER_TILE;
		tcparams.maxObstacles = MAX_OBSTACLES;

		var tileCache = new recast.Native.TileCache();
		tileCache.init(tcparams, vtc.allocator, vtc.compressor, vtc.processor);

        vtc.cache = tileCache;
        return vtc;


	}
}
