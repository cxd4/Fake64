#include "general.h"

#include <SDL/SDL.h>
#include <controller.h>
#include <module.h>

#define KEY_EVENTMASK (SDL_KEYDOWNMASK | SDL_KEYUPMASK)

uint8 *PIFMEM;
uint8 *PIFRAM;

int inited = 0;
int joys_connected = 1;

struct module_info* modules;

#ifdef WIN32
	#ifdef _DEBUG 
		int debug_pif = 1;
	#else
		int debug_pif = 0;
	#endif
#else
	int debug_pif = 0;
#endif

	
struct cont_state conts[8];

void init_keymap();
void reset_keymaps();
void update_pifram();

#ifndef STATIC_MODULES
char
*module_id(void)
{
    return (
#ifdef _DEBUG
        "Bluefyre's basic SDL input core (DEBUG)"
#else
        "Bluefyre's basic SDL input core "
#endif
    );
}
#endif

void init_SDL()
{

	int ret;

	ret = SDL_Init(SDL_INIT_EVENTTHREAD | SDL_INIT_EVERYTHING | SDL_INIT_NOPARACHUTE);
	SDL_EnableUNICODE(1);

}

void init_Joys(int number) {

	int i;

	printd(D_INPUT, D_INFO, "%i joystick(s) initialised\n", number);

	for (i = 0; i < 8; i++) {
		if (i < number) {
			conts[i].status = CONT_CONNECTED;
			conts[i].pack = PACK_NONE;
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

void init_keymap()
{
#ifdef WIN32
	conts[0].keymap[72] = BUTTON_UP;			   // keypad , no numlock
	conts[0].keymap[80] = BUTTON_DOWN;
	conts[0].keymap[75] = BUTTON_LEFT;
	conts[0].keymap[77] = BUTTON_RIGHT;

	conts[0].keymap[28] = BUTTON_START;	 // return

	conts[0].keymap[44] = BUTTON_A;	   //z
	conts[0].keymap[45] = BUTTON_B;	   //x
	conts[0].keymap[46] = BUTTON_Z;	   //c

	conts[0].ystickmap[80] = 0x40;
	conts[0].ystickmap[88] = 0xc0;

	conts[0].xstickmap[83] = 0xc0;
	conts[0].xstickmap[85] = 0x40;

#else
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
#endif

	printd(D_INPUT, D_DEBUG, "Default keyboard keymap initialised\n");

}

void init_pifram(struct module_info *modptr, uint8 *here) {

	modules = modptr;

	printd(D_INPUT, D_DEBUG, "Debugging PIFMEM i see ... good luck :)\n");
	printd(D_INPUT, D_DEBUG, "PIFMEM initiated: %x\n", here);

	PIFMEM = here;
	PIFRAM = here + 0x07c0;

	init_SDL();
	init_Joys(joys_connected);
	if(!inited) { reset_keymaps(); init_keymap(); }
}

void translate_key(SDL_KeyboardEvent *key) {

	uint8 mkey;
	int co;

	printd(D_INPUT, D_DEBUG, "Key: %i\n", key->keysym.scancode);
	
	if (key->keysym.scancode == 9) { exit(0); }

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
			printd(D_INPUT, D_DEBUG, "Hmm?\n");
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

        printd(D_INPUT, D_DEBUG, "PIFMEM debug:\n");

        for (i = 0; i < 8; i++) {
                printd(D_INPUT, D_DEBUG, "Channel %i:\t", i+1);
                printd(D_INPUT, D_DEBUG, "%.2x %.2x %.2x %.2x - %.2x %.2x %.2x %.2x\n", PIFRAM[(i*8)+3], PIFRAM[(i*8)+2], PIFRAM[(i*8)+1], PIFRAM[i*8], PIFRAM[(i*8)+7], PIFRAM[(i*8)+6], PIFRAM[(i*8)+5], PIFRAM[(i*8)+4]);
        }
}

void perform_command(int co, char command, int readlen, int writelen) {

	int coerror = 0;

	if (conts[co].status != CONT_CONNECTED) {
//		printd(D_INPUT, D_DEBUG, "Controller %d supposedly not connected\n",co);
		coerror = 8;
	} else {

	switch(command) {
		case 0x00:
			if (readlen != 1 || writelen != 3) {
				coerror = 4;
			}

			PIFRAM[(co*8)+7] = 0x05; // Say no to drugs children
			PIFRAM[(co*8)+6] = 0x00;
			PIFRAM[(co*8)+5] = conts[co].pack;
// One of these ^ or v is right
//			PIFRAM[(co*8)+7] = 0x00;
//			PIFRAM[(co*8)+6] = 0x05;
//			PIFRAM[(co*8)+5] = conts[co].pack;
			break;
		case 0x01:
			if (readlen != 1 || writelen != 4) {
				coerror = 4;
			} // But apparently we perform the operation anyway

			PIFRAM[(co*8)+7] = conts[co].bset.braw[0];
			PIFRAM[(co*8)+6] = conts[co].bset.braw[1];
			PIFRAM[(co*8)+5] = conts[co].stick_x;
			PIFRAM[(co*8)+4] = conts[co].stick_y;
			break;
		case 0x03:
			if (conts[co].pack == PACK_NONE) {
				coerror = 8;
				printd(D_INPUT, D_WARN, "Rom requested mempack when one isn't plugged in\n");
			} else {
				printd(D_INPUT, D_NOTICE, "Mempacks not supported!\n");
			}
			break;
		default:
			printd(D_INPUT, D_WARN, "Don't know how to perform %#x on controller!\n", command);
			break;
	}

	}

	if (coerror) {
		PIFRAM[(co*8)+1] |= coerror << 4;
	}
}

void pifram_interrupt() {

	dump_pifram();

	update_state();

	if (*(PIFRAM+0x3c) == 0x8) {
		printd(D_INPUT, D_INFO, "Rom is using input ...\n");
	} else {
		update_pifram();
	}

	*(PIFRAM+0x3c) = 0x00;

	dump_pifram();
}

void update_pifram() {

	int i;
	uint8 ffb, bread, bwrite, cmd;

	for (i = 0; ((i < 8) && (PIFRAM[(i*8)+3] != 0xfe)); i++) {
		ffb = PIFRAM[(i*8)+3];
		bread = PIFRAM[(i*8)+2];
		bwrite = PIFRAM[(i*8)+1];
		cmd = PIFRAM[(i*8)];

		if (ffb == 0xff) {
			perform_command(i, PIFRAM[i*8], bread, bwrite);
		}
	}
}

