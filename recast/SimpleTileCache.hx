package recast;

class SimpleTileCache {

    var _tc = new recast.Native.TileCache();
    var _compressor = new recast.Native.FastLZCompressor();
    var _processor = new recast.Native.RemapProcessor();
    var _allocator : recast.Native.LinearAllocator;
    
    static inline final LINEAR_BUFFER_SIZE = 32 * 1024;
    public function new (cacheCfg, bufferSize = LINEAR_BUFFER_SIZE) {
        _allocator = new recast.Native.LinearAllocator(bufferSize);
        _tc.init(cacheCfg, _allocator, _compressor, _processor);    
    }


}