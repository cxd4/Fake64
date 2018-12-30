#ifndef WIN32
	#include <config.h>
#endif

#include <general.h>
#include "debugger.h"
#include "config_options.h"

extern struct sdebugger debugger;

extern void set_patch(uint32, uint32);

void config_module(char* conf)
{

	int conflen, i;
	char *arg_a, *arg_b;
	int val_a, val_b;
	// Variables used for internal jiggery pokery

	conflen = strlen(conf);

	for (i = 0; i < conflen; i++) {
		if (conf[i] == ' ') { conf[i] = 0; }
	}

	if (!strcasecmp(conf, "run")) {
		arg_a = conf + 4;
		val_a = strtoul(arg_a, NULL, 0);

		debugger.run = val_a;
	} else if (!strncasecmp(conf, "cf",2)) {
		arg_a = conf + 3;
		val_a = strtoul(arg_a, NULL, 0);
		counter_factor = val_a;
		printf("Counter factor of %i\n", counter_factor);
	} else if (!strcasecmp(conf, "patch")) {
		arg_a = conf + 6;
		arg_b = arg_a + strlen(arg_a) + 1;
		val_a = strtoul(arg_a, NULL, 0);
		val_b = strtoul(arg_b, NULL, 0);
		set_patch(val_a, val_b);
	} else if (!strncasecmp(conf, "kill",3)) {
		arg_a = conf + 5;
		val_a = strtoul(arg_a, NULL, 0);
		kill_delay = val_a;
	} else if (!strncasecmp(conf, "patches",7)) {
		arg_a = conf + 8;
		val_a = strtoul(arg_a, NULL, 0);
		printf("%i\n", val_a);
		patches_active = val_a;
	}
}
