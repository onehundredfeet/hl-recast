import recast.Native;

class Main {
	
	public static function main() {
		trace("main()");
		test_DetourMath_0();
		// test_rcCalcBounds_0();
		// test_rcCalcBounds_1();
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
	
		var v = x.bmin;
		v[0] = 0.0;
		x.bmin = v;

		var y : Array<Single> = [0., 0., 0.];
		x.bmin = y;

	}

	public static function test_rcCalcBounds_0(){
		// bounds of one vector
	
		
		var verts = new recast.Native.FloatArray(3);
		var bmin = new recast.Native.FloatArray(3);
		var bmax = new recast.Native.FloatArray(3);
		verts.Set(0, 1);
		verts.Set(1, 2);
		verts.Set(2, 3);
		recast.Native.Recast.rcCalcBounds(verts, 1, bmin, bmax);
		
		var test = true;

		test = test && bmin.Get(0) == verts.Get(0);
		test = test && bmin.Get(1) == verts.Get(1);
		test = test && bmin.Get(2) == verts.Get(2);

		test = test && bmax.Get(0) == verts.Get(0);
		test = test && bmax.Get(1) == verts.Get(1);
		test = test && bmax.Get(2) == verts.Get(2);

		trace("rcCalcBounds: bounds of one vector");
		trace(test);
	}

	public static function test_rcCalcBounds_1(){
		// bounds of more than one vector

		var verts = new recast.Native.FloatArray(6);
		var bmin = new recast.Native.FloatArray(3);
		var bmax = new recast.Native.FloatArray(3);
		verts.Set(0, 1);
		verts.Set(1, 2);
		verts.Set(2, 3);
		verts.Set(3, 0);
		verts.Set(4, 2);
		verts.Set(5, 5);
		recast.Native.Recast.rcCalcBounds(verts, 2, bmin, bmax);

		var test = true;

		test = test && bmin.Get(0) == 0.0;
		test = test && bmin.Get(1) == 2.0;
		test = test && bmin.Get(2) == 3.0;

		test = test && bmax.Get(0) == 1.0;
		test = test && bmax.Get(1) == 2.0;
		test = test && bmax.Get(2) == 5.0;

		trace("rcCalcBounds: bounds of more than one vector");
		trace(test);
	}
}

