package recast;
import hvector.Vec3;

class VoxelTileCache {
    var _tc : recast.Native.TileCache;
    var _compressor = new recast.Native.FastLZCompressor();
    var _processor = new recast.Native.RemapProcessor();
    var _allocator : recast.Native.LinearAllocator;
    var _allocatorSuper : recast.Native.TileCacheAlloc;
    var _params = new recast.Native.TileCacheParams();
    public var bounds : {min:Vec3, max:Vec3};
    public var tileCounts : {widthCount : Int, heightCount:Int};

    public var allocator(get, never) : recast.Native.TileCacheAlloc;

    inline function get_allocator() {
        return _allocatorSuper;
    }

    public var compressor(get, never) : recast.Native.TileCacheCompressor;

    inline function get_compressor() {
        return _compressor.asSuper();
    }

    public var processor(get, never) : recast.Native.TileCacheMeshProcess;

    inline function get_processor() {
        return _processor.asSuper();
    }

    public var cache(get, set) : recast.Native.TileCache;

    inline function get_cache() {
        return _tc;
    }

    inline function set_cache(x) {
        return _tc = x;
    }


    public var parameters(get,never) : recast.Native.TileCacheParams;

    inline function get_parameters() {
        return _params;
    }

    static inline final LINEAR_BUFFER_SIZE = 32 * 1024;
    public function new (bufferSize = LINEAR_BUFFER_SIZE) {
        _allocator = new recast.Native.LinearAllocator(bufferSize);
        _allocatorSuper = _allocator.asSuper();
    }

}