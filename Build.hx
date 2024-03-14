package;

import idl.Options;

class Build extends idl.BuildBase{
	// Put any necessary includes in this string and they will be added to the generated files


	var options = {
		idlFile: "lib/recast/recast.idl",
		target: null,
		packageName: "recast",
		nativeLib: "recast",
		outputDir: "src",
		includeCode: null,
		autoGC: true,
		defaultConfig: "Release",
		architecture: ArchAll
	};

	function new() {
		super(options);
	}

	override function getJVMInclude() return "
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
override function getHLInclude() return "
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


	static var hlDir = null;
	public static function main() {
		var b = new Build();
					case "--hl":
						hlDir = args.shift();

		b.parseArgs();

	}
}
