/* typedef enum {r0, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7,        s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, s8, ra} \
        reg_named; */

char* reg_names[] = {"r0", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0",
        "t1", "t2", "t3", "t4", "t5", "t6", "t7", "s0", "s1", "s2",
        "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp",
        "sp", "s8", "ra"};

char* cop0_names[] = {"Index", "Random", "EntryLo0", "EntryLo1", "Context",
		"Page Mask", "Wired", "", "BadVAddr", "Count", "EntryHi",
		"Compare", "Status", "Cause", "EPC", "PRId", "LLAddr",
		"WatchLo", "WatchHi", "XContext", "", "", "", "", "",
		"Parity Error", "CacheErr", "TagLo", "TagHi", "ErrorEPC", ""};

char* cop1_names[] = {"f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8",
	"f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18",
	"f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28",
	"f29", "f30", "f31", "f32" };
