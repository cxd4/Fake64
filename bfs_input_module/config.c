#ifndef WIN32
	#include <config.h>
#endif
#include <general.h>
#include <controller.h>

extern int inited;
extern struct cont_state conts[8];
extern int joys_connected;
extern int debug_pif;
void reset_keymaps();


static char *button_names[16] = {
        "right", "left", "down", "up", "start", "z", "b", "a", "cright",
        "cleft", "cdown", "cup", "pright", "pleft", "tac", "ym reef" };

void config_module(char* conf) {

        int conflen, i;
        char *cont, *button, *scancode;
        int co, buttonid, scan;

        conflen = strlen(conf);

        for (i = 0; i < conflen; i++) {
                if (conf[i] == ' ') { conf[i] = 0; }
        }

        if (!strcasecmp(conf, "mapkey")) {
                if (!inited) { reset_keymaps(); inited = 1; }
                cont = conf + 7;
                button = cont + strlen(cont) + 1;
                scancode = button + strlen(button) + 1;

                co = strtoul(cont, NULL, 10);
                scan = strtoul(scancode, NULL, 10);

                buttonid = 0;

                for (i = 0; i < 16; i++) {
                        if (!strcasecmp(button, button_names[i])) {
                                buttonid = i;
                        }
                }

                if (scan) {
                        conts[co].keymap[scan] = buttonid;
                }
        } else if (!strcasecmp(conf, "resetmaps")) {
                reset_keymaps();
                inited = 1;
        } else if (!strncasecmp(conf, "mapstick", 8)) {
                cont = conf + 10;
                button = cont + strlen(cont) + 1;
                scancode = button + strlen(button) + 1;
                co = strtoul(cont, NULL, 10);
                buttonid = strtoul(button, NULL, 0);
                scan = strtoul(scancode, NULL, 10);

                if (conf[8] == 'x') {
                        conts[co].xstickmap[scan] = buttonid;
                } else if (conf[8] == 'y') {
                        conts[co].ystickmap[scan] = buttonid;
                }
        } else if (!strncasecmp(conf, "controllers", 11)) {
                cont = conf + 12;
                co = strtoul(cont, NULL, 10);
                joys_connected = co;
        } else if (!strncasecmp(conf, "debug", 5)) {
		cont = conf + 6;
		if (!strncasecmp(cont, "pif", 3)) {
	                debug_pif = 1;
		}
        }
}
