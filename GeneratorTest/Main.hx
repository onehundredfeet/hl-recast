import haxe.io.Float32Array;


class Main {
	
	public static function main() {
		trace("Recast main get_list()");
		var anim = new recast.Native.Animal();
	    var list = @:privateAccess anim.get_list();
		trace(anim.scalar);
		trace(anim.list);
		var b = new hl.Bytes(anim.list);
		trace(b.getFloat(0));
		// var listAsArray = cast (anim.list, Float32Array);
		// trace(listAsArray);
	}
}

// class Main {
// 	public static function main() {
// 		var INCLUDE = "
// #ifdef _WIN32
// #pragma warning(disable:4305)
// #pragma warning(disable:4244)
// #pragma warning(disable:4316)
// #endif
// #include <Animal.h>

// ";

// 		var options = { idlFile : "recast.idl", nativeLib : "recast", outputDir : "../src", includeCode : INCLUDE, autoGC : true };
// 		webidl.Generate.generateCpp(options);
// 	}
// }

