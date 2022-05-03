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
		// Leave the defaults
		var pipeline = new Pipeline();

		var obj = ObjFile.loadNative("examples/undulating_small.obj");

		var tileCache = pipeline.createTileCache(obj.vertices, obj.indicies);

		var nm = pipeline.createTileCacheNavMesh(tileCache);

		var mc = new MeshCapture(true);
		mc.captureNavMesh(nm.nativeMesh, 0xffff);
		ObjFile.dump("out.obj", mc);


		var q = nm.createInstantQuery();

		var apos = new Vec3(10., 0., 10.);
		var bpos = new Vec3(20., 0., 20.);
		var polyPickExt = new Vec3(2., 4., 2.);
		var a = q.findStart(apos, polyPickExt );
		var b = q.findEnd(bpos, polyPickExt );
		
		if (a && b) {
			if (q.findPathStartToEnd()) {
				trace('Path is successful! with a length of ${q.pathLength}');

				for (t in q.pathIt) {
					trace('\tPoly: ${t}');
				}

				if (q.refinePath()) {
					trace('Funnel is successful!');
					for (i in 0...q.linePathLength) {
						var x = q.getLinePoint(i);
						trace('\tpt: ${x.x}, ${x.y}, ${x.z}');
					}

				} else {
					trace('Funnel has failed!');
				}
			} else 
				trace('Can\'t find path between stat and end: ' + q.lastStatusString());
		} else {
			trace('Couldn\'t find positions ');
		}
	}
}
