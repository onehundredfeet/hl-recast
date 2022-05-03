package recast;

import haxe.io.BytesBuffer;
import hl.Bytes;
import hl.NativeArray;
import hvector.ShaderMath.vec3;
import recast.Native;
import hvector.Vec3;

@:enum
abstract PathVertexCapture(Int) from Int to Int {
    var PATH_ONLY = 0;
    var PATH_AND_AREAS = 1;
    var PATH_AND_POLYS = 2;
}

class NavQueryInstant extends NavQuery{
    
    static final PATH_TO_LINE_PATH = 3;





    public function new(nm : TiledNavMesh, maxPath = NavQuery.MAX_PATH_LENGTH) {
        super(nm,  NavQuery.MAX_NAV_QUERY_NODES, maxPath);
    }

    public var pathLength(get,never) : Int;
    inline function get_pathLength() return _pathLength;

    // Will be longer than path, do not iterate
    public var pathArrayRaw(get,never) : NativeArray<UInt>;
    inline function  get_pathArrayRaw() return _polys;

    public var pathIt(get,never) : NativeSpanIt<UInt>;
    inline function  get_pathIt() return new NativeSpanIt<UInt>(_polys, _pathLength);


    //Stateless - returns all results
    public function findNearest( center : Vec3, halfExtents : Vec3, nearestPoint : Vec3 ) : Null<Int>  {
        var tri = -1;   // MUST BE LOCAL
        _lastStatus = _query.findNearestPoly( center, halfExtents, _filter, tri, nearestPoint );
        if (Status.isSuccess(_lastStatus)) {
            if (tri <= 0) {
                _lastStatus = Status.FAILURE;
            } else {
                return tri;
            }
        }
        return  null;
    }

    // caches the start location
    inline public function findStart( center : Vec3, halfExtents : Vec3 ) : Bool  {
        var x = findNearest(center, halfExtents, _startPt);
        if (x != null) {
            _startPoly = x;
            return true;
        }
        return false;
    }

    // caches the end location
    inline public function findEnd( center : Vec3, halfExtents : Vec3 ) : Bool  {
        var x = findNearest(center, halfExtents, _endPt);
        if (x != null) {
            _endPoly = x;
            return true;
        }
        return false;
    }

    // 
    inline public function findPathStartToEnd() : Bool  {
        if (_startPoly <= 0) throw "Invalid start poly";
        if (_endPoly <= 0) throw "Invalid end poly";
        
        var pl = -1;
        var res = Status.isSuccess(_lastStatus = _query.findPath( _startPoly, _endPoly, _startPt, _endPt, _filter, _polys, pl, _polys.length ));
        if (!res || pl < 0) {
            return false;
        }
        _pathLength = pl;
        return true;
    }

    inline public function findPath( startPoly : UInt, endPoly : UInt, startPt : Vec3, endPt : Vec3 ) : Bool  {
        return Status.isSuccess(_lastStatus = _query.findPath( startPoly, endPoly, startPt, endPt, _filter, _polys, _pathLength, _polys.length ));
    }

    inline public function refinePath( mode : PathVertexCapture = PATH_ONLY, gatherFlags = true, gatherPolyIDs = true) {        
        var length = _pathLength * PATH_TO_LINE_PATH;
        
        if (_linePoints == null || _linePoints.length < length) _linePoints = new NativeArray<Single>(length);
        if (gatherFlags && (_lineFlags == null || _lineFlags.length < length)) _lineFlags = new NativeArray<hl.UI8>(length);
        if (gatherPolyIDs && (_linePolys == null || _linePolys.length < length)) _linePolys = new NativeArray<UInt>(length);

        var funnelPolyCount = -1;
        var options = switch(mode) {
            case PATH_ONLY: 0;
            case PATH_AND_AREAS:  DtStraightPathOptions.DT_STRAIGHTPATH_AREA_CROSSINGS.toValue();
            case PATH_AND_POLYS:  DtStraightPathOptions.DT_STRAIGHTPATH_ALL_CROSSINGS.toValue();
        };

        _lastStatus = _query.findStraightPath( _startPt, _endPt, _polys, _pathLength,  _linePoints, gatherFlags ? _lineFlags : null, gatherPolyIDs ? _linePolys : null, funnelPolyCount, length, options );
        _lineLength = funnelPolyCount;
        
        return Status.isSuccess(_lastStatus);
    }


        /*
	StatusCode findStraightPath(float3 startPos, float3 endPos,
							  dtPolyRef [] path, int pathSize,
							  float[] straightPath, [Out, Cast="unsigned char *"] uint straightPathFlags, dtPolyRef [] straightPathRefs,
							  [Out] int straightPathCount, int maxStraightPath, int options);
        */

}