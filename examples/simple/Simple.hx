package ;

import recast.tools.ObjFile;
import recast.Pipeline;
import recast.Native.MeshCapture;
import hvector.Vec3;
import recast.Native.NavMeshQuery;
import recast.Native.QueryFilter;

//
// This tries to take the entire complete example and recreate it using the higher level HAXE classes
//
class Simple {
    public static final MAX_NAV_QUERY_NODES = 2048;


	public static function main() {
		trace("main()");

		// Leave the defaults
		var pipeline = new Pipeline();

		var obj = ObjFile.loadNative("examples/undulating_small.obj");

		var tileCache = pipeline.createTileCache(obj.vertices, obj.indicies);

		var nm = pipeline.createTileCacheNavMesh(tileCache);

		var mc = new MeshCapture(true);
		mc.captureNavMesh(nm, 0xffff);
		ObjFile.dump("out.obj", mc);

		var navQuery = new NavMeshQuery();
		navQuery.init(nm, MAX_NAV_QUERY_NODES);

		var queryFilter = new QueryFilter();

		var spos = new Vec3(20., 0., 20.);
		var polyPickExt = new Vec3(2., 4., 2.);
		var startRef = -1;
		var nearestPoint = new Vec3(-1., -1., -1.);
		var isOverPoly = false;
		var status = navQuery.findNearestPoly(spos, polyPickExt, queryFilter, startRef, nearestPoint, isOverPoly);
		if (startRef == 0) {
			trace('--- Didn\'t find failure ${status}');
			status = recast.Status.setFailure(status);
		} else if (startRef == -1) {
			// Odd
			trace('--- Failure did not call ${status}');
			status = recast.Status.setFailure(status);
		} else {
			trace('Nearest isSuccess ${recast.Status.isSuccess(status)} isOverPoly ${isOverPoly} nearestPoint ${nearestPoint} startRef ${startRef}');
		}
	}
}
