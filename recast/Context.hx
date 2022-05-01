package recast;

class Context {

    var _context : recast.Native.RasterContext;
    var _config : recast.Native.RasterConfig;

    public function new ( configDefaults: recast.Native.RasterConfig, logging : Bool ) {
        _context = new recast.Native.RasterContext(logging);
        _config = configDefaults;
    }

	public  function createHeightfield(width : Int, height : Int) : recast.Native.Heightfield {
        var hf = new recast.Native.Heightfield( );

        if (_context.createHeightfield( hf, width, height, _config.bmin, _config.bmax, _config.cs, _config.ch )) {
            return hf;
        }
        return null;
    }  
    
}