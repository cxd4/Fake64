#include <printd.h>
#include <stdarg.h>
#include <stdio.h>

#ifndef STATIC_MODULES
#include "config.h"
#endif

debug_level debug_settings[D_MAX_TYPE];
FILE* debug_fds[D_MAX_TYPE];

void init_debugging(void)
{
	int i;

	for (i = 0; i < D_MAX_TYPE; i++) {
		debug_fds[i] = stdout;
#ifdef DEBUG
		debug_settings[i] = D_INFO;
#else
		debug_settings[i] = D_ERROR;
#endif
	}
}

void set_debug(debug_type type, debug_level level) {

//	printf("%i:%i\n", type, level);

	debug_settings[type] = level;

}

int _test_debug(debug_type type, debug_level level) {

//	printf("%i:%i:%i\n", debug_settings[type], level, (debug_settings[type] <= level));
	return (debug_settings[type] <= level);
}

int _printd(debug_type type, debug_level level, char *format, ...) {

#ifdef DEBUG

	va_list arg;
	int done;

//	printf("%i:%i:%i\n", type, level, debug_settings[type]);
//	printf("%x\n", debug_fds[type]);

	if (debug_settings[type] <= level) {

		va_start(arg, format);
		done = vfprintf(debug_fds[type], format, arg);
		va_end(arg);

		return done;
	} else {
		return 0;
	}

#endif
}
