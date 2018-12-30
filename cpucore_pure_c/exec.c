#include <general.h>
#include <decode.h>
#include "opcodes.h"
#include "registers.h"
#include <romheader.h>
#include <memory.h>

extern cpu_instruction ecpu_instr[64];
extern cpu_instruction ecpu_special[64];
extern cpu_instruction ecpu_regimm[32];
extern cpu_instruction ecpu_cop0[8];

extern struct cpu_reg reg;
extern int lerror;
extern struct rom *currentrom;
extern uint32 op;

// leave this on till we got all memory areas implemented

#define DEBUG



int eCPU_UNIMPLEMENTED(void){
	fprintf(stderr,"Unimplemented opcode (exec) %d pc:0x%x\n",opcode(op)>>26,reg.pc);
	lerror=-1;
}

int eCPU_SPECIAL_UNIMPLEMENTED(void){
	fprintf(stderr,"Unimplemented special opcode (exec) %d pc:0x%x\n",special(op),reg.pc);
	lerror=-2;
}

int eCPU_REGIMM_UNIMPLEMENTED(void){
	fprintf(stderr,"Unimplemented regimm opcode (exec) %d pc:0x%x\n",rt(op),reg.pc);
	lerror=-3;
}

int eCPU_COP0_UNIMPLEMENTED(void) {
	fprintf(stderr,"Unimplemented COP0 opcode (exec) %d pc:0x%x\n", special(op),reg.pc);
	lerror=-4;
}

int eCPU_COP1_UNIMPLEMENTED(void) {
	fprintf(stderr,"Unimplemented COP1 opcode (exec) %d,%d\n",base(op),special(op));
	lerror=-5;
}

int eCPU_SPECIAL(void) {
	ecpu_special[special(op)]();
}

int eCPU_REGIMM(void) {
	ecpu_regimm[rt(op)]();
}

int eCPU_COP0(void) {
	ecpu_cop0[special(op)]();
}

// 9/11
int eCPU_SPECIAL_SLL(void) {
	if(op) {
		reg.gpr[rd(op)] = (int32)((uint32)reg.gpr[rt(op)] << sa(op));
	}
}

int eCPU_SPECIAL_SYNC()
{
}


// 9/11
int eCPU_SPECIAL_SLLV(void) {
    reg.gpr[rd(op)]=(int32)((uint32)reg.gpr[rt(op)] << (uint32)(reg.gpr[rs(op)]&31));
}

// 9/11
int eCPU_SPECIAL_SLT(void) {
	reg.gpr[rd(op)] = (reg.gpr[rs(op)]<reg.gpr[rt(op)]);
}

// 9/11
int eCPU_SPECIAL_SLTU(void) {
	reg.gpr[rd(op)]= ((uint64)reg.gpr[rs(op)] < (uint64)reg.gpr[rt(op)]);
}

// 9/11
int eCPU_LUI(void) {
	reg.gpr[rt(op)] = (int64)((int32)(((int16)immediate(op)) << 16));
}

// 9/11
int eCPU_ORI(void) {
	reg.gpr[rt(op)] = reg.gpr[rs(op)] | (uint16)immediate(op);
}

// 9/11 Not too sure
int eCPU_SPECIAL_ADD(void) {
//  if((reg.gpr[rs(op)]+reg.gpr[rt(op)])>0xFFFFFFFF)
//    exception(overflow);

// reg.gpr[rd(op)]=reg.gpr[rs(op)]+reg.gpr[rt(op)];

 reg.gpr[rd(op)]=(int64)((int32)reg.gpr[rs(op)]+(int32)reg.gpr[rt(op)]);

}

 // 9/11 Not too sure
int eCPU_ADDI(void) {
    reg.gpr[rt(op)]=(int64)((int32)reg.gpr[rs(op)] + (int16)(immediate(op)));
}

// 9/11
int eCPU_ADDIU(void) {
    reg.gpr[rt(op)] = (int64)((int32)reg.gpr[rs(op)] + (int16)(immediate(op)));
}
// 9/11
int eCPU_SPECIAL_ADDU(void) {
    reg.gpr[rd(op)] = (int64)((int32)reg.gpr[rs(op)]+(int32)reg.gpr[rt(op)]);
}
// 9/11
int eCPU_BEQ(void) {
	if(reg.gpr[rs(op)] == reg.gpr[rt(op)]) 
	 { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
	   reg.CPUdelay=1;
	 }
}
// 9/11
int eCPU_BEQL(void) {
        if(reg.gpr[rs(op)] == reg.gpr[rt(op)])
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
	else
	  reg.pc+=4;
}


int eCPU_BGTZL(void) {
        if(reg.gpr[rs(op)] > 0)
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
	else
	  reg.pc+=4;
}


int eCPU_REGIMM_BLTZL(void) {
        if(reg.gpr[rs(op)] < 0)
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
	else
	  reg.pc+=4;
}


// 9/11
int eCPU_BNEL(void) {
        if(reg.gpr[rs(op)] != reg.gpr[rt(op)])
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
        else
          reg.pc+=4;
}

// 9/11
int eCPU_BNE(void) {
	if(reg.gpr[rs(op)] != reg.gpr[rt(op)])
	 { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
	   reg.CPUdelay=1;
	 }
}

// 9/11
int eCPU_BLEZ(void) {
  if(reg.gpr[rs(op)]<=0)
   {
      reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
      reg.CPUdelay=1;
   }
}
// 9/11
int eCPU_REGIMM_BLTZ(void) {
        if(reg.gpr[rs(op)] < 0)
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
}
// 9/11
int eCPU_BGTZ(void) {
        if(reg.gpr[rs(op)] > 0)
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
}


// 9/11
int eCPU_REGIMM_BGEZ(void) {
        if(reg.gpr[rs(op)] >= 0)
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
}

// 9/11
int eCPU_BLEZL(void) {
        if(reg.gpr[rs(op)] <= 0 )
         { reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
           reg.CPUdelay=1;
         }
        else
          reg.pc+=4;
}

 // ??
int eCPU_SH(void) {
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





int eCPU_LHU(void) {
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

int eCPU_SB(void) {
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

int eCPU_LBU(void) {
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
int eCPU_LB(void) {
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
int eCPU_SD(void) {
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

int eCPU_SW(void) {
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

int eCPU_LW(void) {
 uint32 addr;
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
#ifdef DEBUG
 if(!RAM_OFFSET_MAP[addr>>16])
 { printf("LW: Unimplemented memory area. addy: 0x%x\n",addr);
     lerror=-2;
     return;
  }
#endif
 if(addr&0x04000000)
   reg.gpr[rt(op)]= *((int32 *)(Check_Load(addr)));
 else // patch... 1964 doesn't seem to sign extend them mmmh yah it does
 reg.gpr[rt(op)]=*((int32 *)(addr+RAM_OFFSET_MAP[addr>>16]));
}

int eCPU_LH(void) {
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


int eCPU_JAL(void) {
 reg.gpr[31]=reg.pc+8;
// reg.CPUdelayPC=((int32)((reg.pc&0xf0000000)|target(op)<<2));
 reg.CPUdelayPC=(reg.pc&(0xF<<28))|((target(op)<<2));

reg.CPUdelay=1;
}

// 9/11
int eCPU_J() {
 int32 addr;
 addr=(uint32)(target(op)<<2);
 // combine with high order 4 bits of op in delay slot
 addr|=(uint32)(reg.pc & 0xF0000000);// 2 ^ 31 + 2 ^30 + 2^ 29 + 2^28
 reg.CPUdelayPC=addr;
 reg.CPUdelay=1;
}

// 9/11 untested can't find anything that uses it hence the debug
int eCPU_SPECIAL_JALR(void) {
 reg.gpr[rd(op)]=reg.pc+8;
// printf("JALR %d\n",rd(op));
 reg.CPUdelayPC=reg.gpr[rs(op)];
 reg.CPUdelay=1;
}

// 9/11
int eCPU_SPECIAL_JR(void) {
 reg.CPUdelayPC=reg.gpr[rs(op)];
 reg.CPUdelay=1;
}
// 9/11
int eCPU_SPECIAL_OR(void) {
 reg.gpr[rd(op)]=(reg.gpr[rs(op)]|reg.gpr[rt(op)]);
}

// 9/11
int eCPU_SPECIAL_NOR(void) {
 reg.gpr[rd(op)]=~(reg.gpr[rs(op)] | reg.gpr[rt(op)]);
}

// 9/11 not too sure manuals differ
int eCPU_ANDI(void) {
        reg.gpr[rt(op)] = reg.gpr[rs(op)] & (uint16)immediate(op);
}

// needs exceptions
int eCPU_SPECIAL_SUB(void) { // This needs overflow exceptions
   reg.gpr[rd(op)]=(int64)((int32)reg.gpr[rs(op)]-(int32)reg.gpr[rt(op)]);
}
// needs exceptions
int eCPU_SPECIAL_SUBU(void) {
   reg.gpr[rd(op)]=(int64)((int32)reg.gpr[rs(op)]-(int32)reg.gpr[rt(op)]);
}

// 9/11
int eCPU_SPECIAL_SRL(void) {
  reg.gpr[rd(op)]=(int32)((uint32)reg.gpr[rt(op)]>>sa(op));
}
// 9/11
int eCPU_SPECIAL_SRLV(void) {
  
//if (reg.gpr[rs(op)] > 31) { printf("%i, %i\n", reg.gpr[rs(op)], (reg.gpr[rs(op)] & 31)); }
  reg.gpr[rd(op)]=(int32)((uint32)reg.gpr[rt(op)]>>(uint32)(reg.gpr[rs(op)]&31));
}
// 9/11
int eCPU_SPECIAL_AND(void) {
 reg.gpr[rd(op)]=reg.gpr[rs(op)]&reg.gpr[rt(op)];
}

int eCPU_LD(void) {
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
int eCPU_SPECIAL_MULT(void) {
 int64 t;
 t=((int64)((int32)reg.gpr[rs(op)]) * ((int32)reg.gpr[rt(op)]));
 reg.LO=(int32)t; 
 reg.HI=(int32)(t>>32);
}

// 9/11
int eCPU_SPECIAL_MULTU(void) {
 int64 t;
 t=((uint64)((uint32)reg.gpr[rs(op)]) * ((uint32)reg.gpr[rt(op)]));
 reg.LO=(int32)t;
 reg.HI=(int32)(t>>32);
}

// borrowed from 1964
int eCPU_SPECIAL_DMULTU(void) {
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
int eCPU_SPECIAL_MFLO(void){
 reg.gpr[rd(op)]=reg.LO;
}

// 9/11
int eCPU_SPECIAL_MFHI(void){
 reg.gpr[rd(op)]=reg.HI;
}
// 9/11
int eCPU_SPECIAL_DSLL32(void){
 reg.gpr[rd(op)]=reg.gpr[rt(op)]<<(sa(op)+32);
}

// 9/11 someone check it but ask before changing pls
int eCPU_SPECIAL_DSRA32(void){
 if (reg.gpr[rd(op)]>>63)  // if top bit is one negative therefore we extend
 {
   reg.gpr[rd(op)]=reg.gpr[rt(op)]>>(sa(op)+32);
   // 32 + sa to fill in
   reg.gpr[rd(op)]=0xFFFFFFFFFFFFFFFF<<(32-(32+sa(op)));     // =sa
 }
 else
   reg.gpr[rd(op)]=reg.gpr[rt(op)]>>(sa(op)+32);

}



int eCPU_SPECIAL_SRAV()
{
 if (reg.gpr[rd(op)]>>31)  // if top bit is one negative therefore we extend
 {
   reg.gpr[rd(op)]=(int32)reg.gpr[rt(op)]>>(sa(op)+32);
   // 32 + sa to fill in
   reg.gpr[rd(op)]=0xFFFFFFFFFFFFFFFF<<(32-(sa(op)));     // =sa
 }
 else
   reg.gpr[rd(op)]=(int32)reg.gpr[rt(op)]>>(sa(op)+32);
}

// 9/11
int eCPU_SPECIAL_DDIVU(void){
 uint64 tRS=reg.gpr[rs(op)];
 uint64 tRT=reg.gpr[rt(op)];
 if (tRT) // avoid divide by zeros
  {
    reg.LO=tRS / tRT;
    reg.HI=tRS % tRT;
  }
}

// 9/11
int eCPU_SPECIAL_DIV(void){
 int32 tRS=(int32)reg.gpr[rs(op)];
 int32 tRT=(int32)reg.gpr[rt(op)];
 if (tRT) // avoid divide by zeros
  {
    reg.LO=(int64)(tRS / tRT);
    reg.HI=(int64)(tRS % tRT);
  }
}

int eCPU_SPECIAL_DIVU(void){
 uint32 tRS=(uint32)reg.gpr[rs(op)];
 uint32 tRT=(uint32)reg.gpr[rt(op)];
 if (tRT) // avoid divide by zeros
  {
    reg.LO=(int64)(tRS / tRT);
    reg.HI=(int64)(tRS % tRT);
  }
}

// 9/11
int eCPU_SLTI(void){
 reg.gpr[rt(op)]=reg.gpr[rs(op)]<(int16)immediate(op);
}

// 9/11 someone check it but ask before changing pls
int eCPU_SPECIAL_SRA(void){
 reg.gpr[rd(op)]=(int32)((int32)reg.gpr[rt(op)] >> sa(op));
 if (reg.gpr[rt(op)]&0x80000000) //if top bit is one sign extend
   reg.gpr[rd(op)]|=(0xFFFFFFFFFFFFFFFF)<< 32-sa(op); // shift all 1's right to fill 64 bit register
}
// way unsure
int eCPU_SLTIU(void){
 reg.gpr[rt(op)]=((uint64)reg.gpr[rs(op)]<(uint16)immediate(op));
}

// 9/11
int eCPU_SPECIAL_MTLO(void){
 reg.LO=reg.gpr[rs(op)];
}
// 9/11
int eCPU_SPECIAL_MTHI(void){
 reg.HI=reg.gpr[rs(op)];
}
// 9/11
int eCPU_SPECIAL_XOR(void)
{
 reg.gpr[rd(op)]=reg.gpr[rs(op)] ^ reg.gpr[rt(op)];
}
// 9/11
int eCPU_XORI(void){
 reg.gpr[rt(op)]=reg.gpr[rs(op)] ^ (uint16)immediate(op);
}
// 9/11
int eCPU_REGIMM_BGEZAL(void) {
 reg.gpr[31]=reg.pc+8;

 if((int64)(reg.gpr[rs(op)])>=0)
  {
    reg.CPUdelayPC=reg.pc+(int16)(immediate(op) << 2)+4;;
    reg.CPUdelay=1;
  }
}
// 9/11
int eCPU_REGIMM_BGEZL(void) {
 if(((int64)(reg.gpr[rs(op)]))>=0)
  {
    reg.CPUdelayPC=reg.pc+(int16)(immediate(op) << 2)+4;;
    reg.CPUdelay=1;
  }
 else
   reg.pc+=4;
}

int eCPU_SWL(void) {
 uint32 addr;
 uint32 ltmp;
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
 printf("SWL: addy:0x%x\n",addr);
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

int eCPU_SWR(void) {
 uint32 addr; 
 uint32 ltmp;
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
 printf("SWL: addy:0x%x\n",addr);
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

int eCPU_LWL(void) {
 uint32 addr;
 uint32 taddr;
addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
#ifdef DEBUG
 if(!RAM_OFFSET_MAP[addr>>16])
 { printf("LW: Unimplemented memory area. addy: 0x%x\n",addr);
     lerror=-2;
     return;
  }
#endif
 if(addr&0x04000000)
   taddr= *((int32 *)(Check_Load(addr)));
 else // patch... 1964 doesn't seem to sign extend them mmmh yah it does
   taddr=*((int32 *)(addr+RAM_OFFSET_MAP[addr>>16]));

  switch (taddr & 3)
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


int eCPU_LWR(void) {
 uint32 addr;
 uint32 taddr;
addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
#ifdef DEBUG
 if(!RAM_OFFSET_MAP[addr>>16])
 { printf("LW: Unimplemented memory area. addy: 0x%x\n",addr);
     lerror=-2;
     return;
  }
#endif
 if(addr&0x04000000)
   taddr= *((int32 *)(Check_Load(addr)));
 else // patch... 1964 doesn't seem to sign extend them mmmh yah it does
   taddr=*((int32 *)(addr+RAM_OFFSET_MAP[addr>>16]));

  switch (taddr & 3)
  {
  case 0:
      reg.gpr[rt(op)] = (int32)taddr;
      break;
  case 1:
			reg.gpr[rt(op)] = (int32)((reg.gpr[rt(op)] & 0x000000ff) | (taddr >> 8));
      break;
  case 2:
			reg.gpr[rt(op)] = (int32)((reg.gpr[rt(op)] & 0x0000ffff) | (taddr >> 16));
      break;
  case 3:
			reg.gpr[rt(op)] = (int32)((reg.gpr[rt(op)] & 0x00ffffff) | (taddr >> 24));
      break;
  }
}
