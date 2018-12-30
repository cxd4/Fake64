#include <general.h>
#include "decode.h"
#include "opcodes.h"
#include "registers.h"
#include <romheader.h>
#include <memory.h>

extern cpu_instruction ecpu_instr[64];
extern cpu_instruction ecpu_special[64];
extern cpu_instruction ecpu_regimm[32];
extern cpu_instruction ecpu_cop0[8];

extern struct cpu_reg reg;
extern int error;
extern struct rom *currentrom;
extern uint32 CPUdelay;
extern uint32 CPUdelayPC;

// leave this on till we got all memory areas implemented

#define DEBUG

int eCPU_UNIMPLEMENTED(uint32 op){
	fprintf(stderr,"Unimplemented opcode (exec) %d pc:0x%x\n",opcode(op)>>26,reg.pc);
	error=-1;
}

int eCPU_SPECIAL_UNIMPLEMENTED(uint32 op){
	fprintf(stderr,"Unimplemented special opcode (exec) %d pc:0x%x\n",special(op),reg.pc);
	error=-2;
}

int eCPU_REGIMM_UNIMPLEMENTED(uint32 op){
	fprintf(stderr,"Unimplemented regimm opcode (exec) %d pc:0x%x\n",rt(op),reg.pc);
	error=-3;
}

int eCPU_COP0_UNIMPLEMENTED(uint32 op) {
	fprintf(stderr,"Unimplemented COP0 opcode (exec) %d pc:0x%x\n", special(op),reg.pc);
	error=-4;
}

int eCPU_COP1_UNIMPLEMENTED(uint32 op) {
	fprintf(stderr,"Unimplemented COP1 opcode (exec) %d,%d\n",base(op),special(op));
	error=-5;
}

int eCPU_SPECIAL(uint32 op) {
	ecpu_special[special(op)](op);
}

int eCPU_REGIMM(uint32 op) {
	ecpu_regimm[rt(op)](op);
}

int eCPU_COP0(uint32 op) {
	ecpu_cop0[special(op)](op);
}

int eCPU_SPECIAL_SLL(uint32 op) {
	reg.gpr[rd(op)] = ((uint32)reg.gpr[rt(op)] << sa(op));
}

int eCPU_SPECIAL_SLLV(uint32 op) {
  reg.gpr[rd(op)]=(int32)((uint32)reg.gpr[rt(op)] << (uint32)(reg.gpr[rs(op)]&31));
}

int eCPU_SPECIAL_SLT(uint32 op) {
	reg.gpr[rd(op)] = (reg.gpr[rs(op)]<reg.gpr[rt(op)]);
}

int eCPU_SPECIAL_SLTU(uint32 op) {
	reg.gpr[rd(op)]= ((uint64)reg.gpr[rs(op)] < (uint64)reg.gpr[rt(op)]);
}

int eCPU_LUI(uint32 op) {
	reg.gpr[rt(op)] = (int64)((int32)(immediate(op) << 16));
}

int eCPU_ORI(uint32 op) {
	reg.gpr[rt(op)] = reg.gpr[rs(op)] | immediate(op);
}

int eCPU_SPECIAL_ADD(uint32 op) {
//  if((reg.gpr[rs(op)]+reg.gpr[rt(op)])>0xFFFFFFFF)
//    exception(overflow);
  reg.gpr[rd(op)]=reg.gpr[rs(op)]+reg.gpr[rt(op)];

}

int eCPU_ADDI(uint32 op) {
    reg.gpr[rt(op)]=reg.gpr[rs(op)]+(int16)(immediate(op));
}

int eCPU_ADDIU(uint32 op) {
    reg.gpr[rt(op)] = (int32)((int32)reg.gpr[rs(op)] + (int16)(immediate(op)));
}

int eCPU_SPECIAL_ADDU(uint32 op) {
    reg.gpr[rd(op)] = (int64)((int32)reg.gpr[rs(op)]+(int32)reg.gpr[rt(op)]);
}

int eCPU_BEQ(uint32 op) {
	if(reg.gpr[rs(op)] == reg.gpr[rt(op)]) 
	 { CPUdelayPC = reg.pc+(int16)(immediate(op) << 2)+4;
	   CPUdelay=1;
	 }
}

int eCPU_BEQL(uint32 op) {
        if(reg.gpr[rs(op)] == reg.gpr[rt(op)])
         { CPUdelayPC = reg.pc+(int16)(immediate(op) << 2)+4;
           CPUdelay=1;
         }
	else
	  reg.pc+=4;
}

int eCPU_BNEL(uint32 op) {
        if(reg.gpr[rs(op)] != reg.gpr[rt(op)])
         { CPUdelayPC = reg.pc+(int16)(immediate(op) << 2)+4;
           CPUdelay=1;
         }
        else
          reg.pc+=4;
}

int eCPU_BNE(uint32 op) {
	if((uint32)reg.gpr[rs(op)] != (uint32)reg.gpr[rt(op)]) 
	 { CPUdelayPC = reg.pc+(int16)(immediate(op) << 2)+4;
	   CPUdelay=1;
	 }
}

int eCPU_BLEZ(uint32 op) {
  if(reg.gpr[rs(op)]<=0)
   {
      CPUdelayPC = reg.pc+(int16)(immediate(op) << 2)+4;
      CPUdelay=1;
   }
}

int eCPU_BLEZL(uint32 op) {
        if(reg.gpr[rs(op)] <= 0 )
         { CPUdelayPC = reg.pc+(int16)(immediate(op) << 2)+4;
           CPUdelay=1;
         }
        else
          reg.pc+=4;
}

int eCPU_SH(uint32 op) {
 uint32 addr;
 // ok... i'm just following N64OPS#H.TXT here...
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
 // are switch()'s inefficient? -- YES (but these ifs below are even worse)
#ifdef DEBUG
  if(!RAM_OFFSET_MAP[addr>>16])
  { printf("SH: Unimplemented memory area. addy: 0x%x\n",addr);
    error=-2;
    return;
  }
#endif
 
 if(addr&0x04000000) 
   { switch(addr)
      {
	default: printf("SH:Unimplemented interrupt\n"); error=-2; return;
      }
   }
 else
   *((uint16 *)(addr+RAM_OFFSET_MAP[addr>>16]))=reg.gpr[rt(op)];
}

int eCPU_LHU(uint32 op) {
 uint32 addr;
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
#ifdef DEBUG
  if(!RAM_OFFSET_MAP[addr>>16])
  { printf("LHU: Unimplemented memory area. addy: 0x%x\n",addr);
     error=-2;
     return;
  }
#endif
 if(addr&0x04000000)
   { switch(addr)
      {
        default: printf("LHU:Unimplemented interrupt\n"); error=-2; return;
      }
   }
 else
   reg.gpr[rt(op)]=*((uint16 *)(addr+RAM_OFFSET_MAP[addr>>16]));
}

int eCPU_SB(uint32 op) {
 uint32 addr;
 // ok... i'm just following N64OPS#H.TXT here...
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
#ifdef DEBUG
  if(!RAM_OFFSET_MAP[addr>>16])
  { printf("SW: Unimplemented memory area. addy: 0x%x\n",addr);
    error=-2;
    return;
  }
#endif
/* if(addr&0x04000000)
    Check_SW(addr,op);
 else*/
   *((uint8 *)(addr+RAM_OFFSET_MAP[addr>>16]))=reg.gpr[rt(op)];
}

int eCPU_LBU(uint32 op) {
	uint32 addr;
	// I'm copying you
	addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
#ifdef DEBUG
	if(!RAM_OFFSET_MAP[addr>>16]) {
		printf("LBU: Unimplemented memory area. addy: 0x%x\n", addr);
		error = -2;
		return;
	}
#endif
	if(addr&0x04000000) {
//		Check_LBU(addr,op);
		printf("LBU made call to interrupts!\n");
	} else {
		reg.gpr[rt(op)] = *((uint8 *)(addr+RAM_OFFSET_MAP[addr>>16]));
	}
}

int eCPU_SW(uint32 op) { 
 uint32 addr;
 // ok... i'm just following N64OPS#H.TXT here...
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
#ifdef DEBUG
  if(!RAM_OFFSET_MAP[addr>>16])
  { printf("SW: Unimplemented memory area. addy: 0x%x\n",addr);
    error=-2;
    return;
  }
#endif
 if(addr&0x04000000)
    Check_SW(addr,op);
 else
   *((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16]))=reg.gpr[rt(op)];
}

int eCPU_LW(uint32 op) {
 uint32 addr;
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
#ifdef DEBUG
 if(!RAM_OFFSET_MAP[addr>>16])
 { printf("LW: Unimplemented memory area. addy: 0x%x\n",addr);
     error=-2;
     return;
  }
#endif
 if(addr&0x04000000)
   Check_LW(addr,op);
 else // patch... 1964 doesn't seem to sign extend them mmmh yah it does
 reg.gpr[rt(op)]=*((int32 *)(addr+RAM_OFFSET_MAP[addr>>16]));
}

int eCPU_JAL(uint32 op) {
 // ohh somebody... check if this is correct...
 reg.gpr[31]=reg.pc+8;
 CPUdelayPC=(((reg.pc&0xf0000000)|target(op)<<2));
 CPUdelay=1;
}

int eCPU_SPECIAL_JR(uint32 op) {
 CPUdelayPC=reg.gpr[rs(op)];
 CPUdelay=1;
}

int eCPU_SPECIAL_OR(uint32 op) {
 reg.gpr[rd(op)]=(reg.gpr[rs(op)]|reg.gpr[rt(op)]);
}

int eCPU_ANDI(uint32 op) {
        reg.gpr[rt(op)] = reg.gpr[rs(op)] & immediate(op);
}

int eCPU_SPECIAL_SUBU(uint32 op) {
   reg.gpr[rd(op)]=(int64)((int32)reg.gpr[rs(op)]-(int32)reg.gpr[rt(op)]);
}

int eCPU_SPECIAL_SRL(uint32 op) {
  reg.gpr[rd(op)]=((uint32)reg.gpr[rt(op)]>>sa(op));
}

int eCPU_SPECIAL_SRLV(uint32 op) {
  
//if (reg.gpr[rs(op)] > 31) { printf("%i, %i\n", reg.gpr[rs(op)], (reg.gpr[rs(op)] & 31)); }
  reg.gpr[rd(op)]=(int32)((uint32)reg.gpr[rt(op)]>>(uint32)(reg.gpr[rs(op)]&31));
}

int eCPU_SPECIAL_AND(uint32 op) {
 reg.gpr[rd(op)]=reg.gpr[rs(op)]&reg.gpr[rt(op)];
}

int eCPU_LD(uint32 op) {
 uint32 addr;
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;

#ifdef DEBUG
 if(!RAM_OFFSET_MAP[addr>>16])
  { printf("LD: Unimplemented memory area. addy: 0x%x\n",addr);
     error=-2;
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
   reg.gpr[rt(op)]=*((int64 *)(addr+RAM_OFFSET_MAP[addr>>16]));
}

int eCPU_SPECIAL_MULTU(uint32 op) {
 int64 t;
 t=((uint64)((uint32)reg.gpr[rs(op)]) * ((uint32)reg.gpr[rt(op)]));
 reg.LO=(int32)t;
 reg.HI=(int32)(t>>32);
}

// borrowed from 1964
int eCPU_SPECIAL_DMULTU(uint32 op) {
     uint64 hh,hl,lh,ll,b,t1,t2;

     t1 = reg.gpr[rs(op)];
     t2 = reg.gpr[rt(op)];

     hh = ((uint64)(t1 >> 32)  & 0x0ffffffff)  * ((uint64)(t2 >> 32)  &   0x0ffffffff);
     hl =  (uint64)(t1         & 0x0ffffffff)  * ((uint64)(t2 >> 32)  &   0x0ffffffff);
     lh = ((uint64)(t1 >> 32)  & 0x0ffffffff)  *  (uint64)(t2         &   0x0ffffffff);
     ll = ((uint64)(t1         & 0x0ffffffff)  *  (uint64)(t2         &   0x0ffffffff));

     reg.LO = ((hl + lh) << 32) + ll;

     b=(((hl + lh) + (ll >> 32)) & 0x0100000000)>>32;

     reg.HI = (uint64)hh + ((int64)(uint32)(hl >> 32) + (int64)(uint32)(lh >> 32) + b);
}

int eCPU_SPECIAL_MFLO(uint32 op){
 reg.gpr[rd(op)]=reg.LO;
}

int eCPU_SPECIAL_DSLL32(uint32 op){
 reg.gpr[rd(op)]=reg.gpr[rt(op)]<<(sa(op)+32);
}

int eCPU_SPECIAL_DSRA32(uint32 op){
 reg.gpr[rd(op)]=reg.gpr[rt(op)]>>(sa(op)+32);
}

int eCPU_SPECIAL_DDIVU(uint32 op){
 uint64 tRS;
 uint64 tRT=rt(op);
 if (tRT) // avoid divide by zeros
  {
    reg.LO=(tRS=reg.gpr[rs(op)]) / tRT;
    reg.HI=tRS % tRT;
  }
}

int eCPU_SLTI(uint32 op){
 reg.gpr[rt(op)]=reg.gpr[rs(op)]<(int16)immediate(op);
}

int eCPU_SPECIAL_SRA(uint32 op){
 reg.gpr[rd(op)]=(int32)((int32)reg.gpr[rt(op)] >> sa(op));
}

int eCPU_SLTIU(uint32 op){
 reg.gpr[rt(op)]=((uint64)reg.gpr[rs(op)]<(uint16)immediate(op));
}

int eCPU_SPECIAL_MTLO(uint32 op){
 reg.LO=reg.gpr[rs(op)];
}

int eCPU_SPECIAL_MTHI(uint32 op){
 reg.HI=reg.gpr[rs(op)];
}

int eCPU_SPECIAL_XOR(uint32 op)
{
 reg.gpr[rd(op)]=reg.gpr[rs(op)] ^ reg.gpr[rt(op)];
}

int eCPU_XORI(uint32 op){
 reg.gpr[rt(op)]=reg.gpr[rs(op)] ^ immediate(op);
}

int eCPU_REGIMM_BGEZAL(uint32 op) {
 reg.gpr[31]=reg.pc+8;

 if(((int64)(reg.gpr[rs(op)]))>=0)
  {
    CPUdelayPC=reg.pc+(int16)(immediate(op) << 2)+4;;
    CPUdelay=1;
  }
}

int eCPU_REGIMM_BGEZL(uint32 op) {
 if(((int64)(reg.gpr[rs(op)]))>=0)
  {
    CPUdelayPC=reg.pc+(int16)(immediate(op) << 2)+4;;
    CPUdelay=1;
  }
 else
   reg.pc+=4;
}

