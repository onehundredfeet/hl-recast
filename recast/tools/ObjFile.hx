package recast.tools;
import sys.io.File;
import hvector.Vec3;
class ObjFile {

	// Very primitive obj loader - not recommended for anything other than testing
    public static function load(filename:String) {

		var vertices = new Array<Single>();
		var indices = new Array<Int>();

		var fin = sys.io.File.read(filename, false);
		try {
			while (true) {
				var line = fin.readLine();

				if (line.charAt(0) == '#')
					continue;

				if (line.charAt(0) == 'v') {
					if (line.charAt(1) == ' ') {
					var splitted = line.split(" ");
					vertices.push(Std.parseFloat(splitted[1]));
					vertices.push(Std.parseFloat(splitted[2]));
					vertices.push(Std.parseFloat(splitted[3]));
					}
					continue;
				}

				if (line.charAt(0) == 'f') {
					var splitted = line.split(" ");

					var x = splitted[1].indexOf('/');
					if (x > -1) splitted[1] = splitted[1].substr(0, x);
					indices.push(Std.parseInt(splitted[1]) - 1);
					x = splitted[2].indexOf('/');
					if (x > -1) splitted[2] = splitted[2].substr(0, x);
					indices.push(Std.parseInt(splitted[2]) - 1);
					x = splitted[3].indexOf('/');
					if (x > -1) splitted[3] = splitted[3].substr(0, x);
					indices.push(Std.parseInt(splitted[3]) - 1);
				}
			}
			fin.close();
		} catch (e:haxe.io.Eof) {
			trace("Obj reading done");
		}

		return {vertices:vertices, indices : indices };
	}

	public static function loadNative(filename:String) {
		var x = load(filename);
		return {vertices:SingleArrayToNativeArray(x.vertices), indicies: IntArrayToNativeArray(x.indices)};
	}
	
	public static function dump(path:String, mc:recast.Native.MeshCapture) {
		trace('Dumping obj ${path}');
		var vc = mc.numVerts();
		var nt:Int = Std.int(vc / 3);

		var fo = File.write(path, false);

		var vert = new Vec3(0., 0., 0.);
		trace('\t ${vc} verts ${nt} tris');
		fo.writeString('# verts ${vc}\n');

		for (i in 0...vc) {
			mc.getVert(i, vert);

			fo.writeString('v ${vert.x} ${vert.y} ${vert.z}\n');
			if (i % 10000 == 0 && i > 0) {
				trace('${i} verts');
				fo.flush();
			}
		}

		for (i in 0...nt) {
			fo.writeString('f ${(i * 3) + 0 + 1} ${(i * 3) + 1 + 1} ${(i * 3) + 2 + 1}\n');
			if (i % 10000 == 0 && i > 0) {
				trace('${i} faces');
				fo.flush();
			}
		}

		fo.flush();
		fo.close();
	}

	static function SingleArrayToNativeArray(array:Array<Single>) {
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
}