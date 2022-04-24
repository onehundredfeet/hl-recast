package;

import haxe.io.Bytes;
import recast.Native;
import hvector.Float3.NativeArrayFloat3;
import hl.NativeArray;
import hvector.*;
import hvector.ShaderMath;
import hvector.ShaderMathF;

class RawMeshData {
	public function new() {}

	public var vertices = new Array<Float>();
	public var indices = new Array<Int>();
}

class Complete {
	static function loadObj(filename:String) {
		var rawMesh = new RawMeshData();

		var fin = sys.io.File.read(filename, false);
		try {
			while (true) {
				var line = fin.readLine();

				if (line.charAt(0) == '#')
					continue;

				if (line.charAt(0) == 'v') {
					var splitted = line.split(" ");
					rawMesh.vertices.push(Std.parseFloat(splitted[1]));
					rawMesh.vertices.push(Std.parseFloat(splitted[2]));
					rawMesh.vertices.push(Std.parseFloat(splitted[3]));
				}

				if (line.charAt(0) == 'f') {
					var splitted = line.split(" ");
					rawMesh.indices.push(Std.parseInt(splitted[1]));
					rawMesh.indices.push(Std.parseInt(splitted[2]));
					rawMesh.indices.push(Std.parseInt(splitted[3]));
				}
			}
			fin.close();
		} catch (e:haxe.io.Eof) {
			trace("Obj reading done");
		}

		return rawMesh;
	}

	static function FloatArrayToNativeArray(array:Array<Float>) {
		var out = new hl.NativeArray<Single>(array.length);
		for (i in 0...array.length) {
			out[i] = array[i];
		}
		return out;
	}

	static function IntArrayToNativeArray(array:Array<Int>) {
		var out = new hl.NativeArray<Int>(array.length);
		for (i in 0...array.length) {
			out[i] = array[i];
		}
		return out;
	}

	static final _tileSize:Float = 32.;
	static final _cellSize = 0.3;
	static final _cellHeight = 0.2;
	static final _agentMaxSlope = 45.;
	static final _agentHeight = 3.;
	static final _agentRadius = 0.6;
	static final _agentMaxClimb = 0.8;
	static final _edgeMaxLen = 8.;
	static final _edgeMaxError = 1.3;
	static final _regionMinSize = 8.;
	static final _regionMergeSize = 20.;
	static final _vertsPerPoly = 6;
	static final _detailSampleDist = 6.;
	static final _detailSampleMaxError = 1.;
	static final EXPECTED_LAYERS_PER_TILE = 4;
	static final LINEAR_BUFFER_SIZE = 1024 * 32;
	static final MAX_NAV_QUERY_NODES = 2048;
	static final MAX_LAYERS = 32;
	static final TRIANGLES_PER_CHUNK = 256;
	static final MAX_IDS = 512;

	static final _filterLowHangingObstacles = true;
	static final _filterLedgeSpans = true;
	static final _filterWalkableLowHeightSpans = true;

	static var _cacheLayerCount = 0;
	static var _cacheCompressedSize = 0;
	static var _cacheRawSize = 0;

	static function getGridSize(bmin, bmax) {
		// Init cache
		//		const float* bmin = m_geom->getNavMeshBoundsMin();
		//		const float* bmax = m_geom->getNavMeshBoundsMax();
		//		int gw = 0, gh = 0;
		var gwa = new hl.NativeArray<Int>(1);
		var gha = new hl.NativeArray<Int>(1);
		recast.Native.Recast.calcGridSize(bmin, bmax, _cellSize, gwa, gha);
		var gw = gwa[0];
		var gh = gha[0];

		return {width: gw, height: gh};
	}

	static function getRasterConfig(gw, gh, bmin, bmax) {
		trace("Processing Recast...");
		// -------------------------------------------------------------------------------------
		// Step 1. Initialize build config.
		// -------------------------------------------------------------------------------------

		// Values are taken from cpp sample Sample_SoloMesh.cpp
		var cfg = new recast.Native.RasterConfig();
		cfg.cs = _cellSize;
		cfg.ch = _cellHeight;
		cfg.walkableSlopeAngle = _agentMaxSlope;
		cfg.walkableHeight = Std.int(ceil(_agentHeight / cfg.ch));
		cfg.walkableClimb = Std.int(floor(_agentMaxClimb / cfg.ch));
		cfg.walkableRadius = Std.int(ceil(_agentRadius / cfg.cs));
		cfg.maxEdgeLen = Std.int(_edgeMaxLen / _cellSize);
		cfg.maxSimplificationError = _edgeMaxError;
		cfg.minRegionArea = Std.int(_regionMinSize * _regionMinSize); // Note: area = size*size
		cfg.mergeRegionArea = Std.int(_regionMergeSize * _regionMergeSize); // Note: area = size*size
		cfg.maxVertsPerPoly = _vertsPerPoly;
		cfg.tileSize = Std.int(_tileSize);
		cfg.borderSize = cfg.walkableRadius + 3; // Reserve enough padding.
		cfg.width = cfg.tileSize + cfg.borderSize * 2;
		cfg.height = cfg.tileSize + cfg.borderSize * 2;
		cfg.detailSampleDist = _detailSampleDist < 0.9 ? 0 : _cellSize * _detailSampleDist;
		cfg.detailSampleMaxError = _cellHeight * _detailSampleMaxError;

		cfg.bmin = bmin;
		cfg.bmax = bmax;

		var widthBuffer = new hl.NativeArray<Int>(1);
		var heightBuffer = new hl.NativeArray<Int>(1);
		recast.Native.Recast.calcGridSize(cfg.bmin, cfg.bmax, cfg.cs, widthBuffer, heightBuffer);
		cfg.width = widthBuffer[0];
		cfg.height = heightBuffer[0];
		trace('Grid size: ${cfg.width} x ${cfg.height}'); // Expected 330 x 331

		return cfg;
	}

	static function getTileCounts(gw, gh) {
		trace("getTileCounts...");
		final ts = Std.int(_tileSize);
		final tw = Std.int((gw + ts - 1) / ts);
		final th = Std.int((gh + ts - 1) / ts);

		return {tileSizeI: ts, tileWidthCount: tw, tileHeightCount: th};
	}

	static function getTileCacheParameters(gw, gh, tw, th, bmin:Vec3):TileCacheParams {
		trace("getTileCacheParameters...");
		// Tile cache params.
		var tcparams = new TileCacheParams();
		tcparams.orig = bmin;
		tcparams.cs = _cellSize;
		tcparams.ch = _cellHeight;
		tcparams.width = Std.int(_tileSize);
		tcparams.height = Std.int(_tileSize);
		tcparams.walkableHeight = _agentHeight;
		tcparams.walkableRadius = _agentRadius;
		tcparams.walkableClimb = _agentMaxClimb;
		tcparams.maxSimplificationError = _edgeMaxError;
		tcparams.maxTiles = tw * th * EXPECTED_LAYERS_PER_TILE;
		tcparams.maxObstacles = 128;
		return tcparams;
	}

	static function getBounds(nativeVertices, verticesCount) {
		trace("getBounds...");
		// Get bounds
		var bmin = vec3(0., 0., 0.);
		var bmax = vec3(0., 0., 0.);
		// Expected:
		// bmin: Vec3(4999.105957031, -4.054780006, 4999.551757813)
		// bmax: ec3(5098.000000000, 6.813776016, 5098.796875000)
		recast.Native.Recast.calcBounds(nativeVertices, verticesCount, bmin, bmax);
		trace('Mesh bounds: ${bmin}    ${bmax}');
		return {min: bmin, max: bmax};
	}


	static function getTileCache(cacheCfg) {
		trace("getTileCache...");
		var tileCache = new recast.Native.TileCache();
		var compressor = new recast.Native.FastLZCompressor();
		var allocator = new recast.Native.LinearAllocator(LINEAR_BUFFER_SIZE);
		var processor = new recast.Native.RemapProcessor();
		tileCache.init(cacheCfg, allocator.asSuper(), compressor.asSuper(), processor.asSuper());
		return {cache: tileCache, compressor : compressor, allocator : allocator, processor : processor}; // need to hodl onto these references
	}

	static function getNavMeshParameters(bmin, tw, th) {
		trace("getNavMeshParameters...");
		var params = new NavMeshParams();
		params.orig = bmin;
		params.tileWidth = _tileSize * _cellSize;
		params.tileHeight = _tileSize * _cellSize;

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

	static function calcLayerBufferSize(gridWidth:Int, gridHeight:Int):Int {
		var headerSize = DetourCommon.align4(TileCacheLayerHeader.sizeof());
		var gridSize = gridWidth * gridHeight;
		return headerSize + gridSize * 4;
	}

	static function rasterizeTileLayers(chunkyMesh:ChunkyTriMesh, verts:NativeArray<Single>, nverts:Int, tx:Int, ty:Int, rasterCfg:RasterConfig,
			tiles:Array<TileCacheData>, maxTiles:Int):Int {
		if (tx % 16 == 0 && ty % 16 == 0) 
				trace('rasterizeTileLayers...${tx} ${ty}');
		var rc = new RasterContext(false);

		// Tile bounds.
		var tcs = rasterCfg.tileSize * rasterCfg.cs;

		var tempRasterCfg = new RasterConfig();
		tempRasterCfg.copy(rasterCfg);

		tempRasterCfg.bmin[0] = rasterCfg.bmin[0] + tx * tcs;
		tempRasterCfg.bmin[1] = rasterCfg.bmin[1];
		tempRasterCfg.bmin[2] = rasterCfg.bmin[2] + ty * tcs;
		tempRasterCfg.bmax[0] = rasterCfg.bmin[0] + (tx + 1) * tcs;
		tempRasterCfg.bmax[1] = rasterCfg.bmax[1];
		tempRasterCfg.bmax[2] = rasterCfg.bmin[2] + (ty + 1) * tcs;
		tempRasterCfg.bmin[0] -= tempRasterCfg.borderSize * tempRasterCfg.cs;
		tempRasterCfg.bmin[2] -= tempRasterCfg.borderSize * tempRasterCfg.cs;
		tempRasterCfg.bmax[0] += tempRasterCfg.borderSize * tempRasterCfg.cs;
		tempRasterCfg.bmax[2] += tempRasterCfg.borderSize * tempRasterCfg.cs;

		// Allocate voxel heightfield where we rasterize our input data to.

		var solid = new Heightfield();

		rc.createHeightfield(solid, tempRasterCfg.width, tempRasterCfg.height, tempRasterCfg.bmin, tempRasterCfg.bmax, tempRasterCfg.cs, tempRasterCfg.ch);

		// Allocate array that can hold triangle flags.
		// If you have multiple meshes you need to process, allocate
		// and array which can hold the max number of triangles you need to process.
		var triareas = RcAlloc.allocByteArray(chunkyMesh.maxTrisPerChunk, AllocHint.RC_ALLOC_PERM); // new NativeArray<>(chunkyMesh.maxTrisPerChunk);
		trace('Tri areas ${triareas} is ${chunkyMesh.maxTrisPerChunk}');
		var tbmin = new Vec2(tempRasterCfg.bmin[0], tempRasterCfg.bmin[2]);
		var tbmax = new Vec2(tempRasterCfg.bmax[0], tempRasterCfg.bmax[2]);

		trace('temp array');
		var cid = new NativeArray<Int>(MAX_IDS);

		trace('overlapping');
		var ncid = chunkyMesh.getOverlappingRect(tbmin, tbmax, cid, MAX_IDS);
		if (ncid == 0) {
			return 0; // empty
		}

		trace('looping over ${ncid}');
		for (i in 0...ncid) {
			var nodeTriIndex = chunkyMesh.getNodeTriIndex(cid[i]);
			var tris = chunkyMesh.getTriVertIndices(nodeTriIndex * 3);
			var ntris = chunkyMesh.getNodeTriCount(cid[i]);

			RcAlloc.clearByteArray(triareas, ntris);

			rc.markWalkableTriangles(tempRasterCfg.walkableSlopeAngle, verts, nverts, tris, ntris, triareas);

			trace('Marked and now raster ${i} of ${ncid}\n');
			if (!rc.rasterizeTriangles(verts, nverts, tris, triareas, ntris, solid, tempRasterCfg.walkableClimb)) {
				trace('Failed raster ${i}');
				return 0;
			} 
			trace('Done raster ${i}');
		}

		// Once all geometry is rasterized, we do initial pass of filtering to
		// remove unwanted overhangs caused by the conservative rasterization
		// as well as filter spans where the character cannot possibly stand.
		if (_filterLowHangingObstacles)
			rc.filterLowHangingWalkableObstacles(tempRasterCfg.walkableClimb, solid);
		if (_filterLedgeSpans)
			rc.filterLedgeSpans(tempRasterCfg.walkableHeight, tempRasterCfg.walkableClimb, solid);
		if (_filterWalkableLowHeightSpans)
			rc.filterWalkableLowHeightSpans(tempRasterCfg.walkableHeight, solid);

		var chf = new CompactHeightfield();
		rc.buildCompactHeightfield(tempRasterCfg.walkableHeight, tempRasterCfg.walkableClimb, solid, chf);

		// Erode the walkable area by agent radius.
		rc.erodeWalkableArea(tempRasterCfg.walkableRadius, chf);

		// Layer set?
		var lset = new HeightfieldLayerSet();

		rc.buildHeightfieldLayers(chf, tempRasterCfg.borderSize, tempRasterCfg.walkableHeight, lset);

		var ntiles = 0;
		var icount = Std.int(min(lset.nlayers, maxTiles));
		var lzCompressor = new FastLZCompressor();
		var compressor = lzCompressor.asSuper();

		var tint = new NativeArray<Int>(1);

		for (i in 0...icount) {
			var tile = tiles[ntiles++];
			var layer = lset.layers(i);
			// Store header
			var header = new TileCacheLayerHeader();
			header.magic = TILECACHE_MAGIC.toValue();
			header.version = TILECACHE_VERSION.toValue();

			// Tile layer location in the navmesh.
			header.tx = tx;
			header.ty = ty;
			header.tlayer = i;
			header.bmin = layer.bmin;
			header.bmax = layer.bmax;

			// Tile info.
			header.width = layer.width;
			header.height = layer.height;
			header.minx = layer.minx;
			header.maxx = layer.maxx;
			header.miny = layer.miny;
			header.maxy = layer.maxy;
			header.hmin = layer.hmin;
			header.hmax = layer.hmax;

			tile.data = compressor.buildTileCacheLayer(header, layer.heights, layer.areas, layer.cons, tint);
			tile.dataSize = tint[0]; // multi-return values suck
		}

		return icount;

		/*
			FastLZCompressor comp;
			RasterizationContext rc;

			const float* verts = m_geom->getMesh()->getVerts();
			const int nverts = m_geom->getMesh()->getVertCount();
			const rcChunkyTriMesh* chunkyMesh = m_geom->getChunkyMesh();


			rcConfig tcfg;
			memcpy(&tcfg, &cfg, sizeof(tcfg));

			tcfg.bmin[0] = cfg.bmin[0] + tx*tcs;
			tcfg.bmin[1] = cfg.bmin[1];
			tcfg.bmin[2] = cfg.bmin[2] + ty*tcs;
			tcfg.bmax[0] = cfg.bmin[0] + (tx+1)*tcs;
			tcfg.bmax[1] = cfg.bmax[1];
			tcfg.bmax[2] = cfg.bmin[2] + (ty+1)*tcs;
			tcfg.bmin[0] -= tcfg.borderSize*tcfg.cs;
			tcfg.bmin[2] -= tcfg.borderSize*tcfg.cs;
			tcfg.bmax[0] += tcfg.borderSize*tcfg.cs;
			tcfg.bmax[2] += tcfg.borderSize*tcfg.cs;

			// Allocate voxel heightfield where we rasterize our input data to.
			rc.solid = rcAllocHeightfield();
			if (!rc.solid)
			{
				m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
				return 0;
			}
			if (!rcCreateHeightfield(m_ctx, *rc.solid, tcfg.width, tcfg.height, tcfg.bmin, tcfg.bmax, tcfg.cs, tcfg.ch))
			{
				m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
				return 0;
			}

			// Allocate array that can hold triangle flags.
			// If you have multiple meshes you need to process, allocate
			// and array which can hold the max number of triangles you need to process.
			rc.triareas = new unsigned char[chunkyMesh->maxTrisPerChunk];
			if (!rc.triareas)
			{
				m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", chunkyMesh->maxTrisPerChunk);
				return 0;
			}

			float tbmin[2], tbmax[2];
			tbmin[0] = tcfg.bmin[0];
			tbmin[1] = tcfg.bmin[2];
			tbmax[0] = tcfg.bmax[0];
			tbmax[1] = tcfg.bmax[2];
			int cid[512];// TODO: Make grow when returning too many items.
			const int ncid = rcGetChunksOverlappingRect(chunkyMesh, tbmin, tbmax, cid, 512);
			if (!ncid)
			{
				return 0; // empty
			}

			for (int i = 0; i < ncid; ++i)
			{
				const rcChunkyTriMeshNode& node = chunkyMesh->nodes[cid[i]];
				const int* tris = &chunkyMesh->tris[node.i*3];
				const int ntris = node.n;
				
				memset(rc.triareas, 0, ntris*sizeof(unsigned char));
				rcMarkWalkableTriangles(m_ctx, tcfg.walkableSlopeAngle,
										verts, nverts, tris, ntris, rc.triareas);
				
				if (!rcRasterizeTriangles(m_ctx, verts, nverts, tris, rc.triareas, ntris, *rc.solid, tcfg.walkableClimb))
					return 0;
			}

			// Once all geometry is rasterized, we do initial pass of filtering to
			// remove unwanted overhangs caused by the conservative rasterization
			// as well as filter spans where the character cannot possibly stand.
			if (m_filterLowHangingObstacles)
				rcFilterLowHangingWalkableObstacles(m_ctx, tcfg.walkableClimb, *rc.solid);
			if (m_filterLedgeSpans)
				rcFilterLedgeSpans(m_ctx, tcfg.walkableHeight, tcfg.walkableClimb, *rc.solid);
			if (m_filterWalkableLowHeightSpans)
				rcFilterWalkableLowHeightSpans(m_ctx, tcfg.walkableHeight, *rc.solid);


			rc.chf = rcAllocCompactHeightfield();
			if (!rc.chf)
			{
				m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
				return 0;
			}
			if (!rcBuildCompactHeightfield(m_ctx, tcfg.walkableHeight, tcfg.walkableClimb, *rc.solid, *rc.chf))
			{
				m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
				return 0;
			}

			// Erode the walkable area by agent radius.
			if (!rcErodeWalkableArea(m_ctx, tcfg.walkableRadius, *rc.chf))
			{
				m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
				return 0;
			}

			// (Optional) Mark areas.
			const ConvexVolume* vols = m_geom->getConvexVolumes();
			for (int i  = 0; i < m_geom->getConvexVolumeCount(); ++i)
			{
				rcMarkConvexPolyArea(m_ctx, vols[i].verts, vols[i].nverts,
									 vols[i].hmin, vols[i].hmax,
									 (unsigned char)vols[i].area, *rc.chf);
			}

			rc.lset = rcAllocHeightfieldLayerSet();
			if (!rc.lset)
			{
				m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'lset'.");
				return 0;
			}
			if (!rcBuildHeightfieldLayers(m_ctx, *rc.chf, tcfg.borderSize, tcfg.walkableHeight, *rc.lset))
			{
				m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build heighfield layers.");
				return 0;
			}

			rc.ntiles = 0;
			for (int i = 0; i < rcMin(rc.lset->nlayers, MAX_LAYERS); ++i)
			{
				TileCacheData* tile = &rc.tiles[rc.ntiles++];
				const rcHeightfieldLayer* layer = &rc.lset->layers[i];
				
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

				dtStatus status = dtBuildTileCacheLayer(&comp, &header, layer->heights, layer->areas, layer->cons,
														&tile->data, &tile->dataSize);
				if (dtStatusFailed(status))
				{
					return 0;
				}
				
			}

			// Transfer ownsership of tile data from build context to the caller.
			int n = 0;
			for (int i = 0; i < rcMin(rc.ntiles, maxTiles); ++i)
			{
				tiles[n++] = rc.tiles[i];
				rc.tiles[i].data = 0;
				rc.tiles[i].dataSize = 0;
			}
			return n;
		 */

		return 0;
	}

	static function preprocessTiles(chunkyTriMesh, verts:NativeArray<Single>, nverts:Int, tileCache:TileCache, tw : Int, th : Int, rasterCfg:RasterConfig,
			tcparams:TileCacheParams) {
		// Preprocess tiles.
		trace('preprocessTiles... ${tw} by ${th}');
		_cacheLayerCount = 0;
		_cacheCompressedSize = 0;
		_cacheRawSize = 0;

		var tiles = [for (i in 0...MAX_LAYERS) new TileCacheData()];

		for (y in 0...th) {
			trace('Starting row ${y}');
			for (x in 0...tw) {
				var ntiles = rasterizeTileLayers(chunkyTriMesh, verts, nverts, x, y, rasterCfg, tiles, MAX_LAYERS);

				for (i in 0...ntiles) {
					var tile = tiles[i];
					var result = tileCache.addTile(tile.data, tile.dataSize, DT_COMPRESSEDTILE_FREE_DATA.toValue());

					_cacheLayerCount++;
					_cacheCompressedSize += tile.dataSize;
					_cacheRawSize += calcLayerBufferSize(tcparams.width, tcparams.height); // I think this may be a bug, wrong height & width
				}
			}
		}
	}

	static function buildInitialMeshes(tw : Int, th : Int, tileCache:TileCache, navMesh:NavMesh) {
		trace('buildInitialMeshes... ${tw} by ${th} on ${tileCache} nav ${navMesh}');

		for (y in 0...th)
			for (x in 0...tw)
				tileCache.buildNavMeshTilesAt(x, y, navMesh);

		trace("done...");
		/*
				// Build initial meshes
			m_ctx->startTimer(RC_TIMER_TOTAL);
			m_ctx->stopTimer(RC_TIMER_TOTAL);

			m_cacheBuildTimeMs = m_ctx->getAccumulatedTime(RC_TIMER_TOTAL)/1000.0f;
			m_cacheBuildMemUsage = static_cast<unsigned int>(m_talloc->high);


			const dtNavMesh* nav = m_navMesh;
			int navmeshMemUsage = 0;
			for (int i = 0; i < nav->getMaxTiles(); ++i)
			{
				const dtMeshTile* tile = nav->getTile(i);
				if (tile->header)
					navmeshMemUsage += tile->dataSize;
			}
			printf("navmeshMemUsage = %.1f kB", navmeshMemUsage/1024.0f);
		 */
	}

	public static function main() {
		trace("main()");

		//
		/////////////////////////////// Phase 1 - Load mesh
		//
		var rawMesh = loadObj("PathfinderSample/undulating.obj");
		var verticesCount = cast(rawMesh.vertices.length / 3, Int);
		var trianglesCount = cast(rawMesh.indices.length / 3, Int);

		var nativeVertices = FloatArrayToNativeArray(rawMesh.vertices);
		var nativeIndices = IntArrayToNativeArray(rawMesh.indices);

		var chunkyTriMesh = new ChunkyTriMesh();

		// need to build chunkyTriMesh

		var bounds = getBounds(nativeVertices, verticesCount);
		var gridwh = getGridSize(bounds.min, bounds.max);
		var rasterCfg = getRasterConfig(gridwh.width, gridwh.height, bounds.min, bounds.max);
		var tileCounts = getTileCounts(gridwh.width, gridwh.height);
		var cacheCfg = getTileCacheParameters(gridwh.width, gridwh.height, tileCounts.tileWidthCount, tileCounts.tileHeightCount, bounds.min);
		var configuredCache = getTileCache(cacheCfg);

		var navMeshCfg = getNavMeshParameters(bounds.min, tileCounts.tileWidthCount, tileCounts.tileHeightCount);
		var navMesh = new NavMesh();
		navMesh.setParams(navMeshCfg);
		chunkyTriMesh.build(nativeVertices, nativeIndices,  trianglesCount, TRIANGLES_PER_CHUNK);
		preprocessTiles(chunkyTriMesh, nativeVertices, verticesCount, configuredCache.cache, tileCounts.tileWidthCount, tileCounts.tileHeightCount, rasterCfg, cacheCfg);
		buildInitialMeshes(tileCounts.tileWidthCount, tileCounts.tileHeightCount, configuredCache.cache, navMesh);

		var navQuery = new NavMeshQuery();
		navQuery.init(navMesh, MAX_NAV_QUERY_NODES);

		trace("--- Done");
		// nav mesh is now built
	}
}
/*
	bool Sample_TempObstacles::handleBuild()
	{
	dtStatus status;

	if (!m_geom || !m_geom->getMesh())
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: No vertices and triangles.");
		return false;
	}

	m_tmproc->init(m_geom);

	// Init cache
	const float* bmin = m_geom->getNavMeshBoundsMin();
	const float* bmax = m_geom->getNavMeshBoundsMax();
	int gw = 0, gh = 0;
	rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
	const int ts = (int)m_tileSize;
	const int tw = (gw + ts-1) / ts;
	const int th = (gh + ts-1) / ts;

	// Generation params.
	rcConfig cfg;
	memset(&cfg, 0, sizeof(cfg));
	cfg.cs = m_cellSize;
	cfg.ch = m_cellHeight;
	cfg.walkableSlopeAngle = m_agentMaxSlope;
	cfg.walkableHeight = (int)ceilf(m_agentHeight / cfg.ch);
	cfg.walkableClimb = (int)floorf(m_agentMaxClimb / cfg.ch);
	cfg.walkableRadius = (int)ceilf(m_agentRadius / cfg.cs);
	cfg.maxEdgeLen = (int)(m_edgeMaxLen / m_cellSize);
	cfg.maxSimplificationError = m_edgeMaxError;
	cfg.minRegionArea = (int)rcSqr(m_regionMinSize);		// Note: area = size*size
	cfg.mergeRegionArea = (int)rcSqr(m_regionMergeSize);	// Note: area = size*size
	cfg.maxVertsPerPoly = (int)m_vertsPerPoly;
	cfg.tileSize = (int)m_tileSize;
	cfg.borderSize = cfg.walkableRadius + 3; // Reserve enough padding.
	cfg.width = cfg.tileSize + cfg.borderSize*2;
	cfg.height = cfg.tileSize + cfg.borderSize*2;
	cfg.detailSampleDist = m_detailSampleDist < 0.9f ? 0 : m_cellSize * m_detailSampleDist;
	cfg.detailSampleMaxError = m_cellHeight * m_detailSampleMaxError;
	rcVcopy(cfg.bmin, bmin);
	rcVcopy(cfg.bmax, bmax);

	// Tile cache params.
	dtTileCacheParams tcparams;
	memset(&tcparams, 0, sizeof(tcparams));
	rcVcopy(tcparams.orig, bmin);
	tcparams.cs = m_cellSize;
	tcparams.ch = m_cellHeight;
	tcparams.width = (int)m_tileSize;
	tcparams.height = (int)m_tileSize;
	tcparams.walkableHeight = m_agentHeight;
	tcparams.walkableRadius = m_agentRadius;
	tcparams.walkableClimb = m_agentMaxClimb;
	tcparams.maxSimplificationError = m_edgeMaxError;
	tcparams.maxTiles = tw*th*EXPECTED_LAYERS_PER_TILE;
	tcparams.maxObstacles = 128;

	dtFreeTileCache(m_tileCache);

	m_tileCache = dtAllocTileCache();
	if (!m_tileCache)
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not allocate tile cache.");
		return false;
	}
	status = m_tileCache->init(&tcparams, m_talloc, m_tcomp, m_tmproc);
	if (dtStatusFailed(status))
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init tile cache.");
		return false;
	}

	dtFreeNavMesh(m_navMesh);

	m_navMesh = dtAllocNavMesh();
	if (!m_navMesh)
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not allocate navmesh.");
		return false;
	}

	dtNavMeshParams params;
	memset(&params, 0, sizeof(params));
	rcVcopy(params.orig, bmin);
	params.tileWidth = m_tileSize*m_cellSize;
	params.tileHeight = m_tileSize*m_cellSize;
	params.maxTiles = m_maxTiles;
	params.maxPolys = m_maxPolysPerTile;

	status = m_navMesh->init(&params);
	if (dtStatusFailed(status))
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init navmesh.");
		return false;
	}

	status = m_navQuery->init(m_navMesh, 2048);
	if (dtStatusFailed(status))
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init Detour navmesh query");
		return false;
	}


	// Preprocess tiles.

	m_ctx->resetTimers();

	m_cacheLayerCount = 0;
	m_cacheCompressedSize = 0;
	m_cacheRawSize = 0;

	for (int y = 0; y < th; ++y)
	{
		for (int x = 0; x < tw; ++x)
		{
			TileCacheData tiles[MAX_LAYERS];
			memset(tiles, 0, sizeof(tiles));
			int ntiles = rasterizeTileLayers(x, y, cfg, tiles, MAX_LAYERS);

			for (int i = 0; i < ntiles; ++i)
			{
				TileCacheData* tile = &tiles[i];
				status = m_tileCache->addTile(tile->data, tile->dataSize, DT_COMPRESSEDTILE_FREE_DATA, 0);
				if (dtStatusFailed(status))
				{
					dtFree(tile->data);
					tile->data = 0;
					continue;
				}
				
				m_cacheLayerCount++;
				m_cacheCompressedSize += tile->dataSize;
				m_cacheRawSize += calcLayerBufferSize(tcparams.width, tcparams.height);
			}
		}
	}

	// Build initial meshes
	m_ctx->startTimer(RC_TIMER_TOTAL);
	for (int y = 0; y < th; ++y)
		for (int x = 0; x < tw; ++x)
			m_tileCache->buildNavMeshTilesAt(x,y, m_navMesh);
	m_ctx->stopTimer(RC_TIMER_TOTAL);

	m_cacheBuildTimeMs = m_ctx->getAccumulatedTime(RC_TIMER_TOTAL)/1000.0f;
	m_cacheBuildMemUsage = static_cast<unsigned int>(m_talloc->high);


	const dtNavMesh* nav = m_navMesh;
	int navmeshMemUsage = 0;
	for (int i = 0; i < nav->getMaxTiles(); ++i)
	{
		const dtMeshTile* tile = nav->getTile(i);
		if (tile->header)
			navmeshMemUsage += tile->dataSize;
	}
	printf("navmeshMemUsage = %.1f kB", navmeshMemUsage/1024.0f);
		

	if (m_tool)
		m_tool->init(this);
	initToolStates(this);

	return true;
	}

 */
