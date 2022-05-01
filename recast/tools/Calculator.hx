package recast.tools;

import hl.NativeArray;
import hvector.Vec3;
import hvector.ShaderMath;
import recast.Native.TileCacheLayerHeader;
import recast.Native.DetourCommon;

class Calculator {
    public static function getGridSize(cellSize : Float, bmin : Vec3, bmax :Vec3) {
		// Init cache
		//		const float* bmin = m_geom->getNavMeshBoundsMin();
		//		const float* bmax = m_geom->getNavMeshBoundsMax();
		//		int gw = 0, gh = 0;

		var gwa = 0;
		var gha = 0;
		recast.Native.Recast.calcGridSize(bmin, bmax, cellSize, hl.Ref.make(gwa), hl.Ref.make(gha));

		return {width: gwa, height: gha};
	}

    public static function getTileCounts(tileSizeWS, gw, gh) {
		final ts = Std.int(tileSizeWS);
		final tw = Std.int((gw + ts - 1) / ts);
		final th = Std.int((gh + ts - 1) / ts);


		trace('getTileCounts... gw ${gw} gh ${gh} tw ${tw} th ${th} tsws ${tileSizeWS}');



		return {tileSizeI: ts, tileWidthCount: tw, tileHeightCount: th};
	}

    public static function getBounds(nativeVertices : NativeArray<Single>, verticesCount : Int) : {min: Vec3, max: Vec3}{
		// Get bounds
		var bmin = vec3(0., 0., 0.);
		var bmax = vec3(0., 0., 0.);
		// Expected:
		// bmin: Vec3(4999.105957031, -4.054780006, 4999.551757813)
		// bmax: ec3(5098.000000000, 6.813776016, 5098.796875000)
		recast.Native.Recast.calcBounds(nativeVertices, verticesCount, bmin, bmax);
		return {min: bmin, max: bmax};
	}

    public static function calcLayerBufferSize(gridWidth:Int, gridHeight:Int):Int {
		var headerSize = DetourCommon.align4(TileCacheLayerHeader.sizeof());
		var gridSize = gridWidth * gridHeight;
		return headerSize + gridSize * 4;
	}

}