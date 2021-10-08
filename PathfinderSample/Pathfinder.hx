import hl.NativeArray;
import recast.Native;
import h3d.scene.*;
import Float3.NativeArrayFloat3;
import ShaderMath;

class RawMeshData {
	public function new(){}
	public var vertices = new Array<Float>();
	public var indices = new Array<Int>();
}

class Pathfinder extends hxd.App {

	public static var RC_WALKABLE_AREA:hl.UI8 = 63;

	public static var SAMPLE_POLYAREA_GROUND:hl.UI8 = 0;
	public static var SAMPLE_POLYAREA_WATER:hl.UI8 = 1;
	public static var SAMPLE_POLYAREA_ROAD:hl.UI8 = 2;
	public static var SAMPLE_POLYAREA_DOOR:hl.UI8 = 3;
	public static var SAMPLE_POLYAREA_GRASS:hl.UI8 = 4;
	public static var SAMPLE_POLYAREA_JUMP:hl.UI8 = 5;

	public static var SAMPLE_POLYFLAGS_WALK:hl.UI8 = 0x01;		// Ability to walk (ground, grass, road)
	public static var SAMPLE_POLYFLAGS_SWIM:hl.UI8 = 0x02;		// Ability to swim (water).
	public static var SAMPLE_POLYFLAGS_DOOR:hl.UI8 = 0x04;		// Ability to move through doors.
	public static var SAMPLE_POLYFLAGS_JUMP:hl.UI8 = 0x08;		// Ability to jump.
	public static var SAMPLE_POLYFLAGS_DISABLED:hl.UI8 = 0x10;		// Disabled polygon
	public static var SAMPLE_POLYFLAGS_ALL:hl.UI8 = 0xffff;	// All abilities.

	public static var MAX_OFFMESH_CONNECTIONS:Int = 256;

	public static var DT_TILE_FREE_DATA:hl.UI8 = 0x01;

	function FloatArrayToNativeArray(array : Array<Float>){
		var out = new hl.NativeArray<Single>(array.length);
		for (i in 0...array.length){
			out[i] = array[i];
		}
		return out;
	}

	function IntArrayToNativeArray(array : Array<Int>){
		var out = new hl.NativeArray<Int>(array.length);
		for (i in 0...array.length){
			out[i] = array[i];
		}
		return out;
	}

	function loadObj(filename : String) {

		var rawMesh = new RawMeshData();

		var fin = sys.io.File.read(filename, false);
		try {
			while (true){
				var line = fin.readLine();

				if (line.charAt(0) == '#') continue;

				if (line.charAt(0) == 'v')
				{
					var splitted = line.split(" ");
					rawMesh.vertices.push(Std.parseFloat(splitted[1]));
					rawMesh.vertices.push(Std.parseFloat(splitted[2]));
					rawMesh.vertices.push(Std.parseFloat(splitted[3]));
				}

				if (line.charAt(0) == 'f' )
					{
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

    override function init() {

		var rawMesh = loadObj("undulating.obj");
		var verticesCount = cast (rawMesh.vertices.length / 3, Int);
		var trianglesCount = cast (rawMesh.indices.length / 3, Int);

		var nativeVertices = FloatArrayToNativeArray(rawMesh.vertices);
		var nativeIndices = IntArrayToNativeArray(rawMesh.indices);

		// Init context
		var ctx = new recast.Native.RCContext(true);

		// Get bounds 
        var bmin = vec3(0., 0., 0.);
		var bmax = vec3(0., 0., 0.);
		
		
		recast.Native.Recast.rcCalcBounds(nativeVertices, verticesCount, bmin, bmax);	
		
		// Expected:
		// bmin: Vec3(4999.105957031, -4.054780006, 4999.551757813)
		// bmax: ec3(5098.000000000, 6.813776016, 5098.796875000)
		trace('Mesh bounds: ${bmin}    ${bmax}');

	
		trace("Processing Recast...");
		// -------------------------------------------------------------------------------------
		// Step 1. Initialize build config.
		// -------------------------------------------------------------------------------------

		// Values are taken from cpp sample Sample_SoloMesh.cpp
		var cfg = new recast.Native.Config();
		cfg.cs = 0.3;
		cfg.ch = 0.2;
		cfg.walkableSlopeAngle = 45;
		cfg.walkableHeight = 10;
		cfg.walkableClimb = 4;
	    cfg.walkableRadius = 2;
		cfg.maxEdgeLen = 40;
		cfg.maxSimplificationError = 1.3;
		cfg.minRegionArea = 64;
		cfg.mergeRegionArea = 400;
		cfg.maxVertsPerPoly = 6;
		cfg.detailSampleDist = 1.8;
		cfg.detailSampleMaxError = 0.2;
		cfg.bmin = bmin;
		cfg.bmax = bmax;

		var widthBuffer = new hl.NativeArray<Int>(1);
		var heightBuffer = new hl.NativeArray<Int>(1);
		recast.Native.Recast.rcCalcGridSize(cfg.bmin, cfg.bmax, cfg.cs, widthBuffer, heightBuffer);
		cfg.width = widthBuffer[0];
		cfg.height = heightBuffer[0];
		trace('Grid size: ${cfg.width} x ${cfg.height}'); // Expected 330 x 331
		
		
		// -------------------------------------------------------------------------------------
		// Step 2. Rasterize input polygon soup.
		// -------------------------------------------------------------------------------------
		// Allocate voxel heightfield where we rasterize our input data to.
		var solid = new recast.Native.Heightfield();

		if (!ctx.rcCreateHeightfield(solid, cfg.width, cfg.height, cfg.bmin, cfg.bmax, cfg.cs, cfg.ch)){
			trace("Error: rcCreateHeightfield Filed.");
			return;
		}

		var triareas = new hl.NativeArray<hl.UI8>(trianglesCount);
		ctx.rcMarkWalkableTriangles(cfg.walkableSlopeAngle, nativeVertices, verticesCount, nativeIndices, trianglesCount, triareas);

		if (!ctx.rcRasterizeTriangles(nativeVertices, verticesCount, nativeIndices, triareas, trianglesCount, solid, cfg.walkableClimb)){
			trace("Error: rcRasterizeTriangles Filed.");
			return;
		}
		trace(triareas[0]);

		// -------------------------------------------------------------------------------------
		// Step 3. Filter walkables surfaces.
		// -------------------------------------------------------------------------------------

		// Once all geoemtry is rasterized, we do initial pass of filtering to
		// remove unwanted overhangs caused by the conservative rasterization
		// as well as filter spans where the character cannot possibly stand.
		ctx.rcFilterLowHangingWalkableObstacles(cfg.walkableClimb, solid);
		ctx.rcFilterLedgeSpans(cfg.walkableHeight, cfg.walkableClimb, solid);
		ctx.rcFilterWalkableLowHeightSpans(cfg.walkableHeight, solid);


		// -------------------------------------------------------------------------------------
		// Step 4. Partition walkable surface to simple regions.
		// -------------------------------------------------------------------------------------
		var chf = new recast.Native.CompactHeightfield();
		if (!ctx.rcBuildCompactHeightfield(cfg.walkableHeight, cfg.walkableClimb, solid, chf)){
			trace("Error: rcBuildCompactHeightfield Filed.");
			return;
		}
		
		// Erode the walkable area by agent radius.
		if (!ctx.rcErodeWalkableArea(cfg.walkableRadius, chf)){
			trace("Error: rcErodeWalkableArea Filed.");
			return;
		}

		// Prepare for region partitioning, by calculating distance field along the walkable surface.
		if (!ctx.rcBuildDistanceField(chf))
		{
			trace("Error: rcErodeWalkableArea Filed.");
			return;
		}
		
		// Partition the walkable surface into simple regions without holes.
		if (!ctx.rcBuildRegions(chf, 0, cfg.minRegionArea, cfg.mergeRegionArea)){
			trace("Error: rcBuildRegions Filed.");
			return;
		}
		
		trace('chf.spanCount: ${chf.spanCount}'); 

		
		// -------------------------------------------------------------------------------------
		// Step 5. Trace and simplify region contours.
		// -------------------------------------------------------------------------------------
		// Create contours.
		var cset = new recast.Native.ContourSet();
		
		if (!ctx.rcBuildContours(chf, cfg.maxSimplificationError, cfg.maxEdgeLen, cset, 1))
		{
			trace("Error: rcBuildContours Filed.");
			return;
		}
		trace('cset.nconts: ${cset.nconts}');
		

		// -------------------------------------------------------------------------------------
		// Step 6. Build polygons mesh from contours.
		// -------------------------------------------------------------------------------------
		// Build polygon navmesh from the contours.
		var pmesh = new recast.Native.PolyMesh();
		if (!ctx.rcBuildPolyMesh(cset, cfg.maxVertsPerPoly, pmesh)){
			trace("Error: rcBuildPolyMesh Filed.");
			return;
		}


		// -------------------------------------------------------------------------------------
		// Step 7. Create detail mesh which allows to access approximate height on each polygon.
		// -------------------------------------------------------------------------------------
		var dmesh = new recast.Native.PolyMeshDetail();
		if (!ctx.rcBuildPolyMeshDetail(pmesh, chf, cfg.detailSampleDist, cfg.detailSampleMaxError, dmesh)){
			trace("Error: rcBuildPolyMeshDetail Filed.");
			return;
		}

		recast.Native.Recast.rcFreeCompactHeightfield(chf);
		recast.Native.Recast.rcFreeContourSet(cset);

		trace("Recast done");
		// At this point the navigation mesh data is ready, you can access it from m_pmesh.
		// See duDebugDrawPolyMesh or dtCreateNavMeshData as examples how to access the data.


		trace("Processing Detour...");
		// -------------------------------------------------------------------------------------
		//  (Optional) Step 8. Create Detour data from Recast poly mesh.
		// -------------------------------------------------------------------------------------

		// Update poly flags from areas.
		for (i in 0 ... pmesh.npolys){
			if (pmesh.areas[i] == RC_WALKABLE_AREA)
				pmesh.areas[i] = SAMPLE_POLYAREA_GROUND;

			if (pmesh.areas[i] == SAMPLE_POLYAREA_GROUND ||
				pmesh.areas[i] == SAMPLE_POLYAREA_GRASS ||
				pmesh.areas[i] == SAMPLE_POLYAREA_ROAD)
			{
				pmesh.flags[i] = SAMPLE_POLYFLAGS_WALK;
			}

			else if (pmesh.areas[i] == SAMPLE_POLYAREA_WATER)
			{
				pmesh.flags[i] = SAMPLE_POLYFLAGS_SWIM;
			}

			else if (pmesh.areas[i] == SAMPLE_POLYAREA_DOOR)
			{
				pmesh.flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
			}
		}

		
		var params = new recast.Native.DtNavMeshCreateParams();
		params.verts = pmesh.verts;
		params.vertCount = pmesh.nverts;
		params.polys = pmesh.polys;
		params.polyAreas = pmesh.areas;
		params.polyFlags = pmesh.flags;
		params.polyCount = pmesh.npolys;
		params.nvp = pmesh.nvp;
		params.detailMeshes = dmesh.meshes;
		params.detailVerts = dmesh.verts;
		params.detailVertsCount = dmesh.nverts;
		params.detailTris = dmesh.tris;
		params.detailTriCount = dmesh.ntris;
		// params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
		// params.offMeshConRad = m_geom->getOffMeshConnectionRads();
		// params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
		// params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
		// params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
		// params.offMeshConUserID = m_geom->getOffMeshConnectionId();
		// params.offMeshConCount = m_geom->getOffMeshConnectionCount();
		params.walkableHeight = 2;
		params.walkableRadius = 0.6;
		params.walkableClimb = 0.9;
		params.bmin = pmesh.bmin;
		params.bmax = pmesh.bmax;
		params.cs = cfg.cs;
		params.ch = cfg.ch;
		params.buildBvTree = true;
		
		var navData = new hl.NativeArray<hl.UI8>(0);
		var navDataSize:Int = 0;
		// First char** needs to be resolved
		// if (!recast.Native.DetourNavMeshBuilder.dtCreateNavMeshData(params, navData, navDataSize))
		// {
		// 	trace("Error: DetourNavMeshBuilder.dtCreateNavMeshData Filed.");
		// 	return;
		// }
		
		var navMesh = new recast.Native.DtNavMesh();
		var status = navMesh.init(navData, navDataSize, DT_TILE_FREE_DATA);
		if (recast.Native.DetourStatus.dtStatusFailed(status)){
			trace("Error: Could not init Detour navmes");
			return;
		}
		
		var navQuery = new recast.Native.DtNavMeshQuery();
		status = navQuery.init(navMesh, 2048);
		if (recast.Native.DetourStatus.dtStatusFailed(status)){
			trace("Error: Could not init Detour navmesh query");
			return;
		}

		trace("Detour done");

		// Check NavMesh data extraction in CPP sample: 
		// Sample_SoloMesh::handleRender()

		// hxd.Res.initEmbed();

		// var cache = new h3d.prim.ModelCache();
		// cache.loadLibrary(hxd.Res.plane);
		// var obj = cache.loadModel(hxd.Res.plane);

		// s3d.addChild(obj);

		// // Extract geometry
		// var meshes = obj.getMeshes();
		// for (mesh in meshes) {
		// 	var primitive = mesh.primitive;
		// 	trace(primitive.buffer.totalVertices());
		// 	trace(primitive.indexes);
		// }

		// // Clear the cache instance. Note that cache will dispose all cached model textures as well.
		// // cache.dispose();

		// new h3d.scene.CameraController(80, s3d);

    }

    static function main() {
    	new Pathfinder();
    }
}