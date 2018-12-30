#include <general.h>
#include "decode.h"
#include "opcodes.h"
#include "registers.h"
#include <memory.h>

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
  reg.gpr[rt(op)]=reg.gpr1[fs(op)];
}

int eCPU_COP1_CTC1(uint32 op) {
 reg.gpr1[fs(op)]=reg.gpr[rt(op)];
}

int eCPU_COP1_C_F(uint32 op) {
 reg.gpr1[31]&=~0x00800000;
}

int eCPU_LDC1(uint32 op) {
 union {
    uint32 bit32[2];
    uint64 bit64; 
  } temp;
 uint32 addr; 
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;

#ifdef DEBUG
 if(!RAM_OFFSET_MAP[addr>>16])
  { printf("LD: Unimplemented memory area. addy: 0x%x\n",addr);
     lerror=-2;
     return;
  }
#endif

 if(addr&0x04000000)
   { switch(addr)
      {
        default: printf("LD:Unimplemented interrupt\n"); break;
      }
   }
 else
  { temp.bit64= *((uint64 *)(addr+RAM_OFFSET_MAP[addr>>16]));
    reg.gpr1[rt(op)]=((uint64)temp.bit32[0])<<32|((uint64)temp.bit32[1]);
  }
}

