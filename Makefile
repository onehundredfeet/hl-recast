genhl:
	haxe -cp generator  -lib hl-idl --macro "recast.Generator.generateCpp()"
	
genjs:
	haxe -cp generator -lib hl-idl --macro "recast.Generator.generateJs()"
