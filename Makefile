genhl:
	haxe -cp generator  -lib webidl --macro "recast.Generator.generateCpp()"
	