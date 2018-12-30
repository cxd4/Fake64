#include <general.h>
#include "decode.h"
#include "opcodes.h"
#include "registers.h"

extern struct cpu_reg reg;
extern cpu_instruction ecpu_cop1[32];
extern cpu_instruction ecpu_cop1_s[64];
extern cpu_instruction ecpu_cop1_d[64];
extern cpu_instruction ecpu_cop1_wl[64];

int eCPU_COP1(uint32 op) {
  ecpu_cop1[base(op)](op);
}

int eCPU_COP1_S(uint32 op) {
  ecpu_cop1_s[special(op)](op);
}

int eCPU_COP1_D(uint32 op) {
  ecpu_cop1_d[special(op)](op);
}

int eCPU_COP1_WL(uint32 op) {
  ecpu_cop1_wl[special(op)](op);
}

int eCPU_COP1_CFC1(uint32 op) {
  reg.gpr[rt(op)]=(long long)reg.fcr[fs(op)];
}

int eCPU_COP1_CTC1(uint32 op) {
 reg.fcr[fs(op)]=(double)reg.gpr[rt(op)];
}

int eCPU_COP1_C_F(uint32 op) {
 reg.fcr[31]&=~0x00800000;
}
