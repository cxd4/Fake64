struct cont_state {
	uint8 status;
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

#define BPRESS(a)	(1<<a)
#define BRELEASE(a)	(0xffff-(1<<a))

#define BUTTON_A	4
#define BUTTON_B	5
#define BUTTON_Z	6
#define BUTTON_START	7
#define BUTTON_UP	3
#define BUTTON_DOWN	2
#define BUTTON_LEFT	1
#define BUTTON_RIGHT	0

#define BUTTON_PLEFT	14
#define BUTTON_PRIGHT	15
#define BUTTON_CUP	8
#define BUTTON_CDOWN	9
#define BUTTON_CLEFT	10
#define BUTTON_CRIGHT	11
