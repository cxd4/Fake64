#include <general.h>
#include <romheader.h>
#include <decode.h>
#include "opcodes.h"
#include "registers.h"
#include <memory.h>
#include <math.h>


extern struct cpu_reg reg;
extern cpu_instruction dyna_cop1[32];
extern cpu_instruction dyna_cop1_s[64];
extern cpu_instruction dyna_cop1_d[64];
extern cpu_instruction dyna_cop1_w[64];
extern cpu_instruction dyna_cop1_l[64];
extern uint32 op;
extern int run;
extern int lerror;

extern struct cpu_reg reg;
extern uint32 op;

int eCPU_COP1(void) {
 // printf("COP1 format %d fucntion %d at 0x%lx\n",fmt(op),funct(op),reg.pc);
  dyna_cop1[base(op)]();
//	run=0;
}

int eCPU_COP1_S(void) {
  dyna_cop1_s[funct(op)]();
}

int eCPU_COP1_D(void) {
  dyna_cop1_d[funct(op)]();
}

int eCPU_COP1_L(void) {
 // printf("COP1_L subcode %d\n",funct(op));
  dyna_cop1_l[funct(op)]();
}

int eCPU_COP1_W(void) {
 // printf("COP1_W subcode %d\n",funct(op));
  dyna_cop1_w[funct(op)]();
}

int eCPU_COP1_CFC1(void) {
  reg.gpr[rt(op)]=reg.cr1[fs(op)];
}

int eCPU_COP1_CTC1(void) {
 reg.cr1[fs(op)]=(int32)reg.gpr[rt(op)];
}

int eCPU_COP1_SUBS(void)
{
  *((float*)&reg.gpr1[fd(op)])=*((float*)&reg.gpr1[fs(op)])-*((float*)&reg.gpr1[ft(op)]);

}


int eCPU_COP1_MOVS(void)
{
  *((float*)&reg.gpr1[fd(op)])=*((float*)&reg.gpr1[fs(op)]);
}


// 9/11
int eCPU_COP1_MTC1(void)
{
  // move word from gpr to a fpu register
  reg.gpr1[fs(op)]=(int32)reg.gpr[rt(op)]; // copying whole dword since top 32 bits are undefined after, can change though
}
// 9/11
int eCPU_COP1_MFC1(void)
{
  // move word from gpr to a fpu register
 reg.gpr[rt(op)]=(int32)reg.gpr1[fs(op)]; // copying whole dword since top 32 bits are undefined after, can change though
}

int eCPU_COP1_CVTSW(void) // TODO: add rounding support
{
  // convert fixed point word in reg fs to single precision float in fd
  *((float*)&reg.gpr1[fd(op)])=(*((int32*)&reg.gpr1[fs(op)]));
}

int eCPU_COP1_CVTDW(void) // TODO: add rounding support
{
  // convert fixed point word in reg fs to single precision float in fd
  *((double*)&reg.gpr1[fd(op)])=(double)(*((int32*)&reg.gpr1[fs(op)]));
}

int eCPU_COP1_CVTWS(void) // TODO: add rounding support
{
  *((uint32*)&reg.gpr1[fd(op)])=*((float*)&reg.gpr1[fs(op)]);
}

int eCPU_COP1_CVTDS(void) // TODO: add rounding support
{
  *((double*)&reg.gpr1[fd(op)])=*((float*)&reg.gpr1[fs(op)]);
}

int eCPU_COP1_CVTSD(void) // TODO: add rounding support
{
  *((float*)&reg.gpr1[fd(op)])=*((double*)&reg.gpr1[fs(op)]);
}

int eCPU_COP1_MULS(void)
{
 (*((float*)&reg.gpr1[fd(op)]))=(*((float*)&reg.gpr1[fs(op)]))*(*((float*)&reg.gpr1[ft(op)]));
}


int eCPU_COP1_MULD(void)
{
 (*((double*)&reg.gpr1[fd(op)]))=(*((double*)&reg.gpr1[fs(op)]))*(*((double*)&reg.gpr1[ft(op)]));
}

int eCPU_COP1_TRUNCWD(void)
{
 *((uint32*)&reg.gpr1[fd(op)])=(int32)floor(*((double*)&reg.gpr1[fs(op)]));
}

int eCPU_COP1_TRUNCWS(void)
{
  *((uint32*)&reg.gpr1[fd(op)])=(int32)floor(*((float*)&reg.gpr1[fs(op)]));
}

int eCPU_COP1_NEGS(void)
{
 *((float*)&reg.gpr1[fd(op)])=-*((float*)&reg.gpr1[fs(op)]);
}
int eCPU_COP1_NEGD(void)
{
 *((double*)&reg.gpr1[fd(op)])=-*((double*)&reg.gpr1[fs(op)]);
}


int eCPU_LWC1(void) {
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
   Check_LW(addr,op);
 else
 reg.gpr1[rt(op)]=*((int32 *)(addr+RAM_OFFSET_MAP[addr>>16]));
}

int eCPU_COP1_DIVS(void)
{
  (*((float*)&reg.gpr1[fd(op)]))=(*((float*)&reg.gpr1[fs(op)]))/(*((float*)&reg.gpr1[ft(op)]));
}

int eCPU_COP1_DIVD(void)
{
  (*((int32*)&reg.gpr1[fd(op)]))=(*((int32*)&reg.gpr1[fs(op)]))/(*((int32*)&reg.gpr1[ft(op)]));
}

int eCPU_COP1_SQRTS(void)
{
  (*((float*)&reg.gpr1[fd(op)]))=sqrt((double)*((float*)&reg.gpr1[fs(op)]));
}

int eCPU_COP1_ADDS(void)
{
  (*((float*)&reg.gpr1[fd(op)]))=(*((float  *)&reg.gpr1[fs(op)]))+(*((float*)&reg.gpr1[ft(op)]));
}

int eCPU_COP1_ADDD(void)
{
  (*((double*)&reg.gpr1[fd(op)]))=(*((double*)&reg.gpr1[fs(op)]))+(*((double*)&reg.gpr1[ft(op)]));
}


int eCPU_LDC1(void) {
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
  {
   temp.bit64= *((uint64 *)(addr+RAM_OFFSET_MAP[addr>>16]));
    reg.gpr1[rt(op)]=((uint64)temp.bit32[0])<<32|((uint64)temp.bit32[1]);
  }
}

int eCPU_SWC1(void) {
 uint32 addr;
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
#ifdef DEBUG
  if(!RAM_OFFSET_MAP[addr>>16])
  { printf("SWC1: Unimplemented memory area. addy: 0x%x\n",addr);
    lerror=-2;
    return;
  }
#endif
  if(addr&0x04000000)
    { addr=Check_Store(addr,reg.gpr1[rt(op)]);
      if (addr) *((uint32 *)addr)=reg.gpr1[rt(op)];
    }
 else
   *((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16]))=reg.gpr1[rt(op)];
}

int eCPU_SDC1(void) {
 uint32 addr;
 addr=(reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff;
#ifdef DEBUG
  if(!RAM_OFFSET_MAP[addr>>16])
  { printf("SWC1: Unimplemented memory area. addy: 0x%x\n",addr);
    lerror=-2;
    return;
  }
#endif
  if(addr&0x04000000)
    { addr=Check_Store(addr,reg.gpr1[rt(op)]);
      if (addr) *((uint64*)addr)=reg.gpr1[rt(op)];
    }
 else
   *((uint64*)(addr+RAM_OFFSET_MAP[addr>>16]))=reg.gpr1[rt(op)];
}

