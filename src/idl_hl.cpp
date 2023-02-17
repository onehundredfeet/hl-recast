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

class HNativeBuffer {
    unsigned char *_ptr;
    int _size;

   public:
   inline unsigned char * ptr() { return _ptr; }
   inline int size() { return _size; }
   HNativeBuffer(unsigned char *ptr, int size) : _ptr(ptr), _size(size) {}
   HNativeBuffer(int size) : _ptr(new unsigned char[size]), _size(size) {}
    ~HNativeBuffer() {
        if (_ptr != nullptr)
            delete [] _ptr;
    }
};
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

inline void testvector(_h_float3 *v) {
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
	return (int)LogCategory__values[idx];
}
DEFINE_PRIM(_I32, LogCategory_toValue0, _I32);
HL_PRIM int HL_NAME(LogCategory_indexToValue1)( int idx ) {
	return (int)LogCategory__values[idx];
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
	return (int)TimerLabel__values[idx];
}
DEFINE_PRIM(_I32, TimerLabel_toValue0, _I32);
HL_PRIM int HL_NAME(TimerLabel_indexToValue1)( int idx ) {
	return (int)TimerLabel__values[idx];
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
static dtStatus DtStatus__values[] = { DT_FAILURE,DT_SUCCESS,DT_IN_PROGRESS };
HL_PRIM int HL_NAME(DtStatus_toValue0)( int idx ) {
	return (int)DtStatus__values[idx];
}
DEFINE_PRIM(_I32, DtStatus_toValue0, _I32);
HL_PRIM int HL_NAME(DtStatus_indexToValue1)( int idx ) {
	return (int)DtStatus__values[idx];
}
DEFINE_PRIM(_I32, DtStatus_indexToValue1, _I32);
HL_PRIM int HL_NAME(DtStatus_valueToIndex1)( int value ) {
	for( int i = 0; i < 3; i++ ) if ( value == (int)DtStatus__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, DtStatus_valueToIndex1, _I32);
HL_PRIM int HL_NAME(DtStatus_fromValue1)( int value ) {
	for( int i = 0; i < 3; i++ ) if ( value == (int)DtStatus__values[i]) return i; return -1;
}
DEFINE_PRIM(_I32, DtStatus_fromValue1, _I32);
HL_PRIM int HL_NAME(DtStatus_fromIndex1)( int index ) {return index;}
DEFINE_PRIM(_I32, DtStatus_fromIndex1, _I32);
static void finalize_NavBuffer( pref<NavBuffer>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(NavBuffer_delete)( pref<NavBuffer>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, NavBuffer_delete, _IDL);
static void finalize_TriMeshPartition( pref<TriMeshPartition>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TriMeshPartition_delete)( pref<TriMeshPartition>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TriMeshPartition_delete, _IDL);
static void finalize_Heightfield( pref<rcHeightfield>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(Heightfield_delete)( pref<rcHeightfield>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, Heightfield_delete, _IDL);
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
	return (int)AllocHint__values[idx];
}
DEFINE_PRIM(_I32, AllocHint_toValue0, _I32);
HL_PRIM int HL_NAME(AllocHint_indexToValue1)( int idx ) {
	return (int)AllocHint__values[idx];
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
	return (int)CompressedTileFlags__values[idx];
}
DEFINE_PRIM(_I32, CompressedTileFlags_toValue0, _I32);
HL_PRIM int HL_NAME(CompressedTileFlags_indexToValue1)( int idx ) {
	return (int)CompressedTileFlags__values[idx];
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
	return (int)StraightPathFlags__values[idx];
}
DEFINE_PRIM(_I32, StraightPathFlags_toValue0, _I32);
HL_PRIM int HL_NAME(StraightPathFlags_indexToValue1)( int idx ) {
	return (int)StraightPathFlags__values[idx];
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
	return (int)StraightPathOptions__values[idx];
}
DEFINE_PRIM(_I32, StraightPathOptions_toValue0, _I32);
HL_PRIM int HL_NAME(StraightPathOptions_indexToValue1)( int idx ) {
	return (int)StraightPathOptions__values[idx];
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
static void finalize_TriMeshBuilder( pref<TriMeshBuilder>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TriMeshBuilder_delete)( pref<TriMeshBuilder>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TriMeshBuilder_delete, _IDL);
static void finalize_SourceTriChunk( pref<NavWorld::SourceTriChunk>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(SourceTriChunk_delete)( pref<NavWorld::SourceTriChunk>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, SourceTriChunk_delete, _IDL);
static void finalize_TileBuilder( pref<NavWorld::TileBuilder>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(TileBuilder_delete)( pref<NavWorld::TileBuilder>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, TileBuilder_delete, _IDL);
static void finalize_QueryWorker( pref<NavWorld::QueryWorker>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(QueryWorker_delete)( pref<NavWorld::QueryWorker>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, QueryWorker_delete, _IDL);
static void finalize_AgentParameters( pref<AgentParameters>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(AgentParameters_delete)( pref<AgentParameters>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, AgentParameters_delete, _IDL);
static void finalize_NavWorld( pref<NavWorld>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(NavWorld_delete)( pref<NavWorld>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, NavWorld_delete, _IDL);
static void finalize_NavMesh( pref<NavMesh>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(NavMesh_delete)( pref<NavMesh>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, NavMesh_delete, _IDL);
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
static void finalize_NavTileConverter( pref<NavTileConverter>* _this ) { free_ref(_this ); }
HL_PRIM void HL_NAME(NavTileConverter_delete)( pref<NavTileConverter>* _this ) {
	free_ref(_this );
}
DEFINE_PRIM(_VOID, NavTileConverter_delete, _IDL);
HL_PRIM pref<TriMeshPartition>* HL_NAME(TriMeshPartition_new0)() {
	return alloc_ref((new TriMeshPartition()),TriMeshPartition);
}
DEFINE_PRIM(_IDL, TriMeshPartition_new0,);

HL_PRIM int HL_NAME(TriMeshPartition_get_maxTrisPerChunk)( pref<TriMeshPartition>* _this ) {
	return _unref(_this)->maxTrisPerChunk;
}
DEFINE_PRIM(_I32,TriMeshPartition_get_maxTrisPerChunk,_IDL);
HL_PRIM int HL_NAME(TriMeshPartition_set_maxTrisPerChunk)( pref<TriMeshPartition>* _this, int value ) {
	_unref(_this)->maxTrisPerChunk = (value);
	return value;
}
DEFINE_PRIM(_I32,TriMeshPartition_set_maxTrisPerChunk,_IDL _I32);

HL_PRIM int HL_NAME(TriMeshPartition_getNodeTriIndex1)(pref<TriMeshPartition>* _this, int nodeIdx) {
	return (getNodeTriIndex( _unref(_this) , nodeIdx));
}
DEFINE_PRIM(_I32, TriMeshPartition_getNodeTriIndex1, _IDL _I32);

HL_PRIM int HL_NAME(TriMeshPartition_getNodeTriCount1)(pref<TriMeshPartition>* _this, int nodeIdx) {
	return (getNodeTriCount( _unref(_this) , nodeIdx));
}
DEFINE_PRIM(_I32, TriMeshPartition_getNodeTriCount1, _IDL _I32);

HL_PRIM int HL_NAME(TriMeshPartition_getChunksOverlappingRect4)(pref<TriMeshPartition>* _this, h_float2 bmin, h_float2 bmax, varray* ids, int maxIds) {
	return (_unref(_this)->getChunksOverlappingRect((float*)(h_float2)bmin, (float*)(h_float2)bmax, hl_aptr(ids,int), maxIds));
}
DEFINE_PRIM(_I32, TriMeshPartition_getChunksOverlappingRect4, _IDL _STRUCT _STRUCT _ARR _I32);

HL_PRIM pref<rcHeightfield>* HL_NAME(Heightfield_new0)() {
	return alloc_ref((new rcHeightfield()),Heightfield);
}
DEFINE_PRIM(_IDL, Heightfield_new0,);

HL_PRIM pref<rcCompactHeightfield>* HL_NAME(CompactHeightfield_new0)() {
	return alloc_ref((new rcCompactHeightfield()),CompactHeightfield);
}
DEFINE_PRIM(_IDL, CompactHeightfield_new0,);

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

HL_PRIM pref<rcContourSet>* HL_NAME(rcContourSet_new0)() {
	return alloc_ref((new rcContourSet()),rcContourSet);
}
DEFINE_PRIM(_IDL, rcContourSet_new0,);

HL_PRIM pref<rcPolyMesh>* HL_NAME(PolyMesh_new0)() {
	return alloc_ref((new rcPolyMesh()),PolyMesh);
}
DEFINE_PRIM(_IDL, PolyMesh_new0,);

HL_PRIM pref<rcPolyMeshDetail>* HL_NAME(PolyMeshDetail_new0)() {
	return alloc_ref((new rcPolyMeshDetail()),PolyMeshDetail);
}
DEFINE_PRIM(_IDL, PolyMeshDetail_new0,);

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

HL_PRIM void HL_NAME(Recast_calcBounds4)(varray* verts, int nv, h_float3 bmin, h_float3 bmax) {
	(rcCalcBounds(hl_aptr(verts,float), nv, (float*)(h_float3)bmin, (float*)(h_float3)bmax));
}
DEFINE_PRIM(_VOID, Recast_calcBounds4, _ARR _I32 _STRUCT _STRUCT);

HL_PRIM void HL_NAME(Recast_calcGridSize5)(h_float3 bmin, h_float3 bmax, float cs, int* w, int* h) {
	(rcCalcGridSize((float*)(h_float3)bmin, (float*)(h_float3)bmax, cs, w, h));
}
DEFINE_PRIM(_VOID, Recast_calcGridSize5, _STRUCT _STRUCT _F32 _REF(_I32) _REF(_I32));

HL_PRIM int HL_NAME(Recast_offsetPoly5)(h_float3 verts, int nverts, float offset, h_float3 outVerts, int maxOutVerts) {
	return (rcOffsetPoly((float*)(h_float3)verts, nverts, offset, (float*)(h_float3)outVerts, maxOutVerts));
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

HL_PRIM unsigned int HL_NAME(TileCache_buildNavMeshTilesAt3)(pref<dtTileCache>* _this, int tx, int ty, pref<NavMesh>* navmesh) {
	return (_unref(_this)->buildNavMeshTilesAt(tx, ty, _unref_ptr_safe(navmesh)));
}
DEFINE_PRIM(_I32, TileCache_buildNavMeshTilesAt3, _IDL _I32 _I32 _IDL);

HL_PRIM pref<dtTileCacheParams>* HL_NAME(TileCacheParams_new0)() {
	return alloc_ref((new dtTileCacheParams()),TileCacheParams);
}
DEFINE_PRIM(_IDL, TileCacheParams_new0,);

HL_PRIM h_float3 HL_NAME(TileCacheParams_get_orig)( pref<dtTileCacheParams>* _this ) {
	return (h_float3 )(_unref(_this)->orig);
}
HL_PRIM void HL_NAME(TileCacheParams_getorigv)( pref<dtTileCacheParams>* _this, h_float3 value ) {
	 float *src = (float*) & (_unref(_this)->orig)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,TileCacheParams_getorigv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,TileCacheParams_get_orig,_IDL);
HL_PRIM h_float3 HL_NAME(TileCacheParams_set_orig)( pref<dtTileCacheParams>* _this, h_float3 value ) {
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

HL_PRIM bool HL_NAME(TriMeshBuilder_loadObj2)(pref<TriMeshBuilder>* _this, vstring * filename, h_float3 offset) {
	const char* filename__cstr = (filename == nullptr) ? "" : hl_to_utf8( filename->bytes ); // Should be garbage collected
	auto ___retvalue = (_unref(_this)->loadObj(filename__cstr, (h_float3)offset));
	return (___retvalue);
}
DEFINE_PRIM(_BOOL, TriMeshBuilder_loadObj2, _IDL _STRING _STRUCT);

HL_PRIM void HL_NAME(TriMeshBuilder_reserve2)(pref<TriMeshBuilder>* _this, int verts, int tris) {
	(_unref(_this)->reserve(verts, tris));
}
DEFINE_PRIM(_VOID, TriMeshBuilder_reserve2, _IDL _I32 _I32);

HL_PRIM void HL_NAME(TriMeshBuilder_setVerts2)(pref<TriMeshBuilder>* _this, vbyte* verts, int count) {
	(_unref(_this)->setVerts((float*)verts, count));
}
DEFINE_PRIM(_VOID, TriMeshBuilder_setVerts2, _IDL _BYTES _I32);

HL_PRIM void HL_NAME(TriMeshBuilder_setTris2)(pref<TriMeshBuilder>* _this, vbyte* tris, int count) {
	(_unref(_this)->setTris((int*)tris, count));
}
DEFINE_PRIM(_VOID, TriMeshBuilder_setTris2, _IDL _BYTES _I32);

HL_PRIM int HL_NAME(TriMeshBuilder_getVertCount0)(pref<TriMeshBuilder>* _this) {
	return (_unref(_this)->getVertCount());
}
DEFINE_PRIM(_I32, TriMeshBuilder_getVertCount0, _IDL);

HL_PRIM int HL_NAME(TriMeshBuilder_getTriCount0)(pref<TriMeshBuilder>* _this) {
	return (_unref(_this)->getTriCount());
}
DEFINE_PRIM(_I32, TriMeshBuilder_getTriCount0, _IDL);

HL_PRIM int HL_NAME(TriMeshBuilder_addVertex3)(pref<TriMeshBuilder>* _this, float x, float y, float z) {
	return (_unref(_this)->addVertex(x, y, z));
}
DEFINE_PRIM(_I32, TriMeshBuilder_addVertex3, _IDL _F32 _F32 _F32);

HL_PRIM void HL_NAME(TriMeshBuilder_setNormal4)(pref<TriMeshBuilder>* _this, int vidx, float x, float y, float z) {
	(_unref(_this)->setNormal(vidx, x, y, z));
}
DEFINE_PRIM(_VOID, TriMeshBuilder_setNormal4, _IDL _I32 _F32 _F32 _F32);

HL_PRIM void HL_NAME(TriMeshBuilder_addTriangle3)(pref<TriMeshBuilder>* _this, int a, int b, int c) {
	(_unref(_this)->addTriangle(a, b, c));
}
DEFINE_PRIM(_VOID, TriMeshBuilder_addTriangle3, _IDL _I32 _I32 _I32);

HL_PRIM void HL_NAME(TriMeshBuilder_setFlags2)(pref<TriMeshBuilder>* _this, int tidx, int flags) {
	(_unref(_this)->setFlags(tidx, flags));
}
DEFINE_PRIM(_VOID, TriMeshBuilder_setFlags2, _IDL _I32 _I32);

HL_PRIM pref<NavWorld::SourceTriChunk>* HL_NAME(SourceTriChunk_new1)(int maxTrisPerPartitionChunk) {
	return alloc_ref((new NavWorld::SourceTriChunk(maxTrisPerPartitionChunk)),SourceTriChunk);
}
DEFINE_PRIM(_IDL, SourceTriChunk_new1, _I32);

HL_PRIM pref<TriMeshBuilder>* HL_NAME(SourceTriChunk_mesh0)(pref<NavWorld::SourceTriChunk>* _this) {
	return alloc_ref((_unref(_this)->meshPtr()),TriMeshBuilder);
}
DEFINE_PRIM(_IDL, SourceTriChunk_mesh0, _IDL);

HL_PRIM bool HL_NAME(SourceTriChunk_finalize0)(pref<NavWorld::SourceTriChunk>* _this) {
	return (_unref(_this)->finalize());
}
DEFINE_PRIM(_BOOL, SourceTriChunk_finalize0, _IDL);

HL_PRIM bool HL_NAME(SourceTriChunk_isEnabled0)(pref<NavWorld::SourceTriChunk>* _this) {
	return (_unref(_this)->isEnabled());
}
DEFINE_PRIM(_BOOL, SourceTriChunk_isEnabled0, _IDL);

HL_PRIM void HL_NAME(SourceTriChunk_setEnabled1)(pref<NavWorld::SourceTriChunk>* _this, bool enabled) {
	(_unref(_this)->setEnabled(enabled));
}
DEFINE_PRIM(_VOID, SourceTriChunk_setEnabled1, _IDL _BOOL);

HL_PRIM int HL_NAME(TileBuilder_x0)(pref<NavWorld::TileBuilder>* _this) {
	return (_unref(_this)->x());
}
DEFINE_PRIM(_I32, TileBuilder_x0, _IDL);

HL_PRIM int HL_NAME(TileBuilder_y0)(pref<NavWorld::TileBuilder>* _this) {
	return (_unref(_this)->y());
}
DEFINE_PRIM(_I32, TileBuilder_y0, _IDL);

HL_PRIM bool HL_NAME(TileBuilder_buildTileColumnCacheData0)(pref<NavWorld::TileBuilder>* _this) {
	return (_unref(_this)->buildTileColumnCacheData());
}
DEFINE_PRIM(_BOOL, TileBuilder_buildTileColumnCacheData0, _IDL);

HL_PRIM void HL_NAME(TileBuilder_retire0)(pref<NavWorld::TileBuilder>* _this) {
	(_unref(_this)->retire());
}
DEFINE_PRIM(_VOID, TileBuilder_retire0, _IDL);

HL_PRIM void HL_NAME(TileBuilder_queueForSwap0)(pref<NavWorld::TileBuilder>* _this) {
	(_unref(_this)->queueForSwap());
}
DEFINE_PRIM(_VOID, TileBuilder_queueForSwap0, _IDL);

HL_PRIM bool HL_NAME(TileBuilder_isEmpty0)(pref<NavWorld::TileBuilder>* _this) {
	return (_unref(_this)->isEmpty());
}
DEFINE_PRIM(_BOOL, TileBuilder_isEmpty0, _IDL);

HL_PRIM int HL_NAME(TileBuilder_numSourceChunks0)(pref<NavWorld::TileBuilder>* _this) {
	return (_unref(_this)->numSourceChunks());
}
DEFINE_PRIM(_I32, TileBuilder_numSourceChunks0, _IDL);

HL_PRIM bool HL_NAME(TileBuilder_insertIntoCache0)(pref<NavWorld::TileBuilder>* _this) {
	return (_unref(_this)->insertIntoCache());
}
DEFINE_PRIM(_BOOL, TileBuilder_insertIntoCache0, _IDL);

HL_PRIM bool HL_NAME(TileBuilder_inflate0)(pref<NavWorld::TileBuilder>* _this) {
	return (_unref(_this)->inflate());
}
DEFINE_PRIM(_BOOL, TileBuilder_inflate0, _IDL);

HL_PRIM float HL_NAME(QueryWorker_getAreaCost1)(pref<NavWorld::QueryWorker>* _this, int i) {
	return (_unref(_this)->getAreaCost(i));
}
DEFINE_PRIM(_F32, QueryWorker_getAreaCost1, _IDL _I32);

HL_PRIM void HL_NAME(QueryWorker_setAreaCost2)(pref<NavWorld::QueryWorker>* _this, int i, float cost) {
	(_unref(_this)->setAreaCost(i, cost));
}
DEFINE_PRIM(_VOID, QueryWorker_setAreaCost2, _IDL _I32 _F32);

HL_PRIM unsigned short HL_NAME(QueryWorker_getIncludeFlags0)(pref<NavWorld::QueryWorker>* _this) {
	return (_unref(_this)->getIncludeFlags());
}
DEFINE_PRIM(_I16, QueryWorker_getIncludeFlags0, _IDL);

HL_PRIM void HL_NAME(QueryWorker_setIncludeFlags1)(pref<NavWorld::QueryWorker>* _this, unsigned short flags) {
	(_unref(_this)->setIncludeFlags(flags));
}
DEFINE_PRIM(_VOID, QueryWorker_setIncludeFlags1, _IDL _I16);

HL_PRIM unsigned short HL_NAME(QueryWorker_getExcludeFlags0)(pref<NavWorld::QueryWorker>* _this) {
	return (_unref(_this)->getExcludeFlags());
}
DEFINE_PRIM(_I16, QueryWorker_getExcludeFlags0, _IDL);

HL_PRIM void HL_NAME(QueryWorker_setExcludeFlags1)(pref<NavWorld::QueryWorker>* _this, unsigned short flags) {
	(_unref(_this)->setExcludeFlags(flags));
}
DEFINE_PRIM(_VOID, QueryWorker_setExcludeFlags1, _IDL _I16);

HL_PRIM void HL_NAME(QueryWorker_setQueryArea2)(pref<NavWorld::QueryWorker>* _this, h_float3 center, h_float3 halfExtents) {
	(_unref(_this)->setQueryArea((h_float3)center, (h_float3)halfExtents));
}
DEFINE_PRIM(_VOID, QueryWorker_setQueryArea2, _IDL _STRUCT _STRUCT);

HL_PRIM int HL_NAME(QueryWorker_findNearestPoly0)(pref<NavWorld::QueryWorker>* _this) {
	return HL_NAME(DtStatus_valueToIndex1)(_unref(_this)->findNearestPoly());
}
DEFINE_PRIM(_I32, QueryWorker_findNearestPoly0, _IDL);

HL_PRIM unsigned int HL_NAME(QueryWorker_nearestPoly0)(pref<NavWorld::QueryWorker>* _this) {
	return (_unref(_this)->nearestPoly());
}
DEFINE_PRIM(_I32, QueryWorker_nearestPoly0, _IDL);

HL_PRIM void HL_NAME(QueryWorker_getNearestPoint1)(pref<NavWorld::QueryWorker>* _this, h_float3 point) {
	(_unref(_this)->getNearestPoint((h_float3)point));
}
DEFINE_PRIM(_VOID, QueryWorker_getNearestPoint1, _IDL _STRUCT);

HL_PRIM bool HL_NAME(QueryWorker_centerOverNearestPoly0)(pref<NavWorld::QueryWorker>* _this) {
	return (_unref(_this)->centerOverNearestPoly());
}
DEFINE_PRIM(_BOOL, QueryWorker_centerOverNearestPoly0, _IDL);

HL_PRIM void HL_NAME(QueryWorker_setCurrentAsStart0)(pref<NavWorld::QueryWorker>* _this) {
	(_unref(_this)->setCurrentAsStart());
}
DEFINE_PRIM(_VOID, QueryWorker_setCurrentAsStart0, _IDL);

HL_PRIM void HL_NAME(QueryWorker_setCurrentAsEnd0)(pref<NavWorld::QueryWorker>* _this) {
	(_unref(_this)->setCurrentAsEnd());
}
DEFINE_PRIM(_VOID, QueryWorker_setCurrentAsEnd0, _IDL);

HL_PRIM void HL_NAME(QueryWorker_setStartPoint2)(pref<NavWorld::QueryWorker>* _this, unsigned int ref, h_float3 point) {
	(_unref(_this)->setStartPoint(ref, (h_float3)point));
}
DEFINE_PRIM(_VOID, QueryWorker_setStartPoint2, _IDL _I32 _STRUCT);

HL_PRIM void HL_NAME(QueryWorker_setEndPoint2)(pref<NavWorld::QueryWorker>* _this, unsigned int ref, h_float3 point) {
	(_unref(_this)->setEndPoint(ref, (h_float3)point));
}
DEFINE_PRIM(_VOID, QueryWorker_setEndPoint2, _IDL _I32 _STRUCT);

HL_PRIM void HL_NAME(QueryWorker_setMaximumPathLength1)(pref<NavWorld::QueryWorker>* _this, int maxNodes) {
	(_unref(_this)->setMaximumPathLength(maxNodes));
}
DEFINE_PRIM(_VOID, QueryWorker_setMaximumPathLength1, _IDL _I32);

HL_PRIM int HL_NAME(QueryWorker_findPath0)(pref<NavWorld::QueryWorker>* _this) {
	return HL_NAME(DtStatus_valueToIndex1)(_unref(_this)->findPath());
}
DEFINE_PRIM(_I32, QueryWorker_findPath0, _IDL);

HL_PRIM int HL_NAME(QueryWorker_pathLength0)(pref<NavWorld::QueryWorker>* _this) {
	return (_unref(_this)->pathLength());
}
DEFINE_PRIM(_I32, QueryWorker_pathLength0, _IDL);

HL_PRIM void HL_NAME(QueryWorker_getPathNodes1)(pref<NavWorld::QueryWorker>* _this, vbyte* nodes) {
	(_unref(_this)->getPathNodes((unsigned int*)nodes));
}
DEFINE_PRIM(_VOID, QueryWorker_getPathNodes1, _IDL _BYTES);

HL_PRIM int HL_NAME(QueryWorker_straightenPath0)(pref<NavWorld::QueryWorker>* _this) {
	return HL_NAME(DtStatus_valueToIndex1)(_unref(_this)->straightenPath());
}
DEFINE_PRIM(_I32, QueryWorker_straightenPath0, _IDL);

HL_PRIM int HL_NAME(QueryWorker_straightPathLength0)(pref<NavWorld::QueryWorker>* _this) {
	return (_unref(_this)->straightPathLength());
}
DEFINE_PRIM(_I32, QueryWorker_straightPathLength0, _IDL);

HL_PRIM void HL_NAME(QueryWorker_getStraightPathPositions1)(pref<NavWorld::QueryWorker>* _this, vbyte* nodes) {
	(_unref(_this)->getStraightPathPositions((h_float3_array)nodes));
}
DEFINE_PRIM(_VOID, QueryWorker_getStraightPathPositions1, _IDL _BYTES);

HL_PRIM void HL_NAME(QueryWorker_getStraightPathFlags1)(pref<NavWorld::QueryWorker>* _this, vbyte* flags) {
	(_unref(_this)->getStraightPathFlags((unsigned char*)flags));
}
DEFINE_PRIM(_VOID, QueryWorker_getStraightPathFlags1, _IDL _BYTES);

HL_PRIM void HL_NAME(QueryWorker_getStraightPathRefs1)(pref<NavWorld::QueryWorker>* _this, vbyte* refs) {
	(_unref(_this)->getStraightPathRefs((unsigned int*)refs));
}
DEFINE_PRIM(_VOID, QueryWorker_getStraightPathRefs1, _IDL _BYTES);

HL_PRIM void HL_NAME(QueryWorker_getStraightPathPosition2)(pref<NavWorld::QueryWorker>* _this, int i, h_float3 pos) {
	(_unref(_this)->getStraightPathPosition(i, (h_float3)pos));
}
DEFINE_PRIM(_VOID, QueryWorker_getStraightPathPosition2, _IDL _I32 _STRUCT);

HL_PRIM int HL_NAME(QueryWorker_getStraightPathNodeFlags1)(pref<NavWorld::QueryWorker>* _this, int i) {
	return (_unref(_this)->getStraightPathNodeFlags(i));
}
DEFINE_PRIM(_I32, QueryWorker_getStraightPathNodeFlags1, _IDL _I32);

HL_PRIM unsigned int HL_NAME(QueryWorker_getStraightPathNodePoly1)(pref<NavWorld::QueryWorker>* _this, int i) {
	return (_unref(_this)->getStraightPathNodePoly(i));
}
DEFINE_PRIM(_I32, QueryWorker_getStraightPathNodePoly1, _IDL _I32);

HL_PRIM void HL_NAME(QueryWorker_retire0)(pref<NavWorld::QueryWorker>* _this) {
	(_unref(_this)->retire());
}
DEFINE_PRIM(_VOID, QueryWorker_retire0, _IDL);

HL_PRIM pref<AgentParameters>* HL_NAME(AgentParameters_new0)() {
	return alloc_ref((new AgentParameters()),AgentParameters);
}
DEFINE_PRIM(_IDL, AgentParameters_new0,);

HL_PRIM float HL_NAME(AgentParameters_get_walkableSlopeAngle)( pref<AgentParameters>* _this ) {
	return _unref(_this)->walkableSlopeAngle;
}
DEFINE_PRIM(_F32,AgentParameters_get_walkableSlopeAngle,_IDL);
HL_PRIM float HL_NAME(AgentParameters_set_walkableSlopeAngle)( pref<AgentParameters>* _this, float value ) {
	_unref(_this)->walkableSlopeAngle = (value);
	return value;
}
DEFINE_PRIM(_F32,AgentParameters_set_walkableSlopeAngle,_IDL _F32);

HL_PRIM float HL_NAME(AgentParameters_get_walkableClimb)( pref<AgentParameters>* _this ) {
	return _unref(_this)->walkableClimb;
}
DEFINE_PRIM(_F32,AgentParameters_get_walkableClimb,_IDL);
HL_PRIM float HL_NAME(AgentParameters_set_walkableClimb)( pref<AgentParameters>* _this, float value ) {
	_unref(_this)->walkableClimb = (value);
	return value;
}
DEFINE_PRIM(_F32,AgentParameters_set_walkableClimb,_IDL _F32);

HL_PRIM float HL_NAME(AgentParameters_get_walkableRadius)( pref<AgentParameters>* _this ) {
	return _unref(_this)->walkableRadius;
}
DEFINE_PRIM(_F32,AgentParameters_get_walkableRadius,_IDL);
HL_PRIM float HL_NAME(AgentParameters_set_walkableRadius)( pref<AgentParameters>* _this, float value ) {
	_unref(_this)->walkableRadius = (value);
	return value;
}
DEFINE_PRIM(_F32,AgentParameters_set_walkableRadius,_IDL _F32);

HL_PRIM float HL_NAME(AgentParameters_get_walkableHeight)( pref<AgentParameters>* _this ) {
	return _unref(_this)->walkableHeight;
}
DEFINE_PRIM(_F32,AgentParameters_get_walkableHeight,_IDL);
HL_PRIM float HL_NAME(AgentParameters_set_walkableHeight)( pref<AgentParameters>* _this, float value ) {
	_unref(_this)->walkableHeight = (value);
	return value;
}
DEFINE_PRIM(_F32,AgentParameters_set_walkableHeight,_IDL _F32);

HL_PRIM pref<NavWorld>* HL_NAME(NavWorld_create9)(h_float3 origin, h_float3 dim, int tileSizeInCells, float cellSize, float cellHeight, int maxTiles, int maxPolys, int maxObstacles, pref<AgentParameters>* agentParams) {
	return alloc_ref((NavWorld::create((h_float3)origin, (h_float3)dim, tileSizeInCells, cellSize, cellHeight, maxTiles, maxPolys, maxObstacles, _unref_ptr_safe(agentParams))),NavWorld);
}
DEFINE_PRIM(_IDL, NavWorld_create9, _STRUCT _STRUCT _I32 _F32 _F32 _I32 _I32 _I32 _IDL);

HL_PRIM HL_CONST pref<NavWorld::SourceTriChunk>* HL_NAME(NavWorld_addChunk0)(pref<NavWorld>* _this) {
	return alloc_ref_const((_unref(_this)->addChunk()),SourceTriChunk);
}
DEFINE_PRIM(_IDL, NavWorld_addChunk0, _IDL);

HL_PRIM HL_CONST pref<NavWorld::TileBuilder>* HL_NAME(NavWorld_getTileBuilder2)(pref<NavWorld>* _this, int x, int y) {
	return alloc_ref_const((_unref(_this)->getTileBuilder(x, y)),TileBuilder);
}
DEFINE_PRIM(_IDL, NavWorld_getTileBuilder2, _IDL _I32 _I32);

HL_PRIM HL_CONST pref<NavWorld::QueryWorker>* HL_NAME(NavWorld_getQueryWorker0)(pref<NavWorld>* _this) {
	return alloc_ref_const((_unref(_this)->getQueryWorker()),QueryWorker);
}
DEFINE_PRIM(_IDL, NavWorld_getQueryWorker0, _IDL);

HL_PRIM void HL_NAME(NavWorld_getTileRegion4)(pref<NavWorld>* _this, h_float2 in_bmin, h_float2 in_bmax, h_int2 out_tmin, h_int2 out_tmax) {
	(_unref(_this)->getTileRegion((h_float2)in_bmin, (h_float2)in_bmax, (h_int2)out_tmin, (h_int2)out_tmax));
}
DEFINE_PRIM(_VOID, NavWorld_getTileRegion4, _IDL _STRUCT _STRUCT _STRUCT _STRUCT);

HL_PRIM void HL_NAME(NavMesh_create5)(pref<NavMesh>* _this, h_float3 origin, float tileWidth, float tileHeight, int maxTiles, int maxPolys) {
	(_unref(_this)->create((h_float3)origin, tileWidth, tileHeight, maxTiles, maxPolys));
}
DEFINE_PRIM(_VOID, NavMesh_create5, _IDL _STRUCT _F32 _F32 _I32 _I32);

HL_PRIM void HL_NAME(NavMesh_load2)(pref<NavMesh>* _this, pref<NavBuffer>* data, int flags) {
	(_unref(_this)->load(_unref_ptr_safe(data), flags));
}
DEFINE_PRIM(_VOID, NavMesh_load2, _IDL _IDL _I32);

HL_PRIM unsigned int HL_NAME(NavMesh_addTile2)(pref<NavMesh>* _this, pref<NavBuffer>* data, int flags) {
	return (_unref(_this)->addTile(_unref_ptr_safe(data), flags));
}
DEFINE_PRIM(_I32, NavMesh_addTile2, _IDL _IDL _I32);

HL_PRIM void HL_NAME(NavMesh_replaceTile3)(pref<NavMesh>* _this, pref<NavBuffer>* data, unsigned int ref, int flags) {
	(_unref(_this)->replaceTile(_unref_ptr_safe(data), ref, flags));
}
DEFINE_PRIM(_VOID, NavMesh_replaceTile3, _IDL _IDL _I32 _I32);

HL_PRIM unsigned int HL_NAME(NavMesh_removeTile1)(pref<NavMesh>* _this, unsigned int ref) {
	return (_unref(_this)->removeTile(ref));
}
DEFINE_PRIM(_I32, NavMesh_removeTile1, _IDL _I32);

HL_PRIM pref<dtNavMeshQuery>* HL_NAME(NavMesh_createQuery1)(pref<NavMesh>* _this, int maxNodes) {
	return alloc_ref((_unref(_this)->createQuery(maxNodes)),NavMeshQuery);
}
DEFINE_PRIM(_IDL, NavMesh_createQuery1, _IDL _I32);

HL_PRIM bool HL_NAME(NavMesh_isValidPolyRef1)(pref<NavMesh>* _this, unsigned int ref) {
	return (_unref(_this)->isValidPolyRef(ref));
}
DEFINE_PRIM(_BOOL, NavMesh_isValidPolyRef1, _IDL _I32);

HL_PRIM unsigned int HL_NAME(NavMesh_setPolyFlags2)(pref<NavMesh>* _this, unsigned int ref, unsigned short flags) {
	return (_unref(_this)->setPolyFlags(ref, flags));
}
DEFINE_PRIM(_I32, NavMesh_setPolyFlags2, _IDL _I32 _I16);

HL_PRIM unsigned int HL_NAME(NavMesh_getPolyFlags2)(pref<NavMesh>* _this, unsigned int ref, varray* resultFlags) {
	return (_unref(_this)->getPolyFlags(ref, hl_aptr(resultFlags,unsigned short)));
}
DEFINE_PRIM(_I32, NavMesh_getPolyFlags2, _IDL _I32 _ARR);

HL_PRIM unsigned int HL_NAME(NavMesh_setPolyArea2)(pref<NavMesh>* _this, unsigned int ref, unsigned char area) {
	return (_unref(_this)->setPolyArea(ref, area));
}
DEFINE_PRIM(_I32, NavMesh_setPolyArea2, _IDL _I32 _I8);

HL_PRIM unsigned int HL_NAME(NavMesh_getPolyArea2)(pref<NavMesh>* _this, unsigned int ref, varray* resultArea) {
	return (_unref(_this)->getPolyArea(ref, hl_aptr(resultArea,unsigned char)));
}
DEFINE_PRIM(_I32, NavMesh_getPolyArea2, _IDL _I32 _ARR);

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

HL_PRIM h_float3 HL_NAME(RaycastHit_get_hitNormal)( pref<dtRaycastHit>* _this ) {
	return (h_float3 )(_unref(_this)->hitNormal);
}
HL_PRIM void HL_NAME(RaycastHit_gethitNormalv)( pref<dtRaycastHit>* _this, h_float3 value ) {
	 float *src = (float*) & (_unref(_this)->hitNormal)[0];
	 float *dst = (float*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
DEFINE_PRIM(_VOID,RaycastHit_gethitNormalv,_IDL _STRUCT  );
DEFINE_PRIM(_STRUCT,RaycastHit_get_hitNormal,_IDL);
HL_PRIM h_float3 HL_NAME(RaycastHit_set_hitNormal)( pref<dtRaycastHit>* _this, h_float3 value ) {
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

HL_PRIM pref<NavMesh>* HL_NAME(NavMeshQuery_getAttachedNavMesh0)(pref<dtNavMeshQuery>* _this) {
	return alloc_ref((NavMesh*)(_unref(_this)->getAttachedNavMesh()),NavMesh);
}
DEFINE_PRIM(_IDL, NavMeshQuery_getAttachedNavMesh0, _IDL);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_findPath8)(pref<dtNavMeshQuery>* _this, unsigned int startRef, unsigned int endRef, h_float3 startPos, h_float3 endPos, pref<dtQueryFilter>* filter, varray* path, int* pathCount, int maxPath) {
	return (_unref(_this)->findPath(startRef, endRef, (float*)(h_float3)startPos, (float*)(h_float3)endPos, _unref_ptr_safe(filter), hl_aptr(path,unsigned int), pathCount, maxPath));
}
DEFINE_PRIM(_I32, NavMeshQuery_findPath8, _IDL _I32 _I32 _STRUCT _STRUCT _IDL _ARR _REF(_I32) _I32);

HL_PRIM int HL_NAME(NavMeshQuery_findNearestPoly5)(pref<dtNavMeshQuery>* _this, h_float3 center, h_float3 halfExtents, pref<dtQueryFilter>* filter, unsigned int* nearestRef, h_float3 nearestPt) {
	return HL_NAME(DtStatus_valueToIndex1)(_unref(_this)->findNearestPoly((float*)(h_float3)center, (float*)(h_float3)halfExtents, _unref_ptr_safe(filter), nearestRef, (float*)(h_float3)nearestPt));
}
DEFINE_PRIM(_I32, NavMeshQuery_findNearestPoly5, _IDL _STRUCT _STRUCT _IDL _REF(_I32) _STRUCT);

HL_PRIM int HL_NAME(NavMeshQuery_findNearestPoly6)(pref<dtNavMeshQuery>* _this, h_float3 center, h_float3 halfExtents, pref<dtQueryFilter>* filter, unsigned int* nearestRef, h_float3 nearestPt, bool* isOverPoly) {
	return HL_NAME(DtStatus_valueToIndex1)(_unref(_this)->findNearestPoly((float*)(h_float3)center, (float*)(h_float3)halfExtents, _unref_ptr_safe(filter), nearestRef, (float*)(h_float3)nearestPt, isOverPoly));
}
DEFINE_PRIM(_I32, NavMeshQuery_findNearestPoly6, _IDL _STRUCT _STRUCT _IDL _REF(_I32) _STRUCT _REF(_BOOL));

HL_PRIM unsigned int HL_NAME(NavMeshQuery_initSlicedFindPath6)(pref<dtNavMeshQuery>* _this, unsigned int startRef, unsigned int endRef, h_float3 startPos, h_float3 endPos, pref<dtQueryFilter>* filter, unsigned int options) {
	return (_unref(_this)->initSlicedFindPath(startRef, endRef, (float*)(h_float3)startPos, (float*)(h_float3)endPos, _unref_ptr_safe(filter), options));
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

HL_PRIM unsigned int HL_NAME(NavMeshQuery_findStraightPath10)(pref<dtNavMeshQuery>* _this, h_float3 startPos, h_float3 endPos, varray* path, int pathSize, varray* straightPath, varray* straightPathFlags, varray* straightPathRefs, int* straightPathCount, int maxStraightPath, int options) {
	return (_unref(_this)->findStraightPath((float*)(h_float3)startPos, (float*)(h_float3)endPos, hl_aptr(path,unsigned int), pathSize, hl_aptr(straightPath,float), hl_aptr(straightPathFlags,unsigned char), hl_aptr(straightPathRefs,unsigned int), straightPathCount, maxStraightPath, options));
}
DEFINE_PRIM(_I32, NavMeshQuery_findStraightPath10, _IDL _STRUCT _STRUCT _ARR _I32 _ARR _ARR _ARR _REF(_I32) _I32 _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_raycast9)(pref<dtNavMeshQuery>* _this, unsigned int startRef, h_float3 startPos, h_float3 endPos, pref<dtQueryFilter>* filter, float* t, h_float3 hitNormal, varray* path, int* pathCount, int maxPath) {
	return (_unref(_this)->raycast(startRef, (float*)(h_float3)startPos, (float*)(h_float3)endPos, _unref_ptr_safe(filter), t, (float*)(h_float3)hitNormal, hl_aptr(path,unsigned int), pathCount, maxPath));
}
DEFINE_PRIM(_I32, NavMeshQuery_raycast9, _IDL _I32 _STRUCT _STRUCT _IDL _REF(_F32) _STRUCT _ARR _REF(_I32) _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_raycast7)(pref<dtNavMeshQuery>* _this, unsigned int startRef, h_float3 startPos, h_float3 endPos, pref<dtQueryFilter>* filter, int options, pref<dtRaycastHit>* hit, unsigned int prevRef) {
	return (_unref(_this)->raycast(startRef, (float*)(h_float3)startPos, (float*)(h_float3)endPos, _unref_ptr_safe(filter), options, _unref_ptr_safe(hit), prevRef));
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

HL_PRIM unsigned int HL_NAME(NavMeshQuery_queryPolygons6)(pref<dtNavMeshQuery>* _this, h_float3 center, h_float3 halfExtents, pref<dtQueryFilter>* filter, unsigned int* polys, varray* polyCount, int maxPolys) {
	return (_unref(_this)->queryPolygons((float*)(h_float3)center, (float*)(h_float3)halfExtents, _unref_ptr_safe(filter), polys, hl_aptr(polyCount,int), maxPolys));
}
DEFINE_PRIM(_I32, NavMeshQuery_queryPolygons6, _IDL _STRUCT _STRUCT _IDL _REF(_I32) _ARR _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_queryPolygons4)(pref<dtNavMeshQuery>* _this, h_float3 center, h_float3 halfExtents, pref<dtQueryFilter>* filter, pref<dtPolyQuery>* query) {
	return (_unref(_this)->queryPolygons((float*)(h_float3)center, (float*)(h_float3)halfExtents, _unref_ptr_safe(filter), _unref_ptr_safe(query)));
}
DEFINE_PRIM(_I32, NavMeshQuery_queryPolygons4, _IDL _STRUCT _STRUCT _IDL _IDL);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_findLocalNeighbourhood8)(pref<dtNavMeshQuery>* _this, unsigned int startRef, h_float3 centerPos, float radius, pref<dtQueryFilter>* filter, unsigned int* resultRef, unsigned int* resultParent, varray* resultCount, int maxResult) {
	return (_unref(_this)->findLocalNeighbourhood(startRef, (float*)(h_float3)centerPos, radius, _unref_ptr_safe(filter), resultRef, resultParent, hl_aptr(resultCount,int), maxResult));
}
DEFINE_PRIM(_I32, NavMeshQuery_findLocalNeighbourhood8, _IDL _I32 _STRUCT _F32 _IDL _REF(_I32) _REF(_I32) _ARR _I32);

HL_PRIM unsigned int HL_NAME(NavMeshQuery_moveAlongSurface8)(pref<dtNavMeshQuery>* _this, unsigned int startRef, h_float3 startPos, h_float3 endPos, pref<dtQueryFilter>* filter, h_float3 resultPos, unsigned int* visited, varray* visitedCount, int maxVisitedSize) {
	return (_unref(_this)->moveAlongSurface(startRef, (float*)(h_float3)startPos, (float*)(h_float3)endPos, _unref_ptr_safe(filter), (float*)(h_float3)resultPos, visited, hl_aptr(visitedCount,int), maxVisitedSize));
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

HL_PRIM void HL_NAME(NavTileConverter_setLocation5)(pref<NavTileConverter>* _this, int x, int y, int layer, h_float3 min, h_float3 max) {
	(_unref(_this)->setLocation(x, y, layer, (h_float3)min, (h_float3)max));
}
DEFINE_PRIM(_VOID, NavTileConverter_setLocation5, _IDL _I32 _I32 _I32 _STRUCT _STRUCT);

HL_PRIM void HL_NAME(NavTileConverter_setWalkability3)(pref<NavTileConverter>* _this, float walkableHeight, float walkableRadius, float walkableClimb) {
	(_unref(_this)->setWalkability(walkableHeight, walkableRadius, walkableClimb));
}
DEFINE_PRIM(_VOID, NavTileConverter_setWalkability3, _IDL _F32 _F32 _F32);

HL_PRIM pref<NavBuffer>* HL_NAME(NavTileConverter_getTileData0)(pref<NavTileConverter>* _this) {
	return alloc_ref((_unref(_this)->getTileData()),NavBuffer);
}
DEFINE_PRIM(_IDL, NavTileConverter_getTileData0, _IDL);

}
