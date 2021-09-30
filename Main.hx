import Float3.NativeArrayFloat3;
import hl.NativeArray;
import ShaderMath;

class Main {
	
	public static function main() {
		trace("main()");
		test_pointer();
		test_detourMath_0();
		test_rcCalcBounds_0();
		test_rcCalcBounds_1();
		test_detourRandomPointInConvexPoly();
		test_rasterizeTriangle();
	}

	public static function ApproxEqual(a : Float, b : Float){
		if (Math.abs(a - b) < 0.001) {
			return true;
		} else {
			return false;
		}
	}

	public static function ApproxEqualVec3(a : Vec3, b : Vec3){
		return ApproxEqual(a.x, b.x) && ApproxEqual(a.y, b.y) && ApproxEqual(a.z, b.z);
	}

	public static function test_pointer(){
		var verts16 = new hl.NativeArray<hl.UI16>(3);
		var x = new recast.Native.DtNavMeshCreateParams();
		var bb = new hl.Bytes( 100 );	// need to make sure that there's enough bytes
		var bs : hl.BytesAccess<hl.UI16> = bb;
		bs[0] = 1;
		verts16[0] = 1;
		x.verts = bs;
		var xy = x.verts;

		var test = ApproxEqual(xy[0], 1);
		trace('test_pointer: $test');
	}

	public static function test_detourMath_0(){
		
		var test = true;

		var x : Float = -10.0;
		var y : Float = 10.0;

		var abs_x = recast.Native.DetourMath.dtMathFabsf(x);
		var abs_y = recast.Native.DetourMath.dtMathFabsf(y);

		test = test && ApproxEqual(10, abs_x);
		test = test && ApproxEqual(10, abs_y);

		trace('test_DetourMath_0: $test');
	}

	public static function test_rcCalcBounds_0(){
		// bounds of one vector
		
		var verts : Vec3.NativeArrayVec3 = new hl.NativeArray<Single>(3);
		var bmin = vec3(0., 0., 0.);
		var bmax = vec3(0., 0., 0.);
		verts[0] = vec3(1.,2.,3.);
		recast.Native.Recast.rcCalcBounds(verts, 1, bmin, bmax);
		
		var test = true;


		test = test && ApproxEqualVec3(bmin, verts[0]);
		test = test && ApproxEqualVec3(bmax, verts[0]);

		trace('rcCalcBounds (bounds of one vector): $test');
	}

	public static function test_rcCalcBounds_1(){
		// bounds of more than one vector

		var verts = new hl.NativeArray<Single>(6);
		var bmin = vec3(0., 0., 0.);
		var bmax = vec3(0., 0., 0.);
		verts[0] = 1;
		verts[1] =  2;
		verts[2] =  3;
		verts[3] =  0;
		verts[4] =  2;
		verts[5] =  5;
		recast.Native.Recast.rcCalcBounds(verts, 2, bmin, bmax);

		var test = true;

		test = test && ApproxEqual(bmin[0], 0.0);
		test = test && ApproxEqual(bmin[1], 2.0);
		test = test && ApproxEqual(bmin[2], 3.0);
		test = test && ApproxEqual(bmax[0], 1.0);
		test = test && ApproxEqual(bmax[1], 2.0);
		test = test && ApproxEqual(bmax[2], 5.0);

		trace('rcCalcBounds (bounds of more than one vect): $test');
	}

	public static function test_detourRandomPointInConvexPoly(){
		
		var test = true;
		
		var pts = new NativeArray<Single>(9);
		pts[0] =0;
		pts[1] =0;
		pts[2] =0;
		pts[3] =0;
		pts[4] =0;
		pts[5] =1;
		pts[6] =1;
		pts[7] =0;
		pts[8] =0;

		var npts:Int = 3;

		var areas =  new NativeArray<Single>(6);
		var out =  new NativeArray<Single>(3);

		recast.Native.DetourCommon.dtRandomPointInConvexPoly(pts, npts, areas, 0.0, 1.0, out);
		test = test && ApproxEqual(out[0], 0.0);
		test = test && ApproxEqual(out[1], 0.0);
		test = test && ApproxEqual(out[2], 1.0);

		recast.Native.DetourCommon.dtRandomPointInConvexPoly(pts, npts, areas, 0.5, 1.0, out);
		test = test && ApproxEqual(out[0], 1.0 / 2.0);
		test = test && ApproxEqual(out[1], 0.0);
		test = test && ApproxEqual(out[2], 1.0 / 2.0);

		recast.Native.DetourCommon.dtRandomPointInConvexPoly(pts, npts, areas, 1.0, 1.0, out);
		test = test && ApproxEqual(out[0], 1.0);
		test = test && ApproxEqual(out[1], 0.0);
		test = test && ApproxEqual(out[2], 0.0);

		trace('testDetourRandomPointInConvexPoly: $test');
	}

	public static function test_rasterizeTriangle(){
		var test = true;

		var ctx = new recast.Native.RCContext(true);

		var verts = new hl.NativeArray<Single>(9);
		verts[0] = 0;
		verts[1] = 0;
		verts[2] = 0;
		verts[3] = 1;
		verts[4] = 0;
		verts[5] = 0;
		verts[6] = 0;
		verts[7] = 0;
		verts[8] = -1;

		var bmin = vec3(0., 0., 0.);
		var bmax = vec3(0., 0., 0.);
		
		recast.Native.Recast.rcCalcBounds(verts, 3, bmin, bmax);
		var cellSize:Float = 0.5;
		var cellHeight:Float = 0.5;
		var width = new NativeArray<Int>(0);
		var height = new NativeArray<Int>(0);
	
		recast.Native.Recast.rcCalcGridSize(bmin, bmax, cellSize, width, height);
		test = test && width[0] != 0;
		test = test && height[0] != 0;

		var solid = new recast.Native.Heightfield();
		test = test && ctx.rcCreateHeightfield(solid, width[0], height[0], bmin, bmax, cellSize, cellHeight);
	
		var area = 42;
		var flagMergeThr = 1;
		test = test && ctx.rcRasterizeTriangle(
			vec3(verts[0], verts[1], verts[2]), 
			vec3(verts[3], verts[4], verts[5]), 
			vec3(verts[6], verts[7], verts[8]), 
			area, solid, flagMergeThr);

		// TODO . make this tests work after we are able to have rcSpans**
		//  test = test && solid.spans[0 + 0 * width]
		// 	REQUIRE(solid.spans[0 + 0 * width]);
		// 	REQUIRE(!solid.spans[1 + 0 * width]);
		// 	REQUIRE(solid.spans[0 + 1 * width]);
		// 	REQUIRE(solid.spans[1 + 1 * width]);
	
		// 	REQUIRE(solid.spans[0 + 0 * width]->smin == 0);
		// 	REQUIRE(solid.spans[0 + 0 * width]->smax == 1);
		// 	REQUIRE(solid.spans[0 + 0 * width]->area == area);
		// 	REQUIRE(!solid.spans[0 + 0 * width]->next);
	
		// 	REQUIRE(solid.spans[0 + 1 * width]->smin == 0);
		// 	REQUIRE(solid.spans[0 + 1 * width]->smax == 1);
		// 	REQUIRE(solid.spans[0 + 1 * width]->area == area);
		// 	REQUIRE(!solid.spans[0 + 1 * width]->next);
	
		// 	REQUIRE(solid.spans[1 + 1 * width]->smin == 0);
		// 	REQUIRE(solid.spans[1 + 1 * width]->smax == 1);
		// 	REQUIRE(solid.spans[1 + 1 * width]->area == area);
		// 	REQUIRE(!solid.spans[1 + 1 * width]->next);
		// }
		trace ('test_rasterizeTriangle $test');
	}

}

