import recast.Native;
import ShaderMath;

class Main {
	
	public static function main() {
		trace("main()");
		test_DetourMath_0();
		test_rcCalcBounds_0();
		test_rcCalcBounds_1();
		// get_set_rcConfig_bounds();
		testDetourRandomPointInConvexPoly();
	}

	public static function ApproxEqual(a : Float, b : Float){
		if (Math.abs(a - b) < 0.001) {
			return true;
		} else {
			return false;
		}
	}

	public static function testPointer(){
		
		var verts = new hl.NativeArray<Single>(3);
		verts[0] = 1;
		verts[1] = 2;
		verts[2] = 3;

		var x = new DtNavMeshCreateParams();
		var bb = new hl.Bytes( 100 );	// need to make sure that there's enough bytes
		var bs : hl.BytesAccess<hl.UI16> = bb;
		bs[0] = 1;
		x.verts = bs;
		var xy = x.verts;
		trace("Verts: " + xy[0]);
	}

	public static function test_DetourMath_0(){
		
		var test = true;

		var x : Float = -10.0;
		var y : Float = 10.0;

		var abs_x = recast.Native.DetourMath.dtMathFabsf(x);
		var abs_y = recast.Native.DetourMath.dtMathFabsf(y);

		test = test && ApproxEqual(10, abs_x);
		test = test && ApproxEqual(10, abs_y);

		trace('test_DetourMath_0: $test');
	}

	// Converted to new method
	public static function test_rcCalcBounds_0(){
		// bounds of one vector
		
		var verts = new hl.NativeArray<Single>(3);
		var bmin = vec3(0., 0., 0.);
		var bmax = vec3(0., 0., 0.);
		verts[0] = 1;
		verts[1] = 2;
		verts[2] = 3;
		
		recast.Native.Recast.rcCalcBounds(verts, 1, bmin, bmax);
		
		var test = true;

		test = test && ApproxEqual(bmin[0], verts[0]);
		test = test && ApproxEqual(bmin[1], verts[1]);
		test = test && ApproxEqual(bmin[2], verts[2]);
		test = test && ApproxEqual(bmax[0], verts[0]);
		test = test && ApproxEqual(bmax[1], verts[1]);
		test = test && ApproxEqual(bmax[2], verts[2]);

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

	public static function get_set_rcConfig_bounds() {
		var x = new recast.Native.Config();
		
		trace("get");

		var v = x.bmin;
		trace("modify");

		v[0] = 20.0;
		trace("set");
		x.bmin = v;

		var k = x.bmin.x;

		// trace("x: ", k);

		trace("array");
		var y   = vec3(0., 0., 0.);
		x.bmin = y;

		//x.setbmin3(0.,0.,0.);
		trace("Done");
	}

	public static function testDetourRandomPointInConvexPoly(){
		
		var test = true;
		
		var pts = new FloatArray(9);
		pts.Set(0, 0);
		pts.Set(1, 0);
		pts.Set(2, 0);
		pts.Set(3, 0);
		pts.Set(4, 0);
		pts.Set(5, 1);
		pts.Set(6, 1);
		pts.Set(7, 0);
		pts.Set(8, 0);

		var npts:Int = 3;

		var areas = new FloatArray(6);
		var out = new FloatArray(3);

		DetourCommon.dtRandomPointInConvexPoly(pts, npts, areas, 0.0, 1.0, out);
		test = test && ApproxEqual(out.Get(0), 0.0);
		test = test && ApproxEqual(out.Get(1), 0.0);
		test = test && ApproxEqual(out.Get(2), 1.0);

		DetourCommon.dtRandomPointInConvexPoly(pts, npts, areas, 0.5, 1.0, out);
		test = test && ApproxEqual(out.Get(0), 1.0 / 2.0);
		test = test && ApproxEqual(out.Get(1), 0.0);
		test = test && ApproxEqual(out.Get(2), 1.0 / 2.0);

		DetourCommon.dtRandomPointInConvexPoly(pts, npts, areas, 1.0, 1.0, out);
		test = test && ApproxEqual(out.Get(0), 1.0);
		test = test && ApproxEqual(out.Get(1), 0.0);
		test = test && ApproxEqual(out.Get(2), 0.0);

		trace('testDetourRandomPointInConvexPoly: $test');
	}

}

