/* Notes on use of printd:

As always this is open for discussion, but i lifted most of it off syslog :)
so it should be ok.

Use of levels:
	debug - debugging info only useful when working on code in question
	info - stuff which is part of normal operation but worth printing
	notice - something unexpected, not necessarily bad
	warn - something unexpected, probably bad
	error - definitely bad
	crit - a really serious error
	emerg - an error which may cause the emu to become unstable/unusable
	fatal - unable to continue, exiting
*/

#ifndef PRINTD_H
#define PRINTD_H
#define D_MAX_TYPE (16)

#ifndef ROMLOADER
	extern struct module_info *modules;
#endif

#define printd modules->printd_f
#define test_debug modules->test_debug_f

typedef enum {
	D_DEBUG,
	D_INFO,
	D_NOTICE,
	D_WARN,
	D_ERROR,
	D_CRIT,
	D_EMERG,
	D_FATAL,
	D_MAX
} debug_level;

typedef enum {
	D_USER,
	D_DISASS,
	D_RSP,
	D_RDP,
	D_RCP,
	D_MI,
	D_VI,
	D_AI,
	D_PI,
	D_RI,
	D_SI,
	D_VIDEO,
	D_AUDIO,
	D_INPUT,
	D_CPU
} debug_type;

extern int _printd(debug_type, debug_level, char*, ...);
extern void set_debug(debug_type, debug_level);
extern int _test_debug(debug_type, debug_level);

#endif
