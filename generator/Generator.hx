package ;

#if eval
class Generator {
	// Put any necessary includes in this string and they will be added to the generated files
	static var HL_INCLUDE = "
#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif
#include <Recast.h>
#include <RecastAlloc.h>
#include <RecastAssert.h>
#include \"ChunkyTriMesh.h\"

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
#include \"hl-recast.h\"

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

static var options = {
	idlFile: "lib/recast/recast.idl",
	target: null,
	packageName: "recast",
	nativeLib: "recast",
	outputDir: "src",
	includeCode: null,
	autoGC: true
};

static final JVM_INCLUDE = "
#include <Recast.h>
#include <RecastAlloc.h>
#include <RecastAssert.h>
#include \"ChunkyTriMesh.h\"

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
#include \"jvm-recast.h\"

";

public static function generateCpp(target = idl.Options.Target.TargetHL) {
	options.target = target;
	options.includeCode = switch (target) {
		case idl.Options.Target.TargetHL: HL_INCLUDE;
		case idl.Options.Target.TargetJVM: JVM_INCLUDE;
		default: "";
	};
	idl.generator.Generate.generateCpp(options);
}
}
#end
