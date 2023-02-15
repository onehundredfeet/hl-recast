// this file is written in haxe

package ;
import hvector.Float2;
import recast.Native;
import hvector.Vec3;
import hvector.Vec2;
import hvector.Int2;
import haxe.Timer;
import hvector.ShaderMath;

class NavWorldTest {
    public static function main() {
        var origin = new Vec3(-50, -10, -50);
        var extents = new Vec3(100, 20, 100);
        // 100 cells of 0.1 units each = 10 units
        var agentParams = new AgentParameters(); // go with defaults

        final maxObstacles = 64;
        final maxPolygons = 5000;
        final maxTiles = 500;
        var nav = NavWorld.create(origin, extents, 256, 0.01, 40., maxTiles, maxPolygons, maxObstacles, agentParams );
        var chunk = nav.addChunk();
        var mesh = chunk.mesh();
        var offset = new Vec3(0, 0, 0);
        var loaded = Timer.measure(() -> return mesh.loadObj("examples/undulating_2.obj", offset));
        var finalized = Timer.measure(() -> return chunk.finalize());
        var tileRangeMin = new Int2(0, 0);
        var tileRangeMax = new Int2(0, 0);
        var worldMin = new Vec2(-20, -20);
        var worldMax = new Vec2(20, 20);
        nav.getTileRegion(worldMin, worldMax, tileRangeMin, tileRangeMax);
        var empty = 0;
        var notEmpty = 0;
        var emptySource = 0;
        var maxSource = 0;
//        for (i in 0...3)
        // Approx 2 ms
        Timer.measure(() -> for (x in tileRangeMin.x...tileRangeMax.x) {
            for (y in tileRangeMin.y...tileRangeMax.y) {
//                trace('Building tiles ${x} ${y}');
                var tb = nav.getTileBuilder(x,y);
  //              trace('hello world ${loaded} ${finalized}');
                var built =  tb.buildTileColumnCacheData();
                maxSource = Std.int(max(maxSource, tb.numSourceChunks()));
                if (tb.numSourceChunks() == 0) {
                    emptySource++;
                }
                if (tb.isEmpty()) {
                    empty++;
                } else {
                    notEmpty++;
                }
    //            trace('built ${built} empty ${tb.isEmpty()} num chunks ${tb.numSourceChunks()}');
                var inserted =  tb.insertIntoCache();
      //          trace('inserted ${inserted}');
                var inflated =  tb.inflate();
        //        trace('inflated ${inflated}');
                tb.retire();
            }
        });
        trace('empty ${empty} vs ${notEmpty} | emptySource ${emptySource} max ${maxSource}');
        /*
        trace('Building tiles ${tileRangeMin.x} ${tileRangeMin.y} to ${tileRangeMax.x} ${tileRangeMax.y}');
        var tb = nav.getTileBuilder(tileRangeMin.x,tileRangeMin.y);
        trace('hello world ${loaded} ${finalized}');
        var built = Timer.measure(() -> return tb.buildTileColumnCacheData());
        trace('built ${built}');
        var inserted = Timer.measure(() -> return tb.insertIntoCache());
        trace('inserted ${inserted}');
        var inflated = Timer.measure(() -> return tb.inflate());
        trace('inflated ${inflated}');
//        tb.discard();
*/
    }
}