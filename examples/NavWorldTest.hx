// this file is written in haxe

package ;
import recast.Native;
import hvector.Vec3;

class NavWorldTest {
    public static function main() {
        var origin = new Vec3(-1000, -5, -1000);
        var extents = new Vec3(1000, 5, 1000);
        var nav = NavWorld.create(origin, extents, 100, 0.1, 10., 500, 5000 );
        var chunk = nav.addChunk();
        var mesh = chunk.mesh();

        var loaded = mesh.loadObj("examples/undulating.obj");

        trace('hello world ${loaded}');
    }
}