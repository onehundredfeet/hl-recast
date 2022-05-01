package recast;
import recast.Native;

class Status {
	// High level status.
	public static inline final FAILURE = 1 << 31; // Operation failed.
	public static inline final SUCCESS = 1 << 30; // Operation succeed.
	public static inline final IN_PROGRESS = 1 << 29; // Operation still in progress.

	// Detail information for status.
	public static inline final STATUS_DETAIL_MASK = 0x0ffffff;
	public static inline final WRONG_MAGIC = 1 << 0; // Input data is not recognized.
	public static inline final WRONG_VERSION = 1 << 1; // Input data is in wrong version.
	public static inline final OUT_OF_MEMORY = 1 << 2; // Operation ran out of memory.
	public static inline final INVALID_PARAM = 1 << 3; // An input parameter was invalid.
	public static inline final BUFFER_TOO_SMALL = 1 << 4; // Result buffer for the query was too small to store all results.
	public static inline final OUT_OF_NODES = 1 << 5; // Query ran out of nodes during search.
	public static inline final PARTIAL_RESULT = 1 << 6; // Query did not reach the end location, returning best guess.
	public static inline final ALREADY_OCCUPIED = 1 << 7; // A tile has already been assigned to the given x,y coordinate

    public inline static function setFailure( status : Int ) : Int {
        status = (status & ~(SUCCESS | IN_PROGRESS));
        status |= FAILURE;
        return status;
    }

	// Returns true of status is success.
	public inline static function isSuccess( status : Int ):Bool {
		//        printf("Status is %x %d\n", status, status);
		return (status & SUCCESS) != 0;
	}

	// Returns true of status is failure.
	public inline static function isFailure(status:Int) :Bool {
		return (status & FAILURE) != 0;
	}

	// Returns true of status is in progress.
	public inline static function isInProgress(status:Int) :Bool {
		return (status & IN_PROGRESS) != 0;
	}

	// Returns true if specific detail is set.
	public inline static function hasDetail(status:Int, detail: Int) :Bool{
		return (status & detail) != 0;
	}

	public static function statusString(status:Int) :String {
		var str = null;
		if (status & SUCCESS != 0) {
			str = "Success";
		} else if (status & IN_PROGRESS != 0) {
			str = "In Progress";
		} else if (status & FAILURE != 0) {
			str = "Failure";
			if (status & WRONG_MAGIC != 0)str += " :Wrong magic number";
			if (status & WRONG_VERSION != 0)str += " :Wrong version";
			if (status & OUT_OF_MEMORY != 0)str += " :Out of memory";
			if (status & INVALID_PARAM != 0)str += " :Invalid parameter";
			if (status & BUFFER_TOO_SMALL != 0)str += " :Buffer too small";
			if (status & OUT_OF_NODES != 0)str += " :Out of nodes";
			if (status & PARTIAL_RESULT != 0)str += " :Partial result";
			if (status & ALREADY_OCCUPIED != 0)str += " :Already occupied";
		} else 
			throw "unrecognized status code";

		return str;
	}
}
