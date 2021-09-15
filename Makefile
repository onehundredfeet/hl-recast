genhl:
	haxe -cp generator  -lib webidl --macro "recast.Generator.generateCpp()"
	
genjs:
	haxe -cp generator -lib webidl --macro "recast.Generator.generateJs()"
