#include <general.h>
#include <decode.h>
#include "opcodes.h"
#include "registers.h"
#include <romheader.h>
#include <module.h>
#include <memory.h>
#include "debugger.h"

#include "config_options.h"

extern cpu_instruction ecpu_instr[64];
extern cpu_instruction ecpu_special[64];
extern cpu_instruction ecpu_regimm[32];
extern cpu_instruction ecpu_cop0[8];

extern struct cpu_reg reg;
extern int lerror;
extern struct rom *currentrom;
extern uint32 op;


extern struct sdebugger debugger;

// leave this on till we got all memory areas implemented

void eCPU_UNIMPLEMENTED(void){
	fprintf(stderr,"Unimplemented opcode (exec) %d pc:0x%x\n",opcode(op)>>26,reg.pc);
	lerror=-1;
}

void eCPU_SPECIAL_UNIMPLEMENTED(void){
	fprintf(stderr,"Unimplemented special opcode (exec) %d pc:0x%x\n",special(op),reg.pc);
	lerror=-2;
}

void eCPU_REGIMM_UNIMPLEMENTED(void){
	fprintf(stderr,"Unimplemented regimm opcode (exec) %d pc:0x%x\n",rt(op),reg.pc);
	lerror=-3;
}

void eCPU_COP0_UNIMPLEMENTED(void) {
	fprintf(stderr,"Unimplemented COP0 opcode (exec) %d pc:0x%x\n", special(op),reg.pc);
	lerror=-4;
}

void eCPU_COP1_UNIMPLEMENTED(void) {
	fprintf(stderr,"Unimplemented COP1 opcode (exec) %d,%d\n",base(op),special(op));
	lerror=-5;
}

void eCPU_SPECIAL(void) {
	ecpu_special[special(op)]();
}

void eCPU_REGIMM(void) {
	ecpu_regimm[rt(op)]();
}

void eCPU_COP0(void) {
	ecpu_cop0[special(op)]();
}

// 9/11
void eCPU_SPECIAL_SLL(void) {
	if(op) {
		reg.gpr[rd(op)] = (int32)((int32)reg.gpr[rt(op)] << sa(op));
	}
}

void eCPU_SPECIAL_DSLL(void) {
		reg.gpr[rd(op)] = (int64)((int64)reg.gpr[rt(op)] << sa(op));
}



void eCPU_SPECIAL_SYNC()
{
}


// 9/11
void eCPU_SPECIAL_SLLV(void) {
    reg.gpr[rd(op)]=(int32)((uint32)reg.gpr[rt(op)] << (uint32)(reg.gpr[rs(op)]&31));
}


// NB 63 , same for DSLL DSRLV, DSRAV  , but not done yet
void eCPU_SPECIAL_DSLLV(void) {
    reg.gpr[rd(op)]=(int64)((uint64)reg.gpr[rt(op)] << (uint32)(reg.gpr[rs(op)]&63));
}

// 9/11
void eCPU_SPECIAL_SLT(void) {
	reg.gpr[rd(op)] = (reg.gpr[rs(op)]<reg.gpr[rt(op)]);
}

// 9/11
void eCPU_SPECIAL_SLTU(void) {
	reg.gpr[rd(op)]= ((uint64)reg.gpr[rs(op)] < (uint64)reg.gpr[rt(op)]);
}

// 4/12 CORRECT
void eCPU_LUI(void) {
	reg.gpr[rt(op)] = (int64)((int32)(((int16)immediate(op)) << 16));
}

// 9/11
void eCPU_ORI(void) {
	reg.gpr[rt(op)] = reg.gpr[rs(op)] | (uint16)immediate(op);
}

// 4/12 CORRECT
void eCPU_SPECIAL_ADD(void) {
//  if((reg.gpr[rs(op)]+reg.gpr[rt(op)])>0xFFFFFFFF)
//    exception(overflow);

    reg.gpr[rd(op)]=(int64)((int32)reg.gpr[rs(op)]+(int32)reg.gpr[rt(op)]);

}

// 4/12 CORRECT
void eCPU_ADDI(void) {
//  if((reg.gpr[rs(op)]+(int16)(immediate(op)))>0xFFFFFFFF)
//    exception(overflow);

    reg.gpr[rt(op)]=(int64)((int32)reg.gpr[rs(op)] + (int16)(immediate(op)));
}

// 4/12 CORRECT
void eCPU_ADDIU(void) {
    reg.gpr[rt(op)]=(int64)((int32)reg.gpr[rs(op)] + (int16)(immediate(op)));
}


void eCPU_DADDI(void) {
    reg.gpr[rt(op)]=(int64)((int64)reg.gpr[rs(op)] + (int16)(immediate(op)));
}

void eCPU_DADDIU(void) {
    reg.gpr[rt(op)]=(int64)((int64)reg.gpr[rs(op)] + (int16)(immediate(op)));
}



// 4/12 CORRECT
void eCPU_SPECIAL_ADDU(void) {
    reg.gpr[rd(op)]=(int64)((int32)reg.gpr[rs(op)]+(int32)reg.gpr[rt(op)]);
}

// 4/12 CORRECT
void eCPU_BEQ(void) {
	if(reg.gpr[rs(op)] == reg.gpr[rt(op)]) 
	 { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
	   reg.CPUdelay=1;
	 }
}
// 4/12 CORRECT
void eCPU_BEQL(void) {
        if(reg.gpr[rs(op)] == reg.gpr[rt(op)])
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
	else
	  reg.pc+=4;
}

// 4/12 CORRECT
void eCPU_BGTZL(void) {
        if((int64)reg.gpr[rs(op)] > 0)
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
	else
	  reg.pc+=4;
}


// 4/12 CORRECT
void eCPU_REGIMM_BLTZL(void) {
        if((int64)reg.gpr[rs(op)] < 0)
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
	else
	  reg.pc+=4;
}


// 4/12 CORRECT
void eCPU_BNEL(void) {
        if(reg.gpr[rs(op)] != reg.gpr[rt(op)])
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
        else
          reg.pc+=4;
}

// 4/12 CORRECT
void eCPU_BNE(void) {
	if(reg.gpr[rs(op)] != reg.gpr[rt(op)])
	 { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
	   reg.CPUdelay=1;
	 }
}

// 4/12 CORRECT
void eCPU_BLEZ(void) {
  if((int64)reg.gpr[rs(op)]<=0)
   {
      reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
      reg.CPUdelay=1;
   }
}
// 4/12 CORRECT
void eCPU_REGIMM_BLTZ(void) {
        if((int64)reg.gpr[rs(op)] < 0)
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
}
// 9/11
void eCPU_BGTZ(void) {
        if((int64)reg.gpr[rs(op)] > 0)
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
}


// 4/12 CORRECT
void eCPU_REGIMM_BGEZ(void) {
        if((int64)reg.gpr[rs(op)]>=0)
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
}

// 4/12 CORRECT
void eCPU_BLEZL(void) {
        if(reg.gpr[rs(op)] <= 0 )
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
        else
          reg.pc+=4;
}

 // ??
void eCPU_SH(void) {
 uint32 addr;

 // ok... i'm just following N64OPS#H.TXT here...
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
 addr^=2;                                                           // ??
 // are switch()'s inefficient? -- YES (but these ifs below are even worse)
#ifdef DEBUG
  if(!RAM_OFFSET_MAP[addr>>16])
  { printf("SH: Unimplemented memory area. addy: 0x%x\n",addr);
    lerror=-2;
    return;
  }
#endif
 
 if(addr&0x04000000) 
   { switch(addr)
      {
	default: printf("SH:Unimplemented register: 0x%x\n",addr); lerror=-2; return;
      }
   }
 else
   *((uint16 *)(addr+RAM_OFFSET_MAP[addr>>16]))=reg.gpr[rt(op)];
}





void eCPU_LHU(void) {
 uint32 addr;
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
 addr^=2;
#ifdef DEBUG
 if(!RAM_OFFSET_MAP[addr>>16])
  { printf("LHU: Unimplemented memory area. addy: 0x%x\n",addr);
     lerror=-2;
     return;
  }
#endif
 if(addr&0x04000000)
   { 
   reg.gpr[rt(op)]= *((uint16 *)(Check_Load(addr)));

   }
 else
   reg.gpr[rt(op)]=*((uint16 *)(addr+RAM_OFFSET_MAP[addr>>16]));
}

void eCPU_SB(void) {
 uint32 addr;
 // ok... i'm just following N64OPS#H.TXT here...
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
 addr^=3;
#ifdef DEBUG
  if(!RAM_OFFSET_MAP[addr>>16])
  { printf("SW: Unimplemented memory area. addy: 0x%x\n",addr);
    lerror=-2;
    return;
  }
#endif
 if(addr&0x04000000) 
   {  addr=Check_Store(addr,reg.gpr[rt(op)]);
      if (addr) *((uint8 *)addr)=reg.gpr[rt(op)];
   }
 else
   *((uint8 *)(addr+RAM_OFFSET_MAP[addr>>16]))=reg.gpr[rt(op)];
}

void eCPU_LBU(void) {
	uint32 addr;
	// I'm copying you
	addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
	addr^=3;
#ifdef DEBUG
	if(!RAM_OFFSET_MAP[addr>>16]) {
		printf("LBU: Unimplemented memory area. addy: 0x%x\n", addr);
		lerror = -2;
		return;
	}
#endif
	if(addr&0x04000000) {
	     reg.gpr[rt(op)]= *((uint8 *)(Check_Load(addr)));
	} else {
		reg.gpr[rt(op)] = (uint8)*((uint8 *)(addr+RAM_OFFSET_MAP[addr>>16]));
	}
}
void eCPU_LB(void) {
	uint32 addr;
	// I'm copying you
	addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
	addr^=3;
#ifdef DEBUG
	if(!RAM_OFFSET_MAP[addr>>16]) {
		printf("LB: Unimplemented memory area. addy: 0x%x\n", addr);
		lerror = -2;
		return;
	}
#endif
	if(addr&0x04000000) {
             reg.gpr[rt(op)]= *((int8 *)(Check_Load(addr)));
	} else {
		reg.gpr[rt(op)] = *((int8 *)(addr+RAM_OFFSET_MAP[addr>>16]));
	}
}
void eCPU_SD(void) {
 uint32 addr;

 // ok... i'm just following N64OPS#H.TXT here...
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
 // are switch()'s inefficient? -- YES (but these ifs below are even worse)
#ifdef DEBUG
  if(!RAM_OFFSET_MAP[addr>>16])
  { printf("SH: Unimplemented memory area. addy: 0x%x\n",addr);
    lerror=-2;
    return;
  }
#endif

 if(addr&0x04000000)
   { switch(addr)
      {
	default: printf("SD:Unimplemented register 0x%x\n",addr); lerror=-2; return;
      }
   }
 else
  {
   *((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16]))=(reg.gpr[rt(op)])>>32 ; // top word
   *((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16])+1)=reg.gpr[rt(op)]& 0xFFFFFFFF ; // btm word
  }
 
}

void eCPU_SW(void) {
 uint32 addr;
 // ok... i'm just following N64OPS#H.TXT here...
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
#ifdef DEBUG
  if(!RAM_OFFSET_MAP[addr>>16])
  { printf("SW: Unimplemented memory area. addy: 0x%x\n",addr);
    lerror=-2;
    return;
  }
#endif
 if(addr&0x04000000)
    { addr=Check_Store(addr,reg.gpr[rt(op)]);
      if (addr) *((uint32 *)addr)=reg.gpr[rt(op)];
    }
 else
   *((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16]))=reg.gpr[rt(op)];
}


void eCPU_LW(void) {
 uint32 addr;
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
#ifdef DEBUG
 if(!RAM_OFFSET_MAP[addr>>16])
 { printf("LW: Unimplemented memory area. addy: 0x%x\n",addr);
   reg.gpr[rt(op)]=0;
//     lerror=-2;
     return;
  }
#endif
 if(addr&0x04000000)
   reg.gpr[rt(op)]= *((int32 *)(Check_Load(addr)));
 else // patch... 1964 doesn't seem to sign extend them mmmh yah it does
 reg.gpr[rt(op)]=*((int32 *)(addr+RAM_OFFSET_MAP[addr>>16]));
}

void eCPU_LH(void) {
 uint32 addr;
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
 addr^=2;
#ifdef DEBUG
 if(!RAM_OFFSET_MAP[addr>>16])
 { printf("LW: Unimplemented memory area. addy: 0x%x\n",addr);
     lerror=-2;
     return;
  }
#endif
 if(addr&0x04000000)
   reg.gpr[rt(op)]=*((int16 *)(Check_Load(addr)));
 else // patch... 1964 doesn't seem to sign extend them mmmh yah it does
 reg.gpr[rt(op)]=*((int16 *)(addr+RAM_OFFSET_MAP[addr>>16]));
}

// 4/12 CORRECT
void eCPU_JAL(void) {
 reg.gpr[31]=reg.pc+8;
// reg.CPUdelayPC=((int32)((reg.pc&0xf0000000)|target(op)<<2));
 reg.CPUdelayPC=(reg.pc&0xFFFFFFFFF0000000)|((target(op)<<2));

reg.CPUdelay=1;
}

// 4/12 CORRECT
void eCPU_J() {
 // combine with high order 4 bits of op in delay slot

/* if (kill_delay) {
	if ((reg.pc&0xfffffff) == (target(op)<<2)) {
		printf("Killing delay loop\n");
		reg.gpr0[9] = reg.VInextInt - 1;
	}
 } */

 reg.CPUdelayPC=(reg.pc&0xFFFFFFFFF0000000)|((target(op)<<2));
 reg.CPUdelay=1;
}

// 4/12 CORRECT
void eCPU_SPECIAL_JALR(void) {
 reg.gpr[rd(op)]=reg.pc+8;
// printf("JALR %d\n",rd(op));
 reg.CPUdelayPC=reg.gpr[rs(op)];
 reg.CPUdelay=1;
}

// 4/12 CORRECT
void eCPU_SPECIAL_JR(void) {
 reg.CPUdelayPC=reg.gpr[rs(op)];
 reg.CPUdelay=1;
}
// 9/11
void eCPU_SPECIAL_OR(void) {
 reg.gpr[rd(op)]=(reg.gpr[rs(op)]|reg.gpr[rt(op)]);
}

// 9/11
void eCPU_SPECIAL_NOR(void) {
 reg.gpr[rd(op)]=~(reg.gpr[rs(op)] | reg.gpr[rt(op)]);
}

// 4/12 CORRECT
void eCPU_ANDI(void) {
        reg.gpr[rt(op)] = reg.gpr[rs(op)] & (uint16)immediate(op);
}

// needs exceptions
void eCPU_SPECIAL_SUB(void) { // This needs overflow exceptions
   reg.gpr[rd(op)]=(int64)((int32)reg.gpr[rs(op)]-(int32)reg.gpr[rt(op)]);
}
// needs exceptions
void eCPU_SPECIAL_SUBU(void) {
   reg.gpr[rd(op)]=(int64)((int32)reg.gpr[rs(op)]-(int32)reg.gpr[rt(op)]);
}

// 8/12 CORRECT
void eCPU_SPECIAL_SRL(void) {
  reg.gpr[rd(op)]=(int32)((uint32)reg.gpr[rt(op)]>>sa(op));
}

void eCPU_SPECIAL_DSRL(void) {
  reg.gpr[rd(op)]=(int64)((uint64)reg.gpr[rt(op)]>>sa(op));
}

// 8/12 CORRECT
void eCPU_SPECIAL_SRLV(void) {
  reg.gpr[rd(op)]=(int32)((uint32)reg.gpr[rt(op)]>>(uint32)(reg.gpr[rs(op)]&31));
}

// 4/12 CORRECT
void eCPU_SPECIAL_AND(void) {
 reg.gpr[rd(op)]=reg.gpr[rs(op)]&reg.gpr[rt(op)];
}

void eCPU_LD(void) {
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
        default: printf("LD:Unimplemented register 0x%x\n",addr); lerror=-2; break;
      }
   }
 else
  { temp.bit64= *((uint64 *)(addr+RAM_OFFSET_MAP[addr>>16]));
    reg.gpr[rt(op)]=((uint64)temp.bit32[0])<<32|((uint64)temp.bit32[1]);
  }
}

// 9/11
void eCPU_SPECIAL_MULT(void) {
 int64 t;
 t=((int64)((int32)reg.gpr[rs(op)]) * ((int32)reg.gpr[rt(op)]));
 reg.LO=(int32)t; 
 reg.HI=(int32)(t>>32);
}

// 9/11
void eCPU_SPECIAL_MULTU(void) {
 int64 t;
 t=((uint64)((uint32)reg.gpr[rs(op)]) * ((uint32)reg.gpr[rt(op)]));
 reg.LO=(int32)t;
 reg.HI=(int32)(t>>32);
}

// borrowed from 1964
void eCPU_SPECIAL_DMULTU(void) {
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

// 9/11
void eCPU_SPECIAL_MFLO(void){
 reg.gpr[rd(op)]=reg.LO;
}

// 9/11
void eCPU_SPECIAL_MFHI(void){
 reg.gpr[rd(op)]=reg.HI;
}
// 4/12 CORRECT
void eCPU_SPECIAL_DSLL32(void){
 reg.gpr[rd(op)]=reg.gpr[rt(op)]<<(sa(op)+32);
}

void eCPU_SPECIAL_DSRL32(void){
 reg.gpr[rd(op)]=reg.gpr[rt(op)]>>(sa(op)+32);
}


// 4/12 CORRECT
void eCPU_SPECIAL_DSRA32(void){
 if (reg.gpr[rd(op)]>>63)  // if top bit is one negative therefore we extend
 {
   reg.gpr[rd(op)]=reg.gpr[rt(op)]>>(sa(op)+32);
   // 32 + sa to fill in
   reg.gpr[rd(op)]|=0xFFFFFFFFFFFFFFFF<<(sa(op)+32);
 }
 else 
   reg.gpr[rd(op)]=reg.gpr[rt(op)]>>(sa(op)+32);
}


void eCPU_SPECIAL_SYSCALL()
{
	printf("Warning, SYSCALL instruction\n");
	Exception(SYSCALL_EX);
}

void eCPU_SPECIAL_SRAV()
{
 if (reg.gpr[rd(op)]&0x80000000)  // if top bit is one negative therefore we extend
 {
   reg.gpr[rd(op)]=(int32)reg.gpr[rt(op)]>>(reg.gpr[rs(op)]&0x1F);
   // 32 + sa to fill in
   reg.gpr[rd(op)]|=0xFFFFFFFFFFFFFFFF<<(32-(reg.gpr[rs(op)]&0x1F));     // =sa
 }
 else
   reg.gpr[rd(op)]=(int32)reg.gpr[rt(op)]>>(reg.gpr[rs(op)]&0x1F);
}

// 4/12 CORRECT
void eCPU_SPECIAL_DDIVU(void){
 uint64 tRS=reg.gpr[rs(op)];
 uint64 tRT=reg.gpr[rt(op)];
 if (tRT) // avoid divide by zeros
  {
    reg.LO=(uint64)(tRS / tRT);
    reg.HI=(uint64)(tRS % tRT);
  }
}

void eCPU_SPECIAL_DDIV(void){
 int64 tRS=reg.gpr[rs(op)];
 int64 tRT=reg.gpr[rt(op)];
 if (tRT) // avoid divide by zeros
  {
    reg.LO=(int64)(tRS / tRT);
    reg.HI=(int64)(tRS % tRT);
  }
}

// 4/12 CORRECT
void eCPU_SPECIAL_DIV(void){
 int32 tRS=(int32)reg.gpr[rs(op)];
 int32 tRT=(int32)reg.gpr[rt(op)];
 if (tRT) // avoid divide by zeros
  {
    reg.LO=(int64)((int32)(tRS / tRT));
    reg.HI=(int64)((int32)(tRS % tRT));
  }
}

// 4/12 CORRECT
void eCPU_SPECIAL_DIVU(void){
 uint32 tRS=(uint32)reg.gpr[rs(op)];
 uint32 tRT=(uint32)reg.gpr[rt(op)];
 if (tRT) // avoid divide by zeros
  {
    reg.LO=(uint64)((uint32)(tRS / tRT));
    reg.HI=(uint64)((uint32)(tRS % tRT));
  }
}

// 9/11
void eCPU_SLTI(void){
 reg.gpr[rt(op)]=reg.gpr[rs(op)]<(int16)immediate(op);
}

// 9/11 someone check it but ask before changing pls
void eCPU_SPECIAL_SRA(void)
{
 reg.gpr[rd(op)]=(int32)((int32)reg.gpr[rt(op)] >> sa(op));
 if (reg.gpr[rt(op)]&0x80000000) //if top bit is one sign extend
   reg.gpr[rd(op)]|=(0xFFFFFFFFFFFFFFFF)<< (32-sa(op)); // shift all 1's right to fill 64 bit register
}


void eCPU_SPECIAL_TNE(void)
{
	if (reg.gpr[rs(op)]!=reg.gpr[rt(op)])
	{
		printf("TNE Trapping (unhandled)\n");
		debugger.run=0;
	}
}


// unsure
void eCPU_SLTIU(void)
{
	int64 t=(uint16)immediate(op);
	reg.gpr[rt(op)]=((uint64)reg.gpr[rs(op)]<(uint64)t);
}

// 9/11
void eCPU_SPECIAL_MTLO(void){
 reg.LO=reg.gpr[rs(op)];
}
// 9/11
void eCPU_SPECIAL_MTHI(void){
 reg.HI=reg.gpr[rs(op)];
}

// 9/11
void eCPU_SPECIAL_XOR(void)
{
 reg.gpr[rd(op)]=reg.gpr[rs(op)] ^ reg.gpr[rt(op)];
}
// 9/11
void eCPU_XORI(void){
 reg.gpr[rt(op)]=reg.gpr[rs(op)] ^ (uint16)immediate(op);
}
// 4/12 CORRECT
void eCPU_REGIMM_BGEZAL(void) {
 reg.gpr[31]=reg.pc+8;

 if((int64)reg.gpr[rs(op)]>=0)
  {
    reg.CPUdelayPC=reg.pc+((int16)immediate(op) << 2)+4;;
    reg.CPUdelay=1;
  }
}
// 4/12 CORRECT
void eCPU_REGIMM_BGEZL(void) {
 if((int64)reg.gpr[rs(op)]>=0)
  {
    reg.CPUdelayPC=reg.pc+((int16)immediate(op) << 2)+4;;
    reg.CPUdelay=1;
  }
 else
   reg.pc+=4;
}

void eCPU_SWL(void) {
 uint32 addr;
 uint32 ltmp;

 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
 printf("SWL: addy:0x%x at 0x%x\n",addr,reg.pc);
 ltmp=*((int32 *)((addr&0x1ffffffc)+RAM_OFFSET_MAP[(addr&0x1ffffffc)>>16]));
 switch(addr&3)
  { case 0:  ltmp =reg.gpr[rt(op)]; break;
    case 1:  ltmp = ((uint32)reg.gpr[rt(op)]>>8)|(ltmp&0xff000000); break;
    case 2:  ltmp = ((uint32)reg.gpr[rt(op)]>>16)|(ltmp&0xffff0000); break;
    default: ltmp = ((uint32)reg.gpr[rt(op)]>>24)|(ltmp&0xffffff00); break;
  }

 addr&=0x1ffffffc;
 if(addr&0x04000000)
    { addr=Check_Store(addr,ltmp);
      if (addr) *((uint32 *)addr)=ltmp;
    }
 else
   *((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16]))=ltmp; 
}

void eCPU_SWR(void) {
 uint32 addr; 
 uint32 ltmp;
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
 printf("SWR: addy:0x%x at 0x%x\n",addr,reg.pc);
 ltmp=*((int32 *)((addr&0x1ffffffc)+RAM_OFFSET_MAP[(addr&0x1ffffffc)>>16]));
 switch(addr&3)
  { case 3:  ltmp =reg.gpr[rt(op)]; break;
    case 2:  ltmp = ((uint32)reg.gpr[rt(op)]<<8)|(ltmp&0x000000ff); break;
    case 1:  ltmp = ((uint32)reg.gpr[rt(op)]<<16)|(ltmp&0x0000ffff); break;
    default: ltmp = ((uint32)reg.gpr[rt(op)]<<24)|(ltmp&0x00ffffff); break;
  }

 addr&=0x1ffffffc;
 if(addr&0x04000000)
    { addr=Check_Store(addr,ltmp);
      if (addr) *((uint32 *)addr)=ltmp;
    }
 else
   *((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16]))=ltmp;
}

void eCPU_LWL(void) {
 uint32 addr;
 uint32 taddr;

addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
//printf("LWL: addy:0x%x at 0x%x\n",addr,reg.pc);
#ifdef DEBUG
 if(!RAM_OFFSET_MAP[addr>>16])
 { printf("LW: Unimplemented memory area. addy: 0x%x\n",addr);
     lerror=-2;
     return;
  }
#endif
if(addr&0x04000000)
   taddr= *((int32 *)(Check_Load(addr&0x1ffffffc)));
 else // patch... 1964 doesn't seem to sign extend them mmmh yah it does
   taddr=*((int32 *)((addr&0x1ffffffc)+RAM_OFFSET_MAP[(addr&0x1ffffffc)>>16]));

  switch (addr & 3)
  {
  case 0:
      reg.gpr[rt(op)] = (int32)taddr;
      break;
  case 1:
			reg.gpr[rt(op)] = (int32)((reg.gpr[rt(op)] & 0x000000ff) | (taddr << 8));
      break;
  case 2:
			reg.gpr[rt(op)] = (int32)((reg.gpr[rt(op)] & 0x0000ffff) | (taddr << 16));
      break;
  case 3:
			reg.gpr[rt(op)] = (int32)((reg.gpr[rt(op)] & 0x00ffffff) | (taddr << 24));
      break;
  }
}


void eCPU_LWR(void) {
 uint32 addr;
 uint32 taddr;

addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;

//printf("lWR: addy:0x%x at 0x%x\n",addr,reg.pc);
#ifdef DEBUG
 if(!RAM_OFFSET_MAP[addr>>16])
 { printf("LW: Unimplemented memory area. addy: 0x%x\n",addr);
     lerror=-2;
     return;
  }
#endif

if(addr&0x04000000)
   taddr= *((int32 *)(Check_Load(addr&0x1ffffffc)));
 else // patch... 1964 doesn't seem to sign extend them mmmh yah it does
   taddr=*((int32 *)((addr&0x1ffffffc)+RAM_OFFSET_MAP[(addr&0x1ffffffc)>>16]));

  switch (addr & 3)
  {
  case 3:
      reg.gpr[rt(op)] = (int32)taddr;
      break;
  case 2:
			reg.gpr[rt(op)] = (int32)((reg.gpr[rt(op)] & 0xff000000) | (taddr >> 8));
      break;
  case 1:
			reg.gpr[rt(op)] = (int32)((reg.gpr[rt(op)] & 0xffff0000) | (taddr >> 16));
      break;
  case 0:
			reg.gpr[rt(op)] = (int32)((reg.gpr[rt(op)] & 0xffffff00) | (taddr >> 24));
      break;
	default:
		printf("LWR broken\n");
  }
}

void eCPU_STOLEN(void) {
#ifdef DEBUG
	printd(D_CPU, D_DEBUG, "Patch-o-matic operation triggered: %#x\n", immediate(op));
#endif

	switch(immediate(op)) {
		case 0x0001:
			reg.gpr0[9] = reg.VInextInt - 1;
			break;
		case 0x0002:
			reg.gpr0[9] = reg.VInextInt;
			reg.VInextInt += 625000;
			GenerateInterrupt(0x8);
			break;
		default:
			printf("No such clever operation thing\n");
			break;
	}
}
