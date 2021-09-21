package recast;

typedef Native = haxe.macro.MacroType<[webidl.Module.build({ idlFile : "generator/recast.idl", chopPrefix : "rc", autoGC : true, nativeLib : "recast" })]>;
