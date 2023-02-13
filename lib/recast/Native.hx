package recast;

typedef Native = haxe.macro.MacroType<[
	idl.Module.build({
		idlFile: "recast/recast.idl",
        target: #if hl "hl" #else "jvm" #end,
        packageName: "recast",
		autoGC: true,
		nativeLib: "recast"
	})
]>;
