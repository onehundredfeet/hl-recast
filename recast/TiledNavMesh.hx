package recast;

import recast.Native.NavMesh;

class TiledNavMesh {
    var _nm : NavMesh;
    var _cache : VoxelTileCache;

    public function new( nm : NavMesh, cache : VoxelTileCache) {
        _nm = nm;
        _cache = cache;
    }

    public function createInstantQuery() {
        return new NavQueryInstant(this);
    }

    public function createSlicedQuery() {
        return new NavQuerySliced(this);
    }

    public function createAsyncQuery() {
        return new NavQueryAsync(this);
    }

    public var nativeMesh(get,never) : NavMesh;
    public inline function get_nativeMesh() return _nm;

}