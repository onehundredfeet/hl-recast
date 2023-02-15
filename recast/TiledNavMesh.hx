package recast;

import recast.Native;
import hvector.Vec3;

class TiledNavMesh {
    var _nm : NavMesh;
    var _cache : VoxelTileCache;
    var _raster : RasterContext;

    public function makeBlank( origin : Vec3, tileSize : Float, maxTiles = 256, maxPolys = 65535 ) {
        _nm = NavMesh.create(origin, tileSize, tileSize, maxTiles, maxPolys);
        _cache = new VoxelTileCache();
        _raster = new RasterContext();
    }
    function new( nm : NavMesh, cache : VoxelTileCache) {
        _nm = nm;
        _cache = cache;
    }

    public function createInstantQuery() {
        return new NavQueryInstant(this);
    }


    public var nativeMesh(get,never) : NavMesh;
    public inline function get_nativeMesh() return _nm;

}