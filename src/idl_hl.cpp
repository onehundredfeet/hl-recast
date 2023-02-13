#ifdef EMSCRIPTEN

#include <emscripten.h>
#define HL_PRIM
#define HL_NAME(n)	EMSCRIPTEN_KEEPALIVE eb_##n
#define DEFINE_PRIM(ret, name, args)
#define _OPT(t) t*
#define _GET_OPT(value,t) *value
#define alloc_ref(r, _) r
#define alloc_ref_const(r,_) r
#define _ref(t)			t
#define _unref(v)		v
#define free_ref(v) delete (v)
#define HL_CONST const

#else

#define HL_NAME(x) recast_##x
#include <hl.h>
#include "hl-idl-helpers.hpp"
// Need to link in helpers
//HL_API hl_type hltx_ui16;
//HL_API hl_type hltx_ui8;
HL_PRIM hl_type hltx_ui16 = { HUI16 };
HL_PRIM hl_type hltx_ui8 = { HUI8 };

#define _IDL _BYTES
#define _OPT(t) vdynamic *
#define _GET_OPT(value,t) (value)->v.t

static  hl_type *strType = nullptr;
void hl_cache_string_type( vstring *str) {
   strType = str->t;
}

vstring * hl_utf8_to_hlstr( const char *str) {
    int strLen = (int)strlen( str );
    uchar * ubuf = (uchar*)hl_gc_alloc_noptr((strLen + 1) << 1);
    hl_from_utf8( ubuf, strLen, str );

    vstring* vstr = (vstring *)hl_gc_alloc_raw(sizeof(vstring));

    vstr->bytes = ubuf;
    vstr->length = strLen;
    vstr->t = strType;
    return vstr;
}
vstring * hl_utf8_to_hlstr( const std::string &str) {
	return hl_utf8_to_hlstr(str.c_str());
}

HL_PRIM vstring * HL_NAME(getdllversion)(vstring * haxeversion) {
	strType = haxeversion->t;
	return haxeversion;
}
DEFINE_PRIM(_STRING, getdllversion, _STRING);
template <typename T> struct pref {
	void (*finalize)( pref<T> * );
	T *value;
};

#define _ref(t) pref<t>
#define _unref(v) v->value
#define _unref_ptr_safe(v) (v != nullptr ? v->value : nullptr)
#define alloc_ref(r,t) _alloc_ref(r,finalize_##t)
#define alloc_ref_const(r, _) _alloc_const(r)
#define HL_CONST

template<typename T> void free_ref( pref<T> *r ) {
	if( !r->finalize ) hl_error("delete() is not allowed on const value.");
	delete r->value;
	r->value = NULL;
	r->finalize = NULL;
}

template<typename T> void free_ref( pref<T> *r, void (*deleteFunc)(T*) ) {
	if( !r->finalize ) hl_error("delete() is not allowed on const value.");
	deleteFunc( r->value );
	r->value = NULL;
	r->finalize = NULL;
}

inline void testvector(_hl_float3 *v) {
  printf("v: %f %f %f\n", v->x, v->y, v->z);
}
template<typename T> pref<T> *_alloc_ref( T *value, void (*finalize)( pref<T> * ) ) {
	if (value == nullptr) return nullptr;
	pref<T> *r = (pref<T>*)hl_gc_alloc_finalizer(sizeof(pref<T>));
	r->finalize = finalize;
	r->value = value;
	return r;
}

template<typename T> pref<T> *_alloc_const( const T *value ) {
	if (value == nullptr) return nullptr;
	pref<T> *r = (pref<T>*)hl_gc_alloc_noptr(sizeof(pref<T>));
	r->finalize = NULL;
	r->value = (T*)value;
	return r;
}

inline static varray* _idc_alloc_array(float *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	float *p;
	a = hl_alloc_array(&hlt_f32, count);
	p = hl_aptr(a, float);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}
inline static varray* _idc_alloc_array(unsigned char *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	float *p;
	a = hl_alloc_array(&hltx_ui8, count);
	p = hl_aptr(a, float);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static varray* _idc_alloc_array( char *src, int count) {
	return _idc_alloc_array((unsigned char *)src, count);
}

inline static varray* _idc_alloc_array(int *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	int *p;
	a = hl_alloc_array(&hlt_i32, count);
	p = hl_aptr(a, int);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;

}

inline static varray* _idc_alloc_array(double *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	double *p;
	a = hl_alloc_array(&hlt_f64, count);
	p = hl_aptr(a, double);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}


inline static varray* _idc_alloc_array(const unsigned short *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	unsigned short *p;
	a = hl_alloc_array(&hltx_ui16, count);
	p = hl_aptr(a, unsigned short);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static varray* _idc_alloc_array(unsigned short *src, int count) {
	if (src == nullptr) return nullptr;

	varray *a = NULL;
	unsigned short *p;
	a = hl_alloc_array(&hltx_ui16, count);
	p = hl_aptr(a, unsigned short);

	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
	return a;
}

inline static void _idc_copy_array( float *dst, varray *src, int count) {
	float *p = hl_aptr(src, float);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, float *src,  int count) {
	float *p = hl_aptr(dst, float);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}


inline static void _idc_copy_array( int *dst, varray *src, int count) {
	int *p = hl_aptr(src, int);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( unsigned short *dst, varray *src) {
	unsigned short *p = hl_aptr(src, unsigned short);
	for (int i = 0; i < src->size; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( const unsigned short *cdst, varray *src) {
	unsigned short *p = hl_aptr(src, unsigned short);
	unsigned short *dst = (unsigned short *)cdst;
	for (int i = 0; i < src->size; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, int *src,  int count) {
	int *p = hl_aptr(dst, int);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}


inline static void _idc_copy_array( double *dst, varray *src, int count) {
	double *p = hl_aptr(src, double);
	for (int i = 0; i < count; i++) {
		dst[i] = p[i];
	}
}

inline static void _idc_copy_array( varray *dst, double *src,  int count) {
	double *p = hl_aptr(dst, double);
	for (int i = 0; i < count; i++) {
		p[i] = src[i];
	}
}

#endif

#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif
#include <Recast.h>
#include <RecastAlloc.h>
#include <RecastAssert.h>
#include "ChunkyTriMesh.h"

#include <DetourAlloc.h>
#include <DetourAssert.h>
#include <DetourCommon.h>
#include <DetourMath.h>
#include <DetourNavMesh.h>
#include <DetourNavMeshBuilder.h>
#include <DetourNavMeshQuery.h>
#include <DetourNode.h>
#include <DetourStatus.h>
#include <DetourTileCache.h>
#include <DetourTileCacheBuilder.h>
#include "hl-recast.h"

// Dummy decalration to fix some build errors.
class Recast;
class DetourMath;
class DetourCommon;
class DetourStatus;
class DetourNavMesh;
class DetourNavMeshBuilder;
class DetourNavMeshQuery;

bool rcHeightfield_rcSpanIsValidAt(rcHeightfield *context, int pos) {
	return context->spans[pos] ? true : false;
}

rcSpan* rcHeightfield_rcSpanAt(rcHeightfield *context, int pos) {
	return context->spans[pos];
}





extern "C" {

static rcLogCategory LogCategory__values[] = { RC_LOG_PROGRESS,RC_LOG_WARNING,RC_LOG_ERROR };
HL_PRIM int HL_NAME(LogCategory_toValue0)( int idx ) {
	return LogCategory__values[idx];
}
DEFINE_PRIM(_I32, LogCategory_toValue0, _I32);
HL_PRIM int HL_NAME(LogCategory_indexToValue1)( int idx ) {
	return LogCategory__values[idx];
}
DEFINE_PRIM(_I32, LogCategory_indexToValue1, _I32);
HL_PRIM int HL_NAME(LogCategory_valueToIndex1)( int value ) {
	for( int i = 0; i < 3; i++ ) if ( value == (int)LogCategory__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, LogCategory_valueToIndex1, _I32);
HL_PRIM int HL_NAME(LogCategory_fromValue1)( int value ) {
	for( int i = 0; i < 3; i++ ) if ( value == (int)LogCategory__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, LogCategory_fromValue1, _I32);
HL_PRIM int HL_NAME(LogCategory_fromIndex1)( int index ) {return index;}
DEFINE_PRIM(_I32, LogCategory_fromIndex1, _I32);
static rcTimerLabel TimerLabel__values[] = { RC_TIMER_TOTAL,RC_TIMER_TEMP,RC_TIMER_RASTERIZE_TRIANGLES,RC_TIMER_BUILD_COMPACTHEIGHTFIELD,RC_TIMER_BUILD_CONTOURS,RC_TIMER_BUILD_CONTOURS_TRACE,RC_TIMER_BUILD_CONTOURS_SIMPLIFY,RC_TIMER_FILTER_BORDER,RC_TIMER_FILTER_WALKABLE,RC_TIMER_MEDIAN_AREA,RC_TIMER_FILTER_LOW_OBSTACLES,RC_TIMER_BUILD_POLYMESH,RC_TIMER_MERGE_POLYMESH,RC_TIMER_ERODE_AREA,RC_TIMER_MARK_BOX_AREA,RC_TIMER_MARK_CYLINDER_AREA,RC_TIMER_MARK_CONVEXPOLY_AREA,RC_TIMER_BUILD_DISTANCEFIELD,RC_TIMER_BUILD_DISTANCEFIELD_DIST,RC_TIMER_BUILD_DISTANCEFIELD_BLUR,RC_TIMER_BUILD_REGIONS,RC_TIMER_BUILD_REGIONS_WATERSHED,RC_TIMER_BUILD_REGIONS_EXPAND,RC_TIMER_BUILD_REGIONS_FLOOD,RC_TIMER_BUILD_REGIONS_FILTER,RC_TIMER_BUILD_LAYERS,RC_TIMER_BUILD_POLYMESHDETAIL,RC_TIMER_MERGE_POLYMESHDETAIL,RC_MAX_TIMERS };
HL_PRIM int HL_NAME(TimerLabel_toValue0)( int idx ) {
	return TimerLabel__values[idx];
}
DEFINE_PRIM(_I32, TimerLabel_toValue0, _I32);
HL_PRIM int HL_NAME(TimerLabel_indexToValue1)( int idx ) {
	return TimerLabel__values[idx];
}
DEFINE_PRIM(_I32, TimerLabel_indexToValue1, _I32);
HL_PRIM int HL_NAME(TimerLabel_valueToIndex1)( int value ) {
	for( int i = 0; i < 29; i++ ) if ( value == (int)TimerLabel__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, TimerLabel_valueToIndex1, _I32);
HL_PRIM int HL_NAME(TimerLabel_fromValue1)( int value ) {
	for( int i = 0; i < 29; i++ ) if ( value == (int)TimerLabel__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, TimerLabel_fromValue1, _I32);
HL_PRIM int HL_NAME(TimerLabel_fromIndex1)( int index ) {return index;}
DEFINE_PRIM(_I32, TimerLabel_fromIndex1, _I32);
static void finalize_ChunkyTriMesh( pref<rcChunkyTriMesh>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(ChunkyTriMesh_delete)( pref<rcChunkyTriMesh>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, ChunkyTriMesh_delete, _IDL);
static void finalize_RasterContext( pref<rcContext>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(RasterContext_delete)( pref<rcContext>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, RasterContext_delete, _IDL);
static void finalize_rcScopedTimer( pref<rcScopedTimer>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(rcScopedTimer_delete)( pref<rcScopedTimer>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, rcScopedTimer_delete, _IDL);
static void finalize_RasterConfig( pref<rcConfig>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(RasterConfig_delete)( pref<rcConfig>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, RasterConfig_delete, _IDL);
static void finalize_Span( pref<rcSpan>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Span_delete)( pref<rcSpan>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Span_delete, _IDL);
static void finalize_SpanPool( pref<rcSpanPool>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(SpanPool_delete)( pref<rcSpanPool>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, SpanPool_delete, _IDL);
static void finalize_Heightfield( pref<rcHeightfield>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Heightfield_delete)( pref<rcHeightfield>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Heightfield_delete, _IDL);
static void finalize_rcCompactCell( pref<rcCompactCell>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(rcCompactCell_delete)( pref<rcCompactCell>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, rcCompactCell_delete, _IDL);
static void finalize_rcCompactSpan( pref<rcCompactSpan>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(rcCompactSpan_delete)( pref<rcCompactSpan>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, rcCompactSpan_delete, _IDL);
static void finalize_CompactHeightfield( pref<rcCompactHeightfield>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(CompactHeightfield_delete)( pref<rcCompactHeightfield>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, CompactHeightfield_delete, _IDL);
static void finalize_HeightfieldLayer( pref<rcHeightfieldLayer>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(HeightfieldLayer_delete)( pref<rcHeightfieldLayer>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, HeightfieldLayer_delete, _IDL);
static void finalize_HeightfieldLayerSet( pref<rcHeightfieldLayerSet>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(HeightfieldLayerSet_delete)( pref<rcHeightfieldLayerSet>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, HeightfieldLayerSet_delete, _IDL);
static void finalize_rcContour( pref<rcContour>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(rcContour_delete)( pref<rcContour>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, rcContour_delete, _IDL);
static void finalize_rcContourSet( pref<rcContourSet>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(rcContourSet_delete)( pref<rcContourSet>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, rcContourSet_delete, _IDL);
static void finalize_PolyMesh( pref<rcPolyMesh>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(PolyMesh_delete)( pref<rcPolyMesh>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, PolyMesh_delete, _IDL);
static void finalize_PolyMeshDetail( pref<rcPolyMeshDetail>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(PolyMeshDetail_delete)( pref<rcPolyMeshDetail>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, PolyMeshDetail_delete, _IDL);
static rcAllocHint AllocHint__values[] = { RC_ALLOC_PERM,RC_ALLOC_TEMP };
HL_PRIM int HL_NAME(AllocHint_toValue0)( int idx ) {
	return AllocHint__values[idx];
}
DEFINE_PRIM(_I32, AllocHint_toValue0, _I32);
HL_PRIM int HL_NAME(AllocHint_indexToValue1)( int idx ) {
	return AllocHint__values[idx];
}
DEFINE_PRIM(_I32, AllocHint_indexToValue1, _I32);
HL_PRIM int HL_NAME(AllocHint_valueToIndex1)( int value ) {
	for( int i = 0; i < 2; i++ ) if ( value == (int)AllocHint__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, AllocHint_valueToIndex1, _I32);
HL_PRIM int HL_NAME(AllocHint_fromValue1)( int value ) {
	for( int i = 0; i < 2; i++ ) if ( value == (int)AllocHint__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, AllocHint_fromValue1, _I32);
HL_PRIM int HL_NAME(AllocHint_fromIndex1)( int index ) {return index;}
DEFINE_PRIM(_I32, AllocHint_fromIndex1, _I32);
static void finalize_TileCache( pref<dtTileCache>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileCache_delete)( pref<dtTileCache>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileCache_delete, _IDL);
static void finalize_CompressedTile( pref<dtCompressedTile>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(CompressedTile_delete)( pref<dtCompressedTile>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, CompressedTile_delete, _IDL);
static void finalize_TileCacheMeshProcess( pref<dtTileCacheMeshProcess>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileCacheMeshProcess_delete)( pref<dtTileCacheMeshProcess>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileCacheMeshProcess_delete, _IDL);
static void finalize_TileCacheAlloc( pref<dtTileCacheAlloc>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileCacheAlloc_delete)( pref<dtTileCacheAlloc>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileCacheAlloc_delete, _IDL);
static void finalize_TileCacheCompressor( pref<dtTileCacheCompressor>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileCacheCompressor_delete)( pref<dtTileCacheCompressor>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileCacheCompressor_delete, _IDL);
static TileCacheLayerHeaderConstants TileCacheLayerHeaderConstants__values[] = { TILECACHE_MAGIC,TILECACHE_VERSION };
HL_PRIM int HL_NAME(TileCacheLayerHeaderConstants_toValue0)( int idx ) {
	return TileCacheLayerHeaderConstants__values[idx];
}
DEFINE_PRIM(_I32, TileCacheLayerHeaderConstants_toValue0, _I32);
HL_PRIM int HL_NAME(TileCacheLayerHeaderConstants_indexToValue1)( int idx ) {
	return TileCacheLayerHeaderConstants__values[idx];
}
DEFINE_PRIM(_I32, TileCacheLayerHeaderConstants_indexToValue1, _I32);
HL_PRIM int HL_NAME(TileCacheLayerHeaderConstants_valueToIndex1)( int value ) {
	for( int i = 0; i < 2; i++ ) if ( value == (int)TileCacheLayerHeaderConstants__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, TileCacheLayerHeaderConstants_valueToIndex1, _I32);
HL_PRIM int HL_NAME(TileCacheLayerHeaderConstants_fromValue1)( int value ) {
	for( int i = 0; i < 2; i++ ) if ( value == (int)TileCacheLayerHeaderConstants__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, TileCacheLayerHeaderConstants_fromValue1, _I32);
HL_PRIM int HL_NAME(TileCacheLayerHeaderConstants_fromIndex1)( int index ) {return index;}
DEFINE_PRIM(_I32, TileCacheLayerHeaderConstants_fromIndex1, _I32);
static void finalize_TileCacheLayerHeader( pref<dtTileCacheLayerHeader>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileCacheLayerHeader_delete)( pref<dtTileCacheLayerHeader>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileCacheLayerHeader_delete, _IDL);
static void finalize_TileCacheLayer( pref<dtTileCacheLayer>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileCacheLayer_delete)( pref<dtTileCacheLayer>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileCacheLayer_delete, _IDL);
static void finalize_TileCacheContour( pref<dtTileCacheLayer>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileCacheContour_delete)( pref<dtTileCacheLayer>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileCacheContour_delete, _IDL);
static void finalize_TileCacheContourSet( pref<dtTileCacheContourSet>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileCacheContourSet_delete)( pref<dtTileCacheContourSet>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileCacheContourSet_delete, _IDL);
static void finalize_TileCachePolyMesh( pref<dtTileCachePolyMesh>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileCachePolyMesh_delete)( pref<dtTileCachePolyMesh>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileCachePolyMesh_delete, _IDL);
static void finalize_TileCacheObstacle( pref<dtTileCacheObstacle>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileCacheObstacle_delete)( pref<dtTileCacheObstacle>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileCacheObstacle_delete, _IDL);
static void finalize_TileCacheParams( pref<dtTileCacheParams>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileCacheParams_delete)( pref<dtTileCacheParams>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileCacheParams_delete, _IDL);
static void finalize_TileCacheData( pref<TileCacheData>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileCacheData_delete)( pref<TileCacheData>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileCacheData_delete, _IDL);
static void finalize_FastLZCompressor( pref<FastLZCompressor>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(FastLZCompressor_delete)( pref<FastLZCompressor>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, FastLZCompressor_delete, _IDL);
static void finalize_LinearAllocator( pref<LinearAllocator>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(LinearAllocator_delete)( pref<LinearAllocator>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, LinearAllocator_delete, _IDL);
static void finalize_RemapProcessor( pref<RemapProcessor>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(RemapProcessor_delete)( pref<RemapProcessor>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, RemapProcessor_delete, _IDL);
static dtCompressedTileFlags CompressedTileFlags__values[] = { DT_COMPRESSEDTILE_FREE_DATA };
HL_PRIM int HL_NAME(CompressedTileFlags_toValue0)( int idx ) {
	return CompressedTileFlags__values[idx];
}
DEFINE_PRIM(_I32, CompressedTileFlags_toValue0, _I32);
HL_PRIM int HL_NAME(CompressedTileFlags_indexToValue1)( int idx ) {
	return CompressedTileFlags__values[idx];
}
DEFINE_PRIM(_I32, CompressedTileFlags_indexToValue1, _I32);
HL_PRIM int HL_NAME(CompressedTileFlags_valueToIndex1)( int value ) {
	for( int i = 0; i < 1; i++ ) if ( value == (int)CompressedTileFlags__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, CompressedTileFlags_valueToIndex1, _I32);
HL_PRIM int HL_NAME(CompressedTileFlags_fromValue1)( int value ) {
	for( int i = 0; i < 1; i++ ) if ( value == (int)CompressedTileFlags__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, CompressedTileFlags_fromValue1, _I32);
HL_PRIM int HL_NAME(CompressedTileFlags_fromIndex1)( int index ) {return index;}
DEFINE_PRIM(_I32, CompressedTileFlags_fromIndex1, _I32);
static dtStraightPathFlags StraightPathFlags__values[] = { DT_STRAIGHTPATH_START,DT_STRAIGHTPATH_END,DT_STRAIGHTPATH_OFFMESH_CONNECTION };
HL_PRIM int HL_NAME(StraightPathFlags_toValue0)( int idx ) {
	return StraightPathFlags__values[idx];
}
DEFINE_PRIM(_I32, StraightPathFlags_toValue0, _I32);
HL_PRIM int HL_NAME(StraightPathFlags_indexToValue1)( int idx ) {
	return StraightPathFlags__values[idx];
}
DEFINE_PRIM(_I32, StraightPathFlags_indexToValue1, _I32);
HL_PRIM int HL_NAME(StraightPathFlags_valueToIndex1)( int value ) {
	for( int i = 0; i < 3; i++ ) if ( value == (int)StraightPathFlags__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, StraightPathFlags_valueToIndex1, _I32);
HL_PRIM int HL_NAME(StraightPathFlags_fromValue1)( int value ) {
	for( int i = 0; i < 3; i++ ) if ( value == (int)StraightPathFlags__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, StraightPathFlags_fromValue1, _I32);
HL_PRIM int HL_NAME(StraightPathFlags_fromIndex1)( int index ) {return index;}
DEFINE_PRIM(_I32, StraightPathFlags_fromIndex1, _I32);
static dtStraightPathOptions StraightPathOptions__values[] = { DT_STRAIGHTPATH_AREA_CROSSINGS,DT_STRAIGHTPATH_ALL_CROSSINGS };
HL_PRIM int HL_NAME(StraightPathOptions_toValue0)( int idx ) {
	return StraightPathOptions__values[idx];
}
DEFINE_PRIM(_I32, StraightPathOptions_toValue0, _I32);
HL_PRIM int HL_NAME(StraightPathOptions_indexToValue1)( int idx ) {
	return StraightPathOptions__values[idx];
}
DEFINE_PRIM(_I32, StraightPathOptions_indexToValue1, _I32);
HL_PRIM int HL_NAME(StraightPathOptions_valueToIndex1)( int value ) {
	for( int i = 0; i < 2; i++ ) if ( value == (int)StraightPathOptions__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, StraightPathOptions_valueToIndex1, _I32);
HL_PRIM int HL_NAME(StraightPathOptions_fromValue1)( int value ) {
	for( int i = 0; i < 2; i++ ) if ( value == (int)StraightPathOptions__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, StraightPathOptions_fromValue1, _I32);
HL_PRIM int HL_NAME(StraightPathOptions_fromIndex1)( int index ) {return index;}
DEFINE_PRIM(_I32, StraightPathOptions_fromIndex1, _I32);
static void finalize_Poly( pref<dtPoly>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Poly_delete)( pref<dtPoly>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Poly_delete, _IDL);
static void finalize_PolyDetail( pref<dtPolyDetail>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(PolyDetail_delete)( pref<dtPolyDetail>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, PolyDetail_delete, _IDL);
static void finalize_Link( pref<dtLink>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Link_delete)( pref<dtLink>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Link_delete, _IDL);
static void finalize_BVNode( pref<dtBVNode>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(BVNode_delete)( pref<dtBVNode>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, BVNode_delete, _IDL);
static void finalize_OffMeshConnection( pref<dtOffMeshConnection>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(OffMeshConnection_delete)( pref<dtOffMeshConnection>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, OffMeshConnection_delete, _IDL);
static void finalize_MeshHeader( pref<dtMeshHeader>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(MeshHeader_delete)( pref<dtMeshHeader>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, MeshHeader_delete, _IDL);
static void finalize_MeshTile( pref<dtMeshTile>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(MeshTile_delete)( pref<dtMeshTile>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, MeshTile_delete, _IDL);
static void finalize_NavMeshParams( pref<dtNavMeshParams>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(NavMeshParams_delete)( pref<dtNavMeshParams>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, NavMeshParams_delete, _IDL);
static void finalize_NavMesh( pref<dtNavMesh>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(NavMesh_delete)( pref<dtNavMesh>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, NavMesh_delete, _IDL);
static void finalize_Node( pref<dtNode>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Node_delete)( pref<dtNode>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Node_delete, _IDL);
static void finalize_NodePool( pref<dtNodePool>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(NodePool_delete)( pref<dtNodePool>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, NodePool_delete, _IDL);
static void finalize_NodeQueue( pref<dtNodeQueue>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(NodeQueue_delete)( pref<dtNodeQueue>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, NodeQueue_delete, _IDL);
static void finalize_NavMeshCreateParams( pref<dtNavMeshCreateParams>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(NavMeshCreateParams_delete)( pref<dtNavMeshCreateParams>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, NavMeshCreateParams_delete, _IDL);
static void finalize_QueryFilter( pref<dtQueryFilter>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(QueryFilter_delete)( pref<dtQueryFilter>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, QueryFilter_delete, _IDL);
static void finalize_RaycastHit( pref<dtRaycastHit>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(RaycastHit_delete)( pref<dtRaycastHit>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, RaycastHit_delete, _IDL);
static void finalize_PolyQuery( pref<dtPolyQuery>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(PolyQuery_delete)( pref<dtPolyQuery>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, PolyQuery_delete, _IDL);
static void finalize_NavMeshQuery( pref<dtNavMeshQuery>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(NavMeshQuery_delete)( pref<dtNavMeshQuery>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, NavMeshQuery_delete, _IDL);
static void finalize_PerformanceTimer( pref<PerformanceTimer>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(PerformanceTimer_delete)( pref<PerformanceTimer>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, PerformanceTimer_delete, _IDL);
static void finalize_MeshCapture( pref<dtMeshCapture>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(MeshCapture_delete)( pref<dtMeshCapture>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, MeshCapture_delete, _IDL);
static void finalize_NavTileConverter( pref<NavTileConverter>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(NavTileConverter_delete)( pref<NavTileConverter>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, NavTileConverter_delete, _IDL);
HL_PRIM pref<rcChunkyTriMesh>* HL_NAME(ChunkyTriMesh_new0)() {
	return alloc_ref((new rcChunkyTriMesh()),ChunkyTriMesh);
}
DEFINE_PRIM(_IDL, ChunkyTriMesh_new0,);

HL_PRIM int HL_NAME(ChunkyTriMesh_get_maxTrisPerChunk)( pref<rcChunkyTriMesh>* _this ) {
	return _unref(_this)->maxTrisPerChunk;
}
DEFINE_PRIM(_I32,ChunkyTriMesh_get_maxTrisPerChunk,_IDL);
HL_PRIM int HL_NAME(ChunkyTriMesh_set_maxTrisPerChunk)( pref<rcChunkyTriMesh>* _this, int value ) {
	_unref(_this)->maxTrisPerChunk = (value);
	return value;
}
DEFINE_PRIM(_I32,ChunkyTriMesh_set_maxTrisPerChunk,_IDL _I32);

HL_PRIM int HL_NAME(ChunkyTriMesh_getNodeTriIndex1)(pref<rcChunkyTriMesh>* _this, int nodeIdx) {
	return (getNodeTriIndex( _unref(_this) , nodeIdx));
}
DEFINE_PRIM(_I32, ChunkyTriMesh_getNodeTriIndex1, _IDL _I32);

HL_PRIM int HL_NAME(ChunkyTriMesh_getNodeTriCount1)(pref<rcChunkyTriMesh>* _this, int nodeIdx) {
	return (getNodeTriCount( _unref(_this) , nodeIdx));
}
DEFINE_PRIM(_I32, ChunkyTriMesh_getNodeTriCount1, _IDL _I32);

HL_PRIM vbyte* HL_NAME(ChunkyTriMesh_getTriVertIndices1)(pref<rcChunkyTriMesh>* _this, int nodeTriIndex) {
	return (vbyte *)(getTriVertIndices( _unref(_this) , nodeTriIndex));
}
DEFINE_PRIM(_BYTES, ChunkyTriMesh_getTriVertIndices1, _IDL _I32);

HL_PRIM bool HL_NAME(ChunkyTriMesh_build4)(pref<rcChunkyTriMesh>* _this, varray* verts, varray* tris, int ntris, int trisPerChunk) {
	return (rcCreateChunkyTriMesh( _unref(_this) , hl_aptr(verts,HL_CONST float), hl_aptr(tris,HL_CONST int), ntris, trisPerChunk));
}
DEFINE_PRIM(_BOOL, ChunkyTriMesh_build4, _IDL _ARR _ARR _I32 _I32);

HL_PRIM int HL_NAME(ChunkyTriMesh_getOverlappingRect4)(pref<rcChunkyTriMesh>* _this, _hl_float2* bmin, _hl_float2* bmax, varray* ids, int maxIds) {
	return (rcGetChunksOverlappingRect( _unref(_this) , (float*)(_hl_float2*)bmin, (float*)(_hl_float2*)bmax, hl_aptr(ids,int), maxIds));
}
DEFINE_PRIM(_I32, ChunkyTriMesh_getOverlappingRect4, _IDL _STRUCT _STRUCT _ARR _I32);

HL_PRIM pref<rcContext>* HL_NAME(RasterContext_new1)(bool state) {
	return alloc_ref((new rcContext(state)),RasterContext);
}
DEFINE_PRIM(_IDL, RasterContext_new1, _BOOL);

HL_PRIM void HL_NAME(RasterContext_enableLog1)(pref<rcContext>* _this, bool state) {
	(_unref(_this)->enableLog(state));
}
DEFINE_PRIM(_VOID, RasterContext_enableLog1, _IDL _BOOL);

HL_PRIM void HL_NAME(RasterContext_resetLog0)(pref<rcContext>* _this) {
	(_unref(_this)->resetLog());
}
DEFINE_PRIM(_VOID, RasterContext_resetLog0, _IDL);

HL_PRIM void HL_NAME(RasterContext_enableTimer1)(pref<rcContext>* _this, bool state) {
	(_unref(_this)->enableTimer(state));
}
DEFINE_PRIM(_VOID, RasterContext_enableTimer1, _IDL _BOOL);

HL_PRIM void HL_NAME(RasterContext_resetTimers0)(pref<rcContext>* _this) {
	(_unref(_this)->resetTimers());
}
DEFINE_PRIM(_VOID, RasterContext_resetTimers0, _IDL);

HL_PRIM void HL_NAME(RasterContext_startTimer1)(pref<rcContext>* _this, int label) {
	(_unref(_this)->startTimer(TimerLabel__values[label]));
}
DEFINE_PRIM(_VOID, RasterContext_startTimer1, _IDL _I32);

HL_PRIM bool HL_NAME(RasterContext_createHeightfield7)(pref<rcContext>* _this, pref<rcHeightfield>* hf, int width, int height, _hl_float3* bmin, _hl_float3* bmax, float cs, float ch) {
	return (rcCreateHeightfield( _unref(_this) , *_unref_ptr_safe(hf), width, height, (float*)(_hl_float3*)bmin, (float*)(_hl_float3*)bmax, cs, ch));
}
DEFINE_PRIM(_BOOL, RasterContext_createHeightfield7, _IDL _IDL _I32 _I32 _STRUCT _STRUCT _F32 _F32);

HL_PRIM void HL_NAME(RasterContext_markWalkableTriangles6)(pref<rcContext>* _this, float walkableSlopeAngle, varray* verts, int nv, vbyte* tris, int nt, vbyte* areas) {
	(rcMarkWalkableTriangles( _unref(_this) , walkableSlopeAngle, hl_aptr(verts,HL_CONST float), nv, (int*)tris, nt, (unsigned char*)areas));
}
DEFINE_PRIM(_VOID, RasterContext_markWalkableTriangles6, _IDL _F32 _ARR _I32 _BYTES _I32 _BYTES);

HL_PRIM void HL_NAME(RasterContext_rcClearUnwalkableTriangles6)(pref<rcContext>* _this, float walkableSlopeAngle, varray* verts, int nv, vbyte* tris, int nt, vbyte* areas) {
	(rcClearUnwalkableTriangles( _unref(_this) , walkableSlopeAngle, hl_aptr(verts,HL_CONST float), nv, (int*)tris, nt, (unsigned char*)areas));
}
DEFINE_PRIM(_VOID, RasterContext_rcClearUnwalkableTriangles6, _IDL _F32 _ARR _I32 _BYTES _I32 _BYTES);

HL_PRIM bool HL_NAME(RasterContext_rcAddSpan7)(pref<rcContext>* _this, pref<rcHeightfield>* hf, int x, int y, int smin, int smax, int area, int flagMergeThr) {
	return (rcAddSpan( _unref(_this) , *_unref_ptr_safe(hf), x, y, smin, smax, area, flagMergeThr));
}
DEFINE_PRIM(_BOOL, RasterContext_rcAddSpan7, _IDL _IDL _I32 _I32 _I32 _I32 _I32 _I32);

HL_PRIM bool HL_NAME(RasterContext_rasterizeTriangle6)(pref<rcContext>* _this, _hl_float3* v0, _hl_float3* v1, _hl_float3* v2, int area, pref<rcHeightfield>* solid, int flagMergeThr) {
	return (rcRasterizeTriangle( _unref(_this) , (float*)(HL_CONST _hl_float3*)v0, (float*)(HL_CONST _hl_float3*)v1, (float*)(HL_CONST _hl_float3*)v2, area, *_unref_ptr_safe(solid), flagMergeThr));
}
DEFINE_PRIM(_BOOL, RasterContext_rasterizeTriangle6, _IDL _STRUCT _STRUCT _STRUCT _I32 _IDL _I32);

HL_PRIM bool HL_NAME(RasterContext_rasterizeTriangles7)(pref<rcContext>* _this, varray* verts, int nv, vbyte* tris, vbyte* areas, int nt, pref<rcHeightfield>* solid, int flagMergeThr) {
	return (rcRasterizeTriangles( _unref(_this) , hl_aptr(verts,HL_CONST float), nv, (int*)tris, (unsigned char*)areas, nt, *_unref_ptr_safe(solid), flagMergeThr));
}
DEFINE_PRIM(_BOOL, RasterContext_rasterizeTriangles7, _IDL _ARR _I32 _BYTES _BYTES _I32 _IDL _I32);

HL_PRIM bool HL_NAME(RasterContext_rasterizeTriangles5)(pref<rcContext>* _this, varray* verts, varray* areas, int nt, pref<rcHeightfield>* solid, int flagMergeThr) {
	return (rcRasterizeTriangles( _unref(_this) , hl_aptr(verts,HL_CONST float), hl_aptr(areas,HL_CONST unsigned char), nt, *_unref_ptr_safe(solid), flagMergeThr));
}
DEFINE_PRIM(_BOOL, RasterContext_rasterizeTriangles5, _IDL _ARR _ARR _I32 _IDL _I32);

HL_PRIM void HL_NAME(RasterContext_filterLowHangingWalkableObstacles2)(pref<rcContext>* _this, int walkableClimb, pref<rcHeightfield>* solid) {
	(rcFilterLowHangingWalkableObstacles( _unref(_this) , walkableClimb, *_unref_ptr_safe(solid)));
}
DEFINE_PRIM(_VOID, RasterContext_filterLowHangingWalkableObstacles2, _IDL _I32 _IDL);

HL_PRIM void HL_NAME(RasterContext_filterLedgeSpans3)(pref<rcContext>* _this, int walkableHeight, int walkableClimb, pref<rcHeightfield>* solid) {
	(rcFilterLedgeSpans( _unref(_this) , walkableHeight, walkableClimb, *_unref_ptr_safe(solid)));
}
DEFINE_PRIM(_VOID, RasterContext_filterLedgeSpans3, _IDL _I32 _I32 _IDL);

HL_PRIM void HL_NAME(RasterContext_filterWalkableLowHeightSpans2)(pref<rcContext>* _this, int walkableHeight, pref<rcHeightfield>* solid) {
	(rcFilterWalkableLowHeightSpans( _unref(_this) , walkableHeight, *_unref_ptr_safe(solid)));
}
DEFINE_PRIM(_VOID, RasterContext_filterWalkableLowHeightSpans2, _IDL _I32 _IDL);

HL_PRIM int HL_NAME(RasterContext_rcGetHeightFieldSpanCount1)(pref<rcContext>* _this, pref<rcHeightfield>* hf) {
	return (rcGetHeightFieldSpanCount( _unref(_this) , *_unref_ptr_safe(hf)));
}
DEFINE_PRIM(_I32, RasterContext_rcGetHeightFieldSpanCount1, _IDL _IDL);

HL_PRIM bool HL_NAME(RasterContext_buildCompactHeightfield4)(pref<rcContext>* _this, int walkableHeight, int walkableClimb, pref<rcHeightfield>* hf, pref<rcCompactHeightfield>* chf) {
	return (rcBuildCompactHeightfield( _unref(_this) , walkableHeight, walkableClimb, *_unref_ptr_safe(hf), *_unref_ptr_safe(chf)));
}
DEFINE_PRIM(_BOOL, RasterContext_buildCompactHeightfield4, _IDL _I32 _I32 _IDL _IDL);

HL_PRIM bool HL_NAME(RasterContext_erodeWalkableArea2)(pref<rcContext>* _this, int radius, pref<rcCompactHeightfield>* chf) {
	return (rcErodeWalkableArea( _unref(_this) , radius, *_unref_ptr_safe(chf)));
}
DEFINE_PRIM(_BOOL, RasterContext_erodeWalkableArea2, _IDL _I32 _IDL);

HL_PRIM bool HL_NAME(RasterContext_rcMedianFilterWalkableArea1)(pref<rcContext>* _this, pref<rcCompactHeightfield>* chf) {
	return (rcMedianFilterWalkableArea( _unref(_this) , *_unref_ptr_safe(chf)));
}
DEFINE_PRIM(_BOOL, RasterContext_rcMedianFilterWalkableArea1, _IDL _IDL);

HL_PRIM void HL_NAME(RasterContext_rcMarkBoxArea4)(pref<rcContext>* _this, _hl_float3* bmin, _hl_float3* bmax, unsigned char areaId, pref<rcCompactHeightfield>* chf) {
	(rcMarkBoxArea( _unref(_this) , (float*)(_hl_float3*)bmin, (float*)(_hl_float3*)bmax, areaId, *_unref_ptr_safe(chf)));
}
DEFINE_PRIM(_VOID, RasterContext_rcMarkBoxArea4, _IDL _STRUCT _STRUCT _I8 _IDL);

HL_PRIM void HL_NAME(RasterContext_rcMarkConvexPolyArea6)(pref<rcContext>* _this, _hl_float3* verts, int nverts, float hmin, float hmax, unsigned char areaId, pref<rcCompactHeightfield>* chf) {
	(rcMarkConvexPolyArea( _unref(_this) , (float*)(_hl_float3*)verts, nverts, hmin, hmax, areaId, *_unref_ptr_safe(chf)));
}
DEFINE_PRIM(_VOID, RasterContext_rcMarkConvexPolyArea6, _IDL _STRUCT _I32 _F32 _F32 _I8 _IDL);

HL_PRIM void HL_NAME(RasterContext_rcMarkCylinderArea5)(pref<rcContext>* _this, _hl_float3* pos, float r, float h, unsigned char areaId, pref<rcCompactHeightfield>* chf) {
	(rcMarkCylinderArea( _unref(_this) , (float*)(_hl_float3*)pos, r, h, areaId, *_unref_ptr_safe(chf)));
}
DEFINE_PRIM(_VOID, RasterContext_rcMarkCylinderArea5, _IDL _STRUCT _F32 _F32 _I8 _IDL);

HL_PRIM bool HL_NAME(RasterContext_rcBuildDistanceField1)(pref<rcContext>* _this, pref<rcCompactHeightfield>* chf) {
	return (rcBuildDistanceField( _unref(_this) , *_unref_ptr_safe(chf)));
}
DEFINE_PRIM(_BOOL, RasterContext_rcBuildDistanceField1, _IDL _IDL);

HL_PRIM bool HL_NAME(RasterContext_rcBuildRegions4)(pref<rcContext>* _this, pref<rcCompactHeightfield>* chf, int borderSize, int minRegionArea, int mergeRegionArea) {
	return (rcBuildRegions( _unref(_this) , *_unref_ptr_safe(chf), borderSize, minRegionArea, mergeRegionArea));
}
DEFINE_PRIM(_BOOL, RasterContext_rcBuildRegions4, _IDL _IDL _I32 _I32 _I32);

HL_PRIM bool HL_NAME(RasterContext_rcBuildLayerRegions3)(pref<rcContext>* _this, pref<rcCompactHeightfield>* chf, int borderSize, int minRegionArea) {
	return (rcBuildLayerRegions( _unref(_this) , *_unref_ptr_safe(chf), borderSize, minRegionArea));
}
DEFINE_PRIM(_BOOL, RasterContext_rcBuildLayerRegions3, _IDL _IDL _I32 _I32);

HL_PRIM bool HL_NAME(RasterContext_rcBuildRegionsMonotone4)(pref<rcContext>* _this, pref<rcCompactHeightfield>* chf, int borderSize, int minRegionArea, int mergeRegionArea) {
	return (rcBuildRegionsMonotone( _unref(_this) , *_unref_ptr_safe(chf), borderSize, minRegionArea, mergeRegionArea));
}
DEFINE_PRIM(_BOOL, RasterContext_rcBuildRegionsMonotone4, _IDL _IDL _I32 _I32 _I32);

HL_PRIM bool HL_NAME(RasterContext_rcBuildContours5)(pref<rcContext>* _this, pref<rcCompactHeightfield>* chf, float maxError, int maxEdgeLen, pref<rcContourSet>* cset, int buildFlags) {
	return (rcBuildContours( _unref(_this) , *_unref_ptr_safe(chf), maxError, maxEdgeLen, *_unref_ptr_safe(cset), buildFlags));
}
DEFINE_PRIM(_BOOL, RasterContext_rcBuildContours5, _IDL _IDL _F32 _I32 _IDL _I32);

HL_PRIM bool HL_NAME(RasterContext_rcBuildPolyMesh3)(pref<rcContext>* _this, pref<rcContourSet>* cset, int nvp, pref<rcPolyMesh>* mesh) {
	return (rcBuildPolyMesh( _unref(_this) , *_unref_ptr_safe(cset), nvp, *_unref_ptr_safe(mesh)));
}
DEFINE_PRIM(_BOOL, RasterContext_rcBuildPolyMesh3, _IDL _IDL _I32 _IDL);

HL_PRIM bool HL_NAME(RasterContext_rcBuildPolyMeshDetail5)(pref<rcContext>* _this, pref<rcPolyMesh>* mesh, pref<rcCompactHeightfield>* chf, float sampleDist, float sampleMaxError, pref<rcPolyMeshDetail>* dmesh) {
	return (rcBuildPolyMeshDetail( _unref(_this) , *_unref_ptr_safe(mesh), *_unref_ptr_safe(chf), sampleDist, sampleMaxError, *_unref_ptr_safe(dmesh)));
}
DEFINE_PRIM(_BOOL, RasterContext_rcBuildPolyMeshDetail5, _IDL _IDL _IDL _F32 _F32 _IDL);

HL_PRIM bool HL_NAME(RasterContext_buildHeightfieldLayers4)(pref<rcContext>* _this, pref<rcCompactHeightfield>* chf, int borderSize, int walkableHeight, pref<rcHeightfieldLayerSet>* lset) {
	return (rcBuildHeightfieldLayers( _unref(_this) , *_unref_ptr_safe(chf), borderSize, walkableHeight, *_unref_ptr_safe(lset)));
}
DEFINE_PRIM(_BOOL, RasterContext_buildHeightfieldLayers4, _IDL _IDL _I32 _I32 _IDL);

HL_PRIM pref<rcScopedTimer>* HL_NAME(rcScopedTimer_new2)(pref<rcContext>* ctx, int label) {
	return alloc_ref((new rcScopedTimer(_unref_ptr_safe(ctx), TimerLabel__values[label])),rcScopedTimer);
}
DEFINE_PRIM(_IDL, rcScopedTimer_new2, _IDL _I32);

HL_PRIM pref<rcConfig>* HL_NAME(RasterConfig_new0)() {
	return alloc_ref((new rcConfig()),RasterConfig);
}
DEFINE_PRIM(_IDL, RasterConfig_new0,);

HL_PRIM void HL_NAME(RasterConfig_copy1)(pref<rcConfig>* _this, pref<rcConfig>* b) {
	(rcConfigCopy( _unref(_this) , _unref_ptr_safe(b)));
}
DEFINE_PRIM(_VOID, RasterConfig_copy1, _IDL _IDL);

HL_PRIM int HL_NAME(RasterConfig_get_width)( pref<rcConfig>* _this ) {
	return _unref(_this)->width;
}
DEFINE_PRIM(_I32,RasterConfig_get_width,_IDL);
HL_PRIM int HL_NAME(RasterConfig_set_width)( pref<rcConfig>* _this, int value ) {
	_unref(_this)->width = (value);
	return value;
}
DEFINE_PRIM(_I32,RasterConfig_set_width,_IDL _I32);

HL_PRIM int HL_NAME(RasterConfig_get_height)( pref<rcConfig>* _this ) {
	return _unref(_this)->height;
}
DEFINE_PRIM(_I32,RasterConfig_get_height,_IDL);
HL_PRIM int HL_NAME(RasterConfig_set_height)( pref<rcConfig>* _this, int value ) {
	_unref(_this)->height = (value);
	return value;
}
DEFINE_PRIM(_I32,RasterConfig_set_height,_IDL _I32);

HL_PRIM int HL_NAME(RasterConfig_get_tileSize)( pref<rcConfig>* _this ) {
	return _unref(_this)->tileSize;
}
DEFINE_PRIM(_I32,RasterConfig_get_tileSize,_IDL);
HL_PRIM int HL_NAME(RasterConfig_set_tileSize)( pref<rcConfig>* _this, int value ) {
	_unref(_this)->tileSize = (value);
	return value;
}
DEFINE_PRIM(_I32,RasterConfig_set_tileSize,_IDL _I32);

HL_PRIM int HL_NAME(RasterConfig_get_borderSize)( pref<rcConfig>* _this ) {
	return _unref(_this)->borderSize;
}
DEFINE_PRIM(_I32,RasterConfig_get_borderSize,_IDL);
HL_PRIM int HL_NAME(RasterConfig_set_borderSize)( pref<rcConfig>* _this, int value ) {
	_unref(_this)->borderSize = (value);
	return value;
}
DEFINE_PRIM(_I32,RasterConfig_set_borderSize,_IDL _I32);

HL_PRIM float HL_NAME(RasterConfig_get_cs)( pref<rcConfig>* _this ) {
	return _unref(_this)->cs;
}
DEFINE_PRIM(_F32,RasterConfig_get_cs,_IDL);
HL_PRIM float HL_NAME(RasterConfig_set_cs)( pref<rcConfig>* _this, float value ) {
	_unref(_this)->cs = (value);
	return value;
}
DEFINE_PRIM(_F32,RasterConfig_set_cs,_IDL _F32);

HL_PRIM float HL_NAME(RasterConfig_get_ch)( pref<rcConfig>* _this ) {
	return _unref(_this)->ch;
}
DEFINE_PRIM(_F32,RasterConfig_get_ch,_IDL);
HL_PRIM float HL_NAME(RasterConfig_set_ch)( pref<rcConfig>* _this, float value ) {
	_unref(_this)->ch = (value);
	return value;
}
DEFINE_PRIM(_F32,RasterConfig_set_ch,_IDL _F32);

HL_PRIM _hl_float3* HL_NAME(RasterConfig_get_bmin)( pref<rcConfig>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmin);
}
HL_PRIM void HL_NAME(RasterConfig_getbminv)( pref<rcConfig>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmin)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,RasterConfig_getbminv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,RasterConfig_get_bmin,_IDL);
HL_PRIM _hl_float3* HL_NAME(RasterConfig_set_bmin)( pref<rcConfig>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmin)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(RasterConfig_setbmin3)( pref<rcConfig>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,RasterConfig_setbmin3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,RasterConfig_set_bmin,_IDL _STRUCT);

HL_PRIM _hl_float3* HL_NAME(RasterConfig_get_bmax)( pref<rcConfig>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmax);
}
HL_PRIM void HL_NAME(RasterConfig_getbmaxv)( pref<rcConfig>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmax)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,RasterConfig_getbmaxv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,RasterConfig_get_bmax,_IDL);
HL_PRIM _hl_float3* HL_NAME(RasterConfig_set_bmax)( pref<rcConfig>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmax)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(RasterConfig_setbmax3)( pref<rcConfig>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,RasterConfig_setbmax3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,RasterConfig_set_bmax,_IDL _STRUCT);

HL_PRIM float HL_NAME(RasterConfig_get_walkableSlopeAngle)( pref<rcConfig>* _this ) {
	return _unref(_this)->walkableSlopeAngle;
}
DEFINE_PRIM(_F32,RasterConfig_get_walkableSlopeAngle,_IDL);
HL_PRIM float HL_NAME(RasterConfig_set_walkableSlopeAngle)( pref<rcConfig>* _this, float value ) {
	_unref(_this)->walkableSlopeAngle = (value);
	return value;
}
DEFINE_PRIM(_F32,RasterConfig_set_walkableSlopeAngle,_IDL _F32);

HL_PRIM int HL_NAME(RasterConfig_get_walkableHeight)( pref<rcConfig>* _this ) {
	return _unref(_this)->walkableHeight;
}
DEFINE_PRIM(_I32,RasterConfig_get_walkableHeight,_IDL);
HL_PRIM int HL_NAME(RasterConfig_set_walkableHeight)( pref<rcConfig>* _this, int value ) {
	_unref(_this)->walkableHeight = (value);
	return value;
}
DEFINE_PRIM(_I32,RasterConfig_set_walkableHeight,_IDL _I32);

HL_PRIM int HL_NAME(RasterConfig_get_walkableClimb)( pref<rcConfig>* _this ) {
	return _unref(_this)->walkableClimb;
}
DEFINE_PRIM(_I32,RasterConfig_get_walkableClimb,_IDL);
HL_PRIM int HL_NAME(RasterConfig_set_walkableClimb)( pref<rcConfig>* _this, int value ) {
	_unref(_this)->walkableClimb = (value);
	return value;
}
DEFINE_PRIM(_I32,RasterConfig_set_walkableClimb,_IDL _I32);

HL_PRIM int HL_NAME(RasterConfig_get_walkableRadius)( pref<rcConfig>* _this ) {
	return _unref(_this)->walkableRadius;
}
DEFINE_PRIM(_I32,RasterConfig_get_walkableRadius,_IDL);
HL_PRIM int HL_NAME(RasterConfig_set_walkableRadius)( pref<rcConfig>* _this, int value ) {
	_unref(_this)->walkableRadius = (value);
	return value;
}
DEFINE_PRIM(_I32,RasterConfig_set_walkableRadius,_IDL _I32);

HL_PRIM int HL_NAME(RasterConfig_get_maxEdgeLen)( pref<rcConfig>* _this ) {
	return _unref(_this)->maxEdgeLen;
}
DEFINE_PRIM(_I32,RasterConfig_get_maxEdgeLen,_IDL);
HL_PRIM int HL_NAME(RasterConfig_set_maxEdgeLen)( pref<rcConfig>* _this, int value ) {
	_unref(_this)->maxEdgeLen = (value);
	return value;
}
DEFINE_PRIM(_I32,RasterConfig_set_maxEdgeLen,_IDL _I32);

HL_PRIM float HL_NAME(RasterConfig_get_maxSimplificationError)( pref<rcConfig>* _this ) {
	return _unref(_this)->maxSimplificationError;
}
DEFINE_PRIM(_F32,RasterConfig_get_maxSimplificationError,_IDL);
HL_PRIM float HL_NAME(RasterConfig_set_maxSimplificationError)( pref<rcConfig>* _this, float value ) {
	_unref(_this)->maxSimplificationError = (value);
	return value;
}
DEFINE_PRIM(_F32,RasterConfig_set_maxSimplificationError,_IDL _F32);

HL_PRIM int HL_NAME(RasterConfig_get_minRegionArea)( pref<rcConfig>* _this ) {
	return _unref(_this)->minRegionArea;
}
DEFINE_PRIM(_I32,RasterConfig_get_minRegionArea,_IDL);
HL_PRIM int HL_NAME(RasterConfig_set_minRegionArea)( pref<rcConfig>* _this, int value ) {
	_unref(_this)->minRegionArea = (value);
	return value;
}
DEFINE_PRIM(_I32,RasterConfig_set_minRegionArea,_IDL _I32);

HL_PRIM int HL_NAME(RasterConfig_get_mergeRegionArea)( pref<rcConfig>* _this ) {
	return _unref(_this)->mergeRegionArea;
}
DEFINE_PRIM(_I32,RasterConfig_get_mergeRegionArea,_IDL);
HL_PRIM int HL_NAME(RasterConfig_set_mergeRegionArea)( pref<rcConfig>* _this, int value ) {
	_unref(_this)->mergeRegionArea = (value);
	return value;
}
DEFINE_PRIM(_I32,RasterConfig_set_mergeRegionArea,_IDL _I32);

HL_PRIM int HL_NAME(RasterConfig_get_maxVertsPerPoly)( pref<rcConfig>* _this ) {
	return _unref(_this)->maxVertsPerPoly;
}
DEFINE_PRIM(_I32,RasterConfig_get_maxVertsPerPoly,_IDL);
HL_PRIM int HL_NAME(RasterConfig_set_maxVertsPerPoly)( pref<rcConfig>* _this, int value ) {
	_unref(_this)->maxVertsPerPoly = (value);
	return value;
}
DEFINE_PRIM(_I32,RasterConfig_set_maxVertsPerPoly,_IDL _I32);

HL_PRIM float HL_NAME(RasterConfig_get_detailSampleDist)( pref<rcConfig>* _this ) {
	return _unref(_this)->detailSampleDist;
}
DEFINE_PRIM(_F32,RasterConfig_get_detailSampleDist,_IDL);
HL_PRIM float HL_NAME(RasterConfig_set_detailSampleDist)( pref<rcConfig>* _this, float value ) {
	_unref(_this)->detailSampleDist = (value);
	return value;
}
DEFINE_PRIM(_F32,RasterConfig_set_detailSampleDist,_IDL _F32);

HL_PRIM float HL_NAME(RasterConfig_get_detailSampleMaxError)( pref<rcConfig>* _this ) {
	return _unref(_this)->detailSampleMaxError;
}
DEFINE_PRIM(_F32,RasterConfig_get_detailSampleMaxError,_IDL);
HL_PRIM float HL_NAME(RasterConfig_set_detailSampleMaxError)( pref<rcConfig>* _this, float value ) {
	_unref(_this)->detailSampleMaxError = (value);
	return value;
}
DEFINE_PRIM(_F32,RasterConfig_set_detailSampleMaxError,_IDL _F32);

HL_PRIM int HL_NAME(Span_get_smin)( pref<rcSpan>* _this ) {
	return _unref(_this)->smin;
}
DEFINE_PRIM(_I32,Span_get_smin,_IDL);
HL_PRIM int HL_NAME(Span_set_smin)( pref<rcSpan>* _this, int value ) {
	_unref(_this)->smin = (value);
	return value;
}
DEFINE_PRIM(_I32,Span_set_smin,_IDL _I32);

HL_PRIM int HL_NAME(Span_get_smax)( pref<rcSpan>* _this ) {
	return _unref(_this)->smax;
}
DEFINE_PRIM(_I32,Span_get_smax,_IDL);
HL_PRIM int HL_NAME(Span_set_smax)( pref<rcSpan>* _this, int value ) {
	_unref(_this)->smax = (value);
	return value;
}
DEFINE_PRIM(_I32,Span_set_smax,_IDL _I32);

HL_PRIM int HL_NAME(Span_get_area)( pref<rcSpan>* _this ) {
	return _unref(_this)->area;
}
DEFINE_PRIM(_I32,Span_get_area,_IDL);
HL_PRIM int HL_NAME(Span_set_area)( pref<rcSpan>* _this, int value ) {
	_unref(_this)->area = (value);
	return value;
}
DEFINE_PRIM(_I32,Span_set_area,_IDL _I32);

HL_PRIM pref<rcSpan>* HL_NAME(Span_get_next)( pref<rcSpan>* _this ) {
	return alloc_ref(_unref(_this)->next,Span);
}
DEFINE_PRIM(_IDL,Span_get_next,_IDL);
HL_PRIM pref<rcSpan>* HL_NAME(Span_set_next)( pref<rcSpan>* _this, pref<rcSpan>* value ) {
	_unref(_this)->next = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,Span_set_next,_IDL _IDL);

HL_PRIM pref<rcSpanPool>* HL_NAME(SpanPool_get_next)( pref<rcSpanPool>* _this ) {
	return alloc_ref(_unref(_this)->next,SpanPool);
}
DEFINE_PRIM(_IDL,SpanPool_get_next,_IDL);
HL_PRIM pref<rcSpanPool>* HL_NAME(SpanPool_set_next)( pref<rcSpanPool>* _this, pref<rcSpanPool>* value ) {
	_unref(_this)->next = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,SpanPool_set_next,_IDL _IDL);

HL_PRIM pref<rcHeightfield>* HL_NAME(Heightfield_new0)() {
	return alloc_ref((new rcHeightfield()),Heightfield);
}
DEFINE_PRIM(_IDL, Heightfield_new0,);

HL_PRIM int HL_NAME(Heightfield_get_width)( pref<rcHeightfield>* _this ) {
	return _unref(_this)->width;
}
DEFINE_PRIM(_I32,Heightfield_get_width,_IDL);
HL_PRIM int HL_NAME(Heightfield_set_width)( pref<rcHeightfield>* _this, int value ) {
	_unref(_this)->width = (value);
	return value;
}
DEFINE_PRIM(_I32,Heightfield_set_width,_IDL _I32);

HL_PRIM int HL_NAME(Heightfield_get_height)( pref<rcHeightfield>* _this ) {
	return _unref(_this)->height;
}
DEFINE_PRIM(_I32,Heightfield_get_height,_IDL);
HL_PRIM int HL_NAME(Heightfield_set_height)( pref<rcHeightfield>* _this, int value ) {
	_unref(_this)->height = (value);
	return value;
}
DEFINE_PRIM(_I32,Heightfield_set_height,_IDL _I32);

HL_PRIM _hl_float3* HL_NAME(Heightfield_get_bmin)( pref<rcHeightfield>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmin);
}
HL_PRIM void HL_NAME(Heightfield_getbminv)( pref<rcHeightfield>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmin)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,Heightfield_getbminv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,Heightfield_get_bmin,_IDL);
HL_PRIM _hl_float3* HL_NAME(Heightfield_set_bmin)( pref<rcHeightfield>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmin)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(Heightfield_setbmin3)( pref<rcHeightfield>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,Heightfield_setbmin3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,Heightfield_set_bmin,_IDL _STRUCT);

HL_PRIM _hl_float3* HL_NAME(Heightfield_get_bmax)( pref<rcHeightfield>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmax);
}
HL_PRIM void HL_NAME(Heightfield_getbmaxv)( pref<rcHeightfield>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmax)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,Heightfield_getbmaxv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,Heightfield_get_bmax,_IDL);
HL_PRIM _hl_float3* HL_NAME(Heightfield_set_bmax)( pref<rcHeightfield>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmax)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(Heightfield_setbmax3)( pref<rcHeightfield>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,Heightfield_setbmax3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,Heightfield_set_bmax,_IDL _STRUCT);

HL_PRIM float HL_NAME(Heightfield_get_cs)( pref<rcHeightfield>* _this ) {
	return _unref(_this)->cs;
}
DEFINE_PRIM(_F32,Heightfield_get_cs,_IDL);
HL_PRIM float HL_NAME(Heightfield_set_cs)( pref<rcHeightfield>* _this, float value ) {
	_unref(_this)->cs = (value);
	return value;
}
DEFINE_PRIM(_F32,Heightfield_set_cs,_IDL _F32);

HL_PRIM float HL_NAME(Heightfield_get_ch)( pref<rcHeightfield>* _this ) {
	return _unref(_this)->ch;
}
DEFINE_PRIM(_F32,Heightfield_get_ch,_IDL);
HL_PRIM float HL_NAME(Heightfield_set_ch)( pref<rcHeightfield>* _this, float value ) {
	_unref(_this)->ch = (value);
	return value;
}
DEFINE_PRIM(_F32,Heightfield_set_ch,_IDL _F32);

HL_PRIM pref<rcSpanPool>* HL_NAME(Heightfield_get_pools)( pref<rcHeightfield>* _this ) {
	return alloc_ref(_unref(_this)->pools,SpanPool);
}
DEFINE_PRIM(_IDL,Heightfield_get_pools,_IDL);
HL_PRIM pref<rcSpanPool>* HL_NAME(Heightfield_set_pools)( pref<rcHeightfield>* _this, pref<rcSpanPool>* value ) {
	_unref(_this)->pools = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,Heightfield_set_pools,_IDL _IDL);

HL_PRIM pref<rcSpan>* HL_NAME(Heightfield_get_freelist)( pref<rcHeightfield>* _this ) {
	return alloc_ref(_unref(_this)->freelist,Span);
}
DEFINE_PRIM(_IDL,Heightfield_get_freelist,_IDL);
HL_PRIM pref<rcSpan>* HL_NAME(Heightfield_set_freelist)( pref<rcHeightfield>* _this, pref<rcSpan>* value ) {
	_unref(_this)->freelist = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,Heightfield_set_freelist,_IDL _IDL);

HL_PRIM bool HL_NAME(Heightfield_spanIsValidAt1)(pref<rcHeightfield>* _this, int pos) {
	return (rcHeightfield_rcSpanIsValidAt( _unref(_this) , pos));
}
DEFINE_PRIM(_BOOL, Heightfield_spanIsValidAt1, _IDL _I32);

HL_PRIM pref<rcSpan>* HL_NAME(Heightfield_spanAt1)(pref<rcHeightfield>* _this, int pos) {
	return alloc_ref((rcHeightfield_rcSpanAt( _unref(_this) , pos)),Span);
}
DEFINE_PRIM(_IDL, Heightfield_spanAt1, _IDL _I32);

HL_PRIM int HL_NAME(rcCompactCell_get_index)( pref<rcCompactCell>* _this ) {
	return _unref(_this)->index;
}
DEFINE_PRIM(_I32,rcCompactCell_get_index,_IDL);
HL_PRIM int HL_NAME(rcCompactCell_set_index)( pref<rcCompactCell>* _this, int value ) {
	_unref(_this)->index = (value);
	return value;
}
DEFINE_PRIM(_I32,rcCompactCell_set_index,_IDL _I32);

HL_PRIM int HL_NAME(rcCompactCell_get_count)( pref<rcCompactCell>* _this ) {
	return _unref(_this)->count;
}
DEFINE_PRIM(_I32,rcCompactCell_get_count,_IDL);
HL_PRIM int HL_NAME(rcCompactCell_set_count)( pref<rcCompactCell>* _this, int value ) {
	_unref(_this)->count = (value);
	return value;
}
DEFINE_PRIM(_I32,rcCompactCell_set_count,_IDL _I32);

HL_PRIM unsigned short HL_NAME(rcCompactSpan_get_y)( pref<rcCompactSpan>* _this ) {
	return _unref(_this)->y;
}
DEFINE_PRIM(_I16,rcCompactSpan_get_y,_IDL);
HL_PRIM unsigned short HL_NAME(rcCompactSpan_set_y)( pref<rcCompactSpan>* _this, unsigned short value ) {
	_unref(_this)->y = (value);
	return value;
}
DEFINE_PRIM(_I16,rcCompactSpan_set_y,_IDL _I16);

HL_PRIM unsigned short HL_NAME(rcCompactSpan_get_reg)( pref<rcCompactSpan>* _this ) {
	return _unref(_this)->reg;
}
DEFINE_PRIM(_I16,rcCompactSpan_get_reg,_IDL);
HL_PRIM unsigned short HL_NAME(rcCompactSpan_set_reg)( pref<rcCompactSpan>* _this, unsigned short value ) {
	_unref(_this)->reg = (value);
	return value;
}
DEFINE_PRIM(_I16,rcCompactSpan_set_reg,_IDL _I16);

HL_PRIM int HL_NAME(rcCompactSpan_get_con)( pref<rcCompactSpan>* _this ) {
	return _unref(_this)->con;
}
DEFINE_PRIM(_I32,rcCompactSpan_get_con,_IDL);
HL_PRIM int HL_NAME(rcCompactSpan_set_con)( pref<rcCompactSpan>* _this, int value ) {
	_unref(_this)->con = (value);
	return value;
}
DEFINE_PRIM(_I32,rcCompactSpan_set_con,_IDL _I32);

HL_PRIM int HL_NAME(rcCompactSpan_get_h)( pref<rcCompactSpan>* _this ) {
	return _unref(_this)->h;
}
DEFINE_PRIM(_I32,rcCompactSpan_get_h,_IDL);
HL_PRIM int HL_NAME(rcCompactSpan_set_h)( pref<rcCompactSpan>* _this, int value ) {
	_unref(_this)->h = (value);
	return value;
}
DEFINE_PRIM(_I32,rcCompactSpan_set_h,_IDL _I32);

HL_PRIM pref<rcCompactHeightfield>* HL_NAME(CompactHeightfield_new0)() {
	return alloc_ref((new rcCompactHeightfield()),CompactHeightfield);
}
DEFINE_PRIM(_IDL, CompactHeightfield_new0,);

HL_PRIM int HL_NAME(CompactHeightfield_get_width)( pref<rcCompactHeightfield>* _this ) {
	return _unref(_this)->width;
}
DEFINE_PRIM(_I32,CompactHeightfield_get_width,_IDL);
HL_PRIM int HL_NAME(CompactHeightfield_set_width)( pref<rcCompactHeightfield>* _this, int value ) {
	_unref(_this)->width = (value);
	return value;
}
DEFINE_PRIM(_I32,CompactHeightfield_set_width,_IDL _I32);

HL_PRIM int HL_NAME(CompactHeightfield_get_height)( pref<rcCompactHeightfield>* _this ) {
	return _unref(_this)->height;
}
DEFINE_PRIM(_I32,CompactHeightfield_get_height,_IDL);
HL_PRIM int HL_NAME(CompactHeightfield_set_height)( pref<rcCompactHeightfield>* _this, int value ) {
	_unref(_this)->height = (value);
	return value;
}
DEFINE_PRIM(_I32,CompactHeightfield_set_height,_IDL _I32);

HL_PRIM int HL_NAME(CompactHeightfield_get_spanCount)( pref<rcCompactHeightfield>* _this ) {
	return _unref(_this)->spanCount;
}
DEFINE_PRIM(_I32,CompactHeightfield_get_spanCount,_IDL);
HL_PRIM int HL_NAME(CompactHeightfield_set_spanCount)( pref<rcCompactHeightfield>* _this, int value ) {
	_unref(_this)->spanCount = (value);
	return value;
}
DEFINE_PRIM(_I32,CompactHeightfield_set_spanCount,_IDL _I32);

HL_PRIM int HL_NAME(CompactHeightfield_get_walkableHeight)( pref<rcCompactHeightfield>* _this ) {
	return _unref(_this)->walkableHeight;
}
DEFINE_PRIM(_I32,CompactHeightfield_get_walkableHeight,_IDL);
HL_PRIM int HL_NAME(CompactHeightfield_set_walkableHeight)( pref<rcCompactHeightfield>* _this, int value ) {
	_unref(_this)->walkableHeight = (value);
	return value;
}
DEFINE_PRIM(_I32,CompactHeightfield_set_walkableHeight,_IDL _I32);

HL_PRIM int HL_NAME(CompactHeightfield_get_walkableClimb)( pref<rcCompactHeightfield>* _this ) {
	return _unref(_this)->walkableClimb;
}
DEFINE_PRIM(_I32,CompactHeightfield_get_walkableClimb,_IDL);
HL_PRIM int HL_NAME(CompactHeightfield_set_walkableClimb)( pref<rcCompactHeightfield>* _this, int value ) {
	_unref(_this)->walkableClimb = (value);
	return value;
}
DEFINE_PRIM(_I32,CompactHeightfield_set_walkableClimb,_IDL _I32);

HL_PRIM int HL_NAME(CompactHeightfield_get_borderSize)( pref<rcCompactHeightfield>* _this ) {
	return _unref(_this)->borderSize;
}
DEFINE_PRIM(_I32,CompactHeightfield_get_borderSize,_IDL);
HL_PRIM int HL_NAME(CompactHeightfield_set_borderSize)( pref<rcCompactHeightfield>* _this, int value ) {
	_unref(_this)->borderSize = (value);
	return value;
}
DEFINE_PRIM(_I32,CompactHeightfield_set_borderSize,_IDL _I32);

HL_PRIM int HL_NAME(CompactHeightfield_get_maxDistance)( pref<rcCompactHeightfield>* _this ) {
	return _unref(_this)->maxDistance;
}
DEFINE_PRIM(_I32,CompactHeightfield_get_maxDistance,_IDL);
HL_PRIM int HL_NAME(CompactHeightfield_set_maxDistance)( pref<rcCompactHeightfield>* _this, int value ) {
	_unref(_this)->maxDistance = (value);
	return value;
}
DEFINE_PRIM(_I32,CompactHeightfield_set_maxDistance,_IDL _I32);

HL_PRIM int HL_NAME(CompactHeightfield_get_maxRegions)( pref<rcCompactHeightfield>* _this ) {
	return _unref(_this)->maxRegions;
}
DEFINE_PRIM(_I32,CompactHeightfield_get_maxRegions,_IDL);
HL_PRIM int HL_NAME(CompactHeightfield_set_maxRegions)( pref<rcCompactHeightfield>* _this, int value ) {
	_unref(_this)->maxRegions = (value);
	return value;
}
DEFINE_PRIM(_I32,CompactHeightfield_set_maxRegions,_IDL _I32);

HL_PRIM _hl_float3* HL_NAME(CompactHeightfield_get_bmin)( pref<rcCompactHeightfield>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmin);
}
HL_PRIM void HL_NAME(CompactHeightfield_getbminv)( pref<rcCompactHeightfield>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmin)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,CompactHeightfield_getbminv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,CompactHeightfield_get_bmin,_IDL);
HL_PRIM _hl_float3* HL_NAME(CompactHeightfield_set_bmin)( pref<rcCompactHeightfield>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmin)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(CompactHeightfield_setbmin3)( pref<rcCompactHeightfield>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,CompactHeightfield_setbmin3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,CompactHeightfield_set_bmin,_IDL _STRUCT);

HL_PRIM _hl_float3* HL_NAME(CompactHeightfield_get_bmax)( pref<rcCompactHeightfield>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmax);
}
HL_PRIM void HL_NAME(CompactHeightfield_getbmaxv)( pref<rcCompactHeightfield>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmax)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,CompactHeightfield_getbmaxv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,CompactHeightfield_get_bmax,_IDL);
HL_PRIM _hl_float3* HL_NAME(CompactHeightfield_set_bmax)( pref<rcCompactHeightfield>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmax)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(CompactHeightfield_setbmax3)( pref<rcCompactHeightfield>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,CompactHeightfield_setbmax3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,CompactHeightfield_set_bmax,_IDL _STRUCT);

HL_PRIM float HL_NAME(CompactHeightfield_get_cs)( pref<rcCompactHeightfield>* _this ) {
	return _unref(_this)->cs;
}
DEFINE_PRIM(_F32,CompactHeightfield_get_cs,_IDL);
HL_PRIM float HL_NAME(CompactHeightfield_set_cs)( pref<rcCompactHeightfield>* _this, float value ) {
	_unref(_this)->cs = (value);
	return value;
}
DEFINE_PRIM(_F32,CompactHeightfield_set_cs,_IDL _F32);

HL_PRIM float HL_NAME(CompactHeightfield_get_ch)( pref<rcCompactHeightfield>* _this ) {
	return _unref(_this)->ch;
}
DEFINE_PRIM(_F32,CompactHeightfield_get_ch,_IDL);
HL_PRIM float HL_NAME(CompactHeightfield_set_ch)( pref<rcCompactHeightfield>* _this, float value ) {
	_unref(_this)->ch = (value);
	return value;
}
DEFINE_PRIM(_F32,CompactHeightfield_set_ch,_IDL _F32);

HL_PRIM pref<rcCompactCell>* HL_NAME(CompactHeightfield_get_cells)( pref<rcCompactHeightfield>* _this ) {
	return alloc_ref(_unref(_this)->cells,rcCompactCell);
}
DEFINE_PRIM(_IDL,CompactHeightfield_get_cells,_IDL);
HL_PRIM pref<rcCompactCell>* HL_NAME(CompactHeightfield_set_cells)( pref<rcCompactHeightfield>* _this, pref<rcCompactCell>* value ) {
	_unref(_this)->cells = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,CompactHeightfield_set_cells,_IDL _IDL);

HL_PRIM pref<rcCompactSpan>* HL_NAME(CompactHeightfield_get_spans)( pref<rcCompactHeightfield>* _this ) {
	return alloc_ref(_unref(_this)->spans,rcCompactSpan);
}
DEFINE_PRIM(_IDL,CompactHeightfield_get_spans,_IDL);
HL_PRIM pref<rcCompactSpan>* HL_NAME(CompactHeightfield_set_spans)( pref<rcCompactHeightfield>* _this, pref<rcCompactSpan>* value ) {
	_unref(_this)->spans = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,CompactHeightfield_set_spans,_IDL _IDL);

HL_PRIM vbyte* HL_NAME(CompactHeightfield_get_dist)( pref<rcCompactHeightfield>* _this ) {
	return (vbyte *)(&_unref(_this)->dist[0]);
}
DEFINE_PRIM(_BYTES,CompactHeightfield_get_dist,_IDL);
HL_PRIM vbyte* HL_NAME(CompactHeightfield_set_dist)( pref<rcCompactHeightfield>* _this, vbyte* value ) {
	_unref(_this)->dist = (unsigned short*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,CompactHeightfield_set_dist,_IDL _BYTES);

HL_PRIM vbyte* HL_NAME(CompactHeightfield_get_areas)( pref<rcCompactHeightfield>* _this ) {
	return (vbyte *)(&_unref(_this)->areas[0]);
}
DEFINE_PRIM(_BYTES,CompactHeightfield_get_areas,_IDL);
HL_PRIM vbyte* HL_NAME(CompactHeightfield_set_areas)( pref<rcCompactHeightfield>* _this, vbyte* value ) {
	_unref(_this)->areas = (unsigned char*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,CompactHeightfield_set_areas,_IDL _BYTES);

HL_PRIM _hl_float3* HL_NAME(HeightfieldLayer_get_bmin)( pref<rcHeightfieldLayer>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmin);
}
HL_PRIM void HL_NAME(HeightfieldLayer_getbminv)( pref<rcHeightfieldLayer>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmin)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,HeightfieldLayer_getbminv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,HeightfieldLayer_get_bmin,_IDL);
HL_PRIM _hl_float3* HL_NAME(HeightfieldLayer_set_bmin)( pref<rcHeightfieldLayer>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmin)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(HeightfieldLayer_setbmin3)( pref<rcHeightfieldLayer>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,HeightfieldLayer_setbmin3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,HeightfieldLayer_set_bmin,_IDL _STRUCT);

HL_PRIM _hl_float3* HL_NAME(HeightfieldLayer_get_bmax)( pref<rcHeightfieldLayer>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmax);
}
HL_PRIM void HL_NAME(HeightfieldLayer_getbmaxv)( pref<rcHeightfieldLayer>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmax)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,HeightfieldLayer_getbmaxv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,HeightfieldLayer_get_bmax,_IDL);
HL_PRIM _hl_float3* HL_NAME(HeightfieldLayer_set_bmax)( pref<rcHeightfieldLayer>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmax)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(HeightfieldLayer_setbmax3)( pref<rcHeightfieldLayer>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,HeightfieldLayer_setbmax3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,HeightfieldLayer_set_bmax,_IDL _STRUCT);

HL_PRIM float HL_NAME(HeightfieldLayer_get_cs)( pref<rcHeightfieldLayer>* _this ) {
	return _unref(_this)->cs;
}
DEFINE_PRIM(_F32,HeightfieldLayer_get_cs,_IDL);
HL_PRIM float HL_NAME(HeightfieldLayer_set_cs)( pref<rcHeightfieldLayer>* _this, float value ) {
	_unref(_this)->cs = (value);
	return value;
}
DEFINE_PRIM(_F32,HeightfieldLayer_set_cs,_IDL _F32);

HL_PRIM float HL_NAME(HeightfieldLayer_get_ch)( pref<rcHeightfieldLayer>* _this ) {
	return _unref(_this)->ch;
}
DEFINE_PRIM(_F32,HeightfieldLayer_get_ch,_IDL);
HL_PRIM float HL_NAME(HeightfieldLayer_set_ch)( pref<rcHeightfieldLayer>* _this, float value ) {
	_unref(_this)->ch = (value);
	return value;
}
DEFINE_PRIM(_F32,HeightfieldLayer_set_ch,_IDL _F32);

HL_PRIM int HL_NAME(HeightfieldLayer_get_width)( pref<rcHeightfieldLayer>* _this ) {
	return _unref(_this)->width;
}
DEFINE_PRIM(_I32,HeightfieldLayer_get_width,_IDL);
HL_PRIM int HL_NAME(HeightfieldLayer_set_width)( pref<rcHeightfieldLayer>* _this, int value ) {
	_unref(_this)->width = (value);
	return value;
}
DEFINE_PRIM(_I32,HeightfieldLayer_set_width,_IDL _I32);

HL_PRIM int HL_NAME(HeightfieldLayer_get_height)( pref<rcHeightfieldLayer>* _this ) {
	return _unref(_this)->height;
}
DEFINE_PRIM(_I32,HeightfieldLayer_get_height,_IDL);
HL_PRIM int HL_NAME(HeightfieldLayer_set_height)( pref<rcHeightfieldLayer>* _this, int value ) {
	_unref(_this)->height = (value);
	return value;
}
DEFINE_PRIM(_I32,HeightfieldLayer_set_height,_IDL _I32);

HL_PRIM int HL_NAME(HeightfieldLayer_get_minx)( pref<rcHeightfieldLayer>* _this ) {
	return _unref(_this)->minx;
}
DEFINE_PRIM(_I32,HeightfieldLayer_get_minx,_IDL);
HL_PRIM int HL_NAME(HeightfieldLayer_set_minx)( pref<rcHeightfieldLayer>* _this, int value ) {
	_unref(_this)->minx = (value);
	return value;
}
DEFINE_PRIM(_I32,HeightfieldLayer_set_minx,_IDL _I32);

HL_PRIM int HL_NAME(HeightfieldLayer_get_maxx)( pref<rcHeightfieldLayer>* _this ) {
	return _unref(_this)->maxx;
}
DEFINE_PRIM(_I32,HeightfieldLayer_get_maxx,_IDL);
HL_PRIM int HL_NAME(HeightfieldLayer_set_maxx)( pref<rcHeightfieldLayer>* _this, int value ) {
	_unref(_this)->maxx = (value);
	return value;
}
DEFINE_PRIM(_I32,HeightfieldLayer_set_maxx,_IDL _I32);

HL_PRIM int HL_NAME(HeightfieldLayer_get_miny)( pref<rcHeightfieldLayer>* _this ) {
	return _unref(_this)->miny;
}
DEFINE_PRIM(_I32,HeightfieldLayer_get_miny,_IDL);
HL_PRIM int HL_NAME(HeightfieldLayer_set_miny)( pref<rcHeightfieldLayer>* _this, int value ) {
	_unref(_this)->miny = (value);
	return value;
}
DEFINE_PRIM(_I32,HeightfieldLayer_set_miny,_IDL _I32);

HL_PRIM int HL_NAME(HeightfieldLayer_get_maxy)( pref<rcHeightfieldLayer>* _this ) {
	return _unref(_this)->maxy;
}
DEFINE_PRIM(_I32,HeightfieldLayer_get_maxy,_IDL);
HL_PRIM int HL_NAME(HeightfieldLayer_set_maxy)( pref<rcHeightfieldLayer>* _this, int value ) {
	_unref(_this)->maxy = (value);
	return value;
}
DEFINE_PRIM(_I32,HeightfieldLayer_set_maxy,_IDL _I32);

HL_PRIM int HL_NAME(HeightfieldLayer_get_hmin)( pref<rcHeightfieldLayer>* _this ) {
	return _unref(_this)->hmin;
}
DEFINE_PRIM(_I32,HeightfieldLayer_get_hmin,_IDL);
HL_PRIM int HL_NAME(HeightfieldLayer_set_hmin)( pref<rcHeightfieldLayer>* _this, int value ) {
	_unref(_this)->hmin = (value);
	return value;
}
DEFINE_PRIM(_I32,HeightfieldLayer_set_hmin,_IDL _I32);

HL_PRIM int HL_NAME(HeightfieldLayer_get_hmax)( pref<rcHeightfieldLayer>* _this ) {
	return _unref(_this)->hmax;
}
DEFINE_PRIM(_I32,HeightfieldLayer_get_hmax,_IDL);
HL_PRIM int HL_NAME(HeightfieldLayer_set_hmax)( pref<rcHeightfieldLayer>* _this, int value ) {
	_unref(_this)->hmax = (value);
	return value;
}
DEFINE_PRIM(_I32,HeightfieldLayer_set_hmax,_IDL _I32);

HL_PRIM vbyte* HL_NAME(HeightfieldLayer_get_heights)( pref<rcHeightfieldLayer>* _this ) {
	return (vbyte *)(&_unref(_this)->heights[0]);
}
DEFINE_PRIM(_BYTES,HeightfieldLayer_get_heights,_IDL);
HL_PRIM vbyte* HL_NAME(HeightfieldLayer_set_heights)( pref<rcHeightfieldLayer>* _this, vbyte* value ) {
	_unref(_this)->heights = (unsigned char*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,HeightfieldLayer_set_heights,_IDL _BYTES);

HL_PRIM vbyte* HL_NAME(HeightfieldLayer_get_areas)( pref<rcHeightfieldLayer>* _this ) {
	return (vbyte *)(&_unref(_this)->areas[0]);
}
DEFINE_PRIM(_BYTES,HeightfieldLayer_get_areas,_IDL);
HL_PRIM vbyte* HL_NAME(HeightfieldLayer_set_areas)( pref<rcHeightfieldLayer>* _this, vbyte* value ) {
	_unref(_this)->areas = (unsigned char*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,HeightfieldLayer_set_areas,_IDL _BYTES);

HL_PRIM vbyte* HL_NAME(HeightfieldLayer_get_cons)( pref<rcHeightfieldLayer>* _this ) {
	return (vbyte *)(&_unref(_this)->cons[0]);
}
DEFINE_PRIM(_BYTES,HeightfieldLayer_get_cons,_IDL);
HL_PRIM vbyte* HL_NAME(HeightfieldLayer_set_cons)( pref<rcHeightfieldLayer>* _this, vbyte* value ) {
	_unref(_this)->cons = (unsigned char*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,HeightfieldLayer_set_cons,_IDL _BYTES);

HL_PRIM pref<rcHeightfieldLayerSet>* HL_NAME(HeightfieldLayerSet_new0)() {
	return alloc_ref((new rcHeightfieldLayerSet()),HeightfieldLayerSet);
}
DEFINE_PRIM(_IDL, HeightfieldLayerSet_new0,);

HL_PRIM HL_CONST pref<rcHeightfieldLayer>* HL_NAME(HeightfieldLayerSet_layers1)(pref<rcHeightfieldLayerSet>* _this, int idx) {
	return alloc_ref_const(&(_unref(_this)->layers[idx]),HeightfieldLayer);
}
DEFINE_PRIM(_IDL, HeightfieldLayerSet_layers1, _IDL _I32);

HL_PRIM int HL_NAME(HeightfieldLayerSet_get_nlayers)( pref<rcHeightfieldLayerSet>* _this ) {
	return _unref(_this)->nlayers;
}
DEFINE_PRIM(_I32,HeightfieldLayerSet_get_nlayers,_IDL);
HL_PRIM int HL_NAME(HeightfieldLayerSet_set_nlayers)( pref<rcHeightfieldLayerSet>* _this, int value ) {
	_unref(_this)->nlayers = (value);
	return value;
}
DEFINE_PRIM(_I32,HeightfieldLayerSet_set_nlayers,_IDL _I32);

HL_PRIM vbyte* HL_NAME(rcContour_get_verts)( pref<rcContour>* _this ) {
	return (vbyte *)(&_unref(_this)->verts[0]);
}
DEFINE_PRIM(_BYTES,rcContour_get_verts,_IDL);
HL_PRIM vbyte* HL_NAME(rcContour_set_verts)( pref<rcContour>* _this, vbyte* value ) {
	_unref(_this)->verts = (int*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,rcContour_set_verts,_IDL _BYTES);

HL_PRIM int HL_NAME(rcContour_get_nverts)( pref<rcContour>* _this ) {
	return _unref(_this)->nverts;
}
DEFINE_PRIM(_I32,rcContour_get_nverts,_IDL);
HL_PRIM int HL_NAME(rcContour_set_nverts)( pref<rcContour>* _this, int value ) {
	_unref(_this)->nverts = (value);
	return value;
}
DEFINE_PRIM(_I32,rcContour_set_nverts,_IDL _I32);

HL_PRIM vbyte* HL_NAME(rcContour_get_rverts)( pref<rcContour>* _this ) {
	return (vbyte *)(&_unref(_this)->rverts[0]);
}
DEFINE_PRIM(_BYTES,rcContour_get_rverts,_IDL);
HL_PRIM vbyte* HL_NAME(rcContour_set_rverts)( pref<rcContour>* _this, vbyte* value ) {
	_unref(_this)->rverts = (int*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,rcContour_set_rverts,_IDL _BYTES);

HL_PRIM int HL_NAME(rcContour_get_nrverts)( pref<rcContour>* _this ) {
	return _unref(_this)->nrverts;
}
DEFINE_PRIM(_I32,rcContour_get_nrverts,_IDL);
HL_PRIM int HL_NAME(rcContour_set_nrverts)( pref<rcContour>* _this, int value ) {
	_unref(_this)->nrverts = (value);
	return value;
}
DEFINE_PRIM(_I32,rcContour_set_nrverts,_IDL _I32);

HL_PRIM int HL_NAME(rcContour_get_reg)( pref<rcContour>* _this ) {
	return _unref(_this)->reg;
}
DEFINE_PRIM(_I32,rcContour_get_reg,_IDL);
HL_PRIM int HL_NAME(rcContour_set_reg)( pref<rcContour>* _this, int value ) {
	_unref(_this)->reg = (value);
	return value;
}
DEFINE_PRIM(_I32,rcContour_set_reg,_IDL _I32);

HL_PRIM unsigned char HL_NAME(rcContour_get_area)( pref<rcContour>* _this ) {
	return _unref(_this)->area;
}
DEFINE_PRIM(_I8,rcContour_get_area,_IDL);
HL_PRIM unsigned char HL_NAME(rcContour_set_area)( pref<rcContour>* _this, unsigned char value ) {
	_unref(_this)->area = (value);
	return value;
}
DEFINE_PRIM(_I8,rcContour_set_area,_IDL _I8);

HL_PRIM pref<rcContourSet>* HL_NAME(rcContourSet_new0)() {
	return alloc_ref((new rcContourSet()),rcContourSet);
}
DEFINE_PRIM(_IDL, rcContourSet_new0,);

HL_PRIM pref<rcContour>* HL_NAME(rcContourSet_get_conts)( pref<rcContourSet>* _this ) {
	return alloc_ref(_unref(_this)->conts,rcContour);
}
DEFINE_PRIM(_IDL,rcContourSet_get_conts,_IDL);
HL_PRIM pref<rcContour>* HL_NAME(rcContourSet_set_conts)( pref<rcContourSet>* _this, pref<rcContour>* value ) {
	_unref(_this)->conts = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,rcContourSet_set_conts,_IDL _IDL);

HL_PRIM int HL_NAME(rcContourSet_get_nconts)( pref<rcContourSet>* _this ) {
	return _unref(_this)->nconts;
}
DEFINE_PRIM(_I32,rcContourSet_get_nconts,_IDL);
HL_PRIM int HL_NAME(rcContourSet_set_nconts)( pref<rcContourSet>* _this, int value ) {
	_unref(_this)->nconts = (value);
	return value;
}
DEFINE_PRIM(_I32,rcContourSet_set_nconts,_IDL _I32);

HL_PRIM _hl_float3* HL_NAME(rcContourSet_get_bmin)( pref<rcContourSet>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmin);
}
HL_PRIM void HL_NAME(rcContourSet_getbminv)( pref<rcContourSet>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmin)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,rcContourSet_getbminv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,rcContourSet_get_bmin,_IDL);
HL_PRIM _hl_float3* HL_NAME(rcContourSet_set_bmin)( pref<rcContourSet>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmin)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(rcContourSet_setbmin3)( pref<rcContourSet>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,rcContourSet_setbmin3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,rcContourSet_set_bmin,_IDL _STRUCT);

HL_PRIM _hl_float3* HL_NAME(rcContourSet_get_bmax)( pref<rcContourSet>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmax);
}
HL_PRIM void HL_NAME(rcContourSet_getbmaxv)( pref<rcContourSet>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmax)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,rcContourSet_getbmaxv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,rcContourSet_get_bmax,_IDL);
HL_PRIM _hl_float3* HL_NAME(rcContourSet_set_bmax)( pref<rcContourSet>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmax)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(rcContourSet_setbmax3)( pref<rcContourSet>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,rcContourSet_setbmax3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,rcContourSet_set_bmax,_IDL _STRUCT);

HL_PRIM float HL_NAME(rcContourSet_get_cs)( pref<rcContourSet>* _this ) {
	return _unref(_this)->cs;
}
DEFINE_PRIM(_F32,rcContourSet_get_cs,_IDL);
HL_PRIM float HL_NAME(rcContourSet_set_cs)( pref<rcContourSet>* _this, float value ) {
	_unref(_this)->cs = (value);
	return value;
}
DEFINE_PRIM(_F32,rcContourSet_set_cs,_IDL _F32);

HL_PRIM float HL_NAME(rcContourSet_get_ch)( pref<rcContourSet>* _this ) {
	return _unref(_this)->ch;
}
DEFINE_PRIM(_F32,rcContourSet_get_ch,_IDL);
HL_PRIM float HL_NAME(rcContourSet_set_ch)( pref<rcContourSet>* _this, float value ) {
	_unref(_this)->ch = (value);
	return value;
}
DEFINE_PRIM(_F32,rcContourSet_set_ch,_IDL _F32);

HL_PRIM int HL_NAME(rcContourSet_get_width)( pref<rcContourSet>* _this ) {
	return _unref(_this)->width;
}
DEFINE_PRIM(_I32,rcContourSet_get_width,_IDL);
HL_PRIM int HL_NAME(rcContourSet_set_width)( pref<rcContourSet>* _this, int value ) {
	_unref(_this)->width = (value);
	return value;
}
DEFINE_PRIM(_I32,rcContourSet_set_width,_IDL _I32);

HL_PRIM int HL_NAME(rcContourSet_get_height)( pref<rcContourSet>* _this ) {
	return _unref(_this)->height;
}
DEFINE_PRIM(_I32,rcContourSet_get_height,_IDL);
HL_PRIM int HL_NAME(rcContourSet_set_height)( pref<rcContourSet>* _this, int value ) {
	_unref(_this)->height = (value);
	return value;
}
DEFINE_PRIM(_I32,rcContourSet_set_height,_IDL _I32);

HL_PRIM int HL_NAME(rcContourSet_get_borderSize)( pref<rcContourSet>* _this ) {
	return _unref(_this)->borderSize;
}
DEFINE_PRIM(_I32,rcContourSet_get_borderSize,_IDL);
HL_PRIM int HL_NAME(rcContourSet_set_borderSize)( pref<rcContourSet>* _this, int value ) {
	_unref(_this)->borderSize = (value);
	return value;
}
DEFINE_PRIM(_I32,rcContourSet_set_borderSize,_IDL _I32);

HL_PRIM float HL_NAME(rcContourSet_get_maxError)( pref<rcContourSet>* _this ) {
	return _unref(_this)->maxError;
}
DEFINE_PRIM(_F32,rcContourSet_get_maxError,_IDL);
HL_PRIM float HL_NAME(rcContourSet_set_maxError)( pref<rcContourSet>* _this, float value ) {
	_unref(_this)->maxError = (value);
	return value;
}
DEFINE_PRIM(_F32,rcContourSet_set_maxError,_IDL _F32);

HL_PRIM pref<rcPolyMesh>* HL_NAME(PolyMesh_new0)() {
	return alloc_ref((new rcPolyMesh()),PolyMesh);
}
DEFINE_PRIM(_IDL, PolyMesh_new0,);

HL_PRIM vbyte* HL_NAME(PolyMesh_get_verts)( pref<rcPolyMesh>* _this ) {
	return (vbyte *)(&_unref(_this)->verts[0]);
}
DEFINE_PRIM(_BYTES,PolyMesh_get_verts,_IDL);
HL_PRIM vbyte* HL_NAME(PolyMesh_set_verts)( pref<rcPolyMesh>* _this, vbyte* value ) {
	_unref(_this)->verts = (unsigned short*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,PolyMesh_set_verts,_IDL _BYTES);

HL_PRIM vbyte* HL_NAME(PolyMesh_get_polys)( pref<rcPolyMesh>* _this ) {
	return (vbyte *)(&_unref(_this)->polys[0]);
}
DEFINE_PRIM(_BYTES,PolyMesh_get_polys,_IDL);
HL_PRIM vbyte* HL_NAME(PolyMesh_set_polys)( pref<rcPolyMesh>* _this, vbyte* value ) {
	_unref(_this)->polys = (unsigned short*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,PolyMesh_set_polys,_IDL _BYTES);

HL_PRIM vbyte* HL_NAME(PolyMesh_get_regs)( pref<rcPolyMesh>* _this ) {
	return (vbyte *)(&_unref(_this)->regs[0]);
}
DEFINE_PRIM(_BYTES,PolyMesh_get_regs,_IDL);
HL_PRIM vbyte* HL_NAME(PolyMesh_set_regs)( pref<rcPolyMesh>* _this, vbyte* value ) {
	_unref(_this)->regs = (unsigned short*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,PolyMesh_set_regs,_IDL _BYTES);

HL_PRIM vbyte* HL_NAME(PolyMesh_get_flags)( pref<rcPolyMesh>* _this ) {
	return (vbyte *)(&_unref(_this)->flags[0]);
}
DEFINE_PRIM(_BYTES,PolyMesh_get_flags,_IDL);
HL_PRIM vbyte* HL_NAME(PolyMesh_set_flags)( pref<rcPolyMesh>* _this, vbyte* value ) {
	_unref(_this)->flags = (unsigned short*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,PolyMesh_set_flags,_IDL _BYTES);

HL_PRIM vbyte* HL_NAME(PolyMesh_get_areas)( pref<rcPolyMesh>* _this ) {
	return (vbyte *)(&_unref(_this)->areas[0]);
}
DEFINE_PRIM(_BYTES,PolyMesh_get_areas,_IDL);
HL_PRIM vbyte* HL_NAME(PolyMesh_set_areas)( pref<rcPolyMesh>* _this, vbyte* value ) {
	_unref(_this)->areas = (unsigned char*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,PolyMesh_set_areas,_IDL _BYTES);

HL_PRIM int HL_NAME(PolyMesh_get_nverts)( pref<rcPolyMesh>* _this ) {
	return _unref(_this)->nverts;
}
DEFINE_PRIM(_I32,PolyMesh_get_nverts,_IDL);
HL_PRIM int HL_NAME(PolyMesh_set_nverts)( pref<rcPolyMesh>* _this, int value ) {
	_unref(_this)->nverts = (value);
	return value;
}
DEFINE_PRIM(_I32,PolyMesh_set_nverts,_IDL _I32);

HL_PRIM int HL_NAME(PolyMesh_get_npolys)( pref<rcPolyMesh>* _this ) {
	return _unref(_this)->npolys;
}
DEFINE_PRIM(_I32,PolyMesh_get_npolys,_IDL);
HL_PRIM int HL_NAME(PolyMesh_set_npolys)( pref<rcPolyMesh>* _this, int value ) {
	_unref(_this)->npolys = (value);
	return value;
}
DEFINE_PRIM(_I32,PolyMesh_set_npolys,_IDL _I32);

HL_PRIM int HL_NAME(PolyMesh_get_maxpolys)( pref<rcPolyMesh>* _this ) {
	return _unref(_this)->maxpolys;
}
DEFINE_PRIM(_I32,PolyMesh_get_maxpolys,_IDL);
HL_PRIM int HL_NAME(PolyMesh_set_maxpolys)( pref<rcPolyMesh>* _this, int value ) {
	_unref(_this)->maxpolys = (value);
	return value;
}
DEFINE_PRIM(_I32,PolyMesh_set_maxpolys,_IDL _I32);

HL_PRIM int HL_NAME(PolyMesh_get_nvp)( pref<rcPolyMesh>* _this ) {
	return _unref(_this)->nvp;
}
DEFINE_PRIM(_I32,PolyMesh_get_nvp,_IDL);
HL_PRIM int HL_NAME(PolyMesh_set_nvp)( pref<rcPolyMesh>* _this, int value ) {
	_unref(_this)->nvp = (value);
	return value;
}
DEFINE_PRIM(_I32,PolyMesh_set_nvp,_IDL _I32);

HL_PRIM _hl_float3* HL_NAME(PolyMesh_get_bmin)( pref<rcPolyMesh>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmin);
}
HL_PRIM void HL_NAME(PolyMesh_getbminv)( pref<rcPolyMesh>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmin)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,PolyMesh_getbminv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,PolyMesh_get_bmin,_IDL);
HL_PRIM _hl_float3* HL_NAME(PolyMesh_set_bmin)( pref<rcPolyMesh>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmin)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(PolyMesh_setbmin3)( pref<rcPolyMesh>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,PolyMesh_setbmin3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,PolyMesh_set_bmin,_IDL _STRUCT);

HL_PRIM _hl_float3* HL_NAME(PolyMesh_get_bmax)( pref<rcPolyMesh>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmax);
}
HL_PRIM void HL_NAME(PolyMesh_getbmaxv)( pref<rcPolyMesh>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmax)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,PolyMesh_getbmaxv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,PolyMesh_get_bmax,_IDL);
HL_PRIM _hl_float3* HL_NAME(PolyMesh_set_bmax)( pref<rcPolyMesh>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmax)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(PolyMesh_setbmax3)( pref<rcPolyMesh>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,PolyMesh_setbmax3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,PolyMesh_set_bmax,_IDL _STRUCT);

HL_PRIM float HL_NAME(PolyMesh_get_cs)( pref<rcPolyMesh>* _this ) {
	return _unref(_this)->cs;
}
DEFINE_PRIM(_F32,PolyMesh_get_cs,_IDL);
HL_PRIM float HL_NAME(PolyMesh_set_cs)( pref<rcPolyMesh>* _this, float value ) {
	_unref(_this)->cs = (value);
	return value;
}
DEFINE_PRIM(_F32,PolyMesh_set_cs,_IDL _F32);

HL_PRIM float HL_NAME(PolyMesh_get_ch)( pref<rcPolyMesh>* _this ) {
	return _unref(_this)->ch;
}
DEFINE_PRIM(_F32,PolyMesh_get_ch,_IDL);
HL_PRIM float HL_NAME(PolyMesh_set_ch)( pref<rcPolyMesh>* _this, float value ) {
	_unref(_this)->ch = (value);
	return value;
}
DEFINE_PRIM(_F32,PolyMesh_set_ch,_IDL _F32);

HL_PRIM int HL_NAME(PolyMesh_get_borderSize)( pref<rcPolyMesh>* _this ) {
	return _unref(_this)->borderSize;
}
DEFINE_PRIM(_I32,PolyMesh_get_borderSize,_IDL);
HL_PRIM int HL_NAME(PolyMesh_set_borderSize)( pref<rcPolyMesh>* _this, int value ) {
	_unref(_this)->borderSize = (value);
	return value;
}
DEFINE_PRIM(_I32,PolyMesh_set_borderSize,_IDL _I32);

HL_PRIM float HL_NAME(PolyMesh_get_maxEdgeError)( pref<rcPolyMesh>* _this ) {
	return _unref(_this)->maxEdgeError;
}
DEFINE_PRIM(_F32,PolyMesh_get_maxEdgeError,_IDL);
HL_PRIM float HL_NAME(PolyMesh_set_maxEdgeError)( pref<rcPolyMesh>* _this, float value ) {
	_unref(_this)->maxEdgeError = (value);
	return value;
}
DEFINE_PRIM(_F32,PolyMesh_set_maxEdgeError,_IDL _F32);

HL_PRIM pref<rcPolyMeshDetail>* HL_NAME(PolyMeshDetail_new0)() {
	return alloc_ref((new rcPolyMeshDetail()),PolyMeshDetail);
}
DEFINE_PRIM(_IDL, PolyMeshDetail_new0,);

HL_PRIM vbyte* HL_NAME(PolyMeshDetail_get_meshes)( pref<rcPolyMeshDetail>* _this ) {
	return (vbyte *)(&_unref(_this)->meshes[0]);
}
DEFINE_PRIM(_BYTES,PolyMeshDetail_get_meshes,_IDL);
HL_PRIM vbyte* HL_NAME(PolyMeshDetail_set_meshes)( pref<rcPolyMeshDetail>* _this, vbyte* value ) {
	_unref(_this)->meshes = (unsigned int*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,PolyMeshDetail_set_meshes,_IDL _BYTES);

HL_PRIM vbyte* HL_NAME(PolyMeshDetail_get_verts)( pref<rcPolyMeshDetail>* _this ) {
	return (vbyte *)(&_unref(_this)->verts[0]);
}
DEFINE_PRIM(_BYTES,PolyMeshDetail_get_verts,_IDL);
HL_PRIM vbyte* HL_NAME(PolyMeshDetail_set_verts)( pref<rcPolyMeshDetail>* _this, vbyte* value ) {
	_unref(_this)->verts = (float*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,PolyMeshDetail_set_verts,_IDL _BYTES);

HL_PRIM vbyte* HL_NAME(PolyMeshDetail_get_tris)( pref<rcPolyMeshDetail>* _this ) {
	return (vbyte *)(&_unref(_this)->tris[0]);
}
DEFINE_PRIM(_BYTES,PolyMeshDetail_get_tris,_IDL);
HL_PRIM vbyte* HL_NAME(PolyMeshDetail_set_tris)( pref<rcPolyMeshDetail>* _this, vbyte* value ) {
	_unref(_this)->tris = (unsigned char*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,PolyMeshDetail_set_tris,_IDL _BYTES);

HL_PRIM int HL_NAME(PolyMeshDetail_get_nmeshes)( pref<rcPolyMeshDetail>* _this ) {
	return _unref(_this)->nmeshes;
}
DEFINE_PRIM(_I32,PolyMeshDetail_get_nmeshes,_IDL);
HL_PRIM int HL_NAME(PolyMeshDetail_set_nmeshes)( pref<rcPolyMeshDetail>* _this, int value ) {
	_unref(_this)->nmeshes = (value);
	return value;
}
DEFINE_PRIM(_I32,PolyMeshDetail_set_nmeshes,_IDL _I32);

HL_PRIM int HL_NAME(PolyMeshDetail_get_nverts)( pref<rcPolyMeshDetail>* _this ) {
	return _unref(_this)->nverts;
}
DEFINE_PRIM(_I32,PolyMeshDetail_get_nverts,_IDL);
HL_PRIM int HL_NAME(PolyMeshDetail_set_nverts)( pref<rcPolyMeshDetail>* _this, int value ) {
	_unref(_this)->nverts = (value);
	return value;
}
DEFINE_PRIM(_I32,PolyMeshDetail_set_nverts,_IDL _I32);

HL_PRIM int HL_NAME(PolyMeshDetail_get_ntris)( pref<rcPolyMeshDetail>* _this ) {
	return _unref(_this)->ntris;
}
DEFINE_PRIM(_I32,PolyMeshDetail_get_ntris,_IDL);
HL_PRIM int HL_NAME(PolyMeshDetail_set_ntris)( pref<rcPolyMeshDetail>* _this, int value ) {
	_unref(_this)->ntris = (value);
	return value;
}
DEFINE_PRIM(_I32,PolyMeshDetail_set_ntris,_IDL _I32);

HL_PRIM vbyte* HL_NAME(Alloc_allocShortArray2)(int length, int hint) {
	return (vbyte *)(rcAllocShort(length, AllocHint__values[hint]));
}
DEFINE_PRIM(_BYTES, Alloc_allocShortArray2, _I32 _I32);

HL_PRIM vbyte* HL_NAME(Alloc_allocByteArray2)(int length, int hint) {
	return (vbyte *)(rcAlloc(length, AllocHint__values[hint]));
}
DEFINE_PRIM(_BYTES, Alloc_allocByteArray2, _I32 _I32);

HL_PRIM void HL_NAME(Alloc_freeArray1)(vbyte* array) {
	(rcFree((unsigned short*)array));
}
DEFINE_PRIM(_VOID, Alloc_freeArray1, _BYTES);

HL_PRIM void HL_NAME(Alloc_clearByteArray2)(vbyte* array, int length) {
	(rcClear((unsigned char*)array, length));
}
DEFINE_PRIM(_VOID, Alloc_clearByteArray2, _BYTES _I32);

HL_PRIM vbyte* HL_NAME(Alloc_offsetByteArray2)(vbyte* array, int offset) {
	return (rcOffset((unsigned char*)array, offset));
}
DEFINE_PRIM(_BYTES, Alloc_offsetByteArray2, _BYTES _I32);

HL_PRIM void HL_NAME(Recast_rcFreeHeightField1)(pref<rcHeightfield>* hf) {
	(rcFreeHeightField(_unref_ptr_safe(hf)));
}
DEFINE_PRIM(_VOID, Recast_rcFreeHeightField1, _IDL);

HL_PRIM pref<rcHeightfield>* HL_NAME(Recast_rcAllocHeightfield0)() {
	return alloc_ref((rcAllocHeightfield()),Heightfield);
}
DEFINE_PRIM(_IDL, Recast_rcAllocHeightfield0,);

HL_PRIM pref<rcContourSet>* HL_NAME(Recast_rcAllocContourSet0)() {
	return alloc_ref((rcAllocContourSet()),rcContourSet);
}
DEFINE_PRIM(_IDL, Recast_rcAllocContourSet0,);

HL_PRIM void HL_NAME(Recast_rcFreeContourSet1)(pref<rcContourSet>* cset) {
	(rcFreeContourSet(_unref_ptr_safe(cset)));
}
DEFINE_PRIM(_VOID, Recast_rcFreeContourSet1, _IDL);

HL_PRIM void HL_NAME(Recast_calcBounds4)(varray* verts, int nv, _hl_float3* bmin, _hl_float3* bmax) {
	(rcCalcBounds(hl_aptr(verts,float), nv, (float*)(_hl_float3*)bmin, (float*)(_hl_float3*)bmax));
}
DEFINE_PRIM(_VOID, Recast_calcBounds4, _ARR _I32 _STRUCT _STRUCT);

HL_PRIM void HL_NAME(Recast_calcGridSize5)(_hl_float3* bmin, _hl_float3* bmax, float cs, int* w, int* h) {
	(rcCalcGridSize((float*)(_hl_float3*)bmin, (float*)(_hl_float3*)bmax, cs, w, h));
}
DEFINE_PRIM(_VOID, Recast_calcGridSize5, _STRUCT _STRUCT _F32 _REF(_I32) _REF(_I32));

HL_PRIM int HL_NAME(Recast_offsetPoly5)(_hl_float3* verts, int nverts, float offset, _hl_float3* outVerts, int maxOutVerts) {
	return (rcOffsetPoly((float*)(_hl_float3*)verts, nverts, offset, (float*)(_hl_float3*)outVerts, maxOutVerts));
}
DEFINE_PRIM(_I32, Recast_offsetPoly5, _STRUCT _I32 _F32 _STRUCT _I32);

HL_PRIM float HL_NAME(Math_fabsf1)(float x) {
	return (fabsf(x));
}
DEFINE_PRIM(_F32, Math_fabsf1, _F32);

HL_PRIM float HL_NAME(Math_sqrtf1)(float x) {
	return (sqrtf(x));
}
DEFINE_PRIM(_F32, Math_sqrtf1, _F32);

HL_PRIM float HL_NAME(Math_floorf1)(float x) {
	return (floorf(x));
}
DEFINE_PRIM(_F32, Math_floorf1, _F32);

HL_PRIM float HL_NAME(Math_ceilf1)(float x) {
	return (ceilf(x));
}
DEFINE_PRIM(_F32, Math_ceilf1, _F32);

HL_PRIM float HL_NAME(Math_cosf1)(float x) {
	return (cosf(x));
}
DEFINE_PRIM(_F32, Math_cosf1, _F32);

HL_PRIM float HL_NAME(Math_sinf1)(float x) {
	return (sinf(x));
}
DEFINE_PRIM(_F32, Math_sinf1, _F32);

HL_PRIM float HL_NAME(Math_atan2f2)(float y, float x) {
	return (atan2f(y, x));
}
DEFINE_PRIM(_F32, Math_atan2f2, _F32 _F32);

HL_PRIM bool HL_NAME(Math_isfinite1)(float x) {
	return (isfinite(x));
}
DEFINE_PRIM(_BOOL, Math_isfinite1, _F32);

HL_PRIM int HL_NAME(DetourCommon_nextPow21)(int v) {
	return (dtNextPow2(v));
}
DEFINE_PRIM(_I32, DetourCommon_nextPow21, _I32);

HL_PRIM int HL_NAME(DetourCommon_ilog21)(int v) {
	return (dtIlog2(v));
}
DEFINE_PRIM(_I32, DetourCommon_ilog21, _I32);

HL_PRIM int HL_NAME(DetourCommon_align41)(int x) {
	return (dtAlign4(x));
}
DEFINE_PRIM(_I32, DetourCommon_align41, _I32);

HL_PRIM pref<dtTileCache>* HL_NAME(TileCache_new0)() {
	return alloc_ref((new dtTileCache()),TileCache);
}
DEFINE_PRIM(_IDL, TileCache_new0,);

HL_PRIM void HL_NAME(TileCache_init4)(pref<dtTileCache>* _this, pref<dtTileCacheParams>* params, pref<dtTileCacheAlloc>* talloc, pref<dtTileCacheCompressor>* tcomp, pref<dtTileCacheMeshProcess>* tmproc) {
	(_unref(_this)->init(_unref_ptr_safe(params), _unref_ptr_safe(talloc), _unref_ptr_safe(tcomp), _unref_ptr_safe(tmproc)));
}
DEFINE_PRIM(_VOID, TileCache_init4, _IDL _IDL _IDL _IDL _IDL);

HL_PRIM unsigned int HL_NAME(TileCache_addTile4)(pref<dtTileCache>* _this, vbyte* data, int dataSize, int flags, unsigned int* result) {
	return (_unref(_this)->addTile((unsigned char*)data, dataSize, flags, result));
}
DEFINE_PRIM(_I32, TileCache_addTile4, _IDL _BYTES _I32 _I32 _REF(_I32));

HL_PRIM unsigned int HL_NAME(TileCache_buildNavMeshTilesAt3)(pref<dtTileCache>* _this, int tx, int ty, pref<dtNavMesh>* navmesh) {
	return (_unref(_this)->buildNavMeshTilesAt(tx, ty, _unref_ptr_safe(navmesh)));
}
DEFINE_PRIM(_I32, TileCache_buildNavMeshTilesAt3, _IDL _I32 _I32 _IDL);

HL_PRIM vbyte* HL_NAME(TileCacheCompressor_buildTileCacheLayer6)(pref<dtTileCacheCompressor>* _this, pref<dtTileCacheLayerHeader>* header, vbyte* heights, vbyte* areas, vbyte* cons, int* outDataSize) {
	vbyte* __tmpret;
dtBuildTileCacheLayer( _unref(_this) , _unref_ptr_safe(header), (unsigned char*)heights, (unsigned char*)areas, (unsigned char*)cons, (unsigned char **)&__tmpret, outDataSize);
	return (__tmpret);
}
DEFINE_PRIM(_BYTES, TileCacheCompressor_buildTileCacheLayer6, _IDL _IDL _BYTES _BYTES _BYTES _REF(_I32));

HL_PRIM pref<dtTileCacheLayerHeader>* HL_NAME(TileCacheLayerHeader_new0)() {
	return alloc_ref((new dtTileCacheLayerHeader()),TileCacheLayerHeader);
}
DEFINE_PRIM(_IDL, TileCacheLayerHeader_new0,);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_magic)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->magic;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_magic,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_magic)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->magic = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_magic,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_version)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->version;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_version,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_version)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->version = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_version,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_tx)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->tx;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_tx,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_tx)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->tx = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_tx,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_ty)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->ty;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_ty,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_ty)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->ty = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_ty,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_tlayer)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->tlayer;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_tlayer,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_tlayer)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->tlayer = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_tlayer,_IDL _I32);

HL_PRIM _hl_float3* HL_NAME(TileCacheLayerHeader_get_bmin)( pref<dtTileCacheLayerHeader>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmin);
}
HL_PRIM void HL_NAME(TileCacheLayerHeader_getbminv)( pref<dtTileCacheLayerHeader>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmin)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,TileCacheLayerHeader_getbminv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,TileCacheLayerHeader_get_bmin,_IDL);
HL_PRIM _hl_float3* HL_NAME(TileCacheLayerHeader_set_bmin)( pref<dtTileCacheLayerHeader>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmin)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(TileCacheLayerHeader_setbmin3)( pref<dtTileCacheLayerHeader>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,TileCacheLayerHeader_setbmin3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,TileCacheLayerHeader_set_bmin,_IDL _STRUCT);

HL_PRIM _hl_float3* HL_NAME(TileCacheLayerHeader_get_bmax)( pref<dtTileCacheLayerHeader>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmax);
}
HL_PRIM void HL_NAME(TileCacheLayerHeader_getbmaxv)( pref<dtTileCacheLayerHeader>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmax)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,TileCacheLayerHeader_getbmaxv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,TileCacheLayerHeader_get_bmax,_IDL);
HL_PRIM _hl_float3* HL_NAME(TileCacheLayerHeader_set_bmax)( pref<dtTileCacheLayerHeader>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmax)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(TileCacheLayerHeader_setbmax3)( pref<dtTileCacheLayerHeader>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,TileCacheLayerHeader_setbmax3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,TileCacheLayerHeader_set_bmax,_IDL _STRUCT);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_width)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->width;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_width,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_width)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->width = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_width,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_height)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->height;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_height,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_height)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->height = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_height,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_minx)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->minx;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_minx,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_minx)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->minx = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_minx,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_maxx)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->maxx;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_maxx,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_maxx)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->maxx = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_maxx,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_miny)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->miny;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_miny,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_miny)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->miny = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_miny,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_maxy)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->maxy;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_maxy,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_maxy)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->maxy = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_maxy,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_hmin)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->hmin;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_hmin,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_hmin)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->hmin = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_hmin,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheLayerHeader_get_hmax)( pref<dtTileCacheLayerHeader>* _this ) {
	return _unref(_this)->hmax;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_get_hmax,_IDL);
HL_PRIM int HL_NAME(TileCacheLayerHeader_set_hmax)( pref<dtTileCacheLayerHeader>* _this, int value ) {
	_unref(_this)->hmax = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheLayerHeader_set_hmax,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheLayerHeader_sizeof0)() {
	return (sizeof(dtTileCacheLayerHeader));
}
DEFINE_PRIM(_I32, TileCacheLayerHeader_sizeof0,);

HL_PRIM pref<dtTileCacheParams>* HL_NAME(TileCacheParams_new0)() {
	return alloc_ref((new dtTileCacheParams()),TileCacheParams);
}
DEFINE_PRIM(_IDL, TileCacheParams_new0,);

HL_PRIM _hl_float3* HL_NAME(TileCacheParams_get_orig)( pref<dtTileCacheParams>* _this ) {
	return (_hl_float3* )(_unref(_this)->orig);
}
HL_PRIM void HL_NAME(TileCacheParams_getorigv)( pref<dtTileCacheParams>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->orig)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,TileCacheParams_getorigv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,TileCacheParams_get_orig,_IDL);
HL_PRIM _hl_float3* HL_NAME(TileCacheParams_set_orig)( pref<dtTileCacheParams>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->orig)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(TileCacheParams_setorig3)( pref<dtTileCacheParams>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->orig);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,TileCacheParams_setorig3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,TileCacheParams_set_orig,_IDL _STRUCT);

HL_PRIM float HL_NAME(TileCacheParams_get_cs)( pref<dtTileCacheParams>* _this ) {
	return _unref(_this)->cs;
}
DEFINE_PRIM(_F32,TileCacheParams_get_cs,_IDL);
HL_PRIM float HL_NAME(TileCacheParams_set_cs)( pref<dtTileCacheParams>* _this, float value ) {
	_unref(_this)->cs = (value);
	return value;
}
DEFINE_PRIM(_F32,TileCacheParams_set_cs,_IDL _F32);

HL_PRIM float HL_NAME(TileCacheParams_get_ch)( pref<dtTileCacheParams>* _this ) {
	return _unref(_this)->ch;
}
DEFINE_PRIM(_F32,TileCacheParams_get_ch,_IDL);
HL_PRIM float HL_NAME(TileCacheParams_set_ch)( pref<dtTileCacheParams>* _this, float value ) {
	_unref(_this)->ch = (value);
	return value;
}
DEFINE_PRIM(_F32,TileCacheParams_set_ch,_IDL _F32);

HL_PRIM int HL_NAME(TileCacheParams_get_width)( pref<dtTileCacheParams>* _this ) {
	return _unref(_this)->width;
}
DEFINE_PRIM(_I32,TileCacheParams_get_width,_IDL);
HL_PRIM int HL_NAME(TileCacheParams_set_width)( pref<dtTileCacheParams>* _this, int value ) {
	_unref(_this)->width = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheParams_set_width,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheParams_get_height)( pref<dtTileCacheParams>* _this ) {
	return _unref(_this)->height;
}
DEFINE_PRIM(_I32,TileCacheParams_get_height,_IDL);
HL_PRIM int HL_NAME(TileCacheParams_set_height)( pref<dtTileCacheParams>* _this, int value ) {
	_unref(_this)->height = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheParams_set_height,_IDL _I32);

HL_PRIM float HL_NAME(TileCacheParams_get_walkableHeight)( pref<dtTileCacheParams>* _this ) {
	return _unref(_this)->walkableHeight;
}
DEFINE_PRIM(_F32,TileCacheParams_get_walkableHeight,_IDL);
HL_PRIM float HL_NAME(TileCacheParams_set_walkableHeight)( pref<dtTileCacheParams>* _this, float value ) {
	_unref(_this)->walkableHeight = (value);
	return value;
}
DEFINE_PRIM(_F32,TileCacheParams_set_walkableHeight,_IDL _F32);

HL_PRIM float HL_NAME(TileCacheParams_get_walkableRadius)( pref<dtTileCacheParams>* _this ) {
	return _unref(_this)->walkableRadius;
}
DEFINE_PRIM(_F32,TileCacheParams_get_walkableRadius,_IDL);
HL_PRIM float HL_NAME(TileCacheParams_set_walkableRadius)( pref<dtTileCacheParams>* _this, float value ) {
	_unref(_this)->walkableRadius = (value);
	return value;
}
DEFINE_PRIM(_F32,TileCacheParams_set_walkableRadius,_IDL _F32);

HL_PRIM float HL_NAME(TileCacheParams_get_walkableClimb)( pref<dtTileCacheParams>* _this ) {
	return _unref(_this)->walkableClimb;
}
DEFINE_PRIM(_F32,TileCacheParams_get_walkableClimb,_IDL);
HL_PRIM float HL_NAME(TileCacheParams_set_walkableClimb)( pref<dtTileCacheParams>* _this, float value ) {
	_unref(_this)->walkableClimb = (value);
	return value;
}
DEFINE_PRIM(_F32,TileCacheParams_set_walkableClimb,_IDL _F32);

HL_PRIM float HL_NAME(TileCacheParams_get_maxSimplificationError)( pref<dtTileCacheParams>* _this ) {
	return _unref(_this)->maxSimplificationError;
}
DEFINE_PRIM(_F32,TileCacheParams_get_maxSimplificationError,_IDL);
HL_PRIM float HL_NAME(TileCacheParams_set_maxSimplificationError)( pref<dtTileCacheParams>* _this, float value ) {
	_unref(_this)->maxSimplificationError = (value);
	return value;
}
DEFINE_PRIM(_F32,TileCacheParams_set_maxSimplificationError,_IDL _F32);

HL_PRIM int HL_NAME(TileCacheParams_get_maxTiles)( pref<dtTileCacheParams>* _this ) {
	return _unref(_this)->maxTiles;
}
DEFINE_PRIM(_I32,TileCacheParams_get_maxTiles,_IDL);
HL_PRIM int HL_NAME(TileCacheParams_set_maxTiles)( pref<dtTileCacheParams>* _this, int value ) {
	_unref(_this)->maxTiles = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheParams_set_maxTiles,_IDL _I32);

HL_PRIM int HL_NAME(TileCacheParams_get_maxObstacles)( pref<dtTileCacheParams>* _this ) {
	return _unref(_this)->maxObstacles;
}
DEFINE_PRIM(_I32,TileCacheParams_get_maxObstacles,_IDL);
HL_PRIM int HL_NAME(TileCacheParams_set_maxObstacles)( pref<dtTileCacheParams>* _this, int value ) {
	_unref(_this)->maxObstacles = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheParams_set_maxObstacles,_IDL _I32);

HL_PRIM pref<TileCacheData>* HL_NAME(TileCacheData_new0)() {
	return alloc_ref((new TileCacheData()),TileCacheData);
}
DEFINE_PRIM(_IDL, TileCacheData_new0,);

HL_PRIM int HL_NAME(TileCacheData_get_dataSize)( pref<TileCacheData>* _this ) {
	return _unref(_this)->dataSize;
}
DEFINE_PRIM(_I32,TileCacheData_get_dataSize,_IDL);
HL_PRIM int HL_NAME(TileCacheData_set_dataSize)( pref<TileCacheData>* _this, int value ) {
	_unref(_this)->dataSize = (value);
	return value;
}
DEFINE_PRIM(_I32,TileCacheData_set_dataSize,_IDL _I32);

HL_PRIM vbyte* HL_NAME(TileCacheData_get_data)( pref<TileCacheData>* _this ) {
	return (vbyte *)(&_unref(_this)->data[0]);
}
DEFINE_PRIM(_BYTES,TileCacheData_get_data,_IDL);
HL_PRIM vbyte* HL_NAME(TileCacheData_set_data)( pref<TileCacheData>* _this, vbyte* value ) {
	_unref(_this)->data = (unsigned char*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,TileCacheData_set_data,_IDL _BYTES);

HL_PRIM pref<FastLZCompressor>* HL_NAME(FastLZCompressor_new0)() {
	return alloc_ref((new FastLZCompressor()),FastLZCompressor);
}
DEFINE_PRIM(_IDL, FastLZCompressor_new0,);

HL_PRIM HL_CONST pref<dtTileCacheCompressor>* HL_NAME(FastLZCompressor_asSuper0)(pref<FastLZCompressor>* _this) {
	return alloc_ref_const((_unref(_this)->asSuper()),TileCacheCompressor);
}
DEFINE_PRIM(_IDL, FastLZCompressor_asSuper0, _IDL);

HL_PRIM pref<LinearAllocator>* HL_NAME(LinearAllocator_new1)(int capacity) {
	return alloc_ref((new LinearAllocator(capacity)),LinearAllocator);
}
DEFINE_PRIM(_IDL, LinearAllocator_new1, _I32);

HL_PRIM HL_CONST pref<dtTileCacheAlloc>* HL_NAME(LinearAllocator_asSuper0)(pref<LinearAllocator>* _this) {
	return alloc_ref_const((_unref(_this)->asSuper()),TileCacheAlloc);
}
DEFINE_PRIM(_IDL, LinearAllocator_asSuper0, _IDL);

HL_PRIM pref<RemapProcessor>* HL_NAME(RemapProcessor_new0)() {
	return alloc_ref((new RemapProcessor()),RemapProcessor);
}
DEFINE_PRIM(_IDL, RemapProcessor_new0,);

HL_PRIM HL_CONST pref<dtTileCacheMeshProcess>* HL_NAME(RemapProcessor_asSuper0)(pref<RemapProcessor>* _this) {
	return alloc_ref_const((_unref(_this)->asSuper()),TileCacheMeshProcess);
}
DEFINE_PRIM(_IDL, RemapProcessor_asSuper0, _IDL);

HL_PRIM int HL_NAME(Poly_get_firstLink)( pref<dtPoly>* _this ) {
	return _unref(_this)->firstLink;
}
DEFINE_PRIM(_I32,Poly_get_firstLink,_IDL);
HL_PRIM int HL_NAME(Poly_set_firstLink)( pref<dtPoly>* _this, int value ) {
	_unref(_this)->firstLink = (value);
	return value;
}
DEFINE_PRIM(_I32,Poly_set_firstLink,_IDL _I32);

HL_PRIM unsigned short HL_NAME(Poly_get_flags)( pref<dtPoly>* _this ) {
	return _unref(_this)->flags;
}
DEFINE_PRIM(_I16,Poly_get_flags,_IDL);
HL_PRIM unsigned short HL_NAME(Poly_set_flags)( pref<dtPoly>* _this, unsigned short value ) {
	_unref(_this)->flags = (value);
	return value;
}
DEFINE_PRIM(_I16,Poly_set_flags,_IDL _I16);

HL_PRIM unsigned char HL_NAME(Poly_get_vertCount)( pref<dtPoly>* _this ) {
	return _unref(_this)->vertCount;
}
DEFINE_PRIM(_I8,Poly_get_vertCount,_IDL);
HL_PRIM unsigned char HL_NAME(Poly_set_vertCount)( pref<dtPoly>* _this, unsigned char value ) {
	_unref(_this)->vertCount = (value);
	return value;
}
DEFINE_PRIM(_I8,Poly_set_vertCount,_IDL _I8);

HL_PRIM unsigned char HL_NAME(Poly_get_areaAndtype)( pref<dtPoly>* _this ) {
	return _unref(_this)->areaAndtype;
}
DEFINE_PRIM(_I8,Poly_get_areaAndtype,_IDL);
HL_PRIM unsigned char HL_NAME(Poly_set_areaAndtype)( pref<dtPoly>* _this, unsigned char value ) {
	_unref(_this)->areaAndtype = (value);
	return value;
}
DEFINE_PRIM(_I8,Poly_set_areaAndtype,_IDL _I8);

HL_PRIM void HL_NAME(Poly_setArea1)(pref<dtPoly>* _this, unsigned char a) {
	(_unref(_this)->setArea(a));
}
DEFINE_PRIM(_VOID, Poly_setArea1, _IDL _I8);

HL_PRIM void HL_NAME(Poly_setType1)(pref<dtPoly>* _this, unsigned char t) {
	(_unref(_this)->setType(t));
}
DEFINE_PRIM(_VOID, Poly_setType1, _IDL _I8);

HL_PRIM HL_CONST unsigned char HL_NAME(Poly_getArea0)(pref<dtPoly>* _this) {
	return (_unref(_this)->getArea());
}
DEFINE_PRIM(_I8, Poly_getArea0, _IDL);

HL_PRIM HL_CONST unsigned char HL_NAME(Poly_getType0)(pref<dtPoly>* _this) {
	return (_unref(_this)->getType());
}
DEFINE_PRIM(_I8, Poly_getType0, _IDL);

HL_PRIM int HL_NAME(PolyDetail_get_vertBase)( pref<dtPolyDetail>* _this ) {
	return _unref(_this)->vertBase;
}
DEFINE_PRIM(_I32,PolyDetail_get_vertBase,_IDL);
HL_PRIM int HL_NAME(PolyDetail_set_vertBase)( pref<dtPolyDetail>* _this, int value ) {
	_unref(_this)->vertBase = (value);
	return value;
}
DEFINE_PRIM(_I32,PolyDetail_set_vertBase,_IDL _I32);

HL_PRIM int HL_NAME(PolyDetail_get_triBase)( pref<dtPolyDetail>* _this ) {
	return _unref(_this)->triBase;
}
DEFINE_PRIM(_I32,PolyDetail_get_triBase,_IDL);
HL_PRIM int HL_NAME(PolyDetail_set_triBase)( pref<dtPolyDetail>* _this, int value ) {
	_unref(_this)->triBase = (value);
	return value;
}
DEFINE_PRIM(_I32,PolyDetail_set_triBase,_IDL _I32);

HL_PRIM unsigned char HL_NAME(PolyDetail_get_vertCount)( pref<dtPolyDetail>* _this ) {
	return _unref(_this)->vertCount;
}
DEFINE_PRIM(_I8,PolyDetail_get_vertCount,_IDL);
HL_PRIM unsigned char HL_NAME(PolyDetail_set_vertCount)( pref<dtPolyDetail>* _this, unsigned char value ) {
	_unref(_this)->vertCount = (value);
	return value;
}
DEFINE_PRIM(_I8,PolyDetail_set_vertCount,_IDL _I8);

HL_PRIM unsigned char HL_NAME(PolyDetail_get_triCount)( pref<dtPolyDetail>* _this ) {
	return _unref(_this)->triCount;
}
DEFINE_PRIM(_I8,PolyDetail_get_triCount,_IDL);
HL_PRIM unsigned char HL_NAME(PolyDetail_set_triCount)( pref<dtPolyDetail>* _this, unsigned char value ) {
	_unref(_this)->triCount = (value);
	return value;
}
DEFINE_PRIM(_I8,PolyDetail_set_triCount,_IDL _I8);

HL_PRIM unsigned int HL_NAME(Link_get_ref)( pref<dtLink>* _this ) {
	return _unref(_this)->ref;
}
DEFINE_PRIM(_I32,Link_get_ref,_IDL);
HL_PRIM unsigned int HL_NAME(Link_set_ref)( pref<dtLink>* _this, unsigned int value ) {
	_unref(_this)->ref = (value);
	return value;
}
DEFINE_PRIM(_I32,Link_set_ref,_IDL _I32);

HL_PRIM int HL_NAME(Link_get_next)( pref<dtLink>* _this ) {
	return _unref(_this)->next;
}
DEFINE_PRIM(_I32,Link_get_next,_IDL);
HL_PRIM int HL_NAME(Link_set_next)( pref<dtLink>* _this, int value ) {
	_unref(_this)->next = (value);
	return value;
}
DEFINE_PRIM(_I32,Link_set_next,_IDL _I32);

HL_PRIM unsigned char HL_NAME(Link_get_edge)( pref<dtLink>* _this ) {
	return _unref(_this)->edge;
}
DEFINE_PRIM(_I8,Link_get_edge,_IDL);
HL_PRIM unsigned char HL_NAME(Link_set_edge)( pref<dtLink>* _this, unsigned char value ) {
	_unref(_this)->edge = (value);
	return value;
}
DEFINE_PRIM(_I8,Link_set_edge,_IDL _I8);

HL_PRIM unsigned char HL_NAME(Link_get_side)( pref<dtLink>* _this ) {
	return _unref(_this)->side;
}
DEFINE_PRIM(_I8,Link_get_side,_IDL);
HL_PRIM unsigned char HL_NAME(Link_set_side)( pref<dtLink>* _this, unsigned char value ) {
	_unref(_this)->side = (value);
	return value;
}
DEFINE_PRIM(_I8,Link_set_side,_IDL _I8);

HL_PRIM unsigned char HL_NAME(Link_get_bmin)( pref<dtLink>* _this ) {
	return _unref(_this)->bmin;
}
DEFINE_PRIM(_I8,Link_get_bmin,_IDL);
HL_PRIM unsigned char HL_NAME(Link_set_bmin)( pref<dtLink>* _this, unsigned char value ) {
	_unref(_this)->bmin = (value);
	return value;
}
DEFINE_PRIM(_I8,Link_set_bmin,_IDL _I8);

HL_PRIM unsigned char HL_NAME(Link_get_bmax)( pref<dtLink>* _this ) {
	return _unref(_this)->bmax;
}
DEFINE_PRIM(_I8,Link_get_bmax,_IDL);
HL_PRIM unsigned char HL_NAME(Link_set_bmax)( pref<dtLink>* _this, unsigned char value ) {
	_unref(_this)->bmax = (value);
	return value;
}
DEFINE_PRIM(_I8,Link_set_bmax,_IDL _I8);

HL_PRIM int HL_NAME(BVNode_get_i)( pref<dtBVNode>* _this ) {
	return _unref(_this)->i;
}
DEFINE_PRIM(_I32,BVNode_get_i,_IDL);
HL_PRIM int HL_NAME(BVNode_set_i)( pref<dtBVNode>* _this, int value ) {
	_unref(_this)->i = (value);
	return value;
}
DEFINE_PRIM(_I32,BVNode_set_i,_IDL _I32);

HL_PRIM float HL_NAME(OffMeshConnection_get_rad)( pref<dtOffMeshConnection>* _this ) {
	return _unref(_this)->rad;
}
DEFINE_PRIM(_F32,OffMeshConnection_get_rad,_IDL);
HL_PRIM float HL_NAME(OffMeshConnection_set_rad)( pref<dtOffMeshConnection>* _this, float value ) {
	_unref(_this)->rad = (value);
	return value;
}
DEFINE_PRIM(_F32,OffMeshConnection_set_rad,_IDL _F32);

HL_PRIM unsigned short HL_NAME(OffMeshConnection_get_poly)( pref<dtOffMeshConnection>* _this ) {
	return _unref(_this)->poly;
}
DEFINE_PRIM(_I16,OffMeshConnection_get_poly,_IDL);
HL_PRIM unsigned short HL_NAME(OffMeshConnection_set_poly)( pref<dtOffMeshConnection>* _this, unsigned short value ) {
	_unref(_this)->poly = (value);
	return value;
}
DEFINE_PRIM(_I16,OffMeshConnection_set_poly,_IDL _I16);

HL_PRIM unsigned char HL_NAME(OffMeshConnection_get_flags)( pref<dtOffMeshConnection>* _this ) {
	return _unref(_this)->flags;
}
DEFINE_PRIM(_I8,OffMeshConnection_get_flags,_IDL);
HL_PRIM unsigned char HL_NAME(OffMeshConnection_set_flags)( pref<dtOffMeshConnection>* _this, unsigned char value ) {
	_unref(_this)->flags = (value);
	return value;
}
DEFINE_PRIM(_I8,OffMeshConnection_set_flags,_IDL _I8);

HL_PRIM unsigned char HL_NAME(OffMeshConnection_get_side)( pref<dtOffMeshConnection>* _this ) {
	return _unref(_this)->side;
}
DEFINE_PRIM(_I8,OffMeshConnection_get_side,_IDL);
HL_PRIM unsigned char HL_NAME(OffMeshConnection_set_side)( pref<dtOffMeshConnection>* _this, unsigned char value ) {
	_unref(_this)->side = (value);
	return value;
}
DEFINE_PRIM(_I8,OffMeshConnection_set_side,_IDL _I8);

HL_PRIM int HL_NAME(OffMeshConnection_get_userId)( pref<dtOffMeshConnection>* _this ) {
	return _unref(_this)->userId;
}
DEFINE_PRIM(_I32,OffMeshConnection_get_userId,_IDL);
HL_PRIM int HL_NAME(OffMeshConnection_set_userId)( pref<dtOffMeshConnection>* _this, int value ) {
	_unref(_this)->userId = (value);
	return value;
}
DEFINE_PRIM(_I32,OffMeshConnection_set_userId,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_magic)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->magic;
}
DEFINE_PRIM(_I32,MeshHeader_get_magic,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_magic)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->magic = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_magic,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_version)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->version;
}
DEFINE_PRIM(_I32,MeshHeader_get_version,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_version)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->version = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_version,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_x)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->x;
}
DEFINE_PRIM(_I32,MeshHeader_get_x,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_x)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->x = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_x,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_y)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->y;
}
DEFINE_PRIM(_I32,MeshHeader_get_y,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_y)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->y = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_y,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_layer)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->layer;
}
DEFINE_PRIM(_I32,MeshHeader_get_layer,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_layer)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->layer = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_layer,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_userId)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->userId;
}
DEFINE_PRIM(_I32,MeshHeader_get_userId,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_userId)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->userId = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_userId,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_polyCount)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->polyCount;
}
DEFINE_PRIM(_I32,MeshHeader_get_polyCount,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_polyCount)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->polyCount = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_polyCount,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_vertCount)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->vertCount;
}
DEFINE_PRIM(_I32,MeshHeader_get_vertCount,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_vertCount)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->vertCount = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_vertCount,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_maxLinkCount)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->maxLinkCount;
}
DEFINE_PRIM(_I32,MeshHeader_get_maxLinkCount,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_maxLinkCount)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->maxLinkCount = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_maxLinkCount,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_detailMeshCount)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->detailMeshCount;
}
DEFINE_PRIM(_I32,MeshHeader_get_detailMeshCount,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_detailMeshCount)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->detailMeshCount = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_detailMeshCount,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_detailVertCount)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->detailVertCount;
}
DEFINE_PRIM(_I32,MeshHeader_get_detailVertCount,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_detailVertCount)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->detailVertCount = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_detailVertCount,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_detailTriCount)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->detailTriCount;
}
DEFINE_PRIM(_I32,MeshHeader_get_detailTriCount,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_detailTriCount)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->detailTriCount = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_detailTriCount,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_bvNodeCount)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->bvNodeCount;
}
DEFINE_PRIM(_I32,MeshHeader_get_bvNodeCount,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_bvNodeCount)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->bvNodeCount = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_bvNodeCount,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_offMeshConCount)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->offMeshConCount;
}
DEFINE_PRIM(_I32,MeshHeader_get_offMeshConCount,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_offMeshConCount)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->offMeshConCount = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_offMeshConCount,_IDL _I32);

HL_PRIM int HL_NAME(MeshHeader_get_offMeshBase)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->offMeshBase;
}
DEFINE_PRIM(_I32,MeshHeader_get_offMeshBase,_IDL);
HL_PRIM int HL_NAME(MeshHeader_set_offMeshBase)( pref<dtMeshHeader>* _this, int value ) {
	_unref(_this)->offMeshBase = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshHeader_set_offMeshBase,_IDL _I32);

HL_PRIM float HL_NAME(MeshHeader_get_walkableHeight)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->walkableHeight;
}
DEFINE_PRIM(_F32,MeshHeader_get_walkableHeight,_IDL);
HL_PRIM float HL_NAME(MeshHeader_set_walkableHeight)( pref<dtMeshHeader>* _this, float value ) {
	_unref(_this)->walkableHeight = (value);
	return value;
}
DEFINE_PRIM(_F32,MeshHeader_set_walkableHeight,_IDL _F32);

HL_PRIM float HL_NAME(MeshHeader_get_walkableRadius)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->walkableRadius;
}
DEFINE_PRIM(_F32,MeshHeader_get_walkableRadius,_IDL);
HL_PRIM float HL_NAME(MeshHeader_set_walkableRadius)( pref<dtMeshHeader>* _this, float value ) {
	_unref(_this)->walkableRadius = (value);
	return value;
}
DEFINE_PRIM(_F32,MeshHeader_set_walkableRadius,_IDL _F32);

HL_PRIM float HL_NAME(MeshHeader_get_walkableClimb)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->walkableClimb;
}
DEFINE_PRIM(_F32,MeshHeader_get_walkableClimb,_IDL);
HL_PRIM float HL_NAME(MeshHeader_set_walkableClimb)( pref<dtMeshHeader>* _this, float value ) {
	_unref(_this)->walkableClimb = (value);
	return value;
}
DEFINE_PRIM(_F32,MeshHeader_set_walkableClimb,_IDL _F32);

HL_PRIM _hl_float3* HL_NAME(MeshHeader_get_bmin)( pref<dtMeshHeader>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmin);
}
HL_PRIM void HL_NAME(MeshHeader_getbminv)( pref<dtMeshHeader>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmin)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,MeshHeader_getbminv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,MeshHeader_get_bmin,_IDL);
HL_PRIM _hl_float3* HL_NAME(MeshHeader_set_bmin)( pref<dtMeshHeader>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmin)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(MeshHeader_setbmin3)( pref<dtMeshHeader>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,MeshHeader_setbmin3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,MeshHeader_set_bmin,_IDL _STRUCT);

HL_PRIM _hl_float3* HL_NAME(MeshHeader_get_bmax)( pref<dtMeshHeader>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmax);
}
HL_PRIM void HL_NAME(MeshHeader_getbmaxv)( pref<dtMeshHeader>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmax)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,MeshHeader_getbmaxv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,MeshHeader_get_bmax,_IDL);
HL_PRIM _hl_float3* HL_NAME(MeshHeader_set_bmax)( pref<dtMeshHeader>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmax)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(MeshHeader_setbmax3)( pref<dtMeshHeader>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,MeshHeader_setbmax3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,MeshHeader_set_bmax,_IDL _STRUCT);

HL_PRIM float HL_NAME(MeshHeader_get_bvQuantFactor)( pref<dtMeshHeader>* _this ) {
	return _unref(_this)->bvQuantFactor;
}
DEFINE_PRIM(_F32,MeshHeader_get_bvQuantFactor,_IDL);
HL_PRIM float HL_NAME(MeshHeader_set_bvQuantFactor)( pref<dtMeshHeader>* _this, float value ) {
	_unref(_this)->bvQuantFactor = (value);
	return value;
}
DEFINE_PRIM(_F32,MeshHeader_set_bvQuantFactor,_IDL _F32);

HL_PRIM int HL_NAME(MeshTile_get_salt)( pref<dtMeshTile>* _this ) {
	return _unref(_this)->salt;
}
DEFINE_PRIM(_I32,MeshTile_get_salt,_IDL);
HL_PRIM int HL_NAME(MeshTile_set_salt)( pref<dtMeshTile>* _this, int value ) {
	_unref(_this)->salt = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshTile_set_salt,_IDL _I32);

HL_PRIM int HL_NAME(MeshTile_get_linksFreeList)( pref<dtMeshTile>* _this ) {
	return _unref(_this)->linksFreeList;
}
DEFINE_PRIM(_I32,MeshTile_get_linksFreeList,_IDL);
HL_PRIM int HL_NAME(MeshTile_set_linksFreeList)( pref<dtMeshTile>* _this, int value ) {
	_unref(_this)->linksFreeList = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshTile_set_linksFreeList,_IDL _I32);

HL_PRIM pref<dtMeshHeader>* HL_NAME(MeshTile_get_header)( pref<dtMeshTile>* _this ) {
	return alloc_ref(_unref(_this)->header,MeshHeader);
}
DEFINE_PRIM(_IDL,MeshTile_get_header,_IDL);
HL_PRIM pref<dtMeshHeader>* HL_NAME(MeshTile_set_header)( pref<dtMeshTile>* _this, pref<dtMeshHeader>* value ) {
	_unref(_this)->header = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,MeshTile_set_header,_IDL _IDL);

HL_PRIM pref<dtPoly>* HL_NAME(MeshTile_get_polys)( pref<dtMeshTile>* _this ) {
	return alloc_ref(_unref(_this)->polys,Poly);
}
DEFINE_PRIM(_IDL,MeshTile_get_polys,_IDL);
HL_PRIM pref<dtPoly>* HL_NAME(MeshTile_set_polys)( pref<dtMeshTile>* _this, pref<dtPoly>* value ) {
	_unref(_this)->polys = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,MeshTile_set_polys,_IDL _IDL);

HL_PRIM vbyte* HL_NAME(MeshTile_get_verts)( pref<dtMeshTile>* _this ) {
	return (vbyte *)(&_unref(_this)->verts[0]);
}
DEFINE_PRIM(_BYTES,MeshTile_get_verts,_IDL);
HL_PRIM vbyte* HL_NAME(MeshTile_set_verts)( pref<dtMeshTile>* _this, vbyte* value ) {
	_unref(_this)->verts = (float*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,MeshTile_set_verts,_IDL _BYTES);

HL_PRIM pref<dtLink>* HL_NAME(MeshTile_get_links)( pref<dtMeshTile>* _this ) {
	return alloc_ref(_unref(_this)->links,Link);
}
DEFINE_PRIM(_IDL,MeshTile_get_links,_IDL);
HL_PRIM pref<dtLink>* HL_NAME(MeshTile_set_links)( pref<dtMeshTile>* _this, pref<dtLink>* value ) {
	_unref(_this)->links = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,MeshTile_set_links,_IDL _IDL);

HL_PRIM pref<dtPolyDetail>* HL_NAME(MeshTile_get_detailMeshes)( pref<dtMeshTile>* _this ) {
	return alloc_ref(_unref(_this)->detailMeshes,PolyDetail);
}
DEFINE_PRIM(_IDL,MeshTile_get_detailMeshes,_IDL);
HL_PRIM pref<dtPolyDetail>* HL_NAME(MeshTile_set_detailMeshes)( pref<dtMeshTile>* _this, pref<dtPolyDetail>* value ) {
	_unref(_this)->detailMeshes = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,MeshTile_set_detailMeshes,_IDL _IDL);

HL_PRIM vbyte* HL_NAME(MeshTile_get_detailVerts)( pref<dtMeshTile>* _this ) {
	return (vbyte *)(&_unref(_this)->detailVerts[0]);
}
DEFINE_PRIM(_BYTES,MeshTile_get_detailVerts,_IDL);
HL_PRIM vbyte* HL_NAME(MeshTile_set_detailVerts)( pref<dtMeshTile>* _this, vbyte* value ) {
	_unref(_this)->detailVerts = (float*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,MeshTile_set_detailVerts,_IDL _BYTES);

HL_PRIM vbyte* HL_NAME(MeshTile_get_detailTris)( pref<dtMeshTile>* _this ) {
	return (vbyte *)(&_unref(_this)->detailTris[0]);
}
DEFINE_PRIM(_BYTES,MeshTile_get_detailTris,_IDL);
HL_PRIM vbyte* HL_NAME(MeshTile_set_detailTris)( pref<dtMeshTile>* _this, vbyte* value ) {
	_unref(_this)->detailTris = (unsigned char*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,MeshTile_set_detailTris,_IDL _BYTES);

HL_PRIM pref<dtBVNode>* HL_NAME(MeshTile_get_bvTree)( pref<dtMeshTile>* _this ) {
	return alloc_ref(_unref(_this)->bvTree,BVNode);
}
DEFINE_PRIM(_IDL,MeshTile_get_bvTree,_IDL);
HL_PRIM pref<dtBVNode>* HL_NAME(MeshTile_set_bvTree)( pref<dtMeshTile>* _this, pref<dtBVNode>* value ) {
	_unref(_this)->bvTree = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,MeshTile_set_bvTree,_IDL _IDL);

HL_PRIM pref<dtOffMeshConnection>* HL_NAME(MeshTile_get_offMeshCons)( pref<dtMeshTile>* _this ) {
	return alloc_ref(_unref(_this)->offMeshCons,OffMeshConnection);
}
DEFINE_PRIM(_IDL,MeshTile_get_offMeshCons,_IDL);
HL_PRIM pref<dtOffMeshConnection>* HL_NAME(MeshTile_set_offMeshCons)( pref<dtMeshTile>* _this, pref<dtOffMeshConnection>* value ) {
	_unref(_this)->offMeshCons = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,MeshTile_set_offMeshCons,_IDL _IDL);

HL_PRIM int HL_NAME(MeshTile_get_dataSize)( pref<dtMeshTile>* _this ) {
	return _unref(_this)->dataSize;
}
DEFINE_PRIM(_I32,MeshTile_get_dataSize,_IDL);
HL_PRIM int HL_NAME(MeshTile_set_dataSize)( pref<dtMeshTile>* _this, int value ) {
	_unref(_this)->dataSize = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshTile_set_dataSize,_IDL _I32);

HL_PRIM int HL_NAME(MeshTile_get_flags)( pref<dtMeshTile>* _this ) {
	return _unref(_this)->flags;
}
DEFINE_PRIM(_I32,MeshTile_get_flags,_IDL);
HL_PRIM int HL_NAME(MeshTile_set_flags)( pref<dtMeshTile>* _this, int value ) {
	_unref(_this)->flags = (value);
	return value;
}
DEFINE_PRIM(_I32,MeshTile_set_flags,_IDL _I32);

HL_PRIM pref<dtMeshTile>* HL_NAME(MeshTile_get_next)( pref<dtMeshTile>* _this ) {
	return alloc_ref(_unref(_this)->next,MeshTile);
}
DEFINE_PRIM(_IDL,MeshTile_get_next,_IDL);
HL_PRIM pref<dtMeshTile>* HL_NAME(MeshTile_set_next)( pref<dtMeshTile>* _this, pref<dtMeshTile>* value ) {
	_unref(_this)->next = _unref_ptr_safe(value);
	return value;
}
DEFINE_PRIM(_IDL,MeshTile_set_next,_IDL _IDL);

HL_PRIM pref<dtNavMeshParams>* HL_NAME(NavMeshParams_new0)() {
	return alloc_ref((new dtNavMeshParams()),NavMeshParams);
}
DEFINE_PRIM(_IDL, NavMeshParams_new0,);

HL_PRIM _hl_float3* HL_NAME(NavMeshParams_get_orig)( pref<dtNavMeshParams>* _this ) {
	return (_hl_float3* )(_unref(_this)->orig);
}
HL_PRIM void HL_NAME(NavMeshParams_getorigv)( pref<dtNavMeshParams>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->orig)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,NavMeshParams_getorigv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,NavMeshParams_get_orig,_IDL);
HL_PRIM _hl_float3* HL_NAME(NavMeshParams_set_orig)( pref<dtNavMeshParams>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->orig)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(NavMeshParams_setorig3)( pref<dtNavMeshParams>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->orig);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,NavMeshParams_setorig3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,NavMeshParams_set_orig,_IDL _STRUCT);

HL_PRIM float HL_NAME(NavMeshParams_get_tileWidth)( pref<dtNavMeshParams>* _this ) {
	return _unref(_this)->tileWidth;
}
DEFINE_PRIM(_F32,NavMeshParams_get_tileWidth,_IDL);
HL_PRIM float HL_NAME(NavMeshParams_set_tileWidth)( pref<dtNavMeshParams>* _this, float value ) {
	_unref(_this)->tileWidth = (value);
	return value;
}
DEFINE_PRIM(_F32,NavMeshParams_set_tileWidth,_IDL _F32);

HL_PRIM float HL_NAME(NavMeshParams_get_tileHeight)( pref<dtNavMeshParams>* _this ) {
	return _unref(_this)->tileHeight;
}
DEFINE_PRIM(_F32,NavMeshParams_get_tileHeight,_IDL);
HL_PRIM float HL_NAME(NavMeshParams_set_tileHeight)( pref<dtNavMeshParams>* _this, float value ) {
	_unref(_this)->tileHeight = (value);
	return value;
}
DEFINE_PRIM(_F32,NavMeshParams_set_tileHeight,_IDL _F32);

HL_PRIM int HL_NAME(NavMeshParams_get_maxTiles)( pref<dtNavMeshParams>* _this ) {
	return _unref(_this)->maxTiles;
}
DEFINE_PRIM(_I32,NavMeshParams_get_maxTiles,_IDL);
HL_PRIM int HL_NAME(NavMeshParams_set_maxTiles)( pref<dtNavMeshParams>* _this, int value ) {
	_unref(_this)->maxTiles = (value);
	return value;
}
DEFINE_PRIM(_I32,NavMeshParams_set_maxTiles,_IDL _I32);

HL_PRIM int HL_NAME(NavMeshParams_get_maxPolys)( pref<dtNavMeshParams>* _this ) {
	return _unref(_this)->maxPolys;
}
DEFINE_PRIM(_I32,NavMeshParams_get_maxPolys,_IDL);
HL_PRIM int HL_NAME(NavMeshParams_set_maxPolys)( pref<dtNavMeshParams>* _this, int value ) {
	_unref(_this)->maxPolys = (value);
	return value;
}
DEFINE_PRIM(_I32,NavMeshParams_set_maxPolys,_IDL _I32);

HL_PRIM pref<dtNavMesh>* HL_NAME(NavMesh_new0)() {
	return alloc_ref((new dtNavMesh()),NavMesh);
}
DEFINE_PRIM(_IDL, NavMesh_new0,);

HL_PRIM unsigned int HL_NAME(NavMesh_setParams1)(pref<dtNavMesh>* _this, pref<dtNavMeshParams>* params) {
	return (_unref(_this)->init(_unref_ptr_safe(params)));
}
DEFINE_PRIM(_I32, NavMesh_setParams1, _IDL _IDL);

HL_PRIM unsigned int HL_NAME(NavMesh_init3)(pref<dtNavMesh>* _this, void* data, int dataSize, int flags) {
	return (_unref(_this)->init((unsigned char *)data, dataSize, flags));
}
DEFINE_PRIM(_I32, NavMesh_init3, _IDL _BYTES _I32 _I32);

HL_PRIM HL_CONST pref<dtNavMeshParams>* HL_NAME(NavMesh_getParams0)(pref<dtNavMesh>* _this) {
	return alloc_ref_const((_unref(_this)->getParams()),NavMeshParams);
}
DEFINE_PRIM(_IDL, NavMesh_getParams0, _IDL);

HL_PRIM bool HL_NAME(NavMesh_isValidPolyRef1)(pref<dtNavMesh>* _this, unsigned int ref) {
	return (_unref(_this)->isValidPolyRef(ref));
}
DEFINE_PRIM(_BOOL, NavMesh_isValidPolyRef1, _IDL _I32);

HL_PRIM unsigned int HL_NAME(NavMesh_getOffMeshConnectionPolyEndPoints4)(pref<dtNavMesh>* _this, unsigned int prevRef, unsigned int polyRef, varray* startPos, varray* endPos) {
	return (_unref(_this)->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, hl_aptr(startPos,float), hl_aptr(endPos,float)));
}
DEFINE_PRIM(_I32, NavMesh_getOffMeshConnectionPolyEndPoints4, _IDL _I32 _I32 _ARR _ARR);

HL_PRIM HL_CONST pref<dtOffMeshConnection>* HL_NAME(NavMesh_getOffMeshConnectionByRef1)(pref<dtNavMesh>* _this, unsigned int ref) {
	return alloc_ref_const((_unref(_this)->getOffMeshConnectionByRef(ref)),OffMeshConnection);
}
DEFINE_PRIM(_IDL, NavMesh_getOffMeshConnectionByRef1, _IDL _I32);

HL_PRIM unsigned int HL_NAME(NavMesh_setPolyFlags2)(pref<dtNavMesh>* _this, unsigned int ref, unsigned short flags) {
	return (_unref(_this)->setPolyFlags(ref, flags));
}
DEFINE_PRIM(_I32, NavMesh_setPolyFlags2, _IDL _I32 _I16);

HL_PRIM unsigned int HL_NAME(NavMesh_getPolyFlags2)(pref<dtNavMesh>* _this, unsigned int ref, varray* resultFlags) {
	return (_unref(_this)->getPolyFlags(ref, hl_aptr(resultFlags,unsigned short)));
}
DEFINE_PRIM(_I32, NavMesh_getPolyFlags2, _IDL _I32 _ARR);

HL_PRIM unsigned int HL_NAME(NavMesh_setPolyArea2)(pref<dtNavMesh>* _this, unsigned int ref, unsigned char area) {
	return (_unref(_this)->setPolyArea(ref, area));
}
DEFINE_PRIM(_I32, NavMesh_setPolyArea2, _IDL _I32 _I8);

HL_PRIM unsigned int HL_NAME(NavMesh_getPolyArea2)(pref<dtNavMesh>* _this, unsigned int ref, varray* resultArea) {
	return (_unref(_this)->getPolyArea(ref, hl_aptr(resultArea,unsigned char)));
}
DEFINE_PRIM(_I32, NavMesh_getPolyArea2, _IDL _I32 _ARR);

HL_PRIM unsigned int HL_NAME(NavMesh_encodePolyId3)(pref<dtNavMesh>* _this, int salt, int it, int ip) {
	return (_unref(_this)->encodePolyId(salt, it, ip));
}
DEFINE_PRIM(_I32, NavMesh_encodePolyId3, _IDL _I32 _I32 _I32);

HL_PRIM int HL_NAME(NavMesh_decodePolyIdSalt1)(pref<dtNavMesh>* _this, unsigned int ref) {
	return (_unref(_this)->decodePolyIdSalt(ref));
}
DEFINE_PRIM(_I32, NavMesh_decodePolyIdSalt1, _IDL _I32);

HL_PRIM int HL_NAME(NavMesh_decodePolyIdTile1)(pref<dtNavMesh>* _this, unsigned int ref) {
	return (_unref(_this)->decodePolyIdTile(ref));
}
DEFINE_PRIM(_I32, NavMesh_decodePolyIdTile1, _IDL _I32);

HL_PRIM int HL_NAME(NavMesh_decodePolyIdPoly1)(pref<dtNavMesh>* _this, unsigned int ref) {
	return (_unref(_this)->decodePolyIdPoly(ref));
}
DEFINE_PRIM(_I32, NavMesh_decodePolyIdPoly1, _IDL _I32);

HL_PRIM int HL_NAME(NavMesh_getDetailTriEdgeFlags2)(unsigned char triFlags, int edgeIndex) {
	return (dtGetDetailTriEdgeFlags(triFlags, edgeIndex));
}
DEFINE_PRIM(_I32, NavMesh_getDetailTriEdgeFlags2, _I8 _I32);

HL_PRIM _hl_float3* HL_NAME(Node_get_pos)( pref<dtNode>* _this ) {
	return (_hl_float3* )(_unref(_this)->pos);
}
HL_PRIM void HL_NAME(Node_getposv)( pref<dtNode>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->pos)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,Node_getposv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,Node_get_pos,_IDL);
HL_PRIM _hl_float3* HL_NAME(Node_set_pos)( pref<dtNode>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->pos)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(Node_setpos3)( pref<dtNode>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->pos);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,Node_setpos3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,Node_set_pos,_IDL _STRUCT);

HL_PRIM float HL_NAME(Node_get_cost)( pref<dtNode>* _this ) {
	return _unref(_this)->cost;
}
DEFINE_PRIM(_F32,Node_get_cost,_IDL);
HL_PRIM float HL_NAME(Node_set_cost)( pref<dtNode>* _this, float value ) {
	_unref(_this)->cost = (value);
	return value;
}
DEFINE_PRIM(_F32,Node_set_cost,_IDL _F32);

HL_PRIM float HL_NAME(Node_get_total)( pref<dtNode>* _this ) {
	return _unref(_this)->total;
}
DEFINE_PRIM(_F32,Node_get_total,_IDL);
HL_PRIM float HL_NAME(Node_set_total)( pref<dtNode>* _this, float value ) {
	_unref(_this)->total = (value);
	return value;
}
DEFINE_PRIM(_F32,Node_set_total,_IDL _F32);

HL_PRIM unsigned int HL_NAME(Node_get_pidx)( pref<dtNode>* _this ) {
	return _unref(_this)->pidx;
}
DEFINE_PRIM(_I32,Node_get_pidx,_IDL);
HL_PRIM unsigned int HL_NAME(Node_set_pidx)( pref<dtNode>* _this, unsigned int value ) {
	_unref(_this)->pidx = (value);
	return value;
}
DEFINE_PRIM(_I32,Node_set_pidx,_IDL _I32);

HL_PRIM unsigned int HL_NAME(Node_get_state)( pref<dtNode>* _this ) {
	return _unref(_this)->state;
}
DEFINE_PRIM(_I32,Node_get_state,_IDL);
HL_PRIM unsigned int HL_NAME(Node_set_state)( pref<dtNode>* _this, unsigned int value ) {
	_unref(_this)->state = (value);
	return value;
}
DEFINE_PRIM(_I32,Node_set_state,_IDL _I32);

HL_PRIM unsigned int HL_NAME(Node_get_flags)( pref<dtNode>* _this ) {
	return _unref(_this)->flags;
}
DEFINE_PRIM(_I32,Node_get_flags,_IDL);
HL_PRIM unsigned int HL_NAME(Node_set_flags)( pref<dtNode>* _this, unsigned int value ) {
	_unref(_this)->flags = (value);
	return value;
}
DEFINE_PRIM(_I32,Node_set_flags,_IDL _I32);

HL_PRIM unsigned int HL_NAME(Node_get_id)( pref<dtNode>* _this ) {
	return _unref(_this)->id;
}
DEFINE_PRIM(_I32,Node_get_id,_IDL);
HL_PRIM unsigned int HL_NAME(Node_set_id)( pref<dtNode>* _this, unsigned int value ) {
	_unref(_this)->id = (value);
	return value;
}
DEFINE_PRIM(_I32,Node_set_id,_IDL _I32);

HL_PRIM pref<dtNodePool>* HL_NAME(NodePool_new2)(int maxNodes, int hashSize) {
	return alloc_ref((new dtNodePool(maxNodes, hashSize)),NodePool);
}
DEFINE_PRIM(_IDL, NodePool_new2, _I32 _I32);

HL_PRIM void HL_NAME(NodePool_clear0)(pref<dtNodePool>* _this) {
	(_unref(_this)->clear());
}
DEFINE_PRIM(_VOID, NodePool_clear0, _IDL);

HL_PRIM pref<dtNode>* HL_NAME(NodePool_getNode2)(pref<dtNodePool>* _this, unsigned int id, unsigned char state) {
	return alloc_ref((_unref(_this)->getNode(id, state)),Node);
}
DEFINE_PRIM(_IDL, NodePool_getNode2, _IDL _I32 _I8);

HL_PRIM pref<dtNode>* HL_NAME(NodePool_findNode2)(pref<dtNodePool>* _this, unsigned int id, unsigned char state) {
	return alloc_ref((_unref(_this)->findNode(id, state)),Node);
}
DEFINE_PRIM(_IDL, NodePool_findNode2, _IDL _I32 _I8);

HL_PRIM unsigned int HL_NAME(NodePool_getNodeIdx1)(pref<dtNodePool>* _this, pref<dtNode>* node) {
	return (_unref(_this)->getNodeIdx(_unref_ptr_safe(node)));
}
DEFINE_PRIM(_I32, NodePool_getNodeIdx1, _IDL _IDL);

HL_PRIM pref<dtNode>* HL_NAME(NodePool_getNodeAtIdx1)(pref<dtNodePool>* _this, unsigned int idx) {
	return alloc_ref((_unref(_this)->getNodeAtIdx(idx)),Node);
}
DEFINE_PRIM(_IDL, NodePool_getNodeAtIdx1, _IDL _I32);

HL_PRIM int HL_NAME(NodePool_getMemUsed0)(pref<dtNodePool>* _this) {
	return (_unref(_this)->getMemUsed());
}
DEFINE_PRIM(_I32, NodePool_getMemUsed0, _IDL);

HL_PRIM int HL_NAME(NodePool_getMaxNodes0)(pref<dtNodePool>* _this) {
	return (_unref(_this)->getMaxNodes());
}
DEFINE_PRIM(_I32, NodePool_getMaxNodes0, _IDL);

HL_PRIM int HL_NAME(NodePool_getHashSize0)(pref<dtNodePool>* _this) {
	return (_unref(_this)->getHashSize());
}
DEFINE_PRIM(_I32, NodePool_getHashSize0, _IDL);

HL_PRIM unsigned short HL_NAME(NodePool_getFirst1)(pref<dtNodePool>* _this, int bucket) {
	return (_unref(_this)->getFirst(bucket));
}
DEFINE_PRIM(_I16, NodePool_getFirst1, _IDL _I32);

HL_PRIM unsigned short HL_NAME(NodePool_getNext1)(pref<dtNodePool>* _this, int i) {
	return (_unref(_this)->getNext(i));
}
DEFINE_PRIM(_I16, NodePool_getNext1, _IDL _I32);

HL_PRIM int HL_NAME(NodePool_getNodeCount0)(pref<dtNodePool>* _this) {
	return (_unref(_this)->getNodeCount());
}
DEFINE_PRIM(_I32, NodePool_getNodeCount0, _IDL);

HL_PRIM pref<dtNodeQueue>* HL_NAME(NodeQueue_new1)(int n) {
	return alloc_ref((new dtNodeQueue(n)),NodeQueue);
}
DEFINE_PRIM(_IDL, NodeQueue_new1, _I32);

HL_PRIM void HL_NAME(NodeQueue_clear0)(pref<dtNodeQueue>* _this) {
	(_unref(_this)->clear());
}
DEFINE_PRIM(_VOID, NodeQueue_clear0, _IDL);

HL_PRIM pref<dtNode>* HL_NAME(NodeQueue_top0)(pref<dtNodeQueue>* _this) {
	return alloc_ref((_unref(_this)->top()),Node);
}
DEFINE_PRIM(_IDL, NodeQueue_top0, _IDL);

HL_PRIM pref<dtNode>* HL_NAME(NodeQueue_pop0)(pref<dtNodeQueue>* _this) {
	return alloc_ref((_unref(_this)->pop()),Node);
}
DEFINE_PRIM(_IDL, NodeQueue_pop0, _IDL);

HL_PRIM void HL_NAME(NodeQueue_push1)(pref<dtNodeQueue>* _this, pref<dtNode>* node) {
	(_unref(_this)->push(_unref_ptr_safe(node)));
}
DEFINE_PRIM(_VOID, NodeQueue_push1, _IDL _IDL);

HL_PRIM void HL_NAME(NodeQueue_modify1)(pref<dtNodeQueue>* _this, pref<dtNode>* node) {
	(_unref(_this)->modify(_unref_ptr_safe(node)));
}
DEFINE_PRIM(_VOID, NodeQueue_modify1, _IDL _IDL);

HL_PRIM bool HL_NAME(NodeQueue_empty0)(pref<dtNodeQueue>* _this) {
	return (_unref(_this)->empty());
}
DEFINE_PRIM(_BOOL, NodeQueue_empty0, _IDL);

HL_PRIM int HL_NAME(NodeQueue_getMemUsed0)(pref<dtNodeQueue>* _this) {
	return (_unref(_this)->getMemUsed());
}
DEFINE_PRIM(_I32, NodeQueue_getMemUsed0, _IDL);

HL_PRIM int HL_NAME(NodeQueue_getCapacity0)(pref<dtNodeQueue>* _this) {
	return (_unref(_this)->getCapacity());
}
DEFINE_PRIM(_I32, NodeQueue_getCapacity0, _IDL);

HL_PRIM pref<dtNavMeshCreateParams>* HL_NAME(NavMeshCreateParams_new0)() {
	return alloc_ref((new dtNavMeshCreateParams()),NavMeshCreateParams);
}
DEFINE_PRIM(_IDL, NavMeshCreateParams_new0,);

HL_PRIM vbyte* HL_NAME(NavMeshCreateParams_get_verts)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->verts[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_verts,_IDL);
HL_PRIM vbyte* HL_NAME(NavMeshCreateParams_set_verts)( pref<dtNavMeshCreateParams>* _this, vbyte* value ) {
	_unref(_this)->verts = (unsigned short*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_verts,_IDL _BYTES);

HL_PRIM int HL_NAME(NavMeshCreateParams_get_vertCount)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->vertCount;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_get_vertCount,_IDL);
HL_PRIM int HL_NAME(NavMeshCreateParams_set_vertCount)( pref<dtNavMeshCreateParams>* _this, int value ) {
	_unref(_this)->vertCount = (value);
	return value;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_set_vertCount,_IDL _I32);

HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_get_polys)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->polys[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_polys,_IDL);
HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_set_polys)( pref<dtNavMeshCreateParams>* _this, HL_CONST vbyte* value ) {
	_unref(_this)->polys = (unsigned short*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_polys,_IDL _BYTES);

HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_get_polyFlags)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->polyFlags[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_polyFlags,_IDL);
HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_set_polyFlags)( pref<dtNavMeshCreateParams>* _this, HL_CONST vbyte* value ) {
	_unref(_this)->polyFlags = (unsigned short*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_polyFlags,_IDL _BYTES);

HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_get_polyAreas)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->polyAreas[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_polyAreas,_IDL);
HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_set_polyAreas)( pref<dtNavMeshCreateParams>* _this, HL_CONST vbyte* value ) {
	_unref(_this)->polyAreas = (unsigned char*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_polyAreas,_IDL _BYTES);

HL_PRIM int HL_NAME(NavMeshCreateParams_get_polyCount)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->polyCount;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_get_polyCount,_IDL);
HL_PRIM int HL_NAME(NavMeshCreateParams_set_polyCount)( pref<dtNavMeshCreateParams>* _this, int value ) {
	_unref(_this)->polyCount = (value);
	return value;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_set_polyCount,_IDL _I32);

HL_PRIM int HL_NAME(NavMeshCreateParams_get_nvp)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->nvp;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_get_nvp,_IDL);
HL_PRIM int HL_NAME(NavMeshCreateParams_set_nvp)( pref<dtNavMeshCreateParams>* _this, int value ) {
	_unref(_this)->nvp = (value);
	return value;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_set_nvp,_IDL _I32);

HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_get_detailMeshes)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->detailMeshes[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_detailMeshes,_IDL);
HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_set_detailMeshes)( pref<dtNavMeshCreateParams>* _this, HL_CONST vbyte* value ) {
	_unref(_this)->detailMeshes = (unsigned int*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_detailMeshes,_IDL _BYTES);

HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_get_detailVerts)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->detailVerts[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_detailVerts,_IDL);
HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_set_detailVerts)( pref<dtNavMeshCreateParams>* _this, HL_CONST vbyte* value ) {
	_unref(_this)->detailVerts = (float*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_detailVerts,_IDL _BYTES);

HL_PRIM int HL_NAME(NavMeshCreateParams_get_detailVertsCount)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->detailVertsCount;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_get_detailVertsCount,_IDL);
HL_PRIM int HL_NAME(NavMeshCreateParams_set_detailVertsCount)( pref<dtNavMeshCreateParams>* _this, int value ) {
	_unref(_this)->detailVertsCount = (value);
	return value;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_set_detailVertsCount,_IDL _I32);

HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_get_detailTris)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->detailTris[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_detailTris,_IDL);
HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_set_detailTris)( pref<dtNavMeshCreateParams>* _this, HL_CONST vbyte* value ) {
	_unref(_this)->detailTris = (unsigned char*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_detailTris,_IDL _BYTES);

HL_PRIM int HL_NAME(NavMeshCreateParams_get_detailTriCount)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->detailTriCount;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_get_detailTriCount,_IDL);
HL_PRIM int HL_NAME(NavMeshCreateParams_set_detailTriCount)( pref<dtNavMeshCreateParams>* _this, int value ) {
	_unref(_this)->detailTriCount = (value);
	return value;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_set_detailTriCount,_IDL _I32);

HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_get_offMeshConVerts)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->offMeshConVerts[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_offMeshConVerts,_IDL);
HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_set_offMeshConVerts)( pref<dtNavMeshCreateParams>* _this, HL_CONST vbyte* value ) {
	_unref(_this)->offMeshConVerts = (float*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_offMeshConVerts,_IDL _BYTES);

HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_get_offMeshConRad)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->offMeshConRad[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_offMeshConRad,_IDL);
HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_set_offMeshConRad)( pref<dtNavMeshCreateParams>* _this, HL_CONST vbyte* value ) {
	_unref(_this)->offMeshConRad = (float*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_offMeshConRad,_IDL _BYTES);

HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_get_offMeshConFlags)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->offMeshConFlags[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_offMeshConFlags,_IDL);
HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_set_offMeshConFlags)( pref<dtNavMeshCreateParams>* _this, HL_CONST vbyte* value ) {
	_unref(_this)->offMeshConFlags = (unsigned short*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_offMeshConFlags,_IDL _BYTES);

HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_get_offMeshConAreas)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->offMeshConAreas[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_offMeshConAreas,_IDL);
HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_set_offMeshConAreas)( pref<dtNavMeshCreateParams>* _this, HL_CONST vbyte* value ) {
	_unref(_this)->offMeshConAreas = (unsigned char*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_offMeshConAreas,_IDL _BYTES);

HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_get_offMeshConDir)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->offMeshConDir[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_offMeshConDir,_IDL);
HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_set_offMeshConDir)( pref<dtNavMeshCreateParams>* _this, HL_CONST vbyte* value ) {
	_unref(_this)->offMeshConDir = (unsigned char*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_offMeshConDir,_IDL _BYTES);

HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_get_offMeshConUserID)( pref<dtNavMeshCreateParams>* _this ) {
	return (vbyte *)(&_unref(_this)->offMeshConUserID[0]);
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_get_offMeshConUserID,_IDL);
HL_PRIM HL_CONST vbyte* HL_NAME(NavMeshCreateParams_set_offMeshConUserID)( pref<dtNavMeshCreateParams>* _this, HL_CONST vbyte* value ) {
	_unref(_this)->offMeshConUserID = (unsigned int*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,NavMeshCreateParams_set_offMeshConUserID,_IDL _BYTES);

HL_PRIM int HL_NAME(NavMeshCreateParams_get_offMeshConCount)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->offMeshConCount;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_get_offMeshConCount,_IDL);
HL_PRIM int HL_NAME(NavMeshCreateParams_set_offMeshConCount)( pref<dtNavMeshCreateParams>* _this, int value ) {
	_unref(_this)->offMeshConCount = (value);
	return value;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_set_offMeshConCount,_IDL _I32);

HL_PRIM unsigned int HL_NAME(NavMeshCreateParams_get_userId)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->userId;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_get_userId,_IDL);
HL_PRIM unsigned int HL_NAME(NavMeshCreateParams_set_userId)( pref<dtNavMeshCreateParams>* _this, unsigned int value ) {
	_unref(_this)->userId = (value);
	return value;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_set_userId,_IDL _I32);

HL_PRIM int HL_NAME(NavMeshCreateParams_get_tileX)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->tileX;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_get_tileX,_IDL);
HL_PRIM int HL_NAME(NavMeshCreateParams_set_tileX)( pref<dtNavMeshCreateParams>* _this, int value ) {
	_unref(_this)->tileX = (value);
	return value;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_set_tileX,_IDL _I32);

HL_PRIM int HL_NAME(NavMeshCreateParams_get_tileY)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->tileY;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_get_tileY,_IDL);
HL_PRIM int HL_NAME(NavMeshCreateParams_set_tileY)( pref<dtNavMeshCreateParams>* _this, int value ) {
	_unref(_this)->tileY = (value);
	return value;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_set_tileY,_IDL _I32);

HL_PRIM int HL_NAME(NavMeshCreateParams_get_tileLayer)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->tileLayer;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_get_tileLayer,_IDL);
HL_PRIM int HL_NAME(NavMeshCreateParams_set_tileLayer)( pref<dtNavMeshCreateParams>* _this, int value ) {
	_unref(_this)->tileLayer = (value);
	return value;
}
DEFINE_PRIM(_I32,NavMeshCreateParams_set_tileLayer,_IDL _I32);

HL_PRIM _hl_float3* HL_NAME(NavMeshCreateParams_get_bmin)( pref<dtNavMeshCreateParams>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmin);
}
HL_PRIM void HL_NAME(NavMeshCreateParams_getbminv)( pref<dtNavMeshCreateParams>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmin)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,NavMeshCreateParams_getbminv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,NavMeshCreateParams_get_bmin,_IDL);
HL_PRIM _hl_float3* HL_NAME(NavMeshCreateParams_set_bmin)( pref<dtNavMeshCreateParams>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmin)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(NavMeshCreateParams_setbmin3)( pref<dtNavMeshCreateParams>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,NavMeshCreateParams_setbmin3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,NavMeshCreateParams_set_bmin,_IDL _STRUCT);

HL_PRIM _hl_float3* HL_NAME(NavMeshCreateParams_get_bmax)( pref<dtNavMeshCreateParams>* _this ) {
	return (_hl_float3* )(_unref(_this)->bmax);
}
HL_PRIM void HL_NAME(NavMeshCreateParams_getbmaxv)( pref<dtNavMeshCreateParams>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->bmax)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,NavMeshCreateParams_getbmaxv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,NavMeshCreateParams_get_bmax,_IDL);
HL_PRIM _hl_float3* HL_NAME(NavMeshCreateParams_set_bmax)( pref<dtNavMeshCreateParams>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->bmax)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(NavMeshCreateParams_setbmax3)( pref<dtNavMeshCreateParams>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,NavMeshCreateParams_setbmax3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,NavMeshCreateParams_set_bmax,_IDL _STRUCT);

HL_PRIM float HL_NAME(NavMeshCreateParams_get_walkableHeight)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->walkableHeight;
}
DEFINE_PRIM(_F32,NavMeshCreateParams_get_walkableHeight,_IDL);
HL_PRIM float HL_NAME(NavMeshCreateParams_set_walkableHeight)( pref<dtNavMeshCreateParams>* _this, float value ) {
	_unref(_this)->walkableHeight = (value);
	return value;
}
DEFINE_PRIM(_F32,NavMeshCreateParams_set_walkableHeight,_IDL _F32);

HL_PRIM float HL_NAME(NavMeshCreateParams_get_walkableRadius)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->walkableRadius;
}
DEFINE_PRIM(_F32,NavMeshCreateParams_get_walkableRadius,_IDL);
HL_PRIM float HL_NAME(NavMeshCreateParams_set_walkableRadius)( pref<dtNavMeshCreateParams>* _this, float value ) {
	_unref(_this)->walkableRadius = (value);
	return value;
}
DEFINE_PRIM(_F32,NavMeshCreateParams_set_walkableRadius,_IDL _F32);

HL_PRIM float HL_NAME(NavMeshCreateParams_get_walkableClimb)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->walkableClimb;
}
DEFINE_PRIM(_F32,NavMeshCreateParams_get_walkableClimb,_IDL);
HL_PRIM float HL_NAME(NavMeshCreateParams_set_walkableClimb)( pref<dtNavMeshCreateParams>* _this, float value ) {
	_unref(_this)->walkableClimb = (value);
	return value;
}
DEFINE_PRIM(_F32,NavMeshCreateParams_set_walkableClimb,_IDL _F32);

HL_PRIM float HL_NAME(NavMeshCreateParams_get_cs)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->cs;
}
DEFINE_PRIM(_F32,NavMeshCreateParams_get_cs,_IDL);
HL_PRIM float HL_NAME(NavMeshCreateParams_set_cs)( pref<dtNavMeshCreateParams>* _this, float value ) {
	_unref(_this)->cs = (value);
	return value;
}
DEFINE_PRIM(_F32,NavMeshCreateParams_set_cs,_IDL _F32);

HL_PRIM float HL_NAME(NavMeshCreateParams_get_ch)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->ch;
}
DEFINE_PRIM(_F32,NavMeshCreateParams_get_ch,_IDL);
HL_PRIM float HL_NAME(NavMeshCreateParams_set_ch)( pref<dtNavMeshCreateParams>* _this, float value ) {
	_unref(_this)->ch = (value);
	return value;
}
DEFINE_PRIM(_F32,NavMeshCreateParams_set_ch,_IDL _F32);

HL_PRIM bool HL_NAME(NavMeshCreateParams_get_buildBvTree)( pref<dtNavMeshCreateParams>* _this ) {
	return _unref(_this)->buildBvTree;
}
DEFINE_PRIM(_BOOL,NavMeshCreateParams_get_buildBvTree,_IDL);
HL_PRIM bool HL_NAME(NavMeshCreateParams_set_buildBvTree)( pref<dtNavMeshCreateParams>* _this, bool value ) {
	_unref(_this)->buildBvTree = (value);
	return value;
}
DEFINE_PRIM(_BOOL,NavMeshCreateParams_set_buildBvTree,_IDL _BOOL);

HL_PRIM void* HL_NAME(NavMeshBuilder_dtCreateNavMeshData2)(pref<dtNavMeshCreateParams>* params, int* aLength) {
	unsigned char *__tmparray = nullptr;
	int __tmpLength = -1;
	void* __tmpret;
auto ___retvalue = (dtCreateNavMeshData(_unref_ptr_safe(params), &__tmparray, aLength));
	__tmpret = __tmparray;
	return (__tmpret);
}
DEFINE_PRIM(_BYTES, NavMeshBuilder_dtCreateNavMeshData2, _IDL _REF(_I32));

HL_PRIM bool HL_NAME(NavMeshBuilder_dtNavMeshHeaderSwapEndian2)(vbyte* data, int dataSize) {
	return (dtNavMeshHeaderSwapEndian((unsigned char*)data, dataSize));
}
DEFINE_PRIM(_BOOL, NavMeshBuilder_dtNavMeshHeaderSwapEndian2, _BYTES _I32);

HL_PRIM bool HL_NAME(NavMeshBuilder_dtNavMeshDataSwapEndian2)(vbyte* data, int dataSize) {
	return (dtNavMeshDataSwapEndian((unsigned char*)data, dataSize));
}
DEFINE_PRIM(_BOOL, NavMeshBuilder_dtNavMeshDataSwapEndian2, _BYTES _I32);

HL_PRIM pref<dtQueryFilter>* HL_NAME(QueryFilter_new0)() {
	return alloc_ref((new dtQueryFilter()),QueryFilter);
}
DEFINE_PRIM(_IDL, QueryFilter_new0,);

HL_PRIM float HL_NAME(QueryFilter_getAreaCost1)(pref<dtQueryFilter>* _this, int i) {
	return (_unref(_this)->getAreaCost(i));
}
DEFINE_PRIM(_F32, QueryFilter_getAreaCost1, _IDL _I32);

HL_PRIM void HL_NAME(QueryFilter_setAreaCost2)(pref<dtQueryFilter>* _this, int i, float cost) {
	(_unref(_this)->setAreaCost(i, cost));
}
DEFINE_PRIM(_VOID, QueryFilter_setAreaCost2, _IDL _I32 _F32);

HL_PRIM unsigned short HL_NAME(QueryFilter_getIncludeFlags0)(pref<dtQueryFilter>* _this) {
	return (_unref(_this)->getIncludeFlags());
}
DEFINE_PRIM(_I16, QueryFilter_getIncludeFlags0, _IDL);

HL_PRIM void HL_NAME(QueryFilter_setIncludeFlags1)(pref<dtQueryFilter>* _this, unsigned short flags) {
	(_unref(_this)->setIncludeFlags(flags));
}
DEFINE_PRIM(_VOID, QueryFilter_setIncludeFlags1, _IDL _I16);

HL_PRIM unsigned short HL_NAME(QueryFilter_getExcludeFlags0)(pref<dtQueryFilter>* _this) {
	return (_unref(_this)->getExcludeFlags());
}
DEFINE_PRIM(_I16, QueryFilter_getExcludeFlags0, _IDL);

HL_PRIM void HL_NAME(QueryFilter_setExcludeFlags1)(pref<dtQueryFilter>* _this, unsigned short flags) {
	(_unref(_this)->setExcludeFlags(flags));
}
DEFINE_PRIM(_VOID, QueryFilter_setExcludeFlags1, _IDL _I16);

HL_PRIM float HL_NAME(RaycastHit_get_t)( pref<dtRaycastHit>* _this ) {
	return _unref(_this)->t;
}
DEFINE_PRIM(_F32,RaycastHit_get_t,_IDL);
HL_PRIM float HL_NAME(RaycastHit_set_t)( pref<dtRaycastHit>* _this, float value ) {
	_unref(_this)->t = (value);
	return value;
}
DEFINE_PRIM(_F32,RaycastHit_set_t,_IDL _F32);

HL_PRIM _hl_float3* HL_NAME(RaycastHit_get_hitNormal)( pref<dtRaycastHit>* _this ) {
	return (_hl_float3* )(_unref(_this)->hitNormal);
}
HL_PRIM void HL_NAME(RaycastHit_gethitNormalv)( pref<dtRaycastHit>* _this, _hl_float3* value ) {
	 float *src = (float*) & (_unref(_this)->hitNormal)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,RaycastHit_gethitNormalv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,RaycastHit_get_hitNormal,_IDL);
HL_PRIM _hl_float3* HL_NAME(RaycastHit_set_hitNormal)( pref<dtRaycastHit>* _this, _hl_float3* value ) {
	 float *dst = (float*) & (_unref(_this)->hitNormal)[0];
	 float *src = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
HL_PRIM void HL_NAME(RaycastHit_sethitNormal3)( pref<dtRaycastHit>* _this,  float value0, float value1, float value2 ) {
	 float *p = (_unref(_this)->hitNormal);
	p[0] = value0; p[1] = value1; p[2] = value2;
}
DEFINE_PRIM(_VOID,RaycastHit_sethitNormal3,_IDL _F32 _F32 _F32 );
DEFINE_PRIM(_STRUCT,RaycastHit_set_hitNormal,_IDL _STRUCT);

HL_PRIM int HL_NAME(RaycastHit_get_hitEdgeIndex)( pref<dtRaycastHit>* _this ) {
	return _unref(_this)->hitEdgeIndex;
}
DEFINE_PRIM(_I32,RaycastHit_get_hitEdgeIndex,_IDL);
HL_PRIM int HL_NAME(RaycastHit_set_hitEdgeIndex)( pref<dtRaycastHit>* _this, int value ) {
	_unref(_this)->hitEdgeIndex = (value);
	return value;
}
DEFINE_PRIM(_I32,RaycastHit_set_hitEdgeIndex,_IDL _I32);

HL_PRIM vbyte* HL_NAME(RaycastHit_get_path)( pref<dtRaycastHit>* _this ) {
	return (vbyte *)(&_unref(_this)->path[0]);
}
DEFINE_PRIM(_BYTES,RaycastHit_get_path,_IDL);
HL_PRIM vbyte* HL_NAME(RaycastHit_set_path)( pref<dtRaycastHit>* _this, vbyte* value ) {
	_unref(_this)->path = (unsigned int*)(value);
	return value;
}
DEFINE_PRIM(_BYTES,RaycastHit_set_path,_IDL _BYTES);

HL_PRIM int HL_NAME(RaycastHit_get_pathCount)( pref<dtRaycastHit>* _this ) {
	return _unref(_this)->pathCount;
}
DEFINE_PRIM(_I32,RaycastHit_get_pathCount,_IDL);
HL_PRIM int HL_NAME(RaycastHit_set_pathCount)( pref<dtRaycastHit>* _this, int value ) {
	_unref(_this)->pathCount = (value);
	return value;
}
DEFINE_PRIM(_I32,RaycastHit_set_pathCount,_IDL _I32);

HL_PRIM int HL_NAME(RaycastHit_get_maxPath)( pref<dtRaycastHit>* _this ) {
	return _unref(_this)->maxPath;
}
DEFINE_PRIM(_I32,RaycastHit_get_maxPath,_IDL);
HL_PRIM int HL_NAME(RaycastHit_set_maxPath)( pref<dtRaycastHit>* _this, int value ) {
	_unref(_this)->maxPath = (value);
	return value;
}
DEFINE_PRIM(_I32,RaycastHit_set_maxPath,_IDL _I32);

HL_PRIM float HL_NAME(RaycastHit_get_pathCost)( pref<dtRaycastHit>* _this ) {
	return _unref(_this)->pathCost;
}
DEFINE_PRIM(_F32,RaycastHit_get_pathCost,_IDL);
HL_PRIM float HL_NAME(RaycastHit_set_pathCost)( pref<dtRaycastHit>* _this, float value ) {
	_unref(_this)->pathCost = (value);
	return value;
}
DEFINE_PRIM(_F32,RaycastHit_set_pathCost,_IDL _F32);

HL_PRIM pref<dtNavMeshQuery>* HL_NAME(NavMeshQuery_new0)() {
	return alloc_ref((new dtNavMeshQuery()),NavMeshQuery);
}
DEFINE_PRIM(_IDL, NavMeshQuery_new0,);

HL_PRIM HL_CONST pref<dtNavMesh>* HL_NAME(NavMeshQuery_getAttachedNavMesh0)(pref<dtNavMeshQuery>* _this) {
	return alloc_ref_const((_unref(_this)->getAttachedNavMesh()),NavMesh);
}
DEFINE_PRIM(_IDL, NavMeshQuery_getAttachedNavMesh0, _IDL);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_init2)(pref<dtNavMeshQuery>* _this, pref<dtNavMesh>* nav, int maxNodes) {
	return (_unref(_this)->init(_unref_ptr_safe(nav), maxNodes));
}
DEFINE_PRIM(_I32, NavMeshQuery_init2, _IDL _IDL _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_findPath8)(pref<dtNavMeshQuery>* _this, unsigned int startRef, unsigned int endRef, _hl_float3* startPos, _hl_float3* endPos, pref<dtQueryFilter>* filter, varray* path, int* pathCount, int maxPath) {
	return (_unref(_this)->findPath(startRef, endRef, (float*)(_hl_float3*)startPos, (float*)(_hl_float3*)endPos, _unref_ptr_safe(filter), hl_aptr(path,unsigned int), pathCount, maxPath));
}
DEFINE_PRIM(_I32, NavMeshQuery_findPath8, _IDL _I32 _I32 _STRUCT _STRUCT _IDL _ARR _REF(_I32) _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_findNearestPoly5)(pref<dtNavMeshQuery>* _this, _hl_float3* center, _hl_float3* halfExtents, pref<dtQueryFilter>* filter, unsigned int* nearestRef, _hl_float3* nearestPt) {
	return (_unref(_this)->findNearestPoly((float*)(_hl_float3*)center, (float*)(_hl_float3*)halfExtents, _unref_ptr_safe(filter), nearestRef, (float*)(_hl_float3*)nearestPt));
}
DEFINE_PRIM(_I32, NavMeshQuery_findNearestPoly5, _IDL _STRUCT _STRUCT _IDL _REF(_I32) _STRUCT);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_findNearestPoly6)(pref<dtNavMeshQuery>* _this, _hl_float3* center, _hl_float3* halfExtents, pref<dtQueryFilter>* filter, unsigned int* nearestRef, _hl_float3* nearestPt, bool* isOverPoly) {
	return (_unref(_this)->findNearestPoly((float*)(_hl_float3*)center, (float*)(_hl_float3*)halfExtents, _unref_ptr_safe(filter), nearestRef, (float*)(_hl_float3*)nearestPt, isOverPoly));
}
DEFINE_PRIM(_I32, NavMeshQuery_findNearestPoly6, _IDL _STRUCT _STRUCT _IDL _REF(_I32) _STRUCT _REF(_BOOL));

HL_PRIM unsigned int HL_NAME(NavMeshQuery_initSlicedFindPath6)(pref<dtNavMeshQuery>* _this, unsigned int startRef, unsigned int endRef, _hl_float3* startPos, _hl_float3* endPos, pref<dtQueryFilter>* filter, unsigned int options) {
	return (_unref(_this)->initSlicedFindPath(startRef, endRef, (float*)(_hl_float3*)startPos, (float*)(_hl_float3*)endPos, _unref_ptr_safe(filter), options));
}
DEFINE_PRIM(_I32, NavMeshQuery_initSlicedFindPath6, _IDL _I32 _I32 _STRUCT _STRUCT _IDL _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_updateSlicedFindPath2)(pref<dtNavMeshQuery>* _this, int maxIter, int* doneIters) {
	return (_unref(_this)->updateSlicedFindPath(maxIter, doneIters));
}
DEFINE_PRIM(_I32, NavMeshQuery_updateSlicedFindPath2, _IDL _I32 _REF(_I32));

HL_PRIM unsigned int HL_NAME(NavMeshQuery_finalizeSlicedFindPath3)(pref<dtNavMeshQuery>* _this, varray* path, int* pathCount, int maxPath) {
	return (_unref(_this)->finalizeSlicedFindPath(hl_aptr(path,unsigned int), pathCount, maxPath));
}
DEFINE_PRIM(_I32, NavMeshQuery_finalizeSlicedFindPath3, _IDL _ARR _REF(_I32) _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_finalizeSlicedFindPathPartial5)(pref<dtNavMeshQuery>* _this, varray* existing, int existingSize, varray* path, int* pathCount, int maxPath) {
	return (_unref(_this)->finalizeSlicedFindPathPartial(hl_aptr(existing,unsigned int), existingSize, hl_aptr(path,unsigned int), pathCount, maxPath));
}
DEFINE_PRIM(_I32, NavMeshQuery_finalizeSlicedFindPathPartial5, _IDL _ARR _I32 _ARR _REF(_I32) _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_findStraightPath10)(pref<dtNavMeshQuery>* _this, _hl_float3* startPos, _hl_float3* endPos, varray* path, int pathSize, varray* straightPath, varray* straightPathFlags, varray* straightPathRefs, int* straightPathCount, int maxStraightPath, int options) {
	return (_unref(_this)->findStraightPath((float*)(_hl_float3*)startPos, (float*)(_hl_float3*)endPos, hl_aptr(path,unsigned int), pathSize, hl_aptr(straightPath,float), hl_aptr(straightPathFlags,unsigned char), hl_aptr(straightPathRefs,unsigned int), straightPathCount, maxStraightPath, options));
}
DEFINE_PRIM(_I32, NavMeshQuery_findStraightPath10, _IDL _STRUCT _STRUCT _ARR _I32 _ARR _ARR _ARR _REF(_I32) _I32 _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_raycast9)(pref<dtNavMeshQuery>* _this, unsigned int startRef, _hl_float3* startPos, _hl_float3* endPos, pref<dtQueryFilter>* filter, float* t, _hl_float3* hitNormal, varray* path, int* pathCount, int maxPath) {
	return (_unref(_this)->raycast(startRef, (float*)(_hl_float3*)startPos, (float*)(_hl_float3*)endPos, _unref_ptr_safe(filter), t, (float*)(_hl_float3*)hitNormal, hl_aptr(path,unsigned int), pathCount, maxPath));
}
DEFINE_PRIM(_I32, NavMeshQuery_raycast9, _IDL _I32 _STRUCT _STRUCT _IDL _REF(_F32) _STRUCT _ARR _REF(_I32) _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_raycast7)(pref<dtNavMeshQuery>* _this, unsigned int startRef, _hl_float3* startPos, _hl_float3* endPos, pref<dtQueryFilter>* filter, int options, pref<dtRaycastHit>* hit, unsigned int prevRef) {
	return (_unref(_this)->raycast(startRef, (float*)(_hl_float3*)startPos, (float*)(_hl_float3*)endPos, _unref_ptr_safe(filter), options, _unref_ptr_safe(hit), prevRef));
}
DEFINE_PRIM(_I32, NavMeshQuery_raycast7, _IDL _I32 _STRUCT _STRUCT _IDL _I32 _IDL _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_findPolysAroundCircle9)(pref<dtNavMeshQuery>* _this, unsigned int startRef, varray* centerPos, float radius, pref<dtQueryFilter>* filter, unsigned int* resultRef, unsigned int* resultParent, varray* resultCost, varray* resultCount, int maxResult) {
	return (_unref(_this)->findPolysAroundCircle(startRef, hl_aptr(centerPos,HL_CONST float), radius, _unref_ptr_safe(filter), resultRef, resultParent, hl_aptr(resultCost,float), hl_aptr(resultCount,int), maxResult));
}
DEFINE_PRIM(_I32, NavMeshQuery_findPolysAroundCircle9, _IDL _I32 _ARR _F32 _IDL _REF(_I32) _REF(_I32) _ARR _ARR _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_findPolysAroundShape9)(pref<dtNavMeshQuery>* _this, unsigned int startRef, varray* verts, int nverts, pref<dtQueryFilter>* filter, unsigned int* resultRef, unsigned int* resultParent, varray* resultCost, varray* resultCount, int maxResult) {
	return (_unref(_this)->findPolysAroundShape(startRef, hl_aptr(verts,HL_CONST float), nverts, _unref_ptr_safe(filter), resultRef, resultParent, hl_aptr(resultCost,float), hl_aptr(resultCount,int), maxResult));
}
DEFINE_PRIM(_I32, NavMeshQuery_findPolysAroundShape9, _IDL _I32 _ARR _I32 _IDL _REF(_I32) _REF(_I32) _ARR _ARR _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_getPathFromDijkstraSearch4)(pref<dtNavMeshQuery>* _this, unsigned int endRef, unsigned int* path, varray* pathCount, int maxPath) {
	return (_unref(_this)->getPathFromDijkstraSearch(endRef, path, hl_aptr(pathCount,int), maxPath));
}
DEFINE_PRIM(_I32, NavMeshQuery_getPathFromDijkstraSearch4, _IDL _I32 _REF(_I32) _ARR _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_queryPolygons6)(pref<dtNavMeshQuery>* _this, _hl_float3* center, _hl_float3* halfExtents, pref<dtQueryFilter>* filter, unsigned int* polys, varray* polyCount, int maxPolys) {
	return (_unref(_this)->queryPolygons((float*)(_hl_float3*)center, (float*)(_hl_float3*)halfExtents, _unref_ptr_safe(filter), polys, hl_aptr(polyCount,int), maxPolys));
}
DEFINE_PRIM(_I32, NavMeshQuery_queryPolygons6, _IDL _STRUCT _STRUCT _IDL _REF(_I32) _ARR _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_queryPolygons4)(pref<dtNavMeshQuery>* _this, _hl_float3* center, _hl_float3* halfExtents, pref<dtQueryFilter>* filter, pref<dtPolyQuery>* query) {
	return (_unref(_this)->queryPolygons((float*)(_hl_float3*)center, (float*)(_hl_float3*)halfExtents, _unref_ptr_safe(filter), _unref_ptr_safe(query)));
}
DEFINE_PRIM(_I32, NavMeshQuery_queryPolygons4, _IDL _STRUCT _STRUCT _IDL _IDL);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_findLocalNeighbourhood8)(pref<dtNavMeshQuery>* _this, unsigned int startRef, _hl_float3* centerPos, float radius, pref<dtQueryFilter>* filter, unsigned int* resultRef, unsigned int* resultParent, varray* resultCount, int maxResult) {
	return (_unref(_this)->findLocalNeighbourhood(startRef, (float*)(_hl_float3*)centerPos, radius, _unref_ptr_safe(filter), resultRef, resultParent, hl_aptr(resultCount,int), maxResult));
}
DEFINE_PRIM(_I32, NavMeshQuery_findLocalNeighbourhood8, _IDL _I32 _STRUCT _F32 _IDL _REF(_I32) _REF(_I32) _ARR _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_moveAlongSurface8)(pref<dtNavMeshQuery>* _this, unsigned int startRef, _hl_float3* startPos, _hl_float3* endPos, pref<dtQueryFilter>* filter, _hl_float3* resultPos, unsigned int* visited, varray* visitedCount, int maxVisitedSize) {
	return (_unref(_this)->moveAlongSurface(startRef, (float*)(_hl_float3*)startPos, (float*)(_hl_float3*)endPos, _unref_ptr_safe(filter), (float*)(_hl_float3*)resultPos, visited, hl_aptr(visitedCount,int), maxVisitedSize));
}
DEFINE_PRIM(_I32, NavMeshQuery_moveAlongSurface8, _IDL _I32 _STRUCT _STRUCT _IDL _STRUCT _REF(_I32) _ARR _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_findDistanceToWall7)(pref<dtNavMeshQuery>* _this, unsigned int startRef, varray* centerPos, float maxRadius, pref<dtQueryFilter>* filter, varray* hitDist, varray* hitPos, varray* hitNormal) {
	return (_unref(_this)->findDistanceToWall(startRef, hl_aptr(centerPos,HL_CONST float), maxRadius, _unref_ptr_safe(filter), hl_aptr(hitDist,float), hl_aptr(hitPos,float), hl_aptr(hitNormal,float)));
}
DEFINE_PRIM(_I32, NavMeshQuery_findDistanceToWall7, _IDL _I32 _ARR _F32 _IDL _ARR _ARR _ARR);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_getPolyWallSegments6)(pref<dtNavMeshQuery>* _this, unsigned int ref, pref<dtQueryFilter>* filter, varray* segmentVerts, unsigned int* segmentRefs, varray* segmentCount, int maxSegments) {
	return (_unref(_this)->getPolyWallSegments(ref, _unref_ptr_safe(filter), hl_aptr(segmentVerts,float), segmentRefs, hl_aptr(segmentCount,int), maxSegments));
}
DEFINE_PRIM(_I32, NavMeshQuery_getPolyWallSegments6, _IDL _I32 _IDL _ARR _REF(_I32) _ARR _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_closestPointOnPoly4)(pref<dtNavMeshQuery>* _this, unsigned int ref, varray* pos, varray* closest, varray* posOverPoly) {
	return (_unref(_this)->closestPointOnPoly(ref, hl_aptr(pos,HL_CONST float), hl_aptr(closest,float), hl_aptr(posOverPoly,bool)));
}
DEFINE_PRIM(_I32, NavMeshQuery_closestPointOnPoly4, _IDL _I32 _ARR _ARR _ARR);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_closestPointOnPolyBoundary3)(pref<dtNavMeshQuery>* _this, unsigned int ref, varray* pos, varray* closest) {
	return (_unref(_this)->closestPointOnPolyBoundary(ref, hl_aptr(pos,HL_CONST float), hl_aptr(closest,float)));
}
DEFINE_PRIM(_I32, NavMeshQuery_closestPointOnPolyBoundary3, _IDL _I32 _ARR _ARR);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_getPolyHeight3)(pref<dtNavMeshQuery>* _this, unsigned int ref, varray* pos, varray* height) {
	return (_unref(_this)->getPolyHeight(ref, hl_aptr(pos,HL_CONST float), hl_aptr(height,float)));
}
DEFINE_PRIM(_I32, NavMeshQuery_getPolyHeight3, _IDL _I32 _ARR _ARR);

HL_PRIM bool HL_NAME(NavMeshQuery_isValidPolyRef2)(pref<dtNavMeshQuery>* _this, unsigned int ref, pref<dtQueryFilter>* filter) {
	return (_unref(_this)->isValidPolyRef(ref, _unref_ptr_safe(filter)));
}
DEFINE_PRIM(_BOOL, NavMeshQuery_isValidPolyRef2, _IDL _I32 _IDL);

HL_PRIM bool HL_NAME(NavMeshQuery_isInClosedList1)(pref<dtNavMeshQuery>* _this, unsigned int ref) {
	return (_unref(_this)->isInClosedList(ref));
}
DEFINE_PRIM(_BOOL, NavMeshQuery_isInClosedList1, _IDL _I32);

HL_PRIM void HL_NAME(Detour_free1)(void* ptr) {
	(dtFree(ptr));
}
DEFINE_PRIM(_VOID, Detour_free1, _BYTES);

HL_PRIM pref<PerformanceTimer>* HL_NAME(PerformanceTimer_new0)() {
	return alloc_ref((new PerformanceTimer()),PerformanceTimer);
}
DEFINE_PRIM(_IDL, PerformanceTimer_new0,);

HL_PRIM void HL_NAME(PerformanceTimer_start0)(pref<PerformanceTimer>* _this) {
	(_unref(_this)->start());
}
DEFINE_PRIM(_VOID, PerformanceTimer_start0, _IDL);

HL_PRIM void HL_NAME(PerformanceTimer_stop0)(pref<PerformanceTimer>* _this) {
	(_unref(_this)->stop());
}
DEFINE_PRIM(_VOID, PerformanceTimer_stop0, _IDL);

HL_PRIM double HL_NAME(PerformanceTimer_deltaSeconds0)(pref<PerformanceTimer>* _this) {
	return (_unref(_this)->deltaSeconds());
}
DEFINE_PRIM(_F64, PerformanceTimer_deltaSeconds0, _IDL);

HL_PRIM double HL_NAME(PerformanceTimer_deltaMilliseconds0)(pref<PerformanceTimer>* _this) {
	return (_unref(_this)->deltaMilliseconds());
}
DEFINE_PRIM(_F64, PerformanceTimer_deltaMilliseconds0, _IDL);

HL_PRIM double HL_NAME(PerformanceTimer_deltaMicroseconds0)(pref<PerformanceTimer>* _this) {
	return (_unref(_this)->deltaMicroseconds());
}
DEFINE_PRIM(_F64, PerformanceTimer_deltaMicroseconds0, _IDL);

HL_PRIM void HL_NAME(Tests_setToZero4)(int* a, float* b, double* c, bool* d) {
	(setToZero(a, b, c, d));
}
DEFINE_PRIM(_VOID, Tests_setToZero4, _REF(_I32) _REF(_F32) _REF(_F64) _REF(_BOOL));

HL_PRIM void HL_NAME(Tests_setToOne4)(int* a, float* b, double* c, bool* d) {
	(setToOne(a, b, c, d));
}
DEFINE_PRIM(_VOID, Tests_setToOne4, _REF(_I32) _REF(_F32) _REF(_F64) _REF(_BOOL));

HL_PRIM pref<dtMeshCapture>* HL_NAME(MeshCapture_new1)(bool isSurface) {
	return alloc_ref((new dtMeshCapture(isSurface)),MeshCapture);
}
DEFINE_PRIM(_IDL, MeshCapture_new1, _BOOL);

HL_PRIM void HL_NAME(MeshCapture_captureNavMesh2)(pref<dtMeshCapture>* _this, pref<dtNavMesh>* nm, int flags) {
	(_unref(_this)->captureNavMesh(*_unref_ptr_safe(nm), flags));
}
DEFINE_PRIM(_VOID, MeshCapture_captureNavMesh2, _IDL _IDL _I32);

HL_PRIM void HL_NAME(MeshCapture_captureHeightField1)(pref<dtMeshCapture>* _this, pref<rcHeightfield>* hf) {
	(_unref(_this)->captureHeightField(*_unref_ptr_safe(hf)));
}
DEFINE_PRIM(_VOID, MeshCapture_captureHeightField1, _IDL _IDL);

HL_PRIM void HL_NAME(MeshCapture_captureCompactHeightField1)(pref<dtMeshCapture>* _this, pref<rcCompactHeightfield>* chf) {
	(_unref(_this)->captureCompactHeightField(*_unref_ptr_safe(chf)));
}
DEFINE_PRIM(_VOID, MeshCapture_captureCompactHeightField1, _IDL _IDL);

HL_PRIM void HL_NAME(MeshCapture_captureHeighfieldLayerSet1)(pref<dtMeshCapture>* _this, pref<rcHeightfieldLayerSet>* hfls) {
	(_unref(_this)->captureHeighfieldLayerSet(*_unref_ptr_safe(hfls)));
}
DEFINE_PRIM(_VOID, MeshCapture_captureHeighfieldLayerSet1, _IDL _IDL);

HL_PRIM int HL_NAME(MeshCapture_numVerts0)(pref<dtMeshCapture>* _this) {
	return (_unref(_this)->numVerts());
}
DEFINE_PRIM(_I32, MeshCapture_numVerts0, _IDL);

HL_PRIM int HL_NAME(MeshCapture_numSurfacePrims0)(pref<dtMeshCapture>* _this) {
	return (_unref(_this)->numSurfacePrims());
}
DEFINE_PRIM(_I32, MeshCapture_numSurfacePrims0, _IDL);

HL_PRIM bool HL_NAME(MeshCapture_isSurface0)(pref<dtMeshCapture>* _this) {
	return (_unref(_this)->isSurface());
}
DEFINE_PRIM(_BOOL, MeshCapture_isSurface0, _IDL);

HL_PRIM void HL_NAME(MeshCapture_getVert2)(pref<dtMeshCapture>* _this, int idx, _hl_float3* pos) {
	(_unref(_this)->getVert(idx, (float*)(_hl_float3*)pos));
}
DEFINE_PRIM(_VOID, MeshCapture_getVert2, _IDL _I32 _STRUCT);

HL_PRIM pref<NavTileConverter>* HL_NAME(NavTileConverter_new0)() {
	return alloc_ref((new NavTileConverter()),NavTileConverter);
}
DEFINE_PRIM(_IDL, NavTileConverter_new0,);

HL_PRIM void HL_NAME(NavTileConverter_setUnits2)(pref<NavTileConverter>* _this, double cellSize, double cellHeight) {
	(_unref(_this)->setUnits(cellSize, cellHeight));
}
DEFINE_PRIM(_VOID, NavTileConverter_setUnits2, _IDL _F64 _F64);

HL_PRIM void HL_NAME(NavTileConverter_buildF6)(pref<NavTileConverter>* _this, vbyte* verts, int totalVerts, int polyCount, vbyte* polyVertCount, vbyte* polyType, vbyte* polyFlags) {
	(_unref(_this)->buildF((float*)verts, totalVerts, polyCount, (int*)polyVertCount, (int*)polyType, (int*)polyFlags));
}
DEFINE_PRIM(_VOID, NavTileConverter_buildF6, _IDL _BYTES _I32 _I32 _BYTES _BYTES _BYTES);

HL_PRIM void HL_NAME(NavTileConverter_setLocation4)(pref<NavTileConverter>* _this, int x, int y, _hl_float3* min, _hl_float3* max) {
	(_unref(_this)->setLocation(x, y, (_hl_float3*)min, (_hl_float3*)max));
}
DEFINE_PRIM(_VOID, NavTileConverter_setLocation4, _IDL _I32 _I32 _STRUCT _STRUCT);

HL_PRIM void* HL_NAME(NavTileConverter_getTileData0)(pref<NavTileConverter>* _this) {
	return (_unref(_this)->getTileData());
}
DEFINE_PRIM(_BYTES, NavTileConverter_getTileData0, _IDL);

}
