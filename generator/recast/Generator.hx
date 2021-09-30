package recast;

#if eval
class Generator {

	// Put any necessary includes in this string and they will be added to the generated files
	static var INCLUDE = "
#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif
#include <Recast.h>
#include <RecastAlloc.h>
#include <RecastAssert.h>

#include <DetourAlloc.h>
#include <DetourAssert.h>
#include <DetourCommon.h>
#include <DetourMath.h>
#include <DetourNavMesh.h>
#include <DetourNavMeshBuilder.h>
#include <DetourNavMeshQuery.h>
#include <DetourNode.h>
#include <DetourStatus.h>

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

";
	
	public static function generateCpp() {	
		var options = { idlFile : "generator/recast.idl", nativeLib : "recast", outputDir : "src", includeCode : INCLUDE, autoGC : true };
		webidl.Generate.generateCpp(options);
	}

}
#end