package recast;

class Context {

    var _context : recast.Native.RCContext;
    var _config : recast.Native.Config;

    public function new ( configDefaults: recast.Native.Config, logging : Bool ) {
        _context = new recast.Native.RCContext(logging);
        _config = configDefaults;
    }

	public  function createHeightfield(width : Int, height : Int) : recast.Native.Heightfield {
        var hf = new recast.Native.Heightfield( );

        if (_context.rcCreateHeightfield( hf, width, height, _config.bmin, _config.bmax, _config.cs, _config.ch )) {
            return hf;
        }
        return null;
    }  
    
}