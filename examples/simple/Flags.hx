package examples.simple;

class Flags {
	static inline final POLYAREA_GROUND = 0;
	static inline final POLYAREA_WATER = 1;
	static inline final POLYAREA_ROAD = 2;
	static inline final POLYAREA_DOOR = 3;
	static inline final POLYAREA_GRASS = 4;
	static inline final POLYAREA_JUMP = 5;

	static inline final POLYFLAGS_WALK = 0x01; // Ability to walk (ground, grass, road)
	static inline final POLYFLAGS_SWIM = 0x02; // Ability to swim (water).
	static inline final POLYFLAGS_DOOR = 0x04; // Ability to move through doors.
	static inline final POLYFLAGS_JUMP = 0x08; // Ability to jump.
	static inline final POLYFLAGS_DISABLED = 0x10; // Disabled polygon
	static inline final POLYFLAGS_ALL = 0xffff; // All abilities.
}
