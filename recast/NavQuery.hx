package recast;

import hl.NativeArray;
import recast.Native;
import hvector.ShaderMath;

class NavQuery {
    var _nm : TiledNavMesh;
    var _query = new NavMeshQuery();
    var _filter = new QueryFilter();
    var _lastStatus : Int = 0;

    var _startPoly : Int = -1;
    var _startPt = vec3(0.0, 0.0, 0.0);
    var _endPoly : Int = -1;
    var _endPt  = vec3(0.0, 0.0, 0.0);
    
    var _polys : NativeArray<UInt>;
    var _pathLength = 0;

    var _linePoints : NativeArray<Single>;
    var _lineFlags : NativeArray<hl.UI8>;
    var _linePolys : NativeArray<UInt>;
    var _lineLength : Int;

    static inline final MAX_PATH_LENGTH = 256;
    static inline final MAX_NAV_QUERY_NODES = 2048;
    static final PATH_TO_LINE_PATH = 3;
    
    public var pathLength(get,never) : Int;
    inline function get_pathLength() return _pathLength;

    // Will be longer than path, do not iterate
    public var pathArrayRaw(get,never) : NativeArray<UInt>;
    inline function  get_pathArrayRaw() return _polys;

    public var pathIt(get,never) : NativeSpanIt<UInt>;
    inline function  get_pathIt() return new NativeSpanIt<UInt>(_polys, _pathLength);

    public var linePathLength(get,never) : Int;
    inline function get_linePathLength() return _lineLength;

    public inline function getLinePoint(i : Int ) {
        return vec3(_linePoints[i * 3 + 0], _linePoints[i * 3 + 1], _linePoints[i * 3 + 2] );
    }

    public inline function getLinePoly(i : Int ) {
        return _linePolys[i ];
    }
    public inline function getLineFlag(i : Int ) {
        return _lineFlags[i ];
    }

    public function new(nm : TiledNavMesh, maxNodes = MAX_NAV_QUERY_NODES, maxPathLength = MAX_PATH_LENGTH) {
        _nm = nm;

        _query = new NavMeshQuery();
        _polys = new NativeArray<UInt>( maxPathLength);
        checkResult(_query.init(_nm.nativeMesh, maxNodes));        
    }

    public function lastStatusString() {
        return Status.statusString(_lastStatus);
    }


    function checkResult( status : Int,  allowInProgress = false ) {
        if (Status.isSuccess(status)) return ;

        if (Status.isFailure(status)) {
            throw 'Recast failed: ${Status.statusString(status)}';
        }
        if (Status.isInProgress(status)) {
            if (allowInProgress) {
                return;
            }
            throw 'Recast failed: In progress not allowed';
        }

        throw "Illegal recast status";
    }
}