package recast;

typedef Native = haxe.macro.MacroType<[
	idl.Module.build({
		idlFile: "recast/recast.idl",
        target: "hl",
        packageName: "recast",
		autoGC: true,
		nativeLib: "recast"
	})
]>;
