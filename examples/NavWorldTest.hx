// this file is written in haxe

package ;
import hvector.Float2;
import recast.Native;
import hvector.Vec3;
import hvector.Vec2;
import hvector.Int2;

class NavWorldTest {
    public static function main() {
        var origin = new Vec3(-1000, -5, -1000);
        var extents = new Vec3(1000, 5, 1000);
        // 100 cells of 0.1 units each = 10 units
        var nav = NavWorld.create(origin, extents, 100, 0.1, 10., 500, 5000 );
        var chunk = nav.addChunk();
        var mesh = chunk.mesh();

        var loaded = mesh.loadObj("examples/undulating.obj");
        var finalized = chunk.finalize();
        var tileRangeMin = new Int2(0, 0);
        var tileRangeMax = new Int2(0, 0);
        var worldMin = new Vec2(0, 0);
        var worldMax = new Vec2(0, 0);
        nav.getTileRegion(worldMin, worldMax, tileRangeMin, tileRangeMax);
        trace('Building tiles ${tileRangeMin.x} ${tileRangeMin.y} to ${tileRangeMax.x} ${tileRangeMax.y}');
        var tb = nav.getTileBuilder(tileRangeMin.x,tileRangeMin.y);
        trace('hello world ${loaded} ${finalized}');
        tb.retire();
    }
}