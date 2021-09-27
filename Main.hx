import recast.Native;
import ShaderMath;

class Main {
	
	public static function main() {
		trace("main()");
		test_DetourMath_0();

		trace("vectors");
		get_set_rcConfig_bounds();
		 test_rcCalcBounds_0();
		 test_rcCalcBounds_1();
	}

	public static function test_DetourMath_0(){
		
		var x : Float = -10.0;
		var y : Float = 10.0;


//		var abs_x = recast.Native.DetourMath.dtMathFabsf(x);
//		var abs_y = recast.Native.DetourMath.dtMathFabsf(y);

//		trace(x, abs_x);
//		trace(y, abs_y);
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

		trace("x: " + k);

		trace("array");
		var y   = vec3(0., 0., 0.);
		x.bmin = y;

		//x.setbmin3(0.,0.,0.);
		trace("Done");
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
		
		var x = new DtNavMeshCreateParams();
		var bb = new hl.Bytes( 100 );	// need to make sure that there's enough bytes
		var bs : hl.BytesAccess<hl.UI16> = bb;
		bs[0] = 1;
		x.verts = bs;
		var xy = x.verts;
		trace("Verts: " + xy[0]);

		var test = true;

		test = test && bmin[0] == verts[0];
		test = test && bmin[1] == verts[1];
		test = test && bmin[2] == verts[2];

		test = test && bmax[0] == verts[0];
		test = test && bmax[1] == verts[1];
		test = test && bmax[2] == verts[2];

		trace("rcCalcBounds: bounds of one vector");
		trace(test);
	}

	public static function test_rcCalcBounds_1(){
		// bounds of more than one vector

		var verts = new hl.NativeArray<Single>(6);
		var bmin = vec3(0., 0., 0.);
		var bmax = vec3(0., 0., 0.);
		verts[0] =  1;
		verts[1] =  2;
		verts[2] =  3;
		verts[3] =  0;
		verts[4] =  2;
		verts[5] =  5;
		recast.Native.Recast.rcCalcBounds(verts, 2, bmin, bmax);

		var test = true;

		test = test && bmin[0] == 0.0;
		test = test && bmin[1] == 2.0;
		test = test && bmin[2] == 3.0;

		test = test && bmax[0] == 1.0;
		test = test && bmax[1] == 2.0;
		test = test && bmax[2] == 5.0;

		trace("rcCalcBounds: bounds of more than one vector");
		trace(test);
	}
}

