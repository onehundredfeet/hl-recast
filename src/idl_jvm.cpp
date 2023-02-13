
#include <jni.h>

	static JNIEnv *__s_haxe_env = nullptr;

	static inline void cacheJavaEnv(JNIEnv *p ) {
		if (__s_haxe_env == nullptr) {
			__s_haxe_env = p;
		} else if (__s_haxe_env != p) {
			printf("ERROR: Java env changed!\n");
		}
	}

	
// Float vector
struct _h_float2 {
	float x;
	float y;
};

struct _h_float3 {
	float x;
	float y;
	float z;
};

struct _h_float4 {
	float x;
	float y;
	float z;
	float w;
};

// int vector
struct _h_int2 {
	int x;
	int y;
};

struct _h_int3 {
	int x;
	int y;
	int z;
};

struct _h_int4 {
	int x;
	int y;
	int z;
	int w;
};

// double vector
struct _h_double2 {
	double x;
	double y;
};

struct _h_double3 {
	double x;
	double y;
	double z;
};

struct _h_double4 {
	double x;
	double y;
	double z;
	double w;
};


inline jobject _returnPointer( void *p ) {
	return 0;
}

template<class T>
inline T *h_aptr( jobject p ) {
	return nullptr;
}

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
            delete []_ptr;
    }
};
#define free_ref(v)
#define _unref(v) v
#define alloc_ref(r,_) (nullptr)


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
#include "jvm-recast.h"





extern "C" {

JNIEXPORT jstring JNICALL Java_recast_Init_getdllversion(JNIEnv *env) {
			return env->NewStringUTF("undefined");
		}
static rcLogCategory LogCategory__values[] = { RC_LOG_PROGRESS,RC_LOG_WARNING,RC_LOG_ERROR };
JNIEXPORT int JNICALL LogCategory_toValue(JNIEnv *__env, int idx ) {
	return LogCategory__values[idx];
}
JNIEXPORT int JNICALL LogCategory_indexToValue(JNIEnv *__env, int idx ) {
	return LogCategory__values[idx];
}
JNIEXPORT int JNICALL LogCategory_valueToIndex(JNIEnv *__env, int value ) {
	for( int i = 0; i < 3; i++ ) if ( value == (int)LogCategory__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL LogCategory_fromValue(JNIEnv *__env, int value ) {
	for( int i = 0; i < 3; i++ ) if ( value == (int)LogCategory__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL LogCategory_fromIndex(JNIEnv *__env, int index ) {return index;}
static rcTimerLabel TimerLabel__values[] = { RC_TIMER_TOTAL,RC_TIMER_TEMP,RC_TIMER_RASTERIZE_TRIANGLES,RC_TIMER_BUILD_COMPACTHEIGHTFIELD,RC_TIMER_BUILD_CONTOURS,RC_TIMER_BUILD_CONTOURS_TRACE,RC_TIMER_BUILD_CONTOURS_SIMPLIFY,RC_TIMER_FILTER_BORDER,RC_TIMER_FILTER_WALKABLE,RC_TIMER_MEDIAN_AREA,RC_TIMER_FILTER_LOW_OBSTACLES,RC_TIMER_BUILD_POLYMESH,RC_TIMER_MERGE_POLYMESH,RC_TIMER_ERODE_AREA,RC_TIMER_MARK_BOX_AREA,RC_TIMER_MARK_CYLINDER_AREA,RC_TIMER_MARK_CONVEXPOLY_AREA,RC_TIMER_BUILD_DISTANCEFIELD,RC_TIMER_BUILD_DISTANCEFIELD_DIST,RC_TIMER_BUILD_DISTANCEFIELD_BLUR,RC_TIMER_BUILD_REGIONS,RC_TIMER_BUILD_REGIONS_WATERSHED,RC_TIMER_BUILD_REGIONS_EXPAND,RC_TIMER_BUILD_REGIONS_FLOOD,RC_TIMER_BUILD_REGIONS_FILTER,RC_TIMER_BUILD_LAYERS,RC_TIMER_BUILD_POLYMESHDETAIL,RC_TIMER_MERGE_POLYMESHDETAIL,RC_MAX_TIMERS };
JNIEXPORT int JNICALL TimerLabel_toValue(JNIEnv *__env, int idx ) {
	return TimerLabel__values[idx];
}
JNIEXPORT int JNICALL TimerLabel_indexToValue(JNIEnv *__env, int idx ) {
	return TimerLabel__values[idx];
}
JNIEXPORT int JNICALL TimerLabel_valueToIndex(JNIEnv *__env, int value ) {
	for( int i = 0; i < 29; i++ ) if ( value == (int)TimerLabel__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL TimerLabel_fromValue(JNIEnv *__env, int value ) {
	for( int i = 0; i < 29; i++ ) if ( value == (int)TimerLabel__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL TimerLabel_fromIndex(JNIEnv *__env, int index ) {return index;}
static jclass __h_c_NavBuffer;
static jfieldID __h_f_NavBuffer_this;
static jmethodID __h_m_NavBuffer_ctor;
static inline void cache__h_c_NavBuffer( JNIEnv *env){
						if (__h_c_NavBuffer == nullptr){
							__h_c_NavBuffer = env->FindClass("recast/NavBuffer");
							__h_f_NavBuffer_this = env->GetFieldID(__h_c_NavBuffer , "_this", "J");
							__h_m_NavBuffer_ctor = env->GetMethodID(__h_c_NavBuffer ,"<init>", "()V");
						}
					}

static jclass __h_c_ChunkyTriMesh;
static jfieldID __h_f_ChunkyTriMesh_this;
static jmethodID __h_m_ChunkyTriMesh_ctor;
static inline void cache__h_c_ChunkyTriMesh( JNIEnv *env){
						if (__h_c_ChunkyTriMesh == nullptr){
							__h_c_ChunkyTriMesh = env->FindClass("recast/ChunkyTriMesh");
							__h_f_ChunkyTriMesh_this = env->GetFieldID(__h_c_ChunkyTriMesh , "_this", "J");
							__h_m_ChunkyTriMesh_ctor = env->GetMethodID(__h_c_ChunkyTriMesh ,"<init>", "()V");
						}
					}

static jclass __h_c_RasterContext;
static jfieldID __h_f_RasterContext_this;
static jmethodID __h_m_RasterContext_ctor;
static inline void cache__h_c_RasterContext( JNIEnv *env){
						if (__h_c_RasterContext == nullptr){
							__h_c_RasterContext = env->FindClass("recast/RasterContext");
							__h_f_RasterContext_this = env->GetFieldID(__h_c_RasterContext , "_this", "J");
							__h_m_RasterContext_ctor = env->GetMethodID(__h_c_RasterContext ,"<init>", "()V");
						}
					}

static jclass __h_c_rcScopedTimer;
static jfieldID __h_f_rcScopedTimer_this;
static jmethodID __h_m_rcScopedTimer_ctor;
static inline void cache__h_c_rcScopedTimer( JNIEnv *env){
						if (__h_c_rcScopedTimer == nullptr){
							__h_c_rcScopedTimer = env->FindClass("recast/rcScopedTimer");
							__h_f_rcScopedTimer_this = env->GetFieldID(__h_c_rcScopedTimer , "_this", "J");
							__h_m_rcScopedTimer_ctor = env->GetMethodID(__h_c_rcScopedTimer ,"<init>", "()V");
						}
					}

static jclass __h_c_RasterConfig;
static jfieldID __h_f_RasterConfig_this;
static jmethodID __h_m_RasterConfig_ctor;
static inline void cache__h_c_RasterConfig( JNIEnv *env){
						if (__h_c_RasterConfig == nullptr){
							__h_c_RasterConfig = env->FindClass("recast/RasterConfig");
							__h_f_RasterConfig_this = env->GetFieldID(__h_c_RasterConfig , "_this", "J");
							__h_m_RasterConfig_ctor = env->GetMethodID(__h_c_RasterConfig ,"<init>", "()V");
						}
					}

static jclass __h_c_Span;
static jfieldID __h_f_Span_this;
static jmethodID __h_m_Span_ctor;
static inline void cache__h_c_Span( JNIEnv *env){
						if (__h_c_Span == nullptr){
							__h_c_Span = env->FindClass("recast/Span");
							__h_f_Span_this = env->GetFieldID(__h_c_Span , "_this", "J");
							__h_m_Span_ctor = env->GetMethodID(__h_c_Span ,"<init>", "()V");
						}
					}

static jclass __h_c_SpanPool;
static jfieldID __h_f_SpanPool_this;
static jmethodID __h_m_SpanPool_ctor;
static inline void cache__h_c_SpanPool( JNIEnv *env){
						if (__h_c_SpanPool == nullptr){
							__h_c_SpanPool = env->FindClass("recast/SpanPool");
							__h_f_SpanPool_this = env->GetFieldID(__h_c_SpanPool , "_this", "J");
							__h_m_SpanPool_ctor = env->GetMethodID(__h_c_SpanPool ,"<init>", "()V");
						}
					}

static jclass __h_c_Heightfield;
static jfieldID __h_f_Heightfield_this;
static jmethodID __h_m_Heightfield_ctor;
static inline void cache__h_c_Heightfield( JNIEnv *env){
						if (__h_c_Heightfield == nullptr){
							__h_c_Heightfield = env->FindClass("recast/Heightfield");
							__h_f_Heightfield_this = env->GetFieldID(__h_c_Heightfield , "_this", "J");
							__h_m_Heightfield_ctor = env->GetMethodID(__h_c_Heightfield ,"<init>", "()V");
						}
					}

static jclass __h_c_rcCompactCell;
static jfieldID __h_f_rcCompactCell_this;
static jmethodID __h_m_rcCompactCell_ctor;
static inline void cache__h_c_rcCompactCell( JNIEnv *env){
						if (__h_c_rcCompactCell == nullptr){
							__h_c_rcCompactCell = env->FindClass("recast/rcCompactCell");
							__h_f_rcCompactCell_this = env->GetFieldID(__h_c_rcCompactCell , "_this", "J");
							__h_m_rcCompactCell_ctor = env->GetMethodID(__h_c_rcCompactCell ,"<init>", "()V");
						}
					}

static jclass __h_c_rcCompactSpan;
static jfieldID __h_f_rcCompactSpan_this;
static jmethodID __h_m_rcCompactSpan_ctor;
static inline void cache__h_c_rcCompactSpan( JNIEnv *env){
						if (__h_c_rcCompactSpan == nullptr){
							__h_c_rcCompactSpan = env->FindClass("recast/rcCompactSpan");
							__h_f_rcCompactSpan_this = env->GetFieldID(__h_c_rcCompactSpan , "_this", "J");
							__h_m_rcCompactSpan_ctor = env->GetMethodID(__h_c_rcCompactSpan ,"<init>", "()V");
						}
					}

static jclass __h_c_CompactHeightfield;
static jfieldID __h_f_CompactHeightfield_this;
static jmethodID __h_m_CompactHeightfield_ctor;
static inline void cache__h_c_CompactHeightfield( JNIEnv *env){
						if (__h_c_CompactHeightfield == nullptr){
							__h_c_CompactHeightfield = env->FindClass("recast/CompactHeightfield");
							__h_f_CompactHeightfield_this = env->GetFieldID(__h_c_CompactHeightfield , "_this", "J");
							__h_m_CompactHeightfield_ctor = env->GetMethodID(__h_c_CompactHeightfield ,"<init>", "()V");
						}
					}

static jclass __h_c_HeightfieldLayer;
static jfieldID __h_f_HeightfieldLayer_this;
static jmethodID __h_m_HeightfieldLayer_ctor;
static inline void cache__h_c_HeightfieldLayer( JNIEnv *env){
						if (__h_c_HeightfieldLayer == nullptr){
							__h_c_HeightfieldLayer = env->FindClass("recast/HeightfieldLayer");
							__h_f_HeightfieldLayer_this = env->GetFieldID(__h_c_HeightfieldLayer , "_this", "J");
							__h_m_HeightfieldLayer_ctor = env->GetMethodID(__h_c_HeightfieldLayer ,"<init>", "()V");
						}
					}

static jclass __h_c_HeightfieldLayerSet;
static jfieldID __h_f_HeightfieldLayerSet_this;
static jmethodID __h_m_HeightfieldLayerSet_ctor;
static inline void cache__h_c_HeightfieldLayerSet( JNIEnv *env){
						if (__h_c_HeightfieldLayerSet == nullptr){
							__h_c_HeightfieldLayerSet = env->FindClass("recast/HeightfieldLayerSet");
							__h_f_HeightfieldLayerSet_this = env->GetFieldID(__h_c_HeightfieldLayerSet , "_this", "J");
							__h_m_HeightfieldLayerSet_ctor = env->GetMethodID(__h_c_HeightfieldLayerSet ,"<init>", "()V");
						}
					}

static jclass __h_c_rcContour;
static jfieldID __h_f_rcContour_this;
static jmethodID __h_m_rcContour_ctor;
static inline void cache__h_c_rcContour( JNIEnv *env){
						if (__h_c_rcContour == nullptr){
							__h_c_rcContour = env->FindClass("recast/rcContour");
							__h_f_rcContour_this = env->GetFieldID(__h_c_rcContour , "_this", "J");
							__h_m_rcContour_ctor = env->GetMethodID(__h_c_rcContour ,"<init>", "()V");
						}
					}

static jclass __h_c_rcContourSet;
static jfieldID __h_f_rcContourSet_this;
static jmethodID __h_m_rcContourSet_ctor;
static inline void cache__h_c_rcContourSet( JNIEnv *env){
						if (__h_c_rcContourSet == nullptr){
							__h_c_rcContourSet = env->FindClass("recast/rcContourSet");
							__h_f_rcContourSet_this = env->GetFieldID(__h_c_rcContourSet , "_this", "J");
							__h_m_rcContourSet_ctor = env->GetMethodID(__h_c_rcContourSet ,"<init>", "()V");
						}
					}

static jclass __h_c_PolyMesh;
static jfieldID __h_f_PolyMesh_this;
static jmethodID __h_m_PolyMesh_ctor;
static inline void cache__h_c_PolyMesh( JNIEnv *env){
						if (__h_c_PolyMesh == nullptr){
							__h_c_PolyMesh = env->FindClass("recast/PolyMesh");
							__h_f_PolyMesh_this = env->GetFieldID(__h_c_PolyMesh , "_this", "J");
							__h_m_PolyMesh_ctor = env->GetMethodID(__h_c_PolyMesh ,"<init>", "()V");
						}
					}

static jclass __h_c_PolyMeshDetail;
static jfieldID __h_f_PolyMeshDetail_this;
static jmethodID __h_m_PolyMeshDetail_ctor;
static inline void cache__h_c_PolyMeshDetail( JNIEnv *env){
						if (__h_c_PolyMeshDetail == nullptr){
							__h_c_PolyMeshDetail = env->FindClass("recast/PolyMeshDetail");
							__h_f_PolyMeshDetail_this = env->GetFieldID(__h_c_PolyMeshDetail , "_this", "J");
							__h_m_PolyMeshDetail_ctor = env->GetMethodID(__h_c_PolyMeshDetail ,"<init>", "()V");
						}
					}

static rcAllocHint AllocHint__values[] = { RC_ALLOC_PERM,RC_ALLOC_TEMP };
JNIEXPORT int JNICALL AllocHint_toValue(JNIEnv *__env, int idx ) {
	return AllocHint__values[idx];
}
JNIEXPORT int JNICALL AllocHint_indexToValue(JNIEnv *__env, int idx ) {
	return AllocHint__values[idx];
}
JNIEXPORT int JNICALL AllocHint_valueToIndex(JNIEnv *__env, int value ) {
	for( int i = 0; i < 2; i++ ) if ( value == (int)AllocHint__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL AllocHint_fromValue(JNIEnv *__env, int value ) {
	for( int i = 0; i < 2; i++ ) if ( value == (int)AllocHint__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL AllocHint_fromIndex(JNIEnv *__env, int index ) {return index;}
static jclass __h_c_Alloc;
static jfieldID __h_f_Alloc_this;
static jmethodID __h_m_Alloc_ctor;
static inline void cache__h_c_Alloc( JNIEnv *env){
						if (__h_c_Alloc == nullptr){
							__h_c_Alloc = env->FindClass("recast/Alloc");
							__h_f_Alloc_this = env->GetFieldID(__h_c_Alloc , "_this", "J");
							__h_m_Alloc_ctor = env->GetMethodID(__h_c_Alloc ,"<init>", "()V");
						}
					}

static jclass __h_c_Recast;
static jfieldID __h_f_Recast_this;
static jmethodID __h_m_Recast_ctor;
static inline void cache__h_c_Recast( JNIEnv *env){
						if (__h_c_Recast == nullptr){
							__h_c_Recast = env->FindClass("recast/Recast");
							__h_f_Recast_this = env->GetFieldID(__h_c_Recast , "_this", "J");
							__h_m_Recast_ctor = env->GetMethodID(__h_c_Recast ,"<init>", "()V");
						}
					}

static jclass __h_c_Math;
static jfieldID __h_f_Math_this;
static jmethodID __h_m_Math_ctor;
static inline void cache__h_c_Math( JNIEnv *env){
						if (__h_c_Math == nullptr){
							__h_c_Math = env->FindClass("recast/Math");
							__h_f_Math_this = env->GetFieldID(__h_c_Math , "_this", "J");
							__h_m_Math_ctor = env->GetMethodID(__h_c_Math ,"<init>", "()V");
						}
					}

static jclass __h_c_DetourCommon;
static jfieldID __h_f_DetourCommon_this;
static jmethodID __h_m_DetourCommon_ctor;
static inline void cache__h_c_DetourCommon( JNIEnv *env){
						if (__h_c_DetourCommon == nullptr){
							__h_c_DetourCommon = env->FindClass("recast/DetourCommon");
							__h_f_DetourCommon_this = env->GetFieldID(__h_c_DetourCommon , "_this", "J");
							__h_m_DetourCommon_ctor = env->GetMethodID(__h_c_DetourCommon ,"<init>", "()V");
						}
					}

static jclass __h_c_TileCache;
static jfieldID __h_f_TileCache_this;
static jmethodID __h_m_TileCache_ctor;
static inline void cache__h_c_TileCache( JNIEnv *env){
						if (__h_c_TileCache == nullptr){
							__h_c_TileCache = env->FindClass("recast/TileCache");
							__h_f_TileCache_this = env->GetFieldID(__h_c_TileCache , "_this", "J");
							__h_m_TileCache_ctor = env->GetMethodID(__h_c_TileCache ,"<init>", "()V");
						}
					}

static jclass __h_c_CompressedTile;
static jfieldID __h_f_CompressedTile_this;
static jmethodID __h_m_CompressedTile_ctor;
static inline void cache__h_c_CompressedTile( JNIEnv *env){
						if (__h_c_CompressedTile == nullptr){
							__h_c_CompressedTile = env->FindClass("recast/CompressedTile");
							__h_f_CompressedTile_this = env->GetFieldID(__h_c_CompressedTile , "_this", "J");
							__h_m_CompressedTile_ctor = env->GetMethodID(__h_c_CompressedTile ,"<init>", "()V");
						}
					}

static jclass __h_c_TileCacheMeshProcess;
static jfieldID __h_f_TileCacheMeshProcess_this;
static jmethodID __h_m_TileCacheMeshProcess_ctor;
static inline void cache__h_c_TileCacheMeshProcess( JNIEnv *env){
						if (__h_c_TileCacheMeshProcess == nullptr){
							__h_c_TileCacheMeshProcess = env->FindClass("recast/TileCacheMeshProcess");
							__h_f_TileCacheMeshProcess_this = env->GetFieldID(__h_c_TileCacheMeshProcess , "_this", "J");
							__h_m_TileCacheMeshProcess_ctor = env->GetMethodID(__h_c_TileCacheMeshProcess ,"<init>", "()V");
						}
					}

static jclass __h_c_TileCacheAlloc;
static jfieldID __h_f_TileCacheAlloc_this;
static jmethodID __h_m_TileCacheAlloc_ctor;
static inline void cache__h_c_TileCacheAlloc( JNIEnv *env){
						if (__h_c_TileCacheAlloc == nullptr){
							__h_c_TileCacheAlloc = env->FindClass("recast/TileCacheAlloc");
							__h_f_TileCacheAlloc_this = env->GetFieldID(__h_c_TileCacheAlloc , "_this", "J");
							__h_m_TileCacheAlloc_ctor = env->GetMethodID(__h_c_TileCacheAlloc ,"<init>", "()V");
						}
					}

static jclass __h_c_TileCacheCompressor;
static jfieldID __h_f_TileCacheCompressor_this;
static jmethodID __h_m_TileCacheCompressor_ctor;
static inline void cache__h_c_TileCacheCompressor( JNIEnv *env){
						if (__h_c_TileCacheCompressor == nullptr){
							__h_c_TileCacheCompressor = env->FindClass("recast/TileCacheCompressor");
							__h_f_TileCacheCompressor_this = env->GetFieldID(__h_c_TileCacheCompressor , "_this", "J");
							__h_m_TileCacheCompressor_ctor = env->GetMethodID(__h_c_TileCacheCompressor ,"<init>", "()V");
						}
					}

static TileCacheLayerHeaderConstants TileCacheLayerHeaderConstants__values[] = { TILECACHE_MAGIC,TILECACHE_VERSION };
JNIEXPORT int JNICALL TileCacheLayerHeaderConstants_toValue(JNIEnv *__env, int idx ) {
	return TileCacheLayerHeaderConstants__values[idx];
}
JNIEXPORT int JNICALL TileCacheLayerHeaderConstants_indexToValue(JNIEnv *__env, int idx ) {
	return TileCacheLayerHeaderConstants__values[idx];
}
JNIEXPORT int JNICALL TileCacheLayerHeaderConstants_valueToIndex(JNIEnv *__env, int value ) {
	for( int i = 0; i < 2; i++ ) if ( value == (int)TileCacheLayerHeaderConstants__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL TileCacheLayerHeaderConstants_fromValue(JNIEnv *__env, int value ) {
	for( int i = 0; i < 2; i++ ) if ( value == (int)TileCacheLayerHeaderConstants__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL TileCacheLayerHeaderConstants_fromIndex(JNIEnv *__env, int index ) {return index;}
static jclass __h_c_TileCacheLayerHeader;
static jfieldID __h_f_TileCacheLayerHeader_this;
static jmethodID __h_m_TileCacheLayerHeader_ctor;
static inline void cache__h_c_TileCacheLayerHeader( JNIEnv *env){
						if (__h_c_TileCacheLayerHeader == nullptr){
							__h_c_TileCacheLayerHeader = env->FindClass("recast/TileCacheLayerHeader");
							__h_f_TileCacheLayerHeader_this = env->GetFieldID(__h_c_TileCacheLayerHeader , "_this", "J");
							__h_m_TileCacheLayerHeader_ctor = env->GetMethodID(__h_c_TileCacheLayerHeader ,"<init>", "()V");
						}
					}

static jclass __h_c_TileCacheLayer;
static jfieldID __h_f_TileCacheLayer_this;
static jmethodID __h_m_TileCacheLayer_ctor;
static inline void cache__h_c_TileCacheLayer( JNIEnv *env){
						if (__h_c_TileCacheLayer == nullptr){
							__h_c_TileCacheLayer = env->FindClass("recast/TileCacheLayer");
							__h_f_TileCacheLayer_this = env->GetFieldID(__h_c_TileCacheLayer , "_this", "J");
							__h_m_TileCacheLayer_ctor = env->GetMethodID(__h_c_TileCacheLayer ,"<init>", "()V");
						}
					}

static jclass __h_c_TileCacheContour;
static jfieldID __h_f_TileCacheContour_this;
static jmethodID __h_m_TileCacheContour_ctor;
static inline void cache__h_c_TileCacheContour( JNIEnv *env){
						if (__h_c_TileCacheContour == nullptr){
							__h_c_TileCacheContour = env->FindClass("recast/TileCacheContour");
							__h_f_TileCacheContour_this = env->GetFieldID(__h_c_TileCacheContour , "_this", "J");
							__h_m_TileCacheContour_ctor = env->GetMethodID(__h_c_TileCacheContour ,"<init>", "()V");
						}
					}

static jclass __h_c_TileCacheContourSet;
static jfieldID __h_f_TileCacheContourSet_this;
static jmethodID __h_m_TileCacheContourSet_ctor;
static inline void cache__h_c_TileCacheContourSet( JNIEnv *env){
						if (__h_c_TileCacheContourSet == nullptr){
							__h_c_TileCacheContourSet = env->FindClass("recast/TileCacheContourSet");
							__h_f_TileCacheContourSet_this = env->GetFieldID(__h_c_TileCacheContourSet , "_this", "J");
							__h_m_TileCacheContourSet_ctor = env->GetMethodID(__h_c_TileCacheContourSet ,"<init>", "()V");
						}
					}

static jclass __h_c_TileCachePolyMesh;
static jfieldID __h_f_TileCachePolyMesh_this;
static jmethodID __h_m_TileCachePolyMesh_ctor;
static inline void cache__h_c_TileCachePolyMesh( JNIEnv *env){
						if (__h_c_TileCachePolyMesh == nullptr){
							__h_c_TileCachePolyMesh = env->FindClass("recast/TileCachePolyMesh");
							__h_f_TileCachePolyMesh_this = env->GetFieldID(__h_c_TileCachePolyMesh , "_this", "J");
							__h_m_TileCachePolyMesh_ctor = env->GetMethodID(__h_c_TileCachePolyMesh ,"<init>", "()V");
						}
					}

static jclass __h_c_TileCacheObstacle;
static jfieldID __h_f_TileCacheObstacle_this;
static jmethodID __h_m_TileCacheObstacle_ctor;
static inline void cache__h_c_TileCacheObstacle( JNIEnv *env){
						if (__h_c_TileCacheObstacle == nullptr){
							__h_c_TileCacheObstacle = env->FindClass("recast/TileCacheObstacle");
							__h_f_TileCacheObstacle_this = env->GetFieldID(__h_c_TileCacheObstacle , "_this", "J");
							__h_m_TileCacheObstacle_ctor = env->GetMethodID(__h_c_TileCacheObstacle ,"<init>", "()V");
						}
					}

static jclass __h_c_TileCacheParams;
static jfieldID __h_f_TileCacheParams_this;
static jmethodID __h_m_TileCacheParams_ctor;
static inline void cache__h_c_TileCacheParams( JNIEnv *env){
						if (__h_c_TileCacheParams == nullptr){
							__h_c_TileCacheParams = env->FindClass("recast/TileCacheParams");
							__h_f_TileCacheParams_this = env->GetFieldID(__h_c_TileCacheParams , "_this", "J");
							__h_m_TileCacheParams_ctor = env->GetMethodID(__h_c_TileCacheParams ,"<init>", "()V");
						}
					}

static jclass __h_c_TileCacheData;
static jfieldID __h_f_TileCacheData_this;
static jmethodID __h_m_TileCacheData_ctor;
static inline void cache__h_c_TileCacheData( JNIEnv *env){
						if (__h_c_TileCacheData == nullptr){
							__h_c_TileCacheData = env->FindClass("recast/TileCacheData");
							__h_f_TileCacheData_this = env->GetFieldID(__h_c_TileCacheData , "_this", "J");
							__h_m_TileCacheData_ctor = env->GetMethodID(__h_c_TileCacheData ,"<init>", "()V");
						}
					}

static jclass __h_c_FastLZCompressor;
static jfieldID __h_f_FastLZCompressor_this;
static jmethodID __h_m_FastLZCompressor_ctor;
static inline void cache__h_c_FastLZCompressor( JNIEnv *env){
						if (__h_c_FastLZCompressor == nullptr){
							__h_c_FastLZCompressor = env->FindClass("recast/FastLZCompressor");
							__h_f_FastLZCompressor_this = env->GetFieldID(__h_c_FastLZCompressor , "_this", "J");
							__h_m_FastLZCompressor_ctor = env->GetMethodID(__h_c_FastLZCompressor ,"<init>", "()V");
						}
					}

static jclass __h_c_LinearAllocator;
static jfieldID __h_f_LinearAllocator_this;
static jmethodID __h_m_LinearAllocator_ctor;
static inline void cache__h_c_LinearAllocator( JNIEnv *env){
						if (__h_c_LinearAllocator == nullptr){
							__h_c_LinearAllocator = env->FindClass("recast/LinearAllocator");
							__h_f_LinearAllocator_this = env->GetFieldID(__h_c_LinearAllocator , "_this", "J");
							__h_m_LinearAllocator_ctor = env->GetMethodID(__h_c_LinearAllocator ,"<init>", "()V");
						}
					}

static jclass __h_c_RemapProcessor;
static jfieldID __h_f_RemapProcessor_this;
static jmethodID __h_m_RemapProcessor_ctor;
static inline void cache__h_c_RemapProcessor( JNIEnv *env){
						if (__h_c_RemapProcessor == nullptr){
							__h_c_RemapProcessor = env->FindClass("recast/RemapProcessor");
							__h_f_RemapProcessor_this = env->GetFieldID(__h_c_RemapProcessor , "_this", "J");
							__h_m_RemapProcessor_ctor = env->GetMethodID(__h_c_RemapProcessor ,"<init>", "()V");
						}
					}

static dtCompressedTileFlags CompressedTileFlags__values[] = { DT_COMPRESSEDTILE_FREE_DATA };
JNIEXPORT int JNICALL CompressedTileFlags_toValue(JNIEnv *__env, int idx ) {
	return CompressedTileFlags__values[idx];
}
JNIEXPORT int JNICALL CompressedTileFlags_indexToValue(JNIEnv *__env, int idx ) {
	return CompressedTileFlags__values[idx];
}
JNIEXPORT int JNICALL CompressedTileFlags_valueToIndex(JNIEnv *__env, int value ) {
	for( int i = 0; i < 1; i++ ) if ( value == (int)CompressedTileFlags__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL CompressedTileFlags_fromValue(JNIEnv *__env, int value ) {
	for( int i = 0; i < 1; i++ ) if ( value == (int)CompressedTileFlags__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL CompressedTileFlags_fromIndex(JNIEnv *__env, int index ) {return index;}
static dtStraightPathFlags StraightPathFlags__values[] = { DT_STRAIGHTPATH_START,DT_STRAIGHTPATH_END,DT_STRAIGHTPATH_OFFMESH_CONNECTION };
JNIEXPORT int JNICALL StraightPathFlags_toValue(JNIEnv *__env, int idx ) {
	return StraightPathFlags__values[idx];
}
JNIEXPORT int JNICALL StraightPathFlags_indexToValue(JNIEnv *__env, int idx ) {
	return StraightPathFlags__values[idx];
}
JNIEXPORT int JNICALL StraightPathFlags_valueToIndex(JNIEnv *__env, int value ) {
	for( int i = 0; i < 3; i++ ) if ( value == (int)StraightPathFlags__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL StraightPathFlags_fromValue(JNIEnv *__env, int value ) {
	for( int i = 0; i < 3; i++ ) if ( value == (int)StraightPathFlags__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL StraightPathFlags_fromIndex(JNIEnv *__env, int index ) {return index;}
static dtStraightPathOptions StraightPathOptions__values[] = { DT_STRAIGHTPATH_AREA_CROSSINGS,DT_STRAIGHTPATH_ALL_CROSSINGS };
JNIEXPORT int JNICALL StraightPathOptions_toValue(JNIEnv *__env, int idx ) {
	return StraightPathOptions__values[idx];
}
JNIEXPORT int JNICALL StraightPathOptions_indexToValue(JNIEnv *__env, int idx ) {
	return StraightPathOptions__values[idx];
}
JNIEXPORT int JNICALL StraightPathOptions_valueToIndex(JNIEnv *__env, int value ) {
	for( int i = 0; i < 2; i++ ) if ( value == (int)StraightPathOptions__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL StraightPathOptions_fromValue(JNIEnv *__env, int value ) {
	for( int i = 0; i < 2; i++ ) if ( value == (int)StraightPathOptions__values[i]) return i; return -1;
}
JNIEXPORT int JNICALL StraightPathOptions_fromIndex(JNIEnv *__env, int index ) {return index;}
static jclass __h_c_Poly;
static jfieldID __h_f_Poly_this;
static jmethodID __h_m_Poly_ctor;
static inline void cache__h_c_Poly( JNIEnv *env){
						if (__h_c_Poly == nullptr){
							__h_c_Poly = env->FindClass("recast/Poly");
							__h_f_Poly_this = env->GetFieldID(__h_c_Poly , "_this", "J");
							__h_m_Poly_ctor = env->GetMethodID(__h_c_Poly ,"<init>", "()V");
						}
					}

static jclass __h_c_PolyDetail;
static jfieldID __h_f_PolyDetail_this;
static jmethodID __h_m_PolyDetail_ctor;
static inline void cache__h_c_PolyDetail( JNIEnv *env){
						if (__h_c_PolyDetail == nullptr){
							__h_c_PolyDetail = env->FindClass("recast/PolyDetail");
							__h_f_PolyDetail_this = env->GetFieldID(__h_c_PolyDetail , "_this", "J");
							__h_m_PolyDetail_ctor = env->GetMethodID(__h_c_PolyDetail ,"<init>", "()V");
						}
					}

static jclass __h_c_Link;
static jfieldID __h_f_Link_this;
static jmethodID __h_m_Link_ctor;
static inline void cache__h_c_Link( JNIEnv *env){
						if (__h_c_Link == nullptr){
							__h_c_Link = env->FindClass("recast/Link");
							__h_f_Link_this = env->GetFieldID(__h_c_Link , "_this", "J");
							__h_m_Link_ctor = env->GetMethodID(__h_c_Link ,"<init>", "()V");
						}
					}

static jclass __h_c_BVNode;
static jfieldID __h_f_BVNode_this;
static jmethodID __h_m_BVNode_ctor;
static inline void cache__h_c_BVNode( JNIEnv *env){
						if (__h_c_BVNode == nullptr){
							__h_c_BVNode = env->FindClass("recast/BVNode");
							__h_f_BVNode_this = env->GetFieldID(__h_c_BVNode , "_this", "J");
							__h_m_BVNode_ctor = env->GetMethodID(__h_c_BVNode ,"<init>", "()V");
						}
					}

static jclass __h_c_OffMeshConnection;
static jfieldID __h_f_OffMeshConnection_this;
static jmethodID __h_m_OffMeshConnection_ctor;
static inline void cache__h_c_OffMeshConnection( JNIEnv *env){
						if (__h_c_OffMeshConnection == nullptr){
							__h_c_OffMeshConnection = env->FindClass("recast/OffMeshConnection");
							__h_f_OffMeshConnection_this = env->GetFieldID(__h_c_OffMeshConnection , "_this", "J");
							__h_m_OffMeshConnection_ctor = env->GetMethodID(__h_c_OffMeshConnection ,"<init>", "()V");
						}
					}

static jclass __h_c_MeshHeader;
static jfieldID __h_f_MeshHeader_this;
static jmethodID __h_m_MeshHeader_ctor;
static inline void cache__h_c_MeshHeader( JNIEnv *env){
						if (__h_c_MeshHeader == nullptr){
							__h_c_MeshHeader = env->FindClass("recast/MeshHeader");
							__h_f_MeshHeader_this = env->GetFieldID(__h_c_MeshHeader , "_this", "J");
							__h_m_MeshHeader_ctor = env->GetMethodID(__h_c_MeshHeader ,"<init>", "()V");
						}
					}

static jclass __h_c_MeshTile;
static jfieldID __h_f_MeshTile_this;
static jmethodID __h_m_MeshTile_ctor;
static inline void cache__h_c_MeshTile( JNIEnv *env){
						if (__h_c_MeshTile == nullptr){
							__h_c_MeshTile = env->FindClass("recast/MeshTile");
							__h_f_MeshTile_this = env->GetFieldID(__h_c_MeshTile , "_this", "J");
							__h_m_MeshTile_ctor = env->GetMethodID(__h_c_MeshTile ,"<init>", "()V");
						}
					}

static jclass __h_c_NavMesh;
static jfieldID __h_f_NavMesh_this;
static jmethodID __h_m_NavMesh_ctor;
static inline void cache__h_c_NavMesh( JNIEnv *env){
						if (__h_c_NavMesh == nullptr){
							__h_c_NavMesh = env->FindClass("recast/NavMesh");
							__h_f_NavMesh_this = env->GetFieldID(__h_c_NavMesh , "_this", "J");
							__h_m_NavMesh_ctor = env->GetMethodID(__h_c_NavMesh ,"<init>", "()V");
						}
					}

static jclass __h_c_Node;
static jfieldID __h_f_Node_this;
static jmethodID __h_m_Node_ctor;
static inline void cache__h_c_Node( JNIEnv *env){
						if (__h_c_Node == nullptr){
							__h_c_Node = env->FindClass("recast/Node");
							__h_f_Node_this = env->GetFieldID(__h_c_Node , "_this", "J");
							__h_m_Node_ctor = env->GetMethodID(__h_c_Node ,"<init>", "()V");
						}
					}

static jclass __h_c_NodePool;
static jfieldID __h_f_NodePool_this;
static jmethodID __h_m_NodePool_ctor;
static inline void cache__h_c_NodePool( JNIEnv *env){
						if (__h_c_NodePool == nullptr){
							__h_c_NodePool = env->FindClass("recast/NodePool");
							__h_f_NodePool_this = env->GetFieldID(__h_c_NodePool , "_this", "J");
							__h_m_NodePool_ctor = env->GetMethodID(__h_c_NodePool ,"<init>", "()V");
						}
					}

static jclass __h_c_NodeQueue;
static jfieldID __h_f_NodeQueue_this;
static jmethodID __h_m_NodeQueue_ctor;
static inline void cache__h_c_NodeQueue( JNIEnv *env){
						if (__h_c_NodeQueue == nullptr){
							__h_c_NodeQueue = env->FindClass("recast/NodeQueue");
							__h_f_NodeQueue_this = env->GetFieldID(__h_c_NodeQueue , "_this", "J");
							__h_m_NodeQueue_ctor = env->GetMethodID(__h_c_NodeQueue ,"<init>", "()V");
						}
					}

static jclass __h_c_NavMeshCreateParams;
static jfieldID __h_f_NavMeshCreateParams_this;
static jmethodID __h_m_NavMeshCreateParams_ctor;
static inline void cache__h_c_NavMeshCreateParams( JNIEnv *env){
						if (__h_c_NavMeshCreateParams == nullptr){
							__h_c_NavMeshCreateParams = env->FindClass("recast/NavMeshCreateParams");
							__h_f_NavMeshCreateParams_this = env->GetFieldID(__h_c_NavMeshCreateParams , "_this", "J");
							__h_m_NavMeshCreateParams_ctor = env->GetMethodID(__h_c_NavMeshCreateParams ,"<init>", "()V");
						}
					}

static jclass __h_c_QueryFilter;
static jfieldID __h_f_QueryFilter_this;
static jmethodID __h_m_QueryFilter_ctor;
static inline void cache__h_c_QueryFilter( JNIEnv *env){
						if (__h_c_QueryFilter == nullptr){
							__h_c_QueryFilter = env->FindClass("recast/QueryFilter");
							__h_f_QueryFilter_this = env->GetFieldID(__h_c_QueryFilter , "_this", "J");
							__h_m_QueryFilter_ctor = env->GetMethodID(__h_c_QueryFilter ,"<init>", "()V");
						}
					}

static jclass __h_c_RaycastHit;
static jfieldID __h_f_RaycastHit_this;
static jmethodID __h_m_RaycastHit_ctor;
static inline void cache__h_c_RaycastHit( JNIEnv *env){
						if (__h_c_RaycastHit == nullptr){
							__h_c_RaycastHit = env->FindClass("recast/RaycastHit");
							__h_f_RaycastHit_this = env->GetFieldID(__h_c_RaycastHit , "_this", "J");
							__h_m_RaycastHit_ctor = env->GetMethodID(__h_c_RaycastHit ,"<init>", "()V");
						}
					}

static jclass __h_c_PolyQuery;
static jfieldID __h_f_PolyQuery_this;
static jmethodID __h_m_PolyQuery_ctor;
static inline void cache__h_c_PolyQuery( JNIEnv *env){
						if (__h_c_PolyQuery == nullptr){
							__h_c_PolyQuery = env->FindClass("recast/PolyQuery");
							__h_f_PolyQuery_this = env->GetFieldID(__h_c_PolyQuery , "_this", "J");
							__h_m_PolyQuery_ctor = env->GetMethodID(__h_c_PolyQuery ,"<init>", "()V");
						}
					}

static jclass __h_c_NavMeshQuery;
static jfieldID __h_f_NavMeshQuery_this;
static jmethodID __h_m_NavMeshQuery_ctor;
static inline void cache__h_c_NavMeshQuery( JNIEnv *env){
						if (__h_c_NavMeshQuery == nullptr){
							__h_c_NavMeshQuery = env->FindClass("recast/NavMeshQuery");
							__h_f_NavMeshQuery_this = env->GetFieldID(__h_c_NavMeshQuery , "_this", "J");
							__h_m_NavMeshQuery_ctor = env->GetMethodID(__h_c_NavMeshQuery ,"<init>", "()V");
						}
					}

static jclass __h_c_DetourNavMeshQuery;
static jfieldID __h_f_DetourNavMeshQuery_this;
static jmethodID __h_m_DetourNavMeshQuery_ctor;
static inline void cache__h_c_DetourNavMeshQuery( JNIEnv *env){
						if (__h_c_DetourNavMeshQuery == nullptr){
							__h_c_DetourNavMeshQuery = env->FindClass("recast/DetourNavMeshQuery");
							__h_f_DetourNavMeshQuery_this = env->GetFieldID(__h_c_DetourNavMeshQuery , "_this", "J");
							__h_m_DetourNavMeshQuery_ctor = env->GetMethodID(__h_c_DetourNavMeshQuery ,"<init>", "()V");
						}
					}

static jclass __h_c_Detour;
static jfieldID __h_f_Detour_this;
static jmethodID __h_m_Detour_ctor;
static inline void cache__h_c_Detour( JNIEnv *env){
						if (__h_c_Detour == nullptr){
							__h_c_Detour = env->FindClass("recast/Detour");
							__h_f_Detour_this = env->GetFieldID(__h_c_Detour , "_this", "J");
							__h_m_Detour_ctor = env->GetMethodID(__h_c_Detour ,"<init>", "()V");
						}
					}

static jclass __h_c_PerformanceTimer;
static jfieldID __h_f_PerformanceTimer_this;
static jmethodID __h_m_PerformanceTimer_ctor;
static inline void cache__h_c_PerformanceTimer( JNIEnv *env){
						if (__h_c_PerformanceTimer == nullptr){
							__h_c_PerformanceTimer = env->FindClass("recast/PerformanceTimer");
							__h_f_PerformanceTimer_this = env->GetFieldID(__h_c_PerformanceTimer , "_this", "J");
							__h_m_PerformanceTimer_ctor = env->GetMethodID(__h_c_PerformanceTimer ,"<init>", "()V");
						}
					}

static jclass __h_c_Tests;
static jfieldID __h_f_Tests_this;
static jmethodID __h_m_Tests_ctor;
static inline void cache__h_c_Tests( JNIEnv *env){
						if (__h_c_Tests == nullptr){
							__h_c_Tests = env->FindClass("recast/Tests");
							__h_f_Tests_this = env->GetFieldID(__h_c_Tests , "_this", "J");
							__h_m_Tests_ctor = env->GetMethodID(__h_c_Tests ,"<init>", "()V");
						}
					}

static jclass __h_c_MeshCapture;
static jfieldID __h_f_MeshCapture_this;
static jmethodID __h_m_MeshCapture_ctor;
static inline void cache__h_c_MeshCapture( JNIEnv *env){
						if (__h_c_MeshCapture == nullptr){
							__h_c_MeshCapture = env->FindClass("recast/MeshCapture");
							__h_f_MeshCapture_this = env->GetFieldID(__h_c_MeshCapture , "_this", "J");
							__h_m_MeshCapture_ctor = env->GetMethodID(__h_c_MeshCapture ,"<init>", "()V");
						}
					}

static jclass __h_c_NavTileConverter;
static jfieldID __h_f_NavTileConverter_this;
static jmethodID __h_m_NavTileConverter_ctor;
static inline void cache__h_c_NavTileConverter( JNIEnv *env){
						if (__h_c_NavTileConverter == nullptr){
							__h_c_NavTileConverter = env->FindClass("recast/NavTileConverter");
							__h_f_NavTileConverter_this = env->GetFieldID(__h_c_NavTileConverter , "_this", "J");
							__h_m_NavTileConverter_ctor = env->GetMethodID(__h_c_NavTileConverter ,"<init>", "()V");
						}
					}

JNIEXPORT void JNICALL Java_recast_ChunkyTriMesh_nativeNew(JNIEnv *__env, jobject _obj/* ChunkyTriMesh */) {
	cache__h_c_ChunkyTriMesh(__env);
	auto *_this = (new rcChunkyTriMesh());
	__env->SetLongField(_obj, __h_f_ChunkyTriMesh_this, (long long)_this);
}

JNIEXPORT jint JNICALL Java_recast_ChunkyTriMesh_get_1maxTrisPerChunk( JNIEnv *__env, jobject _obj ) {
cache__h_c_ChunkyTriMesh(__env);
rcChunkyTriMesh *_this = (rcChunkyTriMesh*)__env->GetLongField(_obj, __h_f_ChunkyTriMesh_this);
	return _this->maxTrisPerChunk;
}
JNIEXPORT jint JNICALL Java_recast_ChunkyTriMesh_set_1maxTrisPerChunk(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_ChunkyTriMesh(__env);
rcChunkyTriMesh *_this = (rcChunkyTriMesh*)__env->GetLongField(_obj, __h_f_ChunkyTriMesh_this);
	_this->maxTrisPerChunk = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_ChunkyTriMesh_getNodeTriIndex(JNIEnv *__env, jobject _obj/* ChunkyTriMesh */, jint nodeIdx) {
	cache__h_c_ChunkyTriMesh(__env);
	rcChunkyTriMesh *_this = (rcChunkyTriMesh*)__env->GetLongField(_obj, __h_f_ChunkyTriMesh_this);
	return getNodeTriIndex( _this , nodeIdx);
}

JNIEXPORT jint JNICALL Java_recast_ChunkyTriMesh_getNodeTriCount(JNIEnv *__env, jobject _obj/* ChunkyTriMesh */, jint nodeIdx) {
	cache__h_c_ChunkyTriMesh(__env);
	rcChunkyTriMesh *_this = (rcChunkyTriMesh*)__env->GetLongField(_obj, __h_f_ChunkyTriMesh_this);
	return getNodeTriCount( _this , nodeIdx);
}

JNIEXPORT jboolean JNICALL Java_recast_ChunkyTriMesh_build(JNIEnv *__env, jobject _obj/* ChunkyTriMesh */, jfloatArray verts/* TFloat */, jintArray tris/* TInt */, jint ntris, jint trisPerChunk) {
	cache__h_c_ChunkyTriMesh(__env);
	rcChunkyTriMesh *_this = (rcChunkyTriMesh*)__env->GetLongField(_obj, __h_f_ChunkyTriMesh_this);
	return rcCreateChunkyTriMesh( _this , h_aptr</*CONST*/ jfloat>(verts), h_aptr</*CONST*/ jint>(tris), ntris, trisPerChunk);
}

JNIEXPORT jint JNICALL Java_recast_ChunkyTriMesh_getOverlappingRect(JNIEnv *__env, jobject _obj/* ChunkyTriMesh */, jobject bmin, jobject bmax, jintArray ids/* TInt */, jint maxIds) {
	cache__h_c_ChunkyTriMesh(__env);
	rcChunkyTriMesh *_this = (rcChunkyTriMesh*)__env->GetLongField(_obj, __h_f_ChunkyTriMesh_this);
	return rcGetChunksOverlappingRect( _this , (float*)(jobject)bmin, (float*)(jobject)bmax, h_aptr<jint>(ids), maxIds);
}

JNIEXPORT void JNICALL Java_recast_RasterContext_nativeNew(JNIEnv *__env, jobject _obj/* RasterContext */, jboolean state) {
	cache__h_c_RasterContext(__env);
	auto *_this = (new rcContext(state));
	__env->SetLongField(_obj, __h_f_RasterContext_this, (long long)_this);
}

JNIEXPORT void JNICALL Java_recast_RasterContext_enableLog(JNIEnv *__env, jobject _obj/* RasterContext */, jboolean state) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	_this->enableLog(state);
}

JNIEXPORT void JNICALL Java_recast_RasterContext_resetLog(JNIEnv *__env, jobject _obj/* RasterContext */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	_this->resetLog();
}

JNIEXPORT void JNICALL Java_recast_RasterContext_enableTimer(JNIEnv *__env, jobject _obj/* RasterContext */, jboolean state) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	_this->enableTimer(state);
}

JNIEXPORT void JNICALL Java_recast_RasterContext_resetTimers(JNIEnv *__env, jobject _obj/* RasterContext */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	_this->resetTimers();
}

JNIEXPORT void JNICALL Java_recast_RasterContext_startTimer(JNIEnv *__env, jobject _obj/* RasterContext */, int label/* TimerLabel */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_TimerLabel(__env);
	TimerLabel *_label = (TimerLabel*)__env->GetLongField(label, __h_f_TimerLabel_this);
_this->startTimer(TimerLabel__values[label]);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_createHeightfield(JNIEnv *__env, jobject _obj/* RasterContext */, jobject hf/* Heightfield */, jint width, jint height, jobject bmin, jobject bmax, jfloat cs, jfloat ch) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_Heightfield(__env);
	Heightfield *_hf = (Heightfield*)__env->GetLongField(hf, __h_f_Heightfield_this);
auto ___retvalue = rcCreateHeightfield( _this , *_hf, width, height, (float*)(jobject)bmin, (float*)(jobject)bmax, cs, ch);
	return (___retvalue);
}

JNIEXPORT void JNICALL Java_recast_RasterContext_markWalkableTriangles(JNIEnv *__env, jobject _obj/* RasterContext */, jfloat walkableSlopeAngle, jfloatArray verts/* TFloat */, jint nv, jobject tris, jint nt, jobject areas) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	rcMarkWalkableTriangles( _this , walkableSlopeAngle, h_aptr</*CONST*/ jfloat>(verts), nv, (jint*)tris, nt, (jcharr*)areas);
}

JNIEXPORT void JNICALL Java_recast_RasterContext_rcClearUnwalkableTriangles(JNIEnv *__env, jobject _obj/* RasterContext */, jfloat walkableSlopeAngle, jfloatArray verts/* TFloat */, jint nv, jobject tris, jint nt, jobject areas) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	rcClearUnwalkableTriangles( _this , walkableSlopeAngle, h_aptr</*CONST*/ jfloat>(verts), nv, (jint*)tris, nt, (jcharr*)areas);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_rcAddSpan(JNIEnv *__env, jobject _obj/* RasterContext */, jobject hf/* Heightfield */, jint x, jint y, jint smin, jint smax, jint area, jint flagMergeThr) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_Heightfield(__env);
	Heightfield *_hf = (Heightfield*)__env->GetLongField(hf, __h_f_Heightfield_this);
auto ___retvalue = rcAddSpan( _this , *_hf, x, y, smin, smax, area, flagMergeThr);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_rasterizeTriangle(JNIEnv *__env, jobject _obj/* RasterContext */, jobject v0, jobject v1, jobject v2, jint area, jobject solid/* Heightfield */, jint flagMergeThr) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_Heightfield(__env);
	Heightfield *_solid = (Heightfield*)__env->GetLongField(solid, __h_f_Heightfield_this);
auto ___retvalue = rcRasterizeTriangle( _this , (float*)(/*CONST*/ jobject)v0, (float*)(/*CONST*/ jobject)v1, (float*)(/*CONST*/ jobject)v2, area, *_solid, flagMergeThr);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_rasterizeTriangles(JNIEnv *__env, jobject _obj/* RasterContext */, jfloatArray verts/* TFloat */, jint nv, jobject tris, jobject areas, jint nt, jobject solid/* Heightfield */, jint flagMergeThr) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_Heightfield(__env);
	Heightfield *_solid = (Heightfield*)__env->GetLongField(solid, __h_f_Heightfield_this);
auto ___retvalue = rcRasterizeTriangles( _this , h_aptr</*CONST*/ jfloat>(verts), nv, (jint*)tris, (jcharr*)areas, nt, *_solid, flagMergeThr);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_rasterizeTriangles(JNIEnv *__env, jobject _obj/* RasterContext */, jfloatArray verts/* TFloat */, jcharArray areas/* TChar */, jint nt, jobject solid/* Heightfield */, jint flagMergeThr) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_Heightfield(__env);
	Heightfield *_solid = (Heightfield*)__env->GetLongField(solid, __h_f_Heightfield_this);
auto ___retvalue = rcRasterizeTriangles( _this , h_aptr</*CONST*/ jfloat>(verts), h_aptr</*CONST*/ jchar>(areas), nt, *_solid, flagMergeThr);
	return (___retvalue);
}

JNIEXPORT void JNICALL Java_recast_RasterContext_filterLowHangingWalkableObstacles(JNIEnv *__env, jobject _obj/* RasterContext */, jint walkableClimb, jobject solid/* Heightfield */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_Heightfield(__env);
	Heightfield *_solid = (Heightfield*)__env->GetLongField(solid, __h_f_Heightfield_this);
rcFilterLowHangingWalkableObstacles( _this , walkableClimb, *_solid);
}

JNIEXPORT void JNICALL Java_recast_RasterContext_filterLedgeSpans(JNIEnv *__env, jobject _obj/* RasterContext */, jint walkableHeight, jint walkableClimb, jobject solid/* Heightfield */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_Heightfield(__env);
	Heightfield *_solid = (Heightfield*)__env->GetLongField(solid, __h_f_Heightfield_this);
rcFilterLedgeSpans( _this , walkableHeight, walkableClimb, *_solid);
}

JNIEXPORT void JNICALL Java_recast_RasterContext_filterWalkableLowHeightSpans(JNIEnv *__env, jobject _obj/* RasterContext */, jint walkableHeight, jobject solid/* Heightfield */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_Heightfield(__env);
	Heightfield *_solid = (Heightfield*)__env->GetLongField(solid, __h_f_Heightfield_this);
rcFilterWalkableLowHeightSpans( _this , walkableHeight, *_solid);
}

JNIEXPORT jint JNICALL Java_recast_RasterContext_rcGetHeightFieldSpanCount(JNIEnv *__env, jobject _obj/* RasterContext */, jobject hf/* Heightfield */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_Heightfield(__env);
	Heightfield *_hf = (Heightfield*)__env->GetLongField(hf, __h_f_Heightfield_this);
auto ___retvalue = rcGetHeightFieldSpanCount( _this , *_hf);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_buildCompactHeightfield(JNIEnv *__env, jobject _obj/* RasterContext */, jint walkableHeight, jint walkableClimb, jobject hf/* Heightfield */, jobject chf/* CompactHeightfield */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_Heightfield(__env);
	Heightfield *_hf = (Heightfield*)__env->GetLongField(hf, __h_f_Heightfield_this);
cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
auto ___retvalue = rcBuildCompactHeightfield( _this , walkableHeight, walkableClimb, *_hf, *_chf);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_erodeWalkableArea(JNIEnv *__env, jobject _obj/* RasterContext */, jint radius, jobject chf/* CompactHeightfield */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
auto ___retvalue = rcErodeWalkableArea( _this , radius, *_chf);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_rcMedianFilterWalkableArea(JNIEnv *__env, jobject _obj/* RasterContext */, jobject chf/* CompactHeightfield */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
auto ___retvalue = rcMedianFilterWalkableArea( _this , *_chf);
	return (___retvalue);
}

JNIEXPORT void JNICALL Java_recast_RasterContext_rcMarkBoxArea(JNIEnv *__env, jobject _obj/* RasterContext */, jobject bmin, jobject bmax, jchar areaId, jobject chf/* CompactHeightfield */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
rcMarkBoxArea( _this , (float*)(jobject)bmin, (float*)(jobject)bmax, areaId, *_chf);
}

JNIEXPORT void JNICALL Java_recast_RasterContext_rcMarkConvexPolyArea(JNIEnv *__env, jobject _obj/* RasterContext */, jobject verts, jint nverts, jfloat hmin, jfloat hmax, jchar areaId, jobject chf/* CompactHeightfield */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
rcMarkConvexPolyArea( _this , (float*)(jobject)verts, nverts, hmin, hmax, areaId, *_chf);
}

JNIEXPORT void JNICALL Java_recast_RasterContext_rcMarkCylinderArea(JNIEnv *__env, jobject _obj/* RasterContext */, jobject pos, jfloat r, jfloat h, jchar areaId, jobject chf/* CompactHeightfield */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
rcMarkCylinderArea( _this , (float*)(jobject)pos, r, h, areaId, *_chf);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_rcBuildDistanceField(JNIEnv *__env, jobject _obj/* RasterContext */, jobject chf/* CompactHeightfield */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
auto ___retvalue = rcBuildDistanceField( _this , *_chf);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_rcBuildRegions(JNIEnv *__env, jobject _obj/* RasterContext */, jobject chf/* CompactHeightfield */, jint borderSize, jint minRegionArea, jint mergeRegionArea) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
auto ___retvalue = rcBuildRegions( _this , *_chf, borderSize, minRegionArea, mergeRegionArea);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_rcBuildLayerRegions(JNIEnv *__env, jobject _obj/* RasterContext */, jobject chf/* CompactHeightfield */, jint borderSize, jint minRegionArea) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
auto ___retvalue = rcBuildLayerRegions( _this , *_chf, borderSize, minRegionArea);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_rcBuildRegionsMonotone(JNIEnv *__env, jobject _obj/* RasterContext */, jobject chf/* CompactHeightfield */, jint borderSize, jint minRegionArea, jint mergeRegionArea) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
auto ___retvalue = rcBuildRegionsMonotone( _this , *_chf, borderSize, minRegionArea, mergeRegionArea);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_rcBuildContours(JNIEnv *__env, jobject _obj/* RasterContext */, jobject chf/* CompactHeightfield */, jfloat maxError, jint maxEdgeLen, jobject cset/* rcContourSet */, jint buildFlags) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
cache__h_c_rcContourSet(__env);
	rcContourSet *_cset = (rcContourSet*)__env->GetLongField(cset, __h_f_rcContourSet_this);
auto ___retvalue = rcBuildContours( _this , *_chf, maxError, maxEdgeLen, *_cset, buildFlags);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_rcBuildPolyMesh(JNIEnv *__env, jobject _obj/* RasterContext */, jobject cset/* rcContourSet */, jint nvp, jobject mesh/* PolyMesh */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_rcContourSet(__env);
	rcContourSet *_cset = (rcContourSet*)__env->GetLongField(cset, __h_f_rcContourSet_this);
cache__h_c_PolyMesh(__env);
	PolyMesh *_mesh = (PolyMesh*)__env->GetLongField(mesh, __h_f_PolyMesh_this);
auto ___retvalue = rcBuildPolyMesh( _this , *_cset, nvp, *_mesh);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_rcBuildPolyMeshDetail(JNIEnv *__env, jobject _obj/* RasterContext */, jobject mesh/* PolyMesh */, jobject chf/* CompactHeightfield */, jfloat sampleDist, jfloat sampleMaxError, jobject dmesh/* PolyMeshDetail */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_PolyMesh(__env);
	PolyMesh *_mesh = (PolyMesh*)__env->GetLongField(mesh, __h_f_PolyMesh_this);
cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
cache__h_c_PolyMeshDetail(__env);
	PolyMeshDetail *_dmesh = (PolyMeshDetail*)__env->GetLongField(dmesh, __h_f_PolyMeshDetail_this);
auto ___retvalue = rcBuildPolyMeshDetail( _this , *_mesh, *_chf, sampleDist, sampleMaxError, *_dmesh);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_RasterContext_buildHeightfieldLayers(JNIEnv *__env, jobject _obj/* RasterContext */, jobject chf/* CompactHeightfield */, jint borderSize, jint walkableHeight, jobject lset/* HeightfieldLayerSet */) {
	cache__h_c_RasterContext(__env);
	rcContext *_this = (rcContext*)__env->GetLongField(_obj, __h_f_RasterContext_this);
	cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
cache__h_c_HeightfieldLayerSet(__env);
	HeightfieldLayerSet *_lset = (HeightfieldLayerSet*)__env->GetLongField(lset, __h_f_HeightfieldLayerSet_this);
auto ___retvalue = rcBuildHeightfieldLayers( _this , *_chf, borderSize, walkableHeight, *_lset);
	return (___retvalue);
}

JNIEXPORT void JNICALL Java_recast_rcScopedTimer_nativeNew(JNIEnv *__env, jobject _obj/* rcScopedTimer */, jobject ctx/* RasterContext */, int label/* TimerLabel */) {
	cache__h_c_rcScopedTimer(__env);
	cache__h_c_RasterContext(__env);
	RasterContext *_ctx = (RasterContext*)__env->GetLongField(ctx, __h_f_RasterContext_this);
cache__h_c_TimerLabel(__env);
	TimerLabel *_label = (TimerLabel*)__env->GetLongField(label, __h_f_TimerLabel_this);
auto ___retvalue = (new rcScopedTimer(_ctx, TimerLabel__values[label]));
	__env->SetLongField(_obj, __h_f_rcScopedTimer_this, (long long)_this);
}

JNIEXPORT void JNICALL Java_recast_RasterConfig_nativeNew(JNIEnv *__env, jobject _obj/* RasterConfig */) {
	cache__h_c_RasterConfig(__env);
	auto *_this = (new rcConfig());
	__env->SetLongField(_obj, __h_f_RasterConfig_this, (long long)_this);
}

JNIEXPORT void JNICALL Java_recast_RasterConfig_copy(JNIEnv *__env, jobject _obj/* RasterConfig */, jobject b/* RasterConfig */) {
	cache__h_c_RasterConfig(__env);
	rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	cache__h_c_RasterConfig(__env);
	RasterConfig *_b = (RasterConfig*)__env->GetLongField(b, __h_f_RasterConfig_this);
rcConfigCopy( _this , _b);
}

JNIEXPORT jint JNICALL Java_recast_RasterConfig_get_1width( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->width;
}
JNIEXPORT jint JNICALL Java_recast_RasterConfig_set_1width(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->width = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_RasterConfig_get_1height( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->height;
}
JNIEXPORT jint JNICALL Java_recast_RasterConfig_set_1height(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->height = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_RasterConfig_get_1tileSize( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->tileSize;
}
JNIEXPORT jint JNICALL Java_recast_RasterConfig_set_1tileSize(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->tileSize = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_RasterConfig_get_1borderSize( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->borderSize;
}
JNIEXPORT jint JNICALL Java_recast_RasterConfig_set_1borderSize(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->borderSize = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_RasterConfig_get_1cs( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->cs;
}
JNIEXPORT jfloat JNICALL Java_recast_RasterConfig_set_1cs(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->cs = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_RasterConfig_get_1ch( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->ch;
}
JNIEXPORT jfloat JNICALL Java_recast_RasterConfig_set_1ch(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->ch = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_RasterConfig_get_1bmin( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return (jobject )(JNIEnv *__env, _this->bmin);
}
JNIEXPORT void JNICALL RasterConfig_getbminv( JNIEnv *__env, rcConfig _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmin)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_RasterConfig_set_1bmin(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	 jfloat *dst = (jfloat*) & (_this->bmin)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL RasterConfig_setbmin3(JNIEnv *__env, rcConfig _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jobject JNICALL Java_recast_RasterConfig_get_1bmax( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return (jobject )(JNIEnv *__env, _this->bmax);
}
JNIEXPORT void JNICALL RasterConfig_getbmaxv( JNIEnv *__env, rcConfig _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmax)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_RasterConfig_set_1bmax(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	 jfloat *dst = (jfloat*) & (_this->bmax)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL RasterConfig_setbmax3(JNIEnv *__env, rcConfig _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jfloat JNICALL Java_recast_RasterConfig_get_1walkableSlopeAngle( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->walkableSlopeAngle;
}
JNIEXPORT jfloat JNICALL Java_recast_RasterConfig_set_1walkableSlopeAngle(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->walkableSlopeAngle = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_RasterConfig_get_1walkableHeight( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->walkableHeight;
}
JNIEXPORT jint JNICALL Java_recast_RasterConfig_set_1walkableHeight(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->walkableHeight = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_RasterConfig_get_1walkableClimb( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->walkableClimb;
}
JNIEXPORT jint JNICALL Java_recast_RasterConfig_set_1walkableClimb(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->walkableClimb = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_RasterConfig_get_1walkableRadius( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->walkableRadius;
}
JNIEXPORT jint JNICALL Java_recast_RasterConfig_set_1walkableRadius(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->walkableRadius = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_RasterConfig_get_1maxEdgeLen( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->maxEdgeLen;
}
JNIEXPORT jint JNICALL Java_recast_RasterConfig_set_1maxEdgeLen(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->maxEdgeLen = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_RasterConfig_get_1maxSimplificationError( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->maxSimplificationError;
}
JNIEXPORT jfloat JNICALL Java_recast_RasterConfig_set_1maxSimplificationError(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->maxSimplificationError = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_RasterConfig_get_1minRegionArea( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->minRegionArea;
}
JNIEXPORT jint JNICALL Java_recast_RasterConfig_set_1minRegionArea(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->minRegionArea = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_RasterConfig_get_1mergeRegionArea( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->mergeRegionArea;
}
JNIEXPORT jint JNICALL Java_recast_RasterConfig_set_1mergeRegionArea(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->mergeRegionArea = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_RasterConfig_get_1maxVertsPerPoly( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->maxVertsPerPoly;
}
JNIEXPORT jint JNICALL Java_recast_RasterConfig_set_1maxVertsPerPoly(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->maxVertsPerPoly = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_RasterConfig_get_1detailSampleDist( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->detailSampleDist;
}
JNIEXPORT jfloat JNICALL Java_recast_RasterConfig_set_1detailSampleDist(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->detailSampleDist = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_RasterConfig_get_1detailSampleMaxError( JNIEnv *__env, jobject _obj ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	return _this->detailSampleMaxError;
}
JNIEXPORT jfloat JNICALL Java_recast_RasterConfig_set_1detailSampleMaxError(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_RasterConfig(__env);
rcConfig *_this = (rcConfig*)__env->GetLongField(_obj, __h_f_RasterConfig_this);
	_this->detailSampleMaxError = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_Span_get_1smin( JNIEnv *__env, jobject _obj ) {
cache__h_c_Span(__env);
rcSpan *_this = (rcSpan*)__env->GetLongField(_obj, __h_f_Span_this);
	return _this->smin;
}
JNIEXPORT jint JNICALL Java_recast_Span_set_1smin(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_Span(__env);
rcSpan *_this = (rcSpan*)__env->GetLongField(_obj, __h_f_Span_this);
	_this->smin = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_Span_get_1smax( JNIEnv *__env, jobject _obj ) {
cache__h_c_Span(__env);
rcSpan *_this = (rcSpan*)__env->GetLongField(_obj, __h_f_Span_this);
	return _this->smax;
}
JNIEXPORT jint JNICALL Java_recast_Span_set_1smax(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_Span(__env);
rcSpan *_this = (rcSpan*)__env->GetLongField(_obj, __h_f_Span_this);
	_this->smax = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_Span_get_1area( JNIEnv *__env, jobject _obj ) {
cache__h_c_Span(__env);
rcSpan *_this = (rcSpan*)__env->GetLongField(_obj, __h_f_Span_this);
	return _this->area;
}
JNIEXPORT jint JNICALL Java_recast_Span_set_1area(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_Span(__env);
rcSpan *_this = (rcSpan*)__env->GetLongField(_obj, __h_f_Span_this);
	_this->area = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_Span_get_1next( JNIEnv *__env, jobject _obj ) {
cache__h_c_Span(__env);
rcSpan *_this = (rcSpan*)__env->GetLongField(_obj, __h_f_Span_this);
	return alloc_ref(_this->next,Span);
}
JNIEXPORT jobject JNICALL Java_recast_Span_set_1next(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_Span(__env);
rcSpan *_this = (rcSpan*)__env->GetLongField(_obj, __h_f_Span_this);
	_this->next = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_SpanPool_get_1next( JNIEnv *__env, jobject _obj ) {
cache__h_c_SpanPool(__env);
rcSpanPool *_this = (rcSpanPool*)__env->GetLongField(_obj, __h_f_SpanPool_this);
	return alloc_ref(_this->next,SpanPool);
}
JNIEXPORT jobject JNICALL Java_recast_SpanPool_set_1next(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_SpanPool(__env);
rcSpanPool *_this = (rcSpanPool*)__env->GetLongField(_obj, __h_f_SpanPool_this);
	_this->next = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_Heightfield_nativeNew(JNIEnv *__env, jobject _obj/* Heightfield */) {
	cache__h_c_Heightfield(__env);
	auto *_this = (new rcHeightfield());
	__env->SetLongField(_obj, __h_f_Heightfield_this, (long long)_this);
}

JNIEXPORT jint JNICALL Java_recast_Heightfield_get_1width( JNIEnv *__env, jobject _obj ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	return _this->width;
}
JNIEXPORT jint JNICALL Java_recast_Heightfield_set_1width(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	_this->width = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_Heightfield_get_1height( JNIEnv *__env, jobject _obj ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	return _this->height;
}
JNIEXPORT jint JNICALL Java_recast_Heightfield_set_1height(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	_this->height = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_Heightfield_get_1bmin( JNIEnv *__env, jobject _obj ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	return (jobject )(JNIEnv *__env, _this->bmin);
}
JNIEXPORT void JNICALL Heightfield_getbminv( JNIEnv *__env, rcHeightfield _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmin)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_Heightfield_set_1bmin(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	 jfloat *dst = (jfloat*) & (_this->bmin)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL Heightfield_setbmin3(JNIEnv *__env, rcHeightfield _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jobject JNICALL Java_recast_Heightfield_get_1bmax( JNIEnv *__env, jobject _obj ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	return (jobject )(JNIEnv *__env, _this->bmax);
}
JNIEXPORT void JNICALL Heightfield_getbmaxv( JNIEnv *__env, rcHeightfield _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmax)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_Heightfield_set_1bmax(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	 jfloat *dst = (jfloat*) & (_this->bmax)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL Heightfield_setbmax3(JNIEnv *__env, rcHeightfield _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jfloat JNICALL Java_recast_Heightfield_get_1cs( JNIEnv *__env, jobject _obj ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	return _this->cs;
}
JNIEXPORT jfloat JNICALL Java_recast_Heightfield_set_1cs(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	_this->cs = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_Heightfield_get_1ch( JNIEnv *__env, jobject _obj ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	return _this->ch;
}
JNIEXPORT jfloat JNICALL Java_recast_Heightfield_set_1ch(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	_this->ch = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_Heightfield_get_1pools( JNIEnv *__env, jobject _obj ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	return alloc_ref(_this->pools,SpanPool);
}
JNIEXPORT jobject JNICALL Java_recast_Heightfield_set_1pools(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	_this->pools = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_Heightfield_get_1freelist( JNIEnv *__env, jobject _obj ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	return alloc_ref(_this->freelist,Span);
}
JNIEXPORT jobject JNICALL Java_recast_Heightfield_set_1freelist(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_Heightfield(__env);
rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	_this->freelist = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT jboolean JNICALL Java_recast_Heightfield_spanIsValidAt(JNIEnv *__env, jobject _obj/* Heightfield */, jint pos) {
	cache__h_c_Heightfield(__env);
	rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	return rcHeightfield_rcSpanIsValidAt( _this , pos);
}

JNIEXPORT jobject JNICALL Java_recast_Heightfield_spanAt(JNIEnv *__env, jobject _obj/* Heightfield */, jint pos) {
	cache__h_c_Heightfield(__env);
	rcHeightfield *_this = (rcHeightfield*)__env->GetLongField(_obj, __h_f_Heightfield_this);
	auto ___retvalue = (rcHeightfield_rcSpanAt( _this , pos));
cache__h_c_Span(__env);
	auto _new_obj = __env->NewObject( __h_c_Span, __h_m_Span_ctor);
	__env->SetLongField(_new_obj, __h_f_Span_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT jint JNICALL Java_recast_rcCompactCell_get_1index( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcCompactCell(__env);
rcCompactCell *_this = (rcCompactCell*)__env->GetLongField(_obj, __h_f_rcCompactCell_this);
	return _this->index;
}
JNIEXPORT jint JNICALL Java_recast_rcCompactCell_set_1index(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_rcCompactCell(__env);
rcCompactCell *_this = (rcCompactCell*)__env->GetLongField(_obj, __h_f_rcCompactCell_this);
	_this->index = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_rcCompactCell_get_1count( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcCompactCell(__env);
rcCompactCell *_this = (rcCompactCell*)__env->GetLongField(_obj, __h_f_rcCompactCell_this);
	return _this->count;
}
JNIEXPORT jint JNICALL Java_recast_rcCompactCell_set_1count(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_rcCompactCell(__env);
rcCompactCell *_this = (rcCompactCell*)__env->GetLongField(_obj, __h_f_rcCompactCell_this);
	_this->count = (value);
	return value;
}

JNIEXPORT jshort JNICALL Java_recast_rcCompactSpan_get_1y( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcCompactSpan(__env);
rcCompactSpan *_this = (rcCompactSpan*)__env->GetLongField(_obj, __h_f_rcCompactSpan_this);
	return _this->y;
}
JNIEXPORT jshort JNICALL Java_recast_rcCompactSpan_set_1y(JNIEnv *__env, jobject _obj, jshort value ) {
cache__h_c_rcCompactSpan(__env);
rcCompactSpan *_this = (rcCompactSpan*)__env->GetLongField(_obj, __h_f_rcCompactSpan_this);
	_this->y = (value);
	return value;
}

JNIEXPORT jshort JNICALL Java_recast_rcCompactSpan_get_1reg( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcCompactSpan(__env);
rcCompactSpan *_this = (rcCompactSpan*)__env->GetLongField(_obj, __h_f_rcCompactSpan_this);
	return _this->reg;
}
JNIEXPORT jshort JNICALL Java_recast_rcCompactSpan_set_1reg(JNIEnv *__env, jobject _obj, jshort value ) {
cache__h_c_rcCompactSpan(__env);
rcCompactSpan *_this = (rcCompactSpan*)__env->GetLongField(_obj, __h_f_rcCompactSpan_this);
	_this->reg = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_rcCompactSpan_get_1con( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcCompactSpan(__env);
rcCompactSpan *_this = (rcCompactSpan*)__env->GetLongField(_obj, __h_f_rcCompactSpan_this);
	return _this->con;
}
JNIEXPORT jint JNICALL Java_recast_rcCompactSpan_set_1con(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_rcCompactSpan(__env);
rcCompactSpan *_this = (rcCompactSpan*)__env->GetLongField(_obj, __h_f_rcCompactSpan_this);
	_this->con = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_rcCompactSpan_get_1h( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcCompactSpan(__env);
rcCompactSpan *_this = (rcCompactSpan*)__env->GetLongField(_obj, __h_f_rcCompactSpan_this);
	return _this->h;
}
JNIEXPORT jint JNICALL Java_recast_rcCompactSpan_set_1h(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_rcCompactSpan(__env);
rcCompactSpan *_this = (rcCompactSpan*)__env->GetLongField(_obj, __h_f_rcCompactSpan_this);
	_this->h = (value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_CompactHeightfield_nativeNew(JNIEnv *__env, jobject _obj/* CompactHeightfield */) {
	cache__h_c_CompactHeightfield(__env);
	auto *_this = (new rcCompactHeightfield());
	__env->SetLongField(_obj, __h_f_CompactHeightfield_this, (long long)_this);
}

JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_get_1width( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return _this->width;
}
JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_set_1width(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->width = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_get_1height( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return _this->height;
}
JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_set_1height(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->height = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_get_1spanCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return _this->spanCount;
}
JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_set_1spanCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->spanCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_get_1walkableHeight( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return _this->walkableHeight;
}
JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_set_1walkableHeight(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->walkableHeight = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_get_1walkableClimb( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return _this->walkableClimb;
}
JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_set_1walkableClimb(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->walkableClimb = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_get_1borderSize( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return _this->borderSize;
}
JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_set_1borderSize(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->borderSize = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_get_1maxDistance( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return _this->maxDistance;
}
JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_set_1maxDistance(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->maxDistance = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_get_1maxRegions( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return _this->maxRegions;
}
JNIEXPORT jint JNICALL Java_recast_CompactHeightfield_set_1maxRegions(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->maxRegions = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_CompactHeightfield_get_1bmin( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return (jobject )(JNIEnv *__env, _this->bmin);
}
JNIEXPORT void JNICALL CompactHeightfield_getbminv( JNIEnv *__env, rcCompactHeightfield _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmin)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_CompactHeightfield_set_1bmin(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	 jfloat *dst = (jfloat*) & (_this->bmin)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL CompactHeightfield_setbmin3(JNIEnv *__env, rcCompactHeightfield _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jobject JNICALL Java_recast_CompactHeightfield_get_1bmax( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return (jobject )(JNIEnv *__env, _this->bmax);
}
JNIEXPORT void JNICALL CompactHeightfield_getbmaxv( JNIEnv *__env, rcCompactHeightfield _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmax)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_CompactHeightfield_set_1bmax(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	 jfloat *dst = (jfloat*) & (_this->bmax)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL CompactHeightfield_setbmax3(JNIEnv *__env, rcCompactHeightfield _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jfloat JNICALL Java_recast_CompactHeightfield_get_1cs( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return _this->cs;
}
JNIEXPORT jfloat JNICALL Java_recast_CompactHeightfield_set_1cs(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->cs = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_CompactHeightfield_get_1ch( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return _this->ch;
}
JNIEXPORT jfloat JNICALL Java_recast_CompactHeightfield_set_1ch(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->ch = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_CompactHeightfield_get_1cells( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return alloc_ref(_this->cells,rcCompactCell);
}
JNIEXPORT jobject JNICALL Java_recast_CompactHeightfield_set_1cells(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->cells = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_CompactHeightfield_get_1spans( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return alloc_ref(_this->spans,rcCompactSpan);
}
JNIEXPORT jobject JNICALL Java_recast_CompactHeightfield_set_1spans(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->spans = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_CompactHeightfield_get_1dist( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return _returnPointer(&_this->dist[0]);
}
JNIEXPORT jobject JNICALL Java_recast_CompactHeightfield_set_1dist(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->dist = (jshort*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_CompactHeightfield_get_1areas( JNIEnv *__env, jobject _obj ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	return _returnPointer(&_this->areas[0]);
}
JNIEXPORT jobject JNICALL Java_recast_CompactHeightfield_set_1areas(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_CompactHeightfield(__env);
rcCompactHeightfield *_this = (rcCompactHeightfield*)__env->GetLongField(_obj, __h_f_CompactHeightfield_this);
	_this->areas = (jchar*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_HeightfieldLayer_get_1bmin( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return (jobject )(JNIEnv *__env, _this->bmin);
}
JNIEXPORT void JNICALL HeightfieldLayer_getbminv( JNIEnv *__env, rcHeightfieldLayer _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmin)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_HeightfieldLayer_set_1bmin(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	 jfloat *dst = (jfloat*) & (_this->bmin)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL HeightfieldLayer_setbmin3(JNIEnv *__env, rcHeightfieldLayer _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jobject JNICALL Java_recast_HeightfieldLayer_get_1bmax( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return (jobject )(JNIEnv *__env, _this->bmax);
}
JNIEXPORT void JNICALL HeightfieldLayer_getbmaxv( JNIEnv *__env, rcHeightfieldLayer _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmax)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_HeightfieldLayer_set_1bmax(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	 jfloat *dst = (jfloat*) & (_this->bmax)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL HeightfieldLayer_setbmax3(JNIEnv *__env, rcHeightfieldLayer _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jfloat JNICALL Java_recast_HeightfieldLayer_get_1cs( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _this->cs;
}
JNIEXPORT jfloat JNICALL Java_recast_HeightfieldLayer_set_1cs(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->cs = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_HeightfieldLayer_get_1ch( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _this->ch;
}
JNIEXPORT jfloat JNICALL Java_recast_HeightfieldLayer_set_1ch(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->ch = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_get_1width( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _this->width;
}
JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_set_1width(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->width = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_get_1height( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _this->height;
}
JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_set_1height(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->height = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_get_1minx( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _this->minx;
}
JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_set_1minx(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->minx = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_get_1maxx( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _this->maxx;
}
JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_set_1maxx(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->maxx = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_get_1miny( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _this->miny;
}
JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_set_1miny(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->miny = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_get_1maxy( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _this->maxy;
}
JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_set_1maxy(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->maxy = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_get_1hmin( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _this->hmin;
}
JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_set_1hmin(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->hmin = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_get_1hmax( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _this->hmax;
}
JNIEXPORT jint JNICALL Java_recast_HeightfieldLayer_set_1hmax(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->hmax = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_HeightfieldLayer_get_1heights( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _returnPointer(&_this->heights[0]);
}
JNIEXPORT jobject JNICALL Java_recast_HeightfieldLayer_set_1heights(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->heights = (jchar*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_HeightfieldLayer_get_1areas( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _returnPointer(&_this->areas[0]);
}
JNIEXPORT jobject JNICALL Java_recast_HeightfieldLayer_set_1areas(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->areas = (jchar*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_HeightfieldLayer_get_1cons( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	return _returnPointer(&_this->cons[0]);
}
JNIEXPORT jobject JNICALL Java_recast_HeightfieldLayer_set_1cons(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_HeightfieldLayer(__env);
rcHeightfieldLayer *_this = (rcHeightfieldLayer*)__env->GetLongField(_obj, __h_f_HeightfieldLayer_this);
	_this->cons = (jchar*(value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_HeightfieldLayerSet_nativeNew(JNIEnv *__env, jobject _obj/* HeightfieldLayerSet */) {
	cache__h_c_HeightfieldLayerSet(__env);
	auto *_this = (new rcHeightfieldLayerSet());
	__env->SetLongField(_obj, __h_f_HeightfieldLayerSet_this, (long long)_this);
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_HeightfieldLayerSet_layers(JNIEnv *__env, jobject _obj/* HeightfieldLayerSet */, jint idx) {
	cache__h_c_HeightfieldLayerSet(__env);
	rcHeightfieldLayerSet *_this = (rcHeightfieldLayerSet*)__env->GetLongField(_obj, __h_f_HeightfieldLayerSet_this);
	auto ___retvalue = &(_this->layers[idx]);
cache__h_c_HeightfieldLayer(__env);
	auto _new_obj = __env->NewObject( __h_c_HeightfieldLayer, __h_m_HeightfieldLayer_ctor);
	__env->SetLongField(_new_obj, __h_f_HeightfieldLayer_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT jint JNICALL Java_recast_HeightfieldLayerSet_get_1nlayers( JNIEnv *__env, jobject _obj ) {
cache__h_c_HeightfieldLayerSet(__env);
rcHeightfieldLayerSet *_this = (rcHeightfieldLayerSet*)__env->GetLongField(_obj, __h_f_HeightfieldLayerSet_this);
	return _this->nlayers;
}
JNIEXPORT jint JNICALL Java_recast_HeightfieldLayerSet_set_1nlayers(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_HeightfieldLayerSet(__env);
rcHeightfieldLayerSet *_this = (rcHeightfieldLayerSet*)__env->GetLongField(_obj, __h_f_HeightfieldLayerSet_this);
	_this->nlayers = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_rcContour_get_1verts( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContour(__env);
rcContour *_this = (rcContour*)__env->GetLongField(_obj, __h_f_rcContour_this);
	return _returnPointer(&_this->verts[0]);
}
JNIEXPORT jobject JNICALL Java_recast_rcContour_set_1verts(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_rcContour(__env);
rcContour *_this = (rcContour*)__env->GetLongField(_obj, __h_f_rcContour_this);
	_this->verts = (jint*(value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_rcContour_get_1nverts( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContour(__env);
rcContour *_this = (rcContour*)__env->GetLongField(_obj, __h_f_rcContour_this);
	return _this->nverts;
}
JNIEXPORT jint JNICALL Java_recast_rcContour_set_1nverts(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_rcContour(__env);
rcContour *_this = (rcContour*)__env->GetLongField(_obj, __h_f_rcContour_this);
	_this->nverts = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_rcContour_get_1rverts( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContour(__env);
rcContour *_this = (rcContour*)__env->GetLongField(_obj, __h_f_rcContour_this);
	return _returnPointer(&_this->rverts[0]);
}
JNIEXPORT jobject JNICALL Java_recast_rcContour_set_1rverts(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_rcContour(__env);
rcContour *_this = (rcContour*)__env->GetLongField(_obj, __h_f_rcContour_this);
	_this->rverts = (jint*(value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_rcContour_get_1nrverts( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContour(__env);
rcContour *_this = (rcContour*)__env->GetLongField(_obj, __h_f_rcContour_this);
	return _this->nrverts;
}
JNIEXPORT jint JNICALL Java_recast_rcContour_set_1nrverts(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_rcContour(__env);
rcContour *_this = (rcContour*)__env->GetLongField(_obj, __h_f_rcContour_this);
	_this->nrverts = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_rcContour_get_1reg( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContour(__env);
rcContour *_this = (rcContour*)__env->GetLongField(_obj, __h_f_rcContour_this);
	return _this->reg;
}
JNIEXPORT jint JNICALL Java_recast_rcContour_set_1reg(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_rcContour(__env);
rcContour *_this = (rcContour*)__env->GetLongField(_obj, __h_f_rcContour_this);
	_this->reg = (value);
	return value;
}

JNIEXPORT jchar JNICALL Java_recast_rcContour_get_1area( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContour(__env);
rcContour *_this = (rcContour*)__env->GetLongField(_obj, __h_f_rcContour_this);
	return _this->area;
}
JNIEXPORT jchar JNICALL Java_recast_rcContour_set_1area(JNIEnv *__env, jobject _obj, jchar value ) {
cache__h_c_rcContour(__env);
rcContour *_this = (rcContour*)__env->GetLongField(_obj, __h_f_rcContour_this);
	_this->area = (value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_rcContourSet_nativeNew(JNIEnv *__env, jobject _obj/* rcContourSet */) {
	cache__h_c_rcContourSet(__env);
	auto *_this = (new rcContourSet());
	__env->SetLongField(_obj, __h_f_rcContourSet_this, (long long)_this);
}

JNIEXPORT jobject JNICALL Java_recast_rcContourSet_get_1conts( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	return alloc_ref(_this->conts,rcContour);
}
JNIEXPORT jobject JNICALL Java_recast_rcContourSet_set_1conts(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	_this->conts = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_rcContourSet_get_1nconts( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	return _this->nconts;
}
JNIEXPORT jint JNICALL Java_recast_rcContourSet_set_1nconts(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	_this->nconts = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_rcContourSet_get_1bmin( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	return (jobject )(JNIEnv *__env, _this->bmin);
}
JNIEXPORT void JNICALL rcContourSet_getbminv( JNIEnv *__env, rcContourSet _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmin)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_rcContourSet_set_1bmin(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	 jfloat *dst = (jfloat*) & (_this->bmin)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL rcContourSet_setbmin3(JNIEnv *__env, rcContourSet _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jobject JNICALL Java_recast_rcContourSet_get_1bmax( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	return (jobject )(JNIEnv *__env, _this->bmax);
}
JNIEXPORT void JNICALL rcContourSet_getbmaxv( JNIEnv *__env, rcContourSet _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmax)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_rcContourSet_set_1bmax(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	 jfloat *dst = (jfloat*) & (_this->bmax)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL rcContourSet_setbmax3(JNIEnv *__env, rcContourSet _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jfloat JNICALL Java_recast_rcContourSet_get_1cs( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	return _this->cs;
}
JNIEXPORT jfloat JNICALL Java_recast_rcContourSet_set_1cs(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	_this->cs = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_rcContourSet_get_1ch( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	return _this->ch;
}
JNIEXPORT jfloat JNICALL Java_recast_rcContourSet_set_1ch(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	_this->ch = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_rcContourSet_get_1width( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	return _this->width;
}
JNIEXPORT jint JNICALL Java_recast_rcContourSet_set_1width(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	_this->width = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_rcContourSet_get_1height( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	return _this->height;
}
JNIEXPORT jint JNICALL Java_recast_rcContourSet_set_1height(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	_this->height = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_rcContourSet_get_1borderSize( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	return _this->borderSize;
}
JNIEXPORT jint JNICALL Java_recast_rcContourSet_set_1borderSize(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	_this->borderSize = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_rcContourSet_get_1maxError( JNIEnv *__env, jobject _obj ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	return _this->maxError;
}
JNIEXPORT jfloat JNICALL Java_recast_rcContourSet_set_1maxError(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_rcContourSet(__env);
rcContourSet *_this = (rcContourSet*)__env->GetLongField(_obj, __h_f_rcContourSet_this);
	_this->maxError = (value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_PolyMesh_nativeNew(JNIEnv *__env, jobject _obj/* PolyMesh */) {
	cache__h_c_PolyMesh(__env);
	auto *_this = (new rcPolyMesh());
	__env->SetLongField(_obj, __h_f_PolyMesh_this, (long long)_this);
}

JNIEXPORT jobject JNICALL Java_recast_PolyMesh_get_1verts( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _returnPointer(&_this->verts[0]);
}
JNIEXPORT jobject JNICALL Java_recast_PolyMesh_set_1verts(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->verts = (jshort*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_PolyMesh_get_1polys( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _returnPointer(&_this->polys[0]);
}
JNIEXPORT jobject JNICALL Java_recast_PolyMesh_set_1polys(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->polys = (jshort*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_PolyMesh_get_1regs( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _returnPointer(&_this->regs[0]);
}
JNIEXPORT jobject JNICALL Java_recast_PolyMesh_set_1regs(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->regs = (jshort*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_PolyMesh_get_1flags( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _returnPointer(&_this->flags[0]);
}
JNIEXPORT jobject JNICALL Java_recast_PolyMesh_set_1flags(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->flags = (jshort*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_PolyMesh_get_1areas( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _returnPointer(&_this->areas[0]);
}
JNIEXPORT jobject JNICALL Java_recast_PolyMesh_set_1areas(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->areas = (jchar*(value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_PolyMesh_get_1nverts( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _this->nverts;
}
JNIEXPORT jint JNICALL Java_recast_PolyMesh_set_1nverts(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->nverts = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_PolyMesh_get_1npolys( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _this->npolys;
}
JNIEXPORT jint JNICALL Java_recast_PolyMesh_set_1npolys(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->npolys = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_PolyMesh_get_1maxpolys( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _this->maxpolys;
}
JNIEXPORT jint JNICALL Java_recast_PolyMesh_set_1maxpolys(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->maxpolys = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_PolyMesh_get_1nvp( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _this->nvp;
}
JNIEXPORT jint JNICALL Java_recast_PolyMesh_set_1nvp(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->nvp = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_PolyMesh_get_1bmin( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return (jobject )(JNIEnv *__env, _this->bmin);
}
JNIEXPORT void JNICALL PolyMesh_getbminv( JNIEnv *__env, rcPolyMesh _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmin)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_PolyMesh_set_1bmin(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	 jfloat *dst = (jfloat*) & (_this->bmin)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL PolyMesh_setbmin3(JNIEnv *__env, rcPolyMesh _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jobject JNICALL Java_recast_PolyMesh_get_1bmax( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return (jobject )(JNIEnv *__env, _this->bmax);
}
JNIEXPORT void JNICALL PolyMesh_getbmaxv( JNIEnv *__env, rcPolyMesh _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmax)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_PolyMesh_set_1bmax(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	 jfloat *dst = (jfloat*) & (_this->bmax)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL PolyMesh_setbmax3(JNIEnv *__env, rcPolyMesh _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jfloat JNICALL Java_recast_PolyMesh_get_1cs( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _this->cs;
}
JNIEXPORT jfloat JNICALL Java_recast_PolyMesh_set_1cs(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->cs = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_PolyMesh_get_1ch( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _this->ch;
}
JNIEXPORT jfloat JNICALL Java_recast_PolyMesh_set_1ch(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->ch = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_PolyMesh_get_1borderSize( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _this->borderSize;
}
JNIEXPORT jint JNICALL Java_recast_PolyMesh_set_1borderSize(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->borderSize = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_PolyMesh_get_1maxEdgeError( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	return _this->maxEdgeError;
}
JNIEXPORT jfloat JNICALL Java_recast_PolyMesh_set_1maxEdgeError(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_PolyMesh(__env);
rcPolyMesh *_this = (rcPolyMesh*)__env->GetLongField(_obj, __h_f_PolyMesh_this);
	_this->maxEdgeError = (value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_PolyMeshDetail_nativeNew(JNIEnv *__env, jobject _obj/* PolyMeshDetail */) {
	cache__h_c_PolyMeshDetail(__env);
	auto *_this = (new rcPolyMeshDetail());
	__env->SetLongField(_obj, __h_f_PolyMeshDetail_this, (long long)_this);
}

JNIEXPORT jobject JNICALL Java_recast_PolyMeshDetail_get_1meshes( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMeshDetail(__env);
rcPolyMeshDetail *_this = (rcPolyMeshDetail*)__env->GetLongField(_obj, __h_f_PolyMeshDetail_this);
	return _returnPointer(&_this->meshes[0]);
}
JNIEXPORT jobject JNICALL Java_recast_PolyMeshDetail_set_1meshes(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_PolyMeshDetail(__env);
rcPolyMeshDetail *_this = (rcPolyMeshDetail*)__env->GetLongField(_obj, __h_f_PolyMeshDetail_this);
	_this->meshes = (jint*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_PolyMeshDetail_get_1verts( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMeshDetail(__env);
rcPolyMeshDetail *_this = (rcPolyMeshDetail*)__env->GetLongField(_obj, __h_f_PolyMeshDetail_this);
	return _returnPointer(&_this->verts[0]);
}
JNIEXPORT jobject JNICALL Java_recast_PolyMeshDetail_set_1verts(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_PolyMeshDetail(__env);
rcPolyMeshDetail *_this = (rcPolyMeshDetail*)__env->GetLongField(_obj, __h_f_PolyMeshDetail_this);
	_this->verts = (jfloat*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_PolyMeshDetail_get_1tris( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMeshDetail(__env);
rcPolyMeshDetail *_this = (rcPolyMeshDetail*)__env->GetLongField(_obj, __h_f_PolyMeshDetail_this);
	return _returnPointer(&_this->tris[0]);
}
JNIEXPORT jobject JNICALL Java_recast_PolyMeshDetail_set_1tris(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_PolyMeshDetail(__env);
rcPolyMeshDetail *_this = (rcPolyMeshDetail*)__env->GetLongField(_obj, __h_f_PolyMeshDetail_this);
	_this->tris = (jchar*(value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_PolyMeshDetail_get_1nmeshes( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMeshDetail(__env);
rcPolyMeshDetail *_this = (rcPolyMeshDetail*)__env->GetLongField(_obj, __h_f_PolyMeshDetail_this);
	return _this->nmeshes;
}
JNIEXPORT jint JNICALL Java_recast_PolyMeshDetail_set_1nmeshes(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_PolyMeshDetail(__env);
rcPolyMeshDetail *_this = (rcPolyMeshDetail*)__env->GetLongField(_obj, __h_f_PolyMeshDetail_this);
	_this->nmeshes = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_PolyMeshDetail_get_1nverts( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMeshDetail(__env);
rcPolyMeshDetail *_this = (rcPolyMeshDetail*)__env->GetLongField(_obj, __h_f_PolyMeshDetail_this);
	return _this->nverts;
}
JNIEXPORT jint JNICALL Java_recast_PolyMeshDetail_set_1nverts(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_PolyMeshDetail(__env);
rcPolyMeshDetail *_this = (rcPolyMeshDetail*)__env->GetLongField(_obj, __h_f_PolyMeshDetail_this);
	_this->nverts = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_PolyMeshDetail_get_1ntris( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyMeshDetail(__env);
rcPolyMeshDetail *_this = (rcPolyMeshDetail*)__env->GetLongField(_obj, __h_f_PolyMeshDetail_this);
	return _this->ntris;
}
JNIEXPORT jint JNICALL Java_recast_PolyMeshDetail_set_1ntris(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_PolyMeshDetail(__env);
rcPolyMeshDetail *_this = (rcPolyMeshDetail*)__env->GetLongField(_obj, __h_f_PolyMeshDetail_this);
	_this->ntris = (value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_Recast_rcFreeHeightField(JNIEnv *__env, jobject hf/* Heightfield */) {
	cache__h_c_Recast(__env);
	Recast *_this = (Recast*)__env->GetLongField(_obj, __h_f_Recast_this);
	cache__h_c_Heightfield(__env);
	Heightfield *_hf = (Heightfield*)__env->GetLongField(hf, __h_f_Heightfield_this);
rcFreeHeightField(_hf);
}

JNIEXPORT jobject JNICALL Java_recast_Recast_rcAllocHeightfield(JNIEnv *__env) {
	cache__h_c_Recast(__env);
	Recast *_this = (Recast*)__env->GetLongField(_obj, __h_f_Recast_this);
	auto ___retvalue = (rcAllocHeightfield());
cache__h_c_Heightfield(__env);
	auto _new_obj = __env->NewObject( __h_c_Heightfield, __h_m_Heightfield_ctor);
	__env->SetLongField(_new_obj, __h_f_Heightfield_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT jobject JNICALL Java_recast_Recast_rcAllocContourSet(JNIEnv *__env) {
	cache__h_c_Recast(__env);
	Recast *_this = (Recast*)__env->GetLongField(_obj, __h_f_Recast_this);
	auto ___retvalue = (rcAllocContourSet());
cache__h_c_rcContourSet(__env);
	auto _new_obj = __env->NewObject( __h_c_rcContourSet, __h_m_rcContourSet_ctor);
	__env->SetLongField(_new_obj, __h_f_rcContourSet_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT void JNICALL Java_recast_Recast_rcFreeContourSet(JNIEnv *__env, jobject cset/* rcContourSet */) {
	cache__h_c_Recast(__env);
	Recast *_this = (Recast*)__env->GetLongField(_obj, __h_f_Recast_this);
	cache__h_c_rcContourSet(__env);
	rcContourSet *_cset = (rcContourSet*)__env->GetLongField(cset, __h_f_rcContourSet_this);
rcFreeContourSet(_cset);
}

JNIEXPORT void JNICALL Java_recast_Recast_calcBounds(JNIEnv *__env, jfloatArray verts/* TFloat */, jint nv, jobject bmin, jobject bmax) {
	cache__h_c_Recast(__env);
	Recast *_this = (Recast*)__env->GetLongField(_obj, __h_f_Recast_this);
	rcCalcBounds(h_aptr<jfloat>(verts), nv, (float*)(jobject)bmin, (float*)(jobject)bmax);
}

JNIEXPORT void JNICALL Java_recast_Recast_calcGridSize(JNIEnv *__env, jobject bmin, jobject bmax, jfloat cs, jint* w, jint* h) {
	cache__h_c_Recast(__env);
	Recast *_this = (Recast*)__env->GetLongField(_obj, __h_f_Recast_this);
	rcCalcGridSize((float*)(jobject)bmin, (float*)(jobject)bmax, cs, w, h);
}

JNIEXPORT jint JNICALL Java_recast_Recast_offsetPoly(JNIEnv *__env, jobject verts, jint nverts, jfloat offset, jobject outVerts, jint maxOutVerts) {
	cache__h_c_Recast(__env);
	Recast *_this = (Recast*)__env->GetLongField(_obj, __h_f_Recast_this);
	return rcOffsetPoly((float*)(jobject)verts, nverts, offset, (float*)(jobject)outVerts, maxOutVerts);
}

JNIEXPORT jfloat JNICALL Java_recast_Math_fabsf(JNIEnv *__env, jfloat x) {
	cache__h_c_Math(__env);
	Math *_this = (Math*)__env->GetLongField(_obj, __h_f_Math_this);
	return fabsf(x);
}

JNIEXPORT jfloat JNICALL Java_recast_Math_sqrtf(JNIEnv *__env, jfloat x) {
	cache__h_c_Math(__env);
	Math *_this = (Math*)__env->GetLongField(_obj, __h_f_Math_this);
	return sqrtf(x);
}

JNIEXPORT jfloat JNICALL Java_recast_Math_floorf(JNIEnv *__env, jfloat x) {
	cache__h_c_Math(__env);
	Math *_this = (Math*)__env->GetLongField(_obj, __h_f_Math_this);
	return floorf(x);
}

JNIEXPORT jfloat JNICALL Java_recast_Math_ceilf(JNIEnv *__env, jfloat x) {
	cache__h_c_Math(__env);
	Math *_this = (Math*)__env->GetLongField(_obj, __h_f_Math_this);
	return ceilf(x);
}

JNIEXPORT jfloat JNICALL Java_recast_Math_cosf(JNIEnv *__env, jfloat x) {
	cache__h_c_Math(__env);
	Math *_this = (Math*)__env->GetLongField(_obj, __h_f_Math_this);
	return cosf(x);
}

JNIEXPORT jfloat JNICALL Java_recast_Math_sinf(JNIEnv *__env, jfloat x) {
	cache__h_c_Math(__env);
	Math *_this = (Math*)__env->GetLongField(_obj, __h_f_Math_this);
	return sinf(x);
}

JNIEXPORT jfloat JNICALL Java_recast_Math_atan2f(JNIEnv *__env, jfloat y, jfloat x) {
	cache__h_c_Math(__env);
	Math *_this = (Math*)__env->GetLongField(_obj, __h_f_Math_this);
	return atan2f(y, x);
}

JNIEXPORT jboolean JNICALL Java_recast_Math_isfinite(JNIEnv *__env, jfloat x) {
	cache__h_c_Math(__env);
	Math *_this = (Math*)__env->GetLongField(_obj, __h_f_Math_this);
	return isfinite(x);
}

JNIEXPORT jint JNICALL Java_recast_DetourCommon_nextPow2(JNIEnv *__env, jint v) {
	cache__h_c_DetourCommon(__env);
	DetourCommon *_this = (DetourCommon*)__env->GetLongField(_obj, __h_f_DetourCommon_this);
	return dtNextPow2(v);
}

JNIEXPORT jint JNICALL Java_recast_DetourCommon_ilog2(JNIEnv *__env, jint v) {
	cache__h_c_DetourCommon(__env);
	DetourCommon *_this = (DetourCommon*)__env->GetLongField(_obj, __h_f_DetourCommon_this);
	return dtIlog2(v);
}

JNIEXPORT jint JNICALL Java_recast_DetourCommon_align4(JNIEnv *__env, jint x) {
	cache__h_c_DetourCommon(__env);
	DetourCommon *_this = (DetourCommon*)__env->GetLongField(_obj, __h_f_DetourCommon_this);
	return dtAlign4(x);
}

JNIEXPORT void JNICALL Java_recast_TileCache_nativeNew(JNIEnv *__env, jobject _obj/* TileCache */) {
	cache__h_c_TileCache(__env);
	auto *_this = (new dtTileCache());
	__env->SetLongField(_obj, __h_f_TileCache_this, (long long)_this);
}

JNIEXPORT void JNICALL Java_recast_TileCache_init(JNIEnv *__env, jobject _obj/* TileCache */, jobject params/* TileCacheParams */, jobject talloc/* TileCacheAlloc */, jobject tcomp/* TileCacheCompressor */, jobject tmproc/* TileCacheMeshProcess */) {
	cache__h_c_TileCache(__env);
	dtTileCache *_this = (dtTileCache*)__env->GetLongField(_obj, __h_f_TileCache_this);
	cache__h_c_TileCacheParams(__env);
	TileCacheParams *_params = (TileCacheParams*)__env->GetLongField(params, __h_f_TileCacheParams_this);
cache__h_c_TileCacheAlloc(__env);
	TileCacheAlloc *_talloc = (TileCacheAlloc*)__env->GetLongField(talloc, __h_f_TileCacheAlloc_this);
cache__h_c_TileCacheCompressor(__env);
	TileCacheCompressor *_tcomp = (TileCacheCompressor*)__env->GetLongField(tcomp, __h_f_TileCacheCompressor_this);
cache__h_c_TileCacheMeshProcess(__env);
	TileCacheMeshProcess *_tmproc = (TileCacheMeshProcess*)__env->GetLongField(tmproc, __h_f_TileCacheMeshProcess_this);
_this->init(_params, _talloc, _tcomp, _tmproc);
}

JNIEXPORT jint JNICALL Java_recast_TileCache_addTile(JNIEnv *__env, jobject _obj/* TileCache */, jobject data, jint dataSize, jint flags, jint* result) {
	cache__h_c_TileCache(__env);
	dtTileCache *_this = (dtTileCache*)__env->GetLongField(_obj, __h_f_TileCache_this);
	return _this->addTile((jcharr*)data, dataSize, flags, result);
}

JNIEXPORT jint JNICALL Java_recast_TileCache_buildNavMeshTilesAt(JNIEnv *__env, jobject _obj/* TileCache */, jint tx, jint ty, jobject navmesh/* NavMesh */) {
	cache__h_c_TileCache(__env);
	dtTileCache *_this = (dtTileCache*)__env->GetLongField(_obj, __h_f_TileCache_this);
	cache__h_c_NavMesh(__env);
	NavMesh *_navmesh = (NavMesh*)__env->GetLongField(navmesh, __h_f_NavMesh_this);
auto ___retvalue = _this->buildNavMeshTilesAt(tx, ty, _navmesh);
	return (___retvalue);
}

JNIEXPORT jobject JNICALL Java_recast_TileCacheCompressor_buildTileCacheLayer(JNIEnv *__env, jobject _obj/* TileCacheCompressor */, jobject header/* TileCacheLayerHeader */, jobject heights, jobject areas, jobject cons, jint* outDataSize) {
	cache__h_c_TileCacheCompressor(__env);
	dtTileCacheCompressor *_this = (dtTileCacheCompressor*)__env->GetLongField(_obj, __h_f_TileCacheCompressor_this);
	jobject __tmpret;
cache__h_c_TileCacheLayerHeader(__env);
	TileCacheLayerHeader *_header = (TileCacheLayerHeader*)__env->GetLongField(header, __h_f_TileCacheLayerHeader_this);
dtBuildTileCacheLayer( _this , _header, (jcharr*)heights, (jcharr*)areas, (jcharr*)cons, (unsigned char **)&__tmpret, outDataSize;
	return (__tmpret);
}

JNIEXPORT void JNICALL Java_recast_TileCacheLayerHeader_nativeNew(JNIEnv *__env, jobject _obj/* TileCacheLayerHeader */) {
	cache__h_c_TileCacheLayerHeader(__env);
	auto *_this = (new dtTileCacheLayerHeader());
	__env->SetLongField(_obj, __h_f_TileCacheLayerHeader_this, (long long)_this);
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1magic( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->magic;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1magic(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->magic = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1version( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->version;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1version(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->version = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1tx( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->tx;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1tx(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->tx = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1ty( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->ty;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1ty(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->ty = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1tlayer( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->tlayer;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1tlayer(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->tlayer = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_TileCacheLayerHeader_get_1bmin( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return (jobject )(JNIEnv *__env, _this->bmin);
}
JNIEXPORT void JNICALL TileCacheLayerHeader_getbminv( JNIEnv *__env, dtTileCacheLayerHeader _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmin)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_TileCacheLayerHeader_set_1bmin(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	 jfloat *dst = (jfloat*) & (_this->bmin)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL TileCacheLayerHeader_setbmin3(JNIEnv *__env, dtTileCacheLayerHeader _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jobject JNICALL Java_recast_TileCacheLayerHeader_get_1bmax( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return (jobject )(JNIEnv *__env, _this->bmax);
}
JNIEXPORT void JNICALL TileCacheLayerHeader_getbmaxv( JNIEnv *__env, dtTileCacheLayerHeader _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmax)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_TileCacheLayerHeader_set_1bmax(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	 jfloat *dst = (jfloat*) & (_this->bmax)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL TileCacheLayerHeader_setbmax3(JNIEnv *__env, dtTileCacheLayerHeader _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1width( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->width;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1width(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->width = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1height( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->height;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1height(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->height = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1minx( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->minx;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1minx(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->minx = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1maxx( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->maxx;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1maxx(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->maxx = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1miny( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->miny;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1miny(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->miny = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1maxy( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->maxy;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1maxy(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->maxy = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1hmin( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->hmin;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1hmin(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->hmin = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_get_1hmax( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return _this->hmax;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_set_1hmax(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheLayerHeader(__env);
dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	_this->hmax = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheLayerHeader_sizeof(JNIEnv *__env) {
	cache__h_c_TileCacheLayerHeader(__env);
	dtTileCacheLayerHeader *_this = (dtTileCacheLayerHeader*)__env->GetLongField(_obj, __h_f_TileCacheLayerHeader_this);
	return sizeof(dtTileCacheLayerHeader);
}

JNIEXPORT void JNICALL Java_recast_TileCacheParams_nativeNew(JNIEnv *__env, jobject _obj/* TileCacheParams */) {
	cache__h_c_TileCacheParams(__env);
	auto *_this = (new dtTileCacheParams());
	__env->SetLongField(_obj, __h_f_TileCacheParams_this, (long long)_this);
}

JNIEXPORT jobject JNICALL Java_recast_TileCacheParams_get_1orig( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	return (jobject )(JNIEnv *__env, _this->orig);
}
JNIEXPORT void JNICALL TileCacheParams_getorigv( JNIEnv *__env, dtTileCacheParams _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->orig)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_TileCacheParams_set_1orig(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	 jfloat *dst = (jfloat*) & (_this->orig)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL TileCacheParams_setorig3(JNIEnv *__env, dtTileCacheParams _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->orig);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jfloat JNICALL Java_recast_TileCacheParams_get_1cs( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	return _this->cs;
}
JNIEXPORT jfloat JNICALL Java_recast_TileCacheParams_set_1cs(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	_this->cs = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_TileCacheParams_get_1ch( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	return _this->ch;
}
JNIEXPORT jfloat JNICALL Java_recast_TileCacheParams_set_1ch(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	_this->ch = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheParams_get_1width( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	return _this->width;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheParams_set_1width(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	_this->width = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheParams_get_1height( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	return _this->height;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheParams_set_1height(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	_this->height = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_TileCacheParams_get_1walkableHeight( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	return _this->walkableHeight;
}
JNIEXPORT jfloat JNICALL Java_recast_TileCacheParams_set_1walkableHeight(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	_this->walkableHeight = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_TileCacheParams_get_1walkableRadius( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	return _this->walkableRadius;
}
JNIEXPORT jfloat JNICALL Java_recast_TileCacheParams_set_1walkableRadius(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	_this->walkableRadius = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_TileCacheParams_get_1walkableClimb( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	return _this->walkableClimb;
}
JNIEXPORT jfloat JNICALL Java_recast_TileCacheParams_set_1walkableClimb(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	_this->walkableClimb = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_TileCacheParams_get_1maxSimplificationError( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	return _this->maxSimplificationError;
}
JNIEXPORT jfloat JNICALL Java_recast_TileCacheParams_set_1maxSimplificationError(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	_this->maxSimplificationError = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheParams_get_1maxTiles( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	return _this->maxTiles;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheParams_set_1maxTiles(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	_this->maxTiles = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_TileCacheParams_get_1maxObstacles( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	return _this->maxObstacles;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheParams_set_1maxObstacles(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheParams(__env);
dtTileCacheParams *_this = (dtTileCacheParams*)__env->GetLongField(_obj, __h_f_TileCacheParams_this);
	_this->maxObstacles = (value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_TileCacheData_nativeNew(JNIEnv *__env, jobject _obj/* TileCacheData */) {
	cache__h_c_TileCacheData(__env);
	auto *_this = (new TileCacheData());
	__env->SetLongField(_obj, __h_f_TileCacheData_this, (long long)_this);
}

JNIEXPORT jint JNICALL Java_recast_TileCacheData_get_1dataSize( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheData(__env);
TileCacheData *_this = (TileCacheData*)__env->GetLongField(_obj, __h_f_TileCacheData_this);
	return _this->dataSize;
}
JNIEXPORT jint JNICALL Java_recast_TileCacheData_set_1dataSize(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_TileCacheData(__env);
TileCacheData *_this = (TileCacheData*)__env->GetLongField(_obj, __h_f_TileCacheData_this);
	_this->dataSize = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_TileCacheData_get_1data( JNIEnv *__env, jobject _obj ) {
cache__h_c_TileCacheData(__env);
TileCacheData *_this = (TileCacheData*)__env->GetLongField(_obj, __h_f_TileCacheData_this);
	return _returnPointer(&_this->data[0]);
}
JNIEXPORT jobject JNICALL Java_recast_TileCacheData_set_1data(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_TileCacheData(__env);
TileCacheData *_this = (TileCacheData*)__env->GetLongField(_obj, __h_f_TileCacheData_this);
	_this->data = (jchar*(value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_FastLZCompressor_nativeNew(JNIEnv *__env, jobject _obj/* FastLZCompressor */) {
	cache__h_c_FastLZCompressor(__env);
	auto *_this = (new FastLZCompressor());
	__env->SetLongField(_obj, __h_f_FastLZCompressor_this, (long long)_this);
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_FastLZCompressor_asSuper(JNIEnv *__env, jobject _obj/* FastLZCompressor */) {
	cache__h_c_FastLZCompressor(__env);
	FastLZCompressor *_this = (FastLZCompressor*)__env->GetLongField(_obj, __h_f_FastLZCompressor_this);
	auto ___retvalue = (_this->asSuper());
cache__h_c_TileCacheCompressor(__env);
	auto _new_obj = __env->NewObject( __h_c_TileCacheCompressor, __h_m_TileCacheCompressor_ctor);
	__env->SetLongField(_new_obj, __h_f_TileCacheCompressor_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT void JNICALL Java_recast_LinearAllocator_nativeNew(JNIEnv *__env, jobject _obj/* LinearAllocator */, jint capacity) {
	cache__h_c_LinearAllocator(__env);
	auto *_this = (new LinearAllocator(capacity));
	__env->SetLongField(_obj, __h_f_LinearAllocator_this, (long long)_this);
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_LinearAllocator_asSuper(JNIEnv *__env, jobject _obj/* LinearAllocator */) {
	cache__h_c_LinearAllocator(__env);
	LinearAllocator *_this = (LinearAllocator*)__env->GetLongField(_obj, __h_f_LinearAllocator_this);
	auto ___retvalue = (_this->asSuper());
cache__h_c_TileCacheAlloc(__env);
	auto _new_obj = __env->NewObject( __h_c_TileCacheAlloc, __h_m_TileCacheAlloc_ctor);
	__env->SetLongField(_new_obj, __h_f_TileCacheAlloc_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT void JNICALL Java_recast_RemapProcessor_nativeNew(JNIEnv *__env, jobject _obj/* RemapProcessor */) {
	cache__h_c_RemapProcessor(__env);
	auto *_this = (new RemapProcessor());
	__env->SetLongField(_obj, __h_f_RemapProcessor_this, (long long)_this);
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_RemapProcessor_asSuper(JNIEnv *__env, jobject _obj/* RemapProcessor */) {
	cache__h_c_RemapProcessor(__env);
	RemapProcessor *_this = (RemapProcessor*)__env->GetLongField(_obj, __h_f_RemapProcessor_this);
	auto ___retvalue = (_this->asSuper());
cache__h_c_TileCacheMeshProcess(__env);
	auto _new_obj = __env->NewObject( __h_c_TileCacheMeshProcess, __h_m_TileCacheMeshProcess_ctor);
	__env->SetLongField(_new_obj, __h_f_TileCacheMeshProcess_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT jint JNICALL Java_recast_Poly_get_1firstLink( JNIEnv *__env, jobject _obj ) {
cache__h_c_Poly(__env);
dtPoly *_this = (dtPoly*)__env->GetLongField(_obj, __h_f_Poly_this);
	return _this->firstLink;
}
JNIEXPORT jint JNICALL Java_recast_Poly_set_1firstLink(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_Poly(__env);
dtPoly *_this = (dtPoly*)__env->GetLongField(_obj, __h_f_Poly_this);
	_this->firstLink = (value);
	return value;
}

JNIEXPORT jshort JNICALL Java_recast_Poly_get_1flags( JNIEnv *__env, jobject _obj ) {
cache__h_c_Poly(__env);
dtPoly *_this = (dtPoly*)__env->GetLongField(_obj, __h_f_Poly_this);
	return _this->flags;
}
JNIEXPORT jshort JNICALL Java_recast_Poly_set_1flags(JNIEnv *__env, jobject _obj, jshort value ) {
cache__h_c_Poly(__env);
dtPoly *_this = (dtPoly*)__env->GetLongField(_obj, __h_f_Poly_this);
	_this->flags = (value);
	return value;
}

JNIEXPORT jchar JNICALL Java_recast_Poly_get_1vertCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_Poly(__env);
dtPoly *_this = (dtPoly*)__env->GetLongField(_obj, __h_f_Poly_this);
	return _this->vertCount;
}
JNIEXPORT jchar JNICALL Java_recast_Poly_set_1vertCount(JNIEnv *__env, jobject _obj, jchar value ) {
cache__h_c_Poly(__env);
dtPoly *_this = (dtPoly*)__env->GetLongField(_obj, __h_f_Poly_this);
	_this->vertCount = (value);
	return value;
}

JNIEXPORT jchar JNICALL Java_recast_Poly_get_1areaAndtype( JNIEnv *__env, jobject _obj ) {
cache__h_c_Poly(__env);
dtPoly *_this = (dtPoly*)__env->GetLongField(_obj, __h_f_Poly_this);
	return _this->areaAndtype;
}
JNIEXPORT jchar JNICALL Java_recast_Poly_set_1areaAndtype(JNIEnv *__env, jobject _obj, jchar value ) {
cache__h_c_Poly(__env);
dtPoly *_this = (dtPoly*)__env->GetLongField(_obj, __h_f_Poly_this);
	_this->areaAndtype = (value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_Poly_setArea(JNIEnv *__env, jobject _obj/* Poly */, jchar a) {
	cache__h_c_Poly(__env);
	dtPoly *_this = (dtPoly*)__env->GetLongField(_obj, __h_f_Poly_this);
	_this->setArea(a);
}

JNIEXPORT void JNICALL Java_recast_Poly_setType(JNIEnv *__env, jobject _obj/* Poly */, jchar t) {
	cache__h_c_Poly(__env);
	dtPoly *_this = (dtPoly*)__env->GetLongField(_obj, __h_f_Poly_this);
	_this->setType(t);
}

JNIEXPORT /*CONST*/ jchar JNICALL Java_recast_Poly_getArea(JNIEnv *__env, jobject _obj/* Poly */) {
	cache__h_c_Poly(__env);
	dtPoly *_this = (dtPoly*)__env->GetLongField(_obj, __h_f_Poly_this);
	return _this->getArea();
}

JNIEXPORT /*CONST*/ jchar JNICALL Java_recast_Poly_getType(JNIEnv *__env, jobject _obj/* Poly */) {
	cache__h_c_Poly(__env);
	dtPoly *_this = (dtPoly*)__env->GetLongField(_obj, __h_f_Poly_this);
	return _this->getType();
}

JNIEXPORT jint JNICALL Java_recast_PolyDetail_get_1vertBase( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyDetail(__env);
dtPolyDetail *_this = (dtPolyDetail*)__env->GetLongField(_obj, __h_f_PolyDetail_this);
	return _this->vertBase;
}
JNIEXPORT jint JNICALL Java_recast_PolyDetail_set_1vertBase(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_PolyDetail(__env);
dtPolyDetail *_this = (dtPolyDetail*)__env->GetLongField(_obj, __h_f_PolyDetail_this);
	_this->vertBase = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_PolyDetail_get_1triBase( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyDetail(__env);
dtPolyDetail *_this = (dtPolyDetail*)__env->GetLongField(_obj, __h_f_PolyDetail_this);
	return _this->triBase;
}
JNIEXPORT jint JNICALL Java_recast_PolyDetail_set_1triBase(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_PolyDetail(__env);
dtPolyDetail *_this = (dtPolyDetail*)__env->GetLongField(_obj, __h_f_PolyDetail_this);
	_this->triBase = (value);
	return value;
}

JNIEXPORT jchar JNICALL Java_recast_PolyDetail_get_1vertCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyDetail(__env);
dtPolyDetail *_this = (dtPolyDetail*)__env->GetLongField(_obj, __h_f_PolyDetail_this);
	return _this->vertCount;
}
JNIEXPORT jchar JNICALL Java_recast_PolyDetail_set_1vertCount(JNIEnv *__env, jobject _obj, jchar value ) {
cache__h_c_PolyDetail(__env);
dtPolyDetail *_this = (dtPolyDetail*)__env->GetLongField(_obj, __h_f_PolyDetail_this);
	_this->vertCount = (value);
	return value;
}

JNIEXPORT jchar JNICALL Java_recast_PolyDetail_get_1triCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_PolyDetail(__env);
dtPolyDetail *_this = (dtPolyDetail*)__env->GetLongField(_obj, __h_f_PolyDetail_this);
	return _this->triCount;
}
JNIEXPORT jchar JNICALL Java_recast_PolyDetail_set_1triCount(JNIEnv *__env, jobject _obj, jchar value ) {
cache__h_c_PolyDetail(__env);
dtPolyDetail *_this = (dtPolyDetail*)__env->GetLongField(_obj, __h_f_PolyDetail_this);
	_this->triCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_Link_get_1ref( JNIEnv *__env, jobject _obj ) {
cache__h_c_Link(__env);
dtLink *_this = (dtLink*)__env->GetLongField(_obj, __h_f_Link_this);
	return _this->ref;
}
JNIEXPORT jint JNICALL Java_recast_Link_set_1ref(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_Link(__env);
dtLink *_this = (dtLink*)__env->GetLongField(_obj, __h_f_Link_this);
	_this->ref = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_Link_get_1next( JNIEnv *__env, jobject _obj ) {
cache__h_c_Link(__env);
dtLink *_this = (dtLink*)__env->GetLongField(_obj, __h_f_Link_this);
	return _this->next;
}
JNIEXPORT jint JNICALL Java_recast_Link_set_1next(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_Link(__env);
dtLink *_this = (dtLink*)__env->GetLongField(_obj, __h_f_Link_this);
	_this->next = (value);
	return value;
}

JNIEXPORT jchar JNICALL Java_recast_Link_get_1edge( JNIEnv *__env, jobject _obj ) {
cache__h_c_Link(__env);
dtLink *_this = (dtLink*)__env->GetLongField(_obj, __h_f_Link_this);
	return _this->edge;
}
JNIEXPORT jchar JNICALL Java_recast_Link_set_1edge(JNIEnv *__env, jobject _obj, jchar value ) {
cache__h_c_Link(__env);
dtLink *_this = (dtLink*)__env->GetLongField(_obj, __h_f_Link_this);
	_this->edge = (value);
	return value;
}

JNIEXPORT jchar JNICALL Java_recast_Link_get_1side( JNIEnv *__env, jobject _obj ) {
cache__h_c_Link(__env);
dtLink *_this = (dtLink*)__env->GetLongField(_obj, __h_f_Link_this);
	return _this->side;
}
JNIEXPORT jchar JNICALL Java_recast_Link_set_1side(JNIEnv *__env, jobject _obj, jchar value ) {
cache__h_c_Link(__env);
dtLink *_this = (dtLink*)__env->GetLongField(_obj, __h_f_Link_this);
	_this->side = (value);
	return value;
}

JNIEXPORT jchar JNICALL Java_recast_Link_get_1bmin( JNIEnv *__env, jobject _obj ) {
cache__h_c_Link(__env);
dtLink *_this = (dtLink*)__env->GetLongField(_obj, __h_f_Link_this);
	return _this->bmin;
}
JNIEXPORT jchar JNICALL Java_recast_Link_set_1bmin(JNIEnv *__env, jobject _obj, jchar value ) {
cache__h_c_Link(__env);
dtLink *_this = (dtLink*)__env->GetLongField(_obj, __h_f_Link_this);
	_this->bmin = (value);
	return value;
}

JNIEXPORT jchar JNICALL Java_recast_Link_get_1bmax( JNIEnv *__env, jobject _obj ) {
cache__h_c_Link(__env);
dtLink *_this = (dtLink*)__env->GetLongField(_obj, __h_f_Link_this);
	return _this->bmax;
}
JNIEXPORT jchar JNICALL Java_recast_Link_set_1bmax(JNIEnv *__env, jobject _obj, jchar value ) {
cache__h_c_Link(__env);
dtLink *_this = (dtLink*)__env->GetLongField(_obj, __h_f_Link_this);
	_this->bmax = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_BVNode_get_1i( JNIEnv *__env, jobject _obj ) {
cache__h_c_BVNode(__env);
dtBVNode *_this = (dtBVNode*)__env->GetLongField(_obj, __h_f_BVNode_this);
	return _this->i;
}
JNIEXPORT jint JNICALL Java_recast_BVNode_set_1i(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_BVNode(__env);
dtBVNode *_this = (dtBVNode*)__env->GetLongField(_obj, __h_f_BVNode_this);
	_this->i = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_OffMeshConnection_get_1rad( JNIEnv *__env, jobject _obj ) {
cache__h_c_OffMeshConnection(__env);
dtOffMeshConnection *_this = (dtOffMeshConnection*)__env->GetLongField(_obj, __h_f_OffMeshConnection_this);
	return _this->rad;
}
JNIEXPORT jfloat JNICALL Java_recast_OffMeshConnection_set_1rad(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_OffMeshConnection(__env);
dtOffMeshConnection *_this = (dtOffMeshConnection*)__env->GetLongField(_obj, __h_f_OffMeshConnection_this);
	_this->rad = (value);
	return value;
}

JNIEXPORT jshort JNICALL Java_recast_OffMeshConnection_get_1poly( JNIEnv *__env, jobject _obj ) {
cache__h_c_OffMeshConnection(__env);
dtOffMeshConnection *_this = (dtOffMeshConnection*)__env->GetLongField(_obj, __h_f_OffMeshConnection_this);
	return _this->poly;
}
JNIEXPORT jshort JNICALL Java_recast_OffMeshConnection_set_1poly(JNIEnv *__env, jobject _obj, jshort value ) {
cache__h_c_OffMeshConnection(__env);
dtOffMeshConnection *_this = (dtOffMeshConnection*)__env->GetLongField(_obj, __h_f_OffMeshConnection_this);
	_this->poly = (value);
	return value;
}

JNIEXPORT jchar JNICALL Java_recast_OffMeshConnection_get_1flags( JNIEnv *__env, jobject _obj ) {
cache__h_c_OffMeshConnection(__env);
dtOffMeshConnection *_this = (dtOffMeshConnection*)__env->GetLongField(_obj, __h_f_OffMeshConnection_this);
	return _this->flags;
}
JNIEXPORT jchar JNICALL Java_recast_OffMeshConnection_set_1flags(JNIEnv *__env, jobject _obj, jchar value ) {
cache__h_c_OffMeshConnection(__env);
dtOffMeshConnection *_this = (dtOffMeshConnection*)__env->GetLongField(_obj, __h_f_OffMeshConnection_this);
	_this->flags = (value);
	return value;
}

JNIEXPORT jchar JNICALL Java_recast_OffMeshConnection_get_1side( JNIEnv *__env, jobject _obj ) {
cache__h_c_OffMeshConnection(__env);
dtOffMeshConnection *_this = (dtOffMeshConnection*)__env->GetLongField(_obj, __h_f_OffMeshConnection_this);
	return _this->side;
}
JNIEXPORT jchar JNICALL Java_recast_OffMeshConnection_set_1side(JNIEnv *__env, jobject _obj, jchar value ) {
cache__h_c_OffMeshConnection(__env);
dtOffMeshConnection *_this = (dtOffMeshConnection*)__env->GetLongField(_obj, __h_f_OffMeshConnection_this);
	_this->side = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_OffMeshConnection_get_1userId( JNIEnv *__env, jobject _obj ) {
cache__h_c_OffMeshConnection(__env);
dtOffMeshConnection *_this = (dtOffMeshConnection*)__env->GetLongField(_obj, __h_f_OffMeshConnection_this);
	return _this->userId;
}
JNIEXPORT jint JNICALL Java_recast_OffMeshConnection_set_1userId(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_OffMeshConnection(__env);
dtOffMeshConnection *_this = (dtOffMeshConnection*)__env->GetLongField(_obj, __h_f_OffMeshConnection_this);
	_this->userId = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1magic( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->magic;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1magic(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->magic = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1version( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->version;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1version(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->version = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1x( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->x;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1x(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->x = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1y( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->y;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1y(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->y = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1layer( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->layer;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1layer(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->layer = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1userId( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->userId;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1userId(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->userId = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1polyCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->polyCount;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1polyCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->polyCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1vertCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->vertCount;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1vertCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->vertCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1maxLinkCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->maxLinkCount;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1maxLinkCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->maxLinkCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1detailMeshCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->detailMeshCount;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1detailMeshCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->detailMeshCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1detailVertCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->detailVertCount;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1detailVertCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->detailVertCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1detailTriCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->detailTriCount;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1detailTriCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->detailTriCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1bvNodeCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->bvNodeCount;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1bvNodeCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->bvNodeCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1offMeshConCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->offMeshConCount;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1offMeshConCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->offMeshConCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshHeader_get_1offMeshBase( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->offMeshBase;
}
JNIEXPORT jint JNICALL Java_recast_MeshHeader_set_1offMeshBase(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->offMeshBase = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_MeshHeader_get_1walkableHeight( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->walkableHeight;
}
JNIEXPORT jfloat JNICALL Java_recast_MeshHeader_set_1walkableHeight(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->walkableHeight = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_MeshHeader_get_1walkableRadius( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->walkableRadius;
}
JNIEXPORT jfloat JNICALL Java_recast_MeshHeader_set_1walkableRadius(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->walkableRadius = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_MeshHeader_get_1walkableClimb( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->walkableClimb;
}
JNIEXPORT jfloat JNICALL Java_recast_MeshHeader_set_1walkableClimb(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->walkableClimb = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_MeshHeader_get_1bmin( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return (jobject )(JNIEnv *__env, _this->bmin);
}
JNIEXPORT void JNICALL MeshHeader_getbminv( JNIEnv *__env, dtMeshHeader _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmin)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_MeshHeader_set_1bmin(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	 jfloat *dst = (jfloat*) & (_this->bmin)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL MeshHeader_setbmin3(JNIEnv *__env, dtMeshHeader _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jobject JNICALL Java_recast_MeshHeader_get_1bmax( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return (jobject )(JNIEnv *__env, _this->bmax);
}
JNIEXPORT void JNICALL MeshHeader_getbmaxv( JNIEnv *__env, dtMeshHeader _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmax)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_MeshHeader_set_1bmax(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	 jfloat *dst = (jfloat*) & (_this->bmax)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL MeshHeader_setbmax3(JNIEnv *__env, dtMeshHeader _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jfloat JNICALL Java_recast_MeshHeader_get_1bvQuantFactor( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	return _this->bvQuantFactor;
}
JNIEXPORT jfloat JNICALL Java_recast_MeshHeader_set_1bvQuantFactor(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_MeshHeader(__env);
dtMeshHeader *_this = (dtMeshHeader*)__env->GetLongField(_obj, __h_f_MeshHeader_this);
	_this->bvQuantFactor = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshTile_get_1salt( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return _this->salt;
}
JNIEXPORT jint JNICALL Java_recast_MeshTile_set_1salt(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->salt = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshTile_get_1linksFreeList( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return _this->linksFreeList;
}
JNIEXPORT jint JNICALL Java_recast_MeshTile_set_1linksFreeList(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->linksFreeList = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_MeshTile_get_1header( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return alloc_ref(_this->header,MeshHeader);
}
JNIEXPORT jobject JNICALL Java_recast_MeshTile_set_1header(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->header = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_MeshTile_get_1polys( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return alloc_ref(_this->polys,Poly);
}
JNIEXPORT jobject JNICALL Java_recast_MeshTile_set_1polys(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->polys = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_MeshTile_get_1verts( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return _returnPointer(&_this->verts[0]);
}
JNIEXPORT jobject JNICALL Java_recast_MeshTile_set_1verts(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->verts = (jfloat*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_MeshTile_get_1links( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return alloc_ref(_this->links,Link);
}
JNIEXPORT jobject JNICALL Java_recast_MeshTile_set_1links(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->links = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_MeshTile_get_1detailMeshes( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return alloc_ref(_this->detailMeshes,PolyDetail);
}
JNIEXPORT jobject JNICALL Java_recast_MeshTile_set_1detailMeshes(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->detailMeshes = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_MeshTile_get_1detailVerts( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return _returnPointer(&_this->detailVerts[0]);
}
JNIEXPORT jobject JNICALL Java_recast_MeshTile_set_1detailVerts(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->detailVerts = (jfloat*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_MeshTile_get_1detailTris( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return _returnPointer(&_this->detailTris[0]);
}
JNIEXPORT jobject JNICALL Java_recast_MeshTile_set_1detailTris(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->detailTris = (jchar*(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_MeshTile_get_1bvTree( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return alloc_ref(_this->bvTree,BVNode);
}
JNIEXPORT jobject JNICALL Java_recast_MeshTile_set_1bvTree(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->bvTree = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_MeshTile_get_1offMeshCons( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return alloc_ref(_this->offMeshCons,OffMeshConnection);
}
JNIEXPORT jobject JNICALL Java_recast_MeshTile_set_1offMeshCons(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->offMeshCons = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshTile_get_1dataSize( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return _this->dataSize;
}
JNIEXPORT jint JNICALL Java_recast_MeshTile_set_1dataSize(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->dataSize = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_MeshTile_get_1flags( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return _this->flags;
}
JNIEXPORT jint JNICALL Java_recast_MeshTile_set_1flags(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->flags = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_MeshTile_get_1next( JNIEnv *__env, jobject _obj ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	return alloc_ref(_this->next,MeshTile);
}
JNIEXPORT jobject JNICALL Java_recast_MeshTile_set_1next(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_MeshTile(__env);
dtMeshTile *_this = (dtMeshTile*)__env->GetLongField(_obj, __h_f_MeshTile_this);
	_this->next = _unref_ptr_safe(value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_NavMesh_nativeNew(JNIEnv *__env, jobject _obj/* NavMesh */) {
	cache__h_c_NavMesh(__env);
	auto *_this = (new NavMesh());
	__env->SetLongField(_obj, __h_f_NavMesh_this, (long long)_this);
}

JNIEXPORT void JNICALL Java_recast_NavMesh_create(JNIEnv *__env, jobject _obj/* NavMesh */, jobject origin, jfloat tileWidth, jfloat tileHeight, jint maxTiles, jint maxPolys) {
	cache__h_c_NavMesh(__env);
	NavMesh *_this = (NavMesh*)__env->GetLongField(_obj, __h_f_NavMesh_this);
	_this->create((jobject)origin, tileWidth, tileHeight, maxTiles, maxPolys);
}

JNIEXPORT jint JNICALL Java_recast_NavMesh_load(JNIEnv *__env, jobject _obj/* NavMesh */, jobject data/* NavBuffer */, jint flags) {
	cache__h_c_NavMesh(__env);
	NavMesh *_this = (NavMesh*)__env->GetLongField(_obj, __h_f_NavMesh_this);
	cache__h_c_NavBuffer(__env);
	NavBuffer *_data = (NavBuffer*)__env->GetLongField(data, __h_f_NavBuffer_this);
auto ___retvalue = _this->load(_data, flags);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMesh_addTile(JNIEnv *__env, jobject _obj/* NavMesh */, jobject data/* NavBuffer */, jint flags) {
	cache__h_c_NavMesh(__env);
	NavMesh *_this = (NavMesh*)__env->GetLongField(_obj, __h_f_NavMesh_this);
	cache__h_c_NavBuffer(__env);
	NavBuffer *_data = (NavBuffer*)__env->GetLongField(data, __h_f_NavBuffer_this);
auto ___retvalue = _this->addTile(_data, flags);
	return (___retvalue);
}

JNIEXPORT void JNICALL Java_recast_NavMesh_replaceTile(JNIEnv *__env, jobject _obj/* NavMesh */, jobject data/* NavBuffer */, jint ref, jint flags) {
	cache__h_c_NavMesh(__env);
	NavMesh *_this = (NavMesh*)__env->GetLongField(_obj, __h_f_NavMesh_this);
	cache__h_c_NavBuffer(__env);
	NavBuffer *_data = (NavBuffer*)__env->GetLongField(data, __h_f_NavBuffer_this);
_this->replaceTile(_data, ref, flags);
}

JNIEXPORT jint JNICALL Java_recast_NavMesh_removeTile(JNIEnv *__env, jobject _obj/* NavMesh */, jint ref) {
	cache__h_c_NavMesh(__env);
	NavMesh *_this = (NavMesh*)__env->GetLongField(_obj, __h_f_NavMesh_this);
	return _this->removeTile(ref);
}

JNIEXPORT jboolean JNICALL Java_recast_NavMesh_isValidPolyRef(JNIEnv *__env, jobject _obj/* NavMesh */, jint ref) {
	cache__h_c_NavMesh(__env);
	NavMesh *_this = (NavMesh*)__env->GetLongField(_obj, __h_f_NavMesh_this);
	return _this->isValidPolyRef(ref);
}

JNIEXPORT jint JNICALL Java_recast_NavMesh_getOffMeshConnectionPolyEndPoints(JNIEnv *__env, jobject _obj/* NavMesh */, jint prevRef, jint polyRef, jfloatArray startPos/* TFloat */, jfloatArray endPos/* TFloat */) {
	cache__h_c_NavMesh(__env);
	NavMesh *_this = (NavMesh*)__env->GetLongField(_obj, __h_f_NavMesh_this);
	return _this->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, h_aptr<jfloat>(startPos), h_aptr<jfloat>(endPos));
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMesh_getOffMeshConnectionByRef(JNIEnv *__env, jobject _obj/* NavMesh */, jint ref) {
	cache__h_c_NavMesh(__env);
	NavMesh *_this = (NavMesh*)__env->GetLongField(_obj, __h_f_NavMesh_this);
	auto ___retvalue = (_this->getOffMeshConnectionByRef(ref));
cache__h_c_OffMeshConnection(__env);
	auto _new_obj = __env->NewObject( __h_c_OffMeshConnection, __h_m_OffMeshConnection_ctor);
	__env->SetLongField(_new_obj, __h_f_OffMeshConnection_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT jint JNICALL Java_recast_NavMesh_setPolyFlags(JNIEnv *__env, jobject _obj/* NavMesh */, jint ref, jshort flags) {
	cache__h_c_NavMesh(__env);
	NavMesh *_this = (NavMesh*)__env->GetLongField(_obj, __h_f_NavMesh_this);
	return _this->setPolyFlags(ref, flags);
}

JNIEXPORT jint JNICALL Java_recast_NavMesh_getPolyFlags(JNIEnv *__env, jobject _obj/* NavMesh */, jint ref, jshortArray resultFlags/* TShort */) {
	cache__h_c_NavMesh(__env);
	NavMesh *_this = (NavMesh*)__env->GetLongField(_obj, __h_f_NavMesh_this);
	return _this->getPolyFlags(ref, h_aptr<jshort>(resultFlags));
}

JNIEXPORT jint JNICALL Java_recast_NavMesh_setPolyArea(JNIEnv *__env, jobject _obj/* NavMesh */, jint ref, jchar area) {
	cache__h_c_NavMesh(__env);
	NavMesh *_this = (NavMesh*)__env->GetLongField(_obj, __h_f_NavMesh_this);
	return _this->setPolyArea(ref, area);
}

JNIEXPORT jint JNICALL Java_recast_NavMesh_getPolyArea(JNIEnv *__env, jobject _obj/* NavMesh */, jint ref, jcharArray resultArea/* TChar */) {
	cache__h_c_NavMesh(__env);
	NavMesh *_this = (NavMesh*)__env->GetLongField(_obj, __h_f_NavMesh_this);
	return _this->getPolyArea(ref, h_aptr<jchar>(resultArea));
}

JNIEXPORT jobject JNICALL Java_recast_Node_get_1pos( JNIEnv *__env, jobject _obj ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	return (jobject )(JNIEnv *__env, _this->pos);
}
JNIEXPORT void JNICALL Node_getposv( JNIEnv *__env, dtNode _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->pos)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_Node_set_1pos(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	 jfloat *dst = (jfloat*) & (_this->pos)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL Node_setpos3(JNIEnv *__env, dtNode _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->pos);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jfloat JNICALL Java_recast_Node_get_1cost( JNIEnv *__env, jobject _obj ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	return _this->cost;
}
JNIEXPORT jfloat JNICALL Java_recast_Node_set_1cost(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	_this->cost = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_Node_get_1total( JNIEnv *__env, jobject _obj ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	return _this->total;
}
JNIEXPORT jfloat JNICALL Java_recast_Node_set_1total(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	_this->total = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_Node_get_1pidx( JNIEnv *__env, jobject _obj ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	return _this->pidx;
}
JNIEXPORT jint JNICALL Java_recast_Node_set_1pidx(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	_this->pidx = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_Node_get_1state( JNIEnv *__env, jobject _obj ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	return _this->state;
}
JNIEXPORT jint JNICALL Java_recast_Node_set_1state(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	_this->state = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_Node_get_1flags( JNIEnv *__env, jobject _obj ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	return _this->flags;
}
JNIEXPORT jint JNICALL Java_recast_Node_set_1flags(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	_this->flags = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_Node_get_1id( JNIEnv *__env, jobject _obj ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	return _this->id;
}
JNIEXPORT jint JNICALL Java_recast_Node_set_1id(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_Node(__env);
dtNode *_this = (dtNode*)__env->GetLongField(_obj, __h_f_Node_this);
	_this->id = (value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_NodePool_nativeNew(JNIEnv *__env, jobject _obj/* NodePool */, jint maxNodes, jint hashSize) {
	cache__h_c_NodePool(__env);
	auto *_this = (new dtNodePool(maxNodes, hashSize));
	__env->SetLongField(_obj, __h_f_NodePool_this, (long long)_this);
}

JNIEXPORT void JNICALL Java_recast_NodePool_clear(JNIEnv *__env, jobject _obj/* NodePool */) {
	cache__h_c_NodePool(__env);
	dtNodePool *_this = (dtNodePool*)__env->GetLongField(_obj, __h_f_NodePool_this);
	_this->clear();
}

JNIEXPORT jobject JNICALL Java_recast_NodePool_getNode(JNIEnv *__env, jobject _obj/* NodePool */, jint id, jchar state) {
	cache__h_c_NodePool(__env);
	dtNodePool *_this = (dtNodePool*)__env->GetLongField(_obj, __h_f_NodePool_this);
	auto ___retvalue = (_this->getNode(id, state));
cache__h_c_Node(__env);
	auto _new_obj = __env->NewObject( __h_c_Node, __h_m_Node_ctor);
	__env->SetLongField(_new_obj, __h_f_Node_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT jobject JNICALL Java_recast_NodePool_findNode(JNIEnv *__env, jobject _obj/* NodePool */, jint id, jchar state) {
	cache__h_c_NodePool(__env);
	dtNodePool *_this = (dtNodePool*)__env->GetLongField(_obj, __h_f_NodePool_this);
	auto ___retvalue = (_this->findNode(id, state));
cache__h_c_Node(__env);
	auto _new_obj = __env->NewObject( __h_c_Node, __h_m_Node_ctor);
	__env->SetLongField(_new_obj, __h_f_Node_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT jint JNICALL Java_recast_NodePool_getNodeIdx(JNIEnv *__env, jobject _obj/* NodePool */, jobject node/* Node */) {
	cache__h_c_NodePool(__env);
	dtNodePool *_this = (dtNodePool*)__env->GetLongField(_obj, __h_f_NodePool_this);
	cache__h_c_Node(__env);
	Node *_node = (Node*)__env->GetLongField(node, __h_f_Node_this);
auto ___retvalue = _this->getNodeIdx(_node);
	return (___retvalue);
}

JNIEXPORT jobject JNICALL Java_recast_NodePool_getNodeAtIdx(JNIEnv *__env, jobject _obj/* NodePool */, jint idx) {
	cache__h_c_NodePool(__env);
	dtNodePool *_this = (dtNodePool*)__env->GetLongField(_obj, __h_f_NodePool_this);
	auto ___retvalue = (_this->getNodeAtIdx(idx));
cache__h_c_Node(__env);
	auto _new_obj = __env->NewObject( __h_c_Node, __h_m_Node_ctor);
	__env->SetLongField(_new_obj, __h_f_Node_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT jint JNICALL Java_recast_NodePool_getMemUsed(JNIEnv *__env, jobject _obj/* NodePool */) {
	cache__h_c_NodePool(__env);
	dtNodePool *_this = (dtNodePool*)__env->GetLongField(_obj, __h_f_NodePool_this);
	return _this->getMemUsed();
}

JNIEXPORT jint JNICALL Java_recast_NodePool_getMaxNodes(JNIEnv *__env, jobject _obj/* NodePool */) {
	cache__h_c_NodePool(__env);
	dtNodePool *_this = (dtNodePool*)__env->GetLongField(_obj, __h_f_NodePool_this);
	return _this->getMaxNodes();
}

JNIEXPORT jint JNICALL Java_recast_NodePool_getHashSize(JNIEnv *__env, jobject _obj/* NodePool */) {
	cache__h_c_NodePool(__env);
	dtNodePool *_this = (dtNodePool*)__env->GetLongField(_obj, __h_f_NodePool_this);
	return _this->getHashSize();
}

JNIEXPORT jshort JNICALL Java_recast_NodePool_getFirst(JNIEnv *__env, jobject _obj/* NodePool */, jint bucket) {
	cache__h_c_NodePool(__env);
	dtNodePool *_this = (dtNodePool*)__env->GetLongField(_obj, __h_f_NodePool_this);
	return _this->getFirst(bucket);
}

JNIEXPORT jshort JNICALL Java_recast_NodePool_getNext(JNIEnv *__env, jobject _obj/* NodePool */, jint i) {
	cache__h_c_NodePool(__env);
	dtNodePool *_this = (dtNodePool*)__env->GetLongField(_obj, __h_f_NodePool_this);
	return _this->getNext(i);
}

JNIEXPORT jint JNICALL Java_recast_NodePool_getNodeCount(JNIEnv *__env, jobject _obj/* NodePool */) {
	cache__h_c_NodePool(__env);
	dtNodePool *_this = (dtNodePool*)__env->GetLongField(_obj, __h_f_NodePool_this);
	return _this->getNodeCount();
}

JNIEXPORT void JNICALL Java_recast_NodeQueue_nativeNew(JNIEnv *__env, jobject _obj/* NodeQueue */, jint n) {
	cache__h_c_NodeQueue(__env);
	auto *_this = (new dtNodeQueue(n));
	__env->SetLongField(_obj, __h_f_NodeQueue_this, (long long)_this);
}

JNIEXPORT void JNICALL Java_recast_NodeQueue_clear(JNIEnv *__env, jobject _obj/* NodeQueue */) {
	cache__h_c_NodeQueue(__env);
	dtNodeQueue *_this = (dtNodeQueue*)__env->GetLongField(_obj, __h_f_NodeQueue_this);
	_this->clear();
}

JNIEXPORT jobject JNICALL Java_recast_NodeQueue_top(JNIEnv *__env, jobject _obj/* NodeQueue */) {
	cache__h_c_NodeQueue(__env);
	dtNodeQueue *_this = (dtNodeQueue*)__env->GetLongField(_obj, __h_f_NodeQueue_this);
	auto ___retvalue = (_this->top());
cache__h_c_Node(__env);
	auto _new_obj = __env->NewObject( __h_c_Node, __h_m_Node_ctor);
	__env->SetLongField(_new_obj, __h_f_Node_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT jobject JNICALL Java_recast_NodeQueue_pop(JNIEnv *__env, jobject _obj/* NodeQueue */) {
	cache__h_c_NodeQueue(__env);
	dtNodeQueue *_this = (dtNodeQueue*)__env->GetLongField(_obj, __h_f_NodeQueue_this);
	auto ___retvalue = (_this->pop());
cache__h_c_Node(__env);
	auto _new_obj = __env->NewObject( __h_c_Node, __h_m_Node_ctor);
	__env->SetLongField(_new_obj, __h_f_Node_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT void JNICALL Java_recast_NodeQueue_push(JNIEnv *__env, jobject _obj/* NodeQueue */, jobject node/* Node */) {
	cache__h_c_NodeQueue(__env);
	dtNodeQueue *_this = (dtNodeQueue*)__env->GetLongField(_obj, __h_f_NodeQueue_this);
	cache__h_c_Node(__env);
	Node *_node = (Node*)__env->GetLongField(node, __h_f_Node_this);
_this->push(_node);
}

JNIEXPORT void JNICALL Java_recast_NodeQueue_modify(JNIEnv *__env, jobject _obj/* NodeQueue */, jobject node/* Node */) {
	cache__h_c_NodeQueue(__env);
	dtNodeQueue *_this = (dtNodeQueue*)__env->GetLongField(_obj, __h_f_NodeQueue_this);
	cache__h_c_Node(__env);
	Node *_node = (Node*)__env->GetLongField(node, __h_f_Node_this);
_this->modify(_node);
}

JNIEXPORT jboolean JNICALL Java_recast_NodeQueue_empty(JNIEnv *__env, jobject _obj/* NodeQueue */) {
	cache__h_c_NodeQueue(__env);
	dtNodeQueue *_this = (dtNodeQueue*)__env->GetLongField(_obj, __h_f_NodeQueue_this);
	return _this->empty();
}

JNIEXPORT jint JNICALL Java_recast_NodeQueue_getMemUsed(JNIEnv *__env, jobject _obj/* NodeQueue */) {
	cache__h_c_NodeQueue(__env);
	dtNodeQueue *_this = (dtNodeQueue*)__env->GetLongField(_obj, __h_f_NodeQueue_this);
	return _this->getMemUsed();
}

JNIEXPORT jint JNICALL Java_recast_NodeQueue_getCapacity(JNIEnv *__env, jobject _obj/* NodeQueue */) {
	cache__h_c_NodeQueue(__env);
	dtNodeQueue *_this = (dtNodeQueue*)__env->GetLongField(_obj, __h_f_NodeQueue_this);
	return _this->getCapacity();
}

JNIEXPORT void JNICALL Java_recast_NavMeshCreateParams_nativeNew(JNIEnv *__env, jobject _obj/* NavMeshCreateParams */) {
	cache__h_c_NavMeshCreateParams(__env);
	auto *_this = (new dtNavMeshCreateParams());
	__env->SetLongField(_obj, __h_f_NavMeshCreateParams_this, (long long)_this);
}

JNIEXPORT jobject JNICALL Java_recast_NavMeshCreateParams_get_1verts( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->verts[0]);
}
JNIEXPORT jobject JNICALL Java_recast_NavMeshCreateParams_set_1verts(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->verts = (jshort*(value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_get_1vertCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->vertCount;
}
JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_set_1vertCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->vertCount = (value);
	return value;
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_get_1polys( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->polys[0]);
}
JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_set_1polys(JNIEnv *__env, jobject _obj, /*CONST*/ jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->polys = (jshort*(value);
	return value;
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_get_1polyFlags( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->polyFlags[0]);
}
JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_set_1polyFlags(JNIEnv *__env, jobject _obj, /*CONST*/ jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->polyFlags = (jshort*(value);
	return value;
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_get_1polyAreas( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->polyAreas[0]);
}
JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_set_1polyAreas(JNIEnv *__env, jobject _obj, /*CONST*/ jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->polyAreas = (jchar*(value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_get_1polyCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->polyCount;
}
JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_set_1polyCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->polyCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_get_1nvp( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->nvp;
}
JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_set_1nvp(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->nvp = (value);
	return value;
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_get_1detailMeshes( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->detailMeshes[0]);
}
JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_set_1detailMeshes(JNIEnv *__env, jobject _obj, /*CONST*/ jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->detailMeshes = (jint*(value);
	return value;
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_get_1detailVerts( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->detailVerts[0]);
}
JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_set_1detailVerts(JNIEnv *__env, jobject _obj, /*CONST*/ jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->detailVerts = (jfloat*(value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_get_1detailVertsCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->detailVertsCount;
}
JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_set_1detailVertsCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->detailVertsCount = (value);
	return value;
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_get_1detailTris( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->detailTris[0]);
}
JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_set_1detailTris(JNIEnv *__env, jobject _obj, /*CONST*/ jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->detailTris = (jchar*(value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_get_1detailTriCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->detailTriCount;
}
JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_set_1detailTriCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->detailTriCount = (value);
	return value;
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_get_1offMeshConVerts( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->offMeshConVerts[0]);
}
JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_set_1offMeshConVerts(JNIEnv *__env, jobject _obj, /*CONST*/ jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->offMeshConVerts = (jfloat*(value);
	return value;
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_get_1offMeshConRad( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->offMeshConRad[0]);
}
JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_set_1offMeshConRad(JNIEnv *__env, jobject _obj, /*CONST*/ jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->offMeshConRad = (jfloat*(value);
	return value;
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_get_1offMeshConFlags( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->offMeshConFlags[0]);
}
JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_set_1offMeshConFlags(JNIEnv *__env, jobject _obj, /*CONST*/ jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->offMeshConFlags = (jshort*(value);
	return value;
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_get_1offMeshConAreas( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->offMeshConAreas[0]);
}
JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_set_1offMeshConAreas(JNIEnv *__env, jobject _obj, /*CONST*/ jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->offMeshConAreas = (jchar*(value);
	return value;
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_get_1offMeshConDir( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->offMeshConDir[0]);
}
JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_set_1offMeshConDir(JNIEnv *__env, jobject _obj, /*CONST*/ jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->offMeshConDir = (jchar*(value);
	return value;
}

JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_get_1offMeshConUserID( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _returnPointer(&_this->offMeshConUserID[0]);
}
JNIEXPORT /*CONST*/ jobject JNICALL Java_recast_NavMeshCreateParams_set_1offMeshConUserID(JNIEnv *__env, jobject _obj, /*CONST*/ jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->offMeshConUserID = (jint*(value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_get_1offMeshConCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->offMeshConCount;
}
JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_set_1offMeshConCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->offMeshConCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_get_1userId( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->userId;
}
JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_set_1userId(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->userId = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_get_1tileX( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->tileX;
}
JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_set_1tileX(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->tileX = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_get_1tileY( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->tileY;
}
JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_set_1tileY(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->tileY = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_get_1tileLayer( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->tileLayer;
}
JNIEXPORT jint JNICALL Java_recast_NavMeshCreateParams_set_1tileLayer(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->tileLayer = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_NavMeshCreateParams_get_1bmin( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return (jobject )(JNIEnv *__env, _this->bmin);
}
JNIEXPORT void JNICALL NavMeshCreateParams_getbminv( JNIEnv *__env, dtNavMeshCreateParams _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmin)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_NavMeshCreateParams_set_1bmin(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	 jfloat *dst = (jfloat*) & (_this->bmin)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL NavMeshCreateParams_setbmin3(JNIEnv *__env, dtNavMeshCreateParams _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmin);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jobject JNICALL Java_recast_NavMeshCreateParams_get_1bmax( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return (jobject )(JNIEnv *__env, _this->bmax);
}
JNIEXPORT void JNICALL NavMeshCreateParams_getbmaxv( JNIEnv *__env, dtNavMeshCreateParams _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->bmax)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_NavMeshCreateParams_set_1bmax(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	 jfloat *dst = (jfloat*) & (_this->bmax)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL NavMeshCreateParams_setbmax3(JNIEnv *__env, dtNavMeshCreateParams _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->bmax);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jfloat JNICALL Java_recast_NavMeshCreateParams_get_1walkableHeight( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->walkableHeight;
}
JNIEXPORT jfloat JNICALL Java_recast_NavMeshCreateParams_set_1walkableHeight(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->walkableHeight = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_NavMeshCreateParams_get_1walkableRadius( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->walkableRadius;
}
JNIEXPORT jfloat JNICALL Java_recast_NavMeshCreateParams_set_1walkableRadius(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->walkableRadius = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_NavMeshCreateParams_get_1walkableClimb( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->walkableClimb;
}
JNIEXPORT jfloat JNICALL Java_recast_NavMeshCreateParams_set_1walkableClimb(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->walkableClimb = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_NavMeshCreateParams_get_1cs( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->cs;
}
JNIEXPORT jfloat JNICALL Java_recast_NavMeshCreateParams_set_1cs(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->cs = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_NavMeshCreateParams_get_1ch( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->ch;
}
JNIEXPORT jfloat JNICALL Java_recast_NavMeshCreateParams_set_1ch(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->ch = (value);
	return value;
}

JNIEXPORT jboolean JNICALL Java_recast_NavMeshCreateParams_get_1buildBvTree( JNIEnv *__env, jobject _obj ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	return _this->buildBvTree;
}
JNIEXPORT jboolean JNICALL Java_recast_NavMeshCreateParams_set_1buildBvTree(JNIEnv *__env, jobject _obj, jboolean value ) {
cache__h_c_NavMeshCreateParams(__env);
dtNavMeshCreateParams *_this = (dtNavMeshCreateParams*)__env->GetLongField(_obj, __h_f_NavMeshCreateParams_this);
	_this->buildBvTree = (value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_QueryFilter_nativeNew(JNIEnv *__env, jobject _obj/* QueryFilter */) {
	cache__h_c_QueryFilter(__env);
	auto *_this = (new dtQueryFilter());
	__env->SetLongField(_obj, __h_f_QueryFilter_this, (long long)_this);
}

JNIEXPORT jfloat JNICALL Java_recast_QueryFilter_getAreaCost(JNIEnv *__env, jobject _obj/* QueryFilter */, jint i) {
	cache__h_c_QueryFilter(__env);
	dtQueryFilter *_this = (dtQueryFilter*)__env->GetLongField(_obj, __h_f_QueryFilter_this);
	return _this->getAreaCost(i);
}

JNIEXPORT void JNICALL Java_recast_QueryFilter_setAreaCost(JNIEnv *__env, jobject _obj/* QueryFilter */, jint i, jfloat cost) {
	cache__h_c_QueryFilter(__env);
	dtQueryFilter *_this = (dtQueryFilter*)__env->GetLongField(_obj, __h_f_QueryFilter_this);
	_this->setAreaCost(i, cost);
}

JNIEXPORT jshort JNICALL Java_recast_QueryFilter_getIncludeFlags(JNIEnv *__env, jobject _obj/* QueryFilter */) {
	cache__h_c_QueryFilter(__env);
	dtQueryFilter *_this = (dtQueryFilter*)__env->GetLongField(_obj, __h_f_QueryFilter_this);
	return _this->getIncludeFlags();
}

JNIEXPORT void JNICALL Java_recast_QueryFilter_setIncludeFlags(JNIEnv *__env, jobject _obj/* QueryFilter */, jshort flags) {
	cache__h_c_QueryFilter(__env);
	dtQueryFilter *_this = (dtQueryFilter*)__env->GetLongField(_obj, __h_f_QueryFilter_this);
	_this->setIncludeFlags(flags);
}

JNIEXPORT jshort JNICALL Java_recast_QueryFilter_getExcludeFlags(JNIEnv *__env, jobject _obj/* QueryFilter */) {
	cache__h_c_QueryFilter(__env);
	dtQueryFilter *_this = (dtQueryFilter*)__env->GetLongField(_obj, __h_f_QueryFilter_this);
	return _this->getExcludeFlags();
}

JNIEXPORT void JNICALL Java_recast_QueryFilter_setExcludeFlags(JNIEnv *__env, jobject _obj/* QueryFilter */, jshort flags) {
	cache__h_c_QueryFilter(__env);
	dtQueryFilter *_this = (dtQueryFilter*)__env->GetLongField(_obj, __h_f_QueryFilter_this);
	_this->setExcludeFlags(flags);
}

JNIEXPORT jfloat JNICALL Java_recast_RaycastHit_get_1t( JNIEnv *__env, jobject _obj ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	return _this->t;
}
JNIEXPORT jfloat JNICALL Java_recast_RaycastHit_set_1t(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	_this->t = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_RaycastHit_get_1hitNormal( JNIEnv *__env, jobject _obj ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	return (jobject )(JNIEnv *__env, _this->hitNormal);
}
JNIEXPORT void JNICALL RaycastHit_gethitNormalv( JNIEnv *__env, dtRaycastHit _this, jobject value ) {
	 jfloat *src = (jfloat*) & (_this->hitNormal)[0];
	 jfloat *dst = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}
JNIEXPORT jobject JNICALL Java_recast_RaycastHit_set_1hitNormal(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	 jfloat *dst = (jfloat*) & (_this->hitNormal)[0];
	 jfloat *src = (jfloat*) value;
	dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
	return value;
}
JNIEXPORT void JNICALL RaycastHit_sethitNormal3(JNIEnv *__env, dtRaycastHit _this,  jfloat value0, jfloat value1, jfloat value2 ) {
	 jfloat *p = (_this->hitNormal);
	p[0] = value0; p[1] = value1; p[2] = value2;
}

JNIEXPORT jint JNICALL Java_recast_RaycastHit_get_1hitEdgeIndex( JNIEnv *__env, jobject _obj ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	return _this->hitEdgeIndex;
}
JNIEXPORT jint JNICALL Java_recast_RaycastHit_set_1hitEdgeIndex(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	_this->hitEdgeIndex = (value);
	return value;
}

JNIEXPORT jobject JNICALL Java_recast_RaycastHit_get_1path( JNIEnv *__env, jobject _obj ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	return _returnPointer(&_this->path[0]);
}
JNIEXPORT jobject JNICALL Java_recast_RaycastHit_set_1path(JNIEnv *__env, jobject _obj, jobject value ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	_this->path = (jint*(value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_RaycastHit_get_1pathCount( JNIEnv *__env, jobject _obj ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	return _this->pathCount;
}
JNIEXPORT jint JNICALL Java_recast_RaycastHit_set_1pathCount(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	_this->pathCount = (value);
	return value;
}

JNIEXPORT jint JNICALL Java_recast_RaycastHit_get_1maxPath( JNIEnv *__env, jobject _obj ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	return _this->maxPath;
}
JNIEXPORT jint JNICALL Java_recast_RaycastHit_set_1maxPath(JNIEnv *__env, jobject _obj, jint value ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	_this->maxPath = (value);
	return value;
}

JNIEXPORT jfloat JNICALL Java_recast_RaycastHit_get_1pathCost( JNIEnv *__env, jobject _obj ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	return _this->pathCost;
}
JNIEXPORT jfloat JNICALL Java_recast_RaycastHit_set_1pathCost(JNIEnv *__env, jobject _obj, jfloat value ) {
cache__h_c_RaycastHit(__env);
dtRaycastHit *_this = (dtRaycastHit*)__env->GetLongField(_obj, __h_f_RaycastHit_this);
	_this->pathCost = (value);
	return value;
}

JNIEXPORT void JNICALL Java_recast_NavMeshQuery_nativeNew(JNIEnv *__env, jobject _obj/* NavMeshQuery */) {
	cache__h_c_NavMeshQuery(__env);
	auto *_this = (new dtNavMeshQuery());
	__env->SetLongField(_obj, __h_f_NavMeshQuery_this, (long long)_this);
}

JNIEXPORT jobject JNICALL Java_recast_NavMeshQuery_getAttachedNavMesh(JNIEnv *__env, jobject _obj/* NavMeshQuery */) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	auto ___retvalue = (NavMesh*)(NavMesh*)(_this->getAttachedNavMesh());
cache__h_c_NavMesh(__env);
	auto _new_obj = __env->NewObject( __h_c_NavMesh, __h_m_NavMesh_ctor);
	__env->SetLongField(_new_obj, __h_f_NavMesh_this, (long long)___retvalue);
	return _new_obj;
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_init(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jobject nav/* NavMesh */, jint maxNodes) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_NavMesh(__env);
	NavMesh *_nav = (NavMesh*)__env->GetLongField(nav, __h_f_NavMesh_this);
auto ___retvalue = _this->init(_nav, maxNodes);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_findPath(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint startRef, jint endRef, jobject startPos, jobject endPos, jobject filter/* QueryFilter */, jarray path/* TUInt */, jint* pathCount, jint maxPath) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->findPath(startRef, endRef, (float*)(jobject)startPos, (float*)(jobject)endPos, _filter, h_aptr<jint>(path), pathCount, maxPath);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_findNearestPoly(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jobject center, jobject halfExtents, jobject filter/* QueryFilter */, jint* nearestRef, jobject nearestPt) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->findNearestPoly((float*)(jobject)center, (float*)(jobject)halfExtents, _filter, nearestRef, (float*)(jobject)nearestPt);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_findNearestPoly(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jobject center, jobject halfExtents, jobject filter/* QueryFilter */, jint* nearestRef, jobject nearestPt, jboolean* isOverPoly) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->findNearestPoly((float*)(jobject)center, (float*)(jobject)halfExtents, _filter, nearestRef, (float*)(jobject)nearestPt, isOverPoly);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_initSlicedFindPath(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint startRef, jint endRef, jobject startPos, jobject endPos, jobject filter/* QueryFilter */, jint options) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->initSlicedFindPath(startRef, endRef, (float*)(jobject)startPos, (float*)(jobject)endPos, _filter, options);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_updateSlicedFindPath(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint maxIter, jint* doneIters) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	return _this->updateSlicedFindPath(maxIter, doneIters);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_finalizeSlicedFindPath(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jarray path/* TUInt */, jint* pathCount, jint maxPath) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	return _this->finalizeSlicedFindPath(h_aptr<jint>(path), pathCount, maxPath);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_finalizeSlicedFindPathPartial(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jarray existing/* TUInt */, jint existingSize, jarray path/* TUInt */, jint* pathCount, jint maxPath) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	return _this->finalizeSlicedFindPathPartial(h_aptr<jint>(existing), existingSize, h_aptr<jint>(path), pathCount, maxPath);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_findStraightPath(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jobject startPos, jobject endPos, jarray path/* TUInt */, jint pathSize, jfloatArray straightPath/* TFloat */, jcharArray straightPathFlags/* TChar */, jarray straightPathRefs/* TUInt */, jint* straightPathCount, jint maxStraightPath, jint options) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	return _this->findStraightPath((float*)(jobject)startPos, (float*)(jobject)endPos, h_aptr<jint>(path), pathSize, h_aptr<jfloat>(straightPath), h_aptr<jchar>(straightPathFlags), h_aptr<jint>(straightPathRefs), straightPathCount, maxStraightPath, options);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_raycast(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint startRef, jobject startPos, jobject endPos, jobject filter/* QueryFilter */, jfloat* t, jobject hitNormal, jarray path/* TUInt */, jint* pathCount, jint maxPath) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->raycast(startRef, (float*)(jobject)startPos, (float*)(jobject)endPos, _filter, t, (float*)(jobject)hitNormal, h_aptr<jint>(path), pathCount, maxPath);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_raycast(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint startRef, jobject startPos, jobject endPos, jobject filter/* QueryFilter */, jint options, jobject hit/* RaycastHit */, jint prevRef) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
cache__h_c_RaycastHit(__env);
	RaycastHit *_hit = (RaycastHit*)__env->GetLongField(hit, __h_f_RaycastHit_this);
auto ___retvalue = _this->raycast(startRef, (float*)(jobject)startPos, (float*)(jobject)endPos, _filter, options, _hit, prevRef);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_findPolysAroundCircle(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint startRef, jfloatArray centerPos/* TFloat */, jfloat radius, jobject filter/* QueryFilter */, jint* resultRef, jint* resultParent, jfloatArray resultCost/* TFloat */, jintArray resultCount/* TInt */, jint maxResult) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->findPolysAroundCircle(startRef, h_aptr</*CONST*/ jfloat>(centerPos), radius, _filter, resultRef, resultParent, h_aptr<jfloat>(resultCost), h_aptr<jint>(resultCount), maxResult);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_findPolysAroundShape(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint startRef, jfloatArray verts/* TFloat */, jint nverts, jobject filter/* QueryFilter */, jint* resultRef, jint* resultParent, jfloatArray resultCost/* TFloat */, jintArray resultCount/* TInt */, jint maxResult) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->findPolysAroundShape(startRef, h_aptr</*CONST*/ jfloat>(verts), nverts, _filter, resultRef, resultParent, h_aptr<jfloat>(resultCost), h_aptr<jint>(resultCount), maxResult);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_getPathFromDijkstraSearch(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint endRef, jint* path, jintArray pathCount/* TInt */, jint maxPath) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	return _this->getPathFromDijkstraSearch(endRef, path, h_aptr<jint>(pathCount), maxPath);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_queryPolygons(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jobject center, jobject halfExtents, jobject filter/* QueryFilter */, jint* polys, jintArray polyCount/* TInt */, jint maxPolys) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->queryPolygons((float*)(jobject)center, (float*)(jobject)halfExtents, _filter, polys, h_aptr<jint>(polyCount), maxPolys);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_queryPolygons(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jobject center, jobject halfExtents, jobject filter/* QueryFilter */, jobject query/* PolyQuery */) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
cache__h_c_PolyQuery(__env);
	PolyQuery *_query = (PolyQuery*)__env->GetLongField(query, __h_f_PolyQuery_this);
auto ___retvalue = _this->queryPolygons((float*)(jobject)center, (float*)(jobject)halfExtents, _filter, _query);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_findLocalNeighbourhood(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint startRef, jobject centerPos, jfloat radius, jobject filter/* QueryFilter */, jint* resultRef, jint* resultParent, jintArray resultCount/* TInt */, jint maxResult) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->findLocalNeighbourhood(startRef, (float*)(jobject)centerPos, radius, _filter, resultRef, resultParent, h_aptr<jint>(resultCount), maxResult);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_moveAlongSurface(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint startRef, jobject startPos, jobject endPos, jobject filter/* QueryFilter */, jobject resultPos, jint* visited, jintArray visitedCount/* TInt */, jint maxVisitedSize) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->moveAlongSurface(startRef, (float*)(jobject)startPos, (float*)(jobject)endPos, _filter, (float*)(jobject)resultPos, visited, h_aptr<jint>(visitedCount), maxVisitedSize);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_findDistanceToWall(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint startRef, jfloatArray centerPos/* TFloat */, jfloat maxRadius, jobject filter/* QueryFilter */, jfloatArray hitDist/* TFloat */, jfloatArray hitPos/* TFloat */, jfloatArray hitNormal/* TFloat */) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->findDistanceToWall(startRef, h_aptr</*CONST*/ jfloat>(centerPos), maxRadius, _filter, h_aptr<jfloat>(hitDist), h_aptr<jfloat>(hitPos), h_aptr<jfloat>(hitNormal));
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_getPolyWallSegments(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint ref, jobject filter/* QueryFilter */, jfloatArray segmentVerts/* TFloat */, jint* segmentRefs, jintArray segmentCount/* TInt */, jint maxSegments) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->getPolyWallSegments(ref, _filter, h_aptr<jfloat>(segmentVerts), segmentRefs, h_aptr<jint>(segmentCount), maxSegments);
	return (___retvalue);
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_closestPointOnPoly(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint ref, jfloatArray pos/* TFloat */, jfloatArray closest/* TFloat */, jarray posOverPoly/* TBool */) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	return _this->closestPointOnPoly(ref, h_aptr</*CONST*/ jfloat>(pos), h_aptr<jfloat>(closest), h_aptr<jboolean>(posOverPoly));
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_closestPointOnPolyBoundary(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint ref, jfloatArray pos/* TFloat */, jfloatArray closest/* TFloat */) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	return _this->closestPointOnPolyBoundary(ref, h_aptr</*CONST*/ jfloat>(pos), h_aptr<jfloat>(closest));
}

JNIEXPORT jint JNICALL Java_recast_NavMeshQuery_getPolyHeight(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint ref, jfloatArray pos/* TFloat */, jfloatArray height/* TFloat */) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	return _this->getPolyHeight(ref, h_aptr</*CONST*/ jfloat>(pos), h_aptr<jfloat>(height));
}

JNIEXPORT jboolean JNICALL Java_recast_NavMeshQuery_isValidPolyRef(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint ref, jobject filter/* QueryFilter */) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	cache__h_c_QueryFilter(__env);
	QueryFilter *_filter = (QueryFilter*)__env->GetLongField(filter, __h_f_QueryFilter_this);
auto ___retvalue = _this->isValidPolyRef(ref, _filter);
	return (___retvalue);
}

JNIEXPORT jboolean JNICALL Java_recast_NavMeshQuery_isInClosedList(JNIEnv *__env, jobject _obj/* NavMeshQuery */, jint ref) {
	cache__h_c_NavMeshQuery(__env);
	dtNavMeshQuery *_this = (dtNavMeshQuery*)__env->GetLongField(_obj, __h_f_NavMeshQuery_this);
	return _this->isInClosedList(ref);
}

JNIEXPORT void JNICALL Java_recast_Detour_free(JNIEnv *__env, void* ptr) {
	cache__h_c_Detour(__env);
	Detour *_this = (Detour*)__env->GetLongField(_obj, __h_f_Detour_this);
	dtFree(ptr);
}

JNIEXPORT void JNICALL Java_recast_PerformanceTimer_nativeNew(JNIEnv *__env, jobject _obj/* PerformanceTimer */) {
	cache__h_c_PerformanceTimer(__env);
	auto *_this = (new PerformanceTimer());
	__env->SetLongField(_obj, __h_f_PerformanceTimer_this, (long long)_this);
}

JNIEXPORT void JNICALL Java_recast_PerformanceTimer_start(JNIEnv *__env, jobject _obj/* PerformanceTimer */) {
	cache__h_c_PerformanceTimer(__env);
	PerformanceTimer *_this = (PerformanceTimer*)__env->GetLongField(_obj, __h_f_PerformanceTimer_this);
	_this->start();
}

JNIEXPORT void JNICALL Java_recast_PerformanceTimer_stop(JNIEnv *__env, jobject _obj/* PerformanceTimer */) {
	cache__h_c_PerformanceTimer(__env);
	PerformanceTimer *_this = (PerformanceTimer*)__env->GetLongField(_obj, __h_f_PerformanceTimer_this);
	_this->stop();
}

JNIEXPORT jdouble JNICALL Java_recast_PerformanceTimer_deltaSeconds(JNIEnv *__env, jobject _obj/* PerformanceTimer */) {
	cache__h_c_PerformanceTimer(__env);
	PerformanceTimer *_this = (PerformanceTimer*)__env->GetLongField(_obj, __h_f_PerformanceTimer_this);
	return _this->deltaSeconds();
}

JNIEXPORT jdouble JNICALL Java_recast_PerformanceTimer_deltaMilliseconds(JNIEnv *__env, jobject _obj/* PerformanceTimer */) {
	cache__h_c_PerformanceTimer(__env);
	PerformanceTimer *_this = (PerformanceTimer*)__env->GetLongField(_obj, __h_f_PerformanceTimer_this);
	return _this->deltaMilliseconds();
}

JNIEXPORT jdouble JNICALL Java_recast_PerformanceTimer_deltaMicroseconds(JNIEnv *__env, jobject _obj/* PerformanceTimer */) {
	cache__h_c_PerformanceTimer(__env);
	PerformanceTimer *_this = (PerformanceTimer*)__env->GetLongField(_obj, __h_f_PerformanceTimer_this);
	return _this->deltaMicroseconds();
}

JNIEXPORT void JNICALL Java_recast_Tests_setToZero(JNIEnv *__env, jint* a, jfloat* b, jdouble* c, jboolean* d) {
	cache__h_c_Tests(__env);
	Tests *_this = (Tests*)__env->GetLongField(_obj, __h_f_Tests_this);
	setToZero(a, b, c, d);
}

JNIEXPORT void JNICALL Java_recast_Tests_setToOne(JNIEnv *__env, jint* a, jfloat* b, jdouble* c, jboolean* d) {
	cache__h_c_Tests(__env);
	Tests *_this = (Tests*)__env->GetLongField(_obj, __h_f_Tests_this);
	setToOne(a, b, c, d);
}

JNIEXPORT void JNICALL Java_recast_MeshCapture_nativeNew(JNIEnv *__env, jobject _obj/* MeshCapture */, jboolean isSurface) {
	cache__h_c_MeshCapture(__env);
	auto *_this = (new dtMeshCapture(isSurface));
	__env->SetLongField(_obj, __h_f_MeshCapture_this, (long long)_this);
}

JNIEXPORT void JNICALL Java_recast_MeshCapture_captureNavMesh(JNIEnv *__env, jobject _obj/* MeshCapture */, jobject nm/* NavMesh */, jint flags) {
	cache__h_c_MeshCapture(__env);
	dtMeshCapture *_this = (dtMeshCapture*)__env->GetLongField(_obj, __h_f_MeshCapture_this);
	cache__h_c_NavMesh(__env);
	NavMesh *_nm = (NavMesh*)__env->GetLongField(nm, __h_f_NavMesh_this);
_this->captureNavMesh(*_nm, flags);
}

JNIEXPORT void JNICALL Java_recast_MeshCapture_captureHeightField(JNIEnv *__env, jobject _obj/* MeshCapture */, jobject hf/* Heightfield */) {
	cache__h_c_MeshCapture(__env);
	dtMeshCapture *_this = (dtMeshCapture*)__env->GetLongField(_obj, __h_f_MeshCapture_this);
	cache__h_c_Heightfield(__env);
	Heightfield *_hf = (Heightfield*)__env->GetLongField(hf, __h_f_Heightfield_this);
_this->captureHeightField(*_hf);
}

JNIEXPORT void JNICALL Java_recast_MeshCapture_captureCompactHeightField(JNIEnv *__env, jobject _obj/* MeshCapture */, jobject chf/* CompactHeightfield */) {
	cache__h_c_MeshCapture(__env);
	dtMeshCapture *_this = (dtMeshCapture*)__env->GetLongField(_obj, __h_f_MeshCapture_this);
	cache__h_c_CompactHeightfield(__env);
	CompactHeightfield *_chf = (CompactHeightfield*)__env->GetLongField(chf, __h_f_CompactHeightfield_this);
_this->captureCompactHeightField(*_chf);
}

JNIEXPORT void JNICALL Java_recast_MeshCapture_captureHeighfieldLayerSet(JNIEnv *__env, jobject _obj/* MeshCapture */, jobject hfls/* HeightfieldLayerSet */) {
	cache__h_c_MeshCapture(__env);
	dtMeshCapture *_this = (dtMeshCapture*)__env->GetLongField(_obj, __h_f_MeshCapture_this);
	cache__h_c_HeightfieldLayerSet(__env);
	HeightfieldLayerSet *_hfls = (HeightfieldLayerSet*)__env->GetLongField(hfls, __h_f_HeightfieldLayerSet_this);
_this->captureHeighfieldLayerSet(*_hfls);
}

JNIEXPORT jint JNICALL Java_recast_MeshCapture_numVerts(JNIEnv *__env, jobject _obj/* MeshCapture */) {
	cache__h_c_MeshCapture(__env);
	dtMeshCapture *_this = (dtMeshCapture*)__env->GetLongField(_obj, __h_f_MeshCapture_this);
	return _this->numVerts();
}

JNIEXPORT jint JNICALL Java_recast_MeshCapture_numSurfacePrims(JNIEnv *__env, jobject _obj/* MeshCapture */) {
	cache__h_c_MeshCapture(__env);
	dtMeshCapture *_this = (dtMeshCapture*)__env->GetLongField(_obj, __h_f_MeshCapture_this);
	return _this->numSurfacePrims();
}

JNIEXPORT jboolean JNICALL Java_recast_MeshCapture_isSurface(JNIEnv *__env, jobject _obj/* MeshCapture */) {
	cache__h_c_MeshCapture(__env);
	dtMeshCapture *_this = (dtMeshCapture*)__env->GetLongField(_obj, __h_f_MeshCapture_this);
	return _this->isSurface();
}

JNIEXPORT void JNICALL Java_recast_MeshCapture_getVert(JNIEnv *__env, jobject _obj/* MeshCapture */, jint idx, jobject pos) {
	cache__h_c_MeshCapture(__env);
	dtMeshCapture *_this = (dtMeshCapture*)__env->GetLongField(_obj, __h_f_MeshCapture_this);
	_this->getVert(idx, (float*)(jobject)pos);
}

JNIEXPORT void JNICALL Java_recast_NavTileConverter_nativeNew(JNIEnv *__env, jobject _obj/* NavTileConverter */) {
	cache__h_c_NavTileConverter(__env);
	auto *_this = (new NavTileConverter());
	__env->SetLongField(_obj, __h_f_NavTileConverter_this, (long long)_this);
}

JNIEXPORT void JNICALL Java_recast_NavTileConverter_setUnits(JNIEnv *__env, jobject _obj/* NavTileConverter */, jdouble cellSize, jdouble cellHeight) {
	cache__h_c_NavTileConverter(__env);
	NavTileConverter *_this = (NavTileConverter*)__env->GetLongField(_obj, __h_f_NavTileConverter_this);
	_this->setUnits(cellSize, cellHeight);
}

JNIEXPORT void JNICALL Java_recast_NavTileConverter_buildF(JNIEnv *__env, jobject _obj/* NavTileConverter */, jobject verts, jint totalVerts, jint polyCount, jobject polyVertCount, jobject polyType, jobject polyFlags) {
	cache__h_c_NavTileConverter(__env);
	NavTileConverter *_this = (NavTileConverter*)__env->GetLongField(_obj, __h_f_NavTileConverter_this);
	_this->buildF((jfloat*)verts, totalVerts, polyCount, (jint*)polyVertCount, (jint*)polyType, (jint*)polyFlags);
}

JNIEXPORT void JNICALL Java_recast_NavTileConverter_setLocation(JNIEnv *__env, jobject _obj/* NavTileConverter */, jint x, jint y, jint layer, jobject min, jobject max) {
	cache__h_c_NavTileConverter(__env);
	NavTileConverter *_this = (NavTileConverter*)__env->GetLongField(_obj, __h_f_NavTileConverter_this);
	_this->setLocation(x, y, layer, (jobject)min, (jobject)max);
}

JNIEXPORT void JNICALL Java_recast_NavTileConverter_setWalkability(JNIEnv *__env, jobject _obj/* NavTileConverter */, jfloat walkableHeight, jfloat walkableRadius, jfloat walkableClimb) {
	cache__h_c_NavTileConverter(__env);
	NavTileConverter *_this = (NavTileConverter*)__env->GetLongField(_obj, __h_f_NavTileConverter_this);
	_this->setWalkability(walkableHeight, walkableRadius, walkableClimb);
}

JNIEXPORT jobject JNICALL Java_recast_NavTileConverter_getTileData(JNIEnv *__env, jobject _obj/* NavTileConverter */) {
	cache__h_c_NavTileConverter(__env);
	NavTileConverter *_this = (NavTileConverter*)__env->GetLongField(_obj, __h_f_NavTileConverter_this);
	auto ___retvalue = (_this->getTileData());
cache__h_c_NavBuffer(__env);
	auto _new_obj = __env->NewObject( __h_c_NavBuffer, __h_m_NavBuffer_ctor);
	__env->SetLongField(_new_obj, __h_f_NavBuffer_this, (long long)___retvalue);
	return _new_obj;
}

}
