#include <string.h>
#include <stdlib.h>

#include "general.h"
#include "config_options.h"

#ifndef STATIC_MODULES
void config_module(char* conf)
{
	int i, conflen;
	char *cont;
	int co;

	conflen = strlen(conf);

	for (i = 0; i < conflen; i++) {
		if (conf[i] == ' ') { conf[i] = 0; }
	}

	if (!strcasecmp(conf, "width")) {
		cont = conf + 6;
		co = strtoul(cont, NULL, 0);
		win_width = co;
		win_scalex = (float)co / 320.0;
	} else if (!strcasecmp(conf, "height")) {
		cont = conf + 7;
		co = strtoul(cont, NULL, 0);
		win_height = co;
		win_scaley = (float)co / 240.0;
	} else if (!strcasecmp(conf, "2d")) {
		cont = conf + 3;
		co = strtoul(cont, NULL, 0);
		fb2d_active = co;
	} else if (!strcasecmp(conf, "fpslimit")) {
		cont = conf + 9;
		co = strtoul(cont, NULL, 0);
		fps_limit = co;
	} else if (!strcasecmp(conf, "fpslimiter")) {
		cont = conf + 11;
		co = strtoul(cont, NULL, 0);
		fps_limiter = co;
	} else if (!strcasecmp(conf, "rcpbreakunim")) {
		rcp_break_unim = 1;
	}
}
#endif
