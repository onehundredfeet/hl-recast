package recast;

import hl.NativeArray;

@:generic
class NativeSpanIt<T> {

    var _array : NativeArray<T>;
    var _current : Int;
    var _len : Int;

    public function new(a : NativeArray<T>, len = -1) {
        _array = a;
        _current = 0;
        _len = len == -1 ? _array.length : len;
    }

    public inline function hasNext():Bool {
        return _current < _len;
    }
    public inline function next():T {
        return _array[_current++];
    }
}