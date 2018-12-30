#include <general.h>
#include <decode.h>
#include "registers.h"

extern cpu_instruction *dcpu_instr;
extern cpu_instruction *ecpu_instr;
extern struct cpu_reg *reg;
uint32 *cpuop;

void fpu_test_s(uint32, float, float, float, int32);

void perform_truncws() {

	uint32 op;

	op = encodeop(17, 16, 0, 2, 4, 13, 0, 0);

	*cpuop = op;

	fpu_test_s(op, 1.234, 2.345, 3.456, 2);

	fpu_test_s(op, (float)rand(), (float)rand(), (float)rand(), 0); 
}

void fpu_test_s(uint32 op, float f1, float f2, float f3, int32 exp) {

	*(float*)(&reg->gpr1[1]) = f1;
	*(float*)(&reg->gpr1[2]) = f2;
	*(float*)(&reg->gpr1[3]) = f3;

	ecpu_instr[opcode(op)>>26]();

	if (reg->gpr1[4] != exp) {
		printf("Unexpected result: \n");
		dcpu_instr[opcode(op)>>26]();
		printf("$1: %f $2: %f $3: %f\n", f1, f2, f3);
		printf("Got: %i Expected: %i\n", reg->gpr1[4], exp);
	}
}
