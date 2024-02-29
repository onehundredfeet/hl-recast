package;

class Build {
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

	static function generate() {
		options.target =  switch (target) {
			case "hl": idl.Options.Target.TargetHL;
			case "jvm": idl.Options.Target.TargetJVM;
			default: idl.Options.Target.TargetHL;
		};
		options.includeCode = switch (target) {
			case idl.Options.Target.TargetHL: HL_INCLUDE;
			case idl.Options.Target.TargetJVM: JVM_INCLUDE;
			default: "";
		};

		options.outputDir = "src";

		trace('Generating target ${target}');
		idl.generator.Generate.generateCpp(options);
	}

	static var target = "hl";
	static var builder = "Ninja";
	static var arch = "x86_64";
    static var idlPath = "ext/hl-idl";
    static var config = "Debug";

	static var installDir = null;

    static function cmake(){
    }
	static function build() {
		sys.FileSystem.createDirectory('build/${target}/${arch}/${config}');
		sys.FileSystem.createDirectory(installDir);
        var cmd = 'cmake -G"${builder}" -DTARGET_ARCH=${arch} -DPATH_TO_IDL=${idlPath} -DTARGET_HOST=${target} -DCMAKE_BUILD_TYPE=${config} -DCMAKE_INSTALL_PREFIX=${installDir} -B build/${target}/${arch}/${config}';
        trace('$cmd');
        Sys.command(cmd);

        cmd = 'cmake --build build/${target}/${arch}/${config}';
        trace('$cmd');
        Sys.command(cmd);
	}

	static function install() {

		var cmd = 'cmake --install build/${target}/${arch}/${config}';
		trace('$cmd');
		Sys.command(cmd);
	}

	public static function main() {
		var args = Sys.args();

		if (args.length > 0) {
			var cmd = args.shift();

            while (args.length > 0) {
                var arg = args.shift();
                switch (arg) {
                    case "--target":
                        target = args.shift();
                    case "--builder":
                        builder = args.shift();
                    case "--arch":
                        arch = args.shift();
                    case "--idl":
                        idlPath = args.shift();
                    case "--config":
                        config = args.shift();
					case "--dir":
						installDir = args.shift();
                    default:
                }
            }
			if (installDir == null) {
				installDir = 'installed/${target}/${arch}/${config}';
			}
			switch (cmd) {
				case "generate":
					generate();
				case "build":
					build();
				case "install":
					install();
				default:
			}
		} else {
            trace("Usage: haxe config.hxml [generate|build|install] [options]");
			trace("  generate: Generate the target code");
			trace("  build: Build the target code");
			trace("  install: Install the target code");
			trace("  --target: The target platform (hl, jvm)");
			trace("  --builder: The build system (Ninja, Make)");
			trace("  --arch: The target architecture (x86_64, arm64)");
			trace("  --idl: The path to the idl directory");
			trace("  --config: The build configuration (Debug, Release)");
        }
	}
}
