#ifndef CONTROLLER_H
#define CONTROLLER_H

struct cont_state {
	uint8 status;
	uint8 pack;
	union {
		uint16 buttons;
		uint8 braw[2];
	} bset;
	uint8 stick_x;
	uint8 stick_y;
	uint8 keymap[0xff];
	uint8 xstickmap[0xff];
	uint8 ystickmap[0xff];
};

#define CONT_UNKNOWN 0
#define CONT_CONNECTED 1

#define PACK_NONE 0x02
#define PACK_PRESENT 0x01

#define BPRESS(a)	(1<<a)
#define BRELEASE(a)	(0xffff-(1<<a))

#define BUTTON_A	7
#define BUTTON_B	6
#define BUTTON_Z	5
#define BUTTON_START	4
#define BUTTON_UP	3
#define BUTTON_DOWN	2
#define BUTTON_LEFT	1
#define BUTTON_RIGHT	0

#define BUTTON_PLEFT	13
#define BUTTON_PRIGHT	12
#define BUTTON_CUP	11
#define BUTTON_CDOWN	10
#define BUTTON_CLEFT	9
#define BUTTON_CRIGHT	8
#endif
