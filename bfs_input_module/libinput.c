#include <config.h>
#include <general.h>
#include <SDL.h>
#include <controller.h>

#define KEY_EVENTMASK (SDL_KEYDOWNMASK | SDL_KEYUPMASK)

static char *button_names[16] = {
	"right", "left", "down", "up", "a", "b", "z", "start", "cup", "cdown",
	"cleft", "cright", "unf unf", "xphantom", "pleft", "pright" };

uint8 *PIFMEM;
uint8 *PIFRAM;

int inited = 0;
int joys_connected = 1;

struct cont_state conts[8];

extern int run;

void init_keymap();
void reset_keymaps();

#ifndef GPROF

char *module_id(void) {
	return("Bluefyre's basic input core (SDL)");
}

#endif

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
	}
}

void init_SDL() {

	int ret;

	ret = SDL_Init(SDL_INIT_EVENTTHREAD | SDL_INIT_EVERYTHING | SDL_INIT_NOPARACHUTE);
	SDL_EnableUNICODE(1);

}

void init_Joys(int number) {

	int i;

	printf("%i joystick(s) initialised\n", number);

	for (i = 0; i < 8; i++) {
		if (i < number) {
			conts[i].status = CONT_CONNECTED;
		}
	}
}

void reset_keymaps() {

	int i;

	for (i = 0; i < 8; i++) {
		memset(conts[i].keymap, 0xff, 0xff);
		memset(conts[i].xstickmap, 0x00, 0xff);
		memset(conts[i].ystickmap, 0x00, 0xff);
	}
	
}

void init_keymap() {

	conts[0].keymap[98] = BUTTON_UP;
	conts[0].keymap[104] = BUTTON_DOWN;
	conts[0].keymap[100] = BUTTON_LEFT;
	conts[0].keymap[102] = BUTTON_RIGHT;

	conts[0].keymap[36] = BUTTON_START;

	conts[0].keymap[52] = BUTTON_A;
	conts[0].keymap[53] = BUTTON_B;
	conts[0].keymap[54] = BUTTON_Z;

	conts[0].ystickmap[80] = 0x40;
	conts[0].ystickmap[88] = 0xc0;

	conts[0].xstickmap[83] = 0xc0;
	conts[0].xstickmap[85] = 0x40;

	printf("Keyboard keymap initialised\n");

}

void init_pifram(uint8 *here) {

#ifdef DEBUG_PIF
	printf("Debugging PIFMEM i see ... good luck :)\n");
	printf("PIFMEM inited: %x\n", here);
#endif
	PIFMEM = here;
	PIFRAM = here + 0x07c0;

	init_SDL();
	init_Joys(joys_connected);
	if(!inited) { reset_keymaps(); init_keymap(); }
}

void translate_key(SDL_KeyboardEvent *key) {

	uint8 mkey;
	int co;

	printf("%i\n", key->keysym.scancode);

//	printf("%i:%i %i\n", key->keysym.scancode, co, conts[co].keymap[key->keysym.scancode]);

	for (co = 0; co < 8; co++) {

	switch(key->type) {
		case SDL_KEYDOWN:
			if ((mkey = conts[co].keymap[key->keysym.scancode]) != 0xff) {
				conts[co].bset.buttons |= BPRESS(mkey);
			}
			conts[co].stick_x += conts[co].xstickmap[key->keysym.scancode];
			conts[co].stick_y += conts[co].ystickmap[key->keysym.scancode];
			break;
		case SDL_KEYUP:
			if ((mkey = conts[co].keymap[key->keysym.scancode]) != 0xff) {
				conts[co].bset.buttons &= BRELEASE(mkey);
			}
			conts[co].stick_x -= conts[co].xstickmap[key->keysym.scancode];
			conts[co].stick_y -= conts[co].ystickmap[key->keysym.scancode];
			break;
		default:
			printf("Hmm?\n");
	}
	}
}

void update_state() {

	int ret;
	SDL_Event dave;

	SDL_PumpEvents();
	while(ret = SDL_PeepEvents(&dave, 1, SDL_GETEVENT, KEY_EVENTMASK)) {
		translate_key(&dave.key);
	}

	// Throw away all the other shit which clogs up the queue
	while(SDL_PeepEvents(&dave, 1, SDL_GETEVENT, SDL_ALLEVENTS));

} 

void dump_pifram() {

        int i;
        int j;

        printf("PIFMEM debug:\n");

        for (i = 0; i < 8; i++) {
                printf("Channel %i:\t", i+1);
                printf("%.2x %.2x %.2x %.2x - %.2x %.2x %.2x %.2x\n", PIFRAM[(i*8)+3], PIFRAM[(i*8)+2], PIFRAM[(i*8)+1], PIFRAM[i*8], PIFRAM[(i*8)+7], PIFRAM[(i*8)+6], PIFRAM[(i*8)+5], PIFRAM[(i*8)+4]);
        }
}

void perform_command(int co, char command) {

	switch(command) {
		case 0x01:
			PIFRAM[(co*8)+7] = conts[co].bset.braw[0];
			PIFRAM[(co*8)+6] = conts[co].bset.braw[1];
			PIFRAM[(co*8)+5] = conts[co].stick_x;
			PIFRAM[(co*8)+4] = conts[co].stick_y;
			break;
		default:
			printf("Don't know how to perform %#x on controller!\n", command);
			break;
	}
}

void pifram_interrupt() {

	int i;

#ifdef DEBUG_PIF
	dump_pifram();
#endif

/*	if (!conts[0].bset.buttons) {
		conts[0].bset.buttons = 1;
	} else {
		conts[0].bset.buttons <<= 1;
	} */

	update_state();

	for (i = 0; ((i < 8) && (PIFRAM[(i*8)+3] != 0xfe)); i++) {
		if ((((uint32*)PIFRAM)[i*2] & 0xffffff00) != 0xff010400) {
			printf("I don't handle this kind of controller access\n");
		} else {
			perform_command(i, PIFRAM[i*8]);
		}
	}

#ifdef DEBUG_PIF
	dump_pifram();
#endif
//	run=0;
}
