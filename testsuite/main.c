#include <dlfcn.h>
#include <general.h>
#include <decode.h>
#include "registers.h"
#include <stdlib.h>
#include <time.h>

void* load_cpu_module(char*);
uint32 encodeop(int, int, int, int, int, int, int, int);
void perform_sll();
void arith_test32(uint32, uint32, uint32, uint32, uint32);

cpu_instruction *dcpu_instr;
cpu_instruction *ecpu_instr;
struct cpu_reg *reg;
uint32 *cpuop;

int main(int argc, char **argv) {

	void *module;
	FILE **disfd;

	if (argc < 2) {
		printf("Please supply cpucore module path in argv[1]\n");
		exit(0);
	}

	module = load_cpu_module(argv[1]);

	dcpu_instr = dlsym(module, "dcpu_instr");
	ecpu_instr = dlsym(module, "ecpu_instr");
	disfd = dlsym(module, "disfd");
	reg = dlsym(module, "reg");
	cpuop = dlsym(module, "op");

	if (!(dcpu_instr && ecpu_instr && disfd && reg && cpuop)) {
		printf("Couldn't resolve required symbols\n");
		exit(1);
	}

	*disfd = stderr;

	srand(time(NULL));

	perform_sll();
	perform_truncws();
}

void* load_cpu_module(char *mod) {

	void *module;

	module = dlopen(mod, RTLD_LAZY | RTLD_GLOBAL);

	if (!module) {
		printf("%s: %s\n", "Opening cpucore:", dlerror());
		exit(1);
	}

	return module;
}

uint32 encodeop(int opcode, int rs, int rt, int rd, int sa, int funct, int code, int cc) {

	uint32 op;

	op = 0;

	if (opcode) {
		op |= opcode<<26;
	}

	if (rs) {
		op |= rs<<21;
	}

	if (rt) {
		op |= rt<<16;
	}

	if (rd) {
		op |= rd<<11;
	}

	if (sa) {
		op |= sa<<6;
	}

	if (funct) {
		op |= funct;
	}

	if (code) {
		op |= code<<6;
	}

	if (cc) {
		op |= cc<<8;
	}

	return op;
}

void perform_sll() {

	uint32 op;

	uint32 val;
	int shifta, i;

	printf("Testing SLL:\n");

	for (shifta = 0; shifta < 32; shifta++) {
		op = encodeop(0, 0, 1, 4, shifta, 0, 0, 0);
		*cpuop = op;

		arith_test32(op, 0x1, 0, 0, (0x1 << shifta));
		arith_test32(op, 0xffffffff, 0, 0, (0xffffffff << shifta));
		arith_test32(op, 0x0, 0, 0, 0);

		for (i = 0; i < 10000; i++) {
			val = rand()<<1;
			arith_test32(op, val, 0, 0, (val << shifta));
		}
	}

	printf("SLL complete\n");

}

void arith_test32(uint32 op, uint32 gpr1, uint32 gpr2, uint32 gpr3, uint32 exp) {

	reg->gpr[1] = gpr1;
	reg->gpr[2] = gpr2;
	reg->gpr[3] = gpr3;

	ecpu_instr[opcode(op)>>26]();

	if ((uint32)reg->gpr[4] != exp) {
		printf("Unexpected result: \n");
		dcpu_instr[opcode(op)>>26]();
		printf("$1: %#.8x $2: %#.8x $3: %#.8x\n", gpr1, gpr2, gpr3);
		printf("Got: %#.8x Expected: %#.8x\n", reg->gpr[4], exp);
	}
}
