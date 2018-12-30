#include <general.h>
#include <romheader.h>
#include <decode.h>
#include "opcodes.h"
#include "registers.h"
#include <memory.h>
#include <math.h>
								
extern struct cpu_reg reg;
extern cpu_instruction ecpu_cop1[32];
extern cpu_instruction ecpu_cop1_s[64];
extern cpu_instruction ecpu_cop1_d[64];
extern cpu_instruction ecpu_cop1_w[64];
extern cpu_instruction ecpu_cop1_l[64];
extern uint32 op;
extern int run;
extern int lerror;

void eCPU_COP1(void) {
 // printf("COP1 format %d fucntion %d at 0x%lx\n",fmt(op),funct(op),reg.pc);
  ecpu_cop1[base(op)]();
//	run=0;
}

void eCPU_COP1_S(void) {
  ecpu_cop1_s[funct(op)]();
}

void eCPU_COP1_D(void) {
  ecpu_cop1_d[funct(op)]();
}

void eCPU_COP1_L(void) {
 // printf("COP1_L subcode %d\n",funct(op));
  ecpu_cop1_l[funct(op)]();
}

void eCPU_COP1_W(void) {
 // printf("COP1_W subcode %d\n",funct(op));
  ecpu_cop1_w[funct(op)]();
}

void eCPU_COP1_CFC1(void) {
  reg.gpr[rt(op)]=(int64)reg.cr1[fs(op)];
}

void eCPU_COP1_CTC1(void) {
 reg.cr1[fs(op)]=(int32)reg.gpr[rt(op)];
}

void eCPU_COP1_SUBS(void)
{
  *((float*)&reg.gpr1[fd(op)])=*((float*)&reg.gpr1[fs(op)])-*((float*)&reg.gpr1[ft(op)]);
}
void eCPU_COP1_SUBD(void)
{
  *((double*)&reg.gpr1[fd(op)])=*((double*)&reg.gpr1[fs(op)])-*((double*)&reg.gpr1[ft(op)]);
}


void eCPU_COP1_MOVS(void)
{
  *((float*)&reg.gpr1[fd(op)])=*((float*)&reg.gpr1[fs(op)]); // copying whole dword since top 32 bits are undefined after, can change though
}


// 9/11
void eCPU_COP1_MTC1(void)
{
  // move word from gpr to a fpu register
  reg.gpr1[fs(op)]=(int32)reg.gpr[rt(op)]; // copying whole dword since top 32 bits are undefined after, can change though
}
// 9/11
void eCPU_COP1_MFC1(void)
{
  // move word from gpr to a fpu register
 reg.gpr[rt(op)]=(int32)reg.gpr1[fs(op)]; // copying whole dword since top 32 bits are undefined after, can change though
}

void eCPU_COP1_CVTSW(void) // TODO: add rounding support
{
  // convert fixed point word in reg fs to single precision float in fd
  *((float*)&reg.gpr1[fd(op)])=(float)(*((int32*)&reg.gpr1[fs(op)]));
}



void eCPU_COP1_CVTSL(void) // TODO: add rounding support
{
  // convert fixed point word in reg fs to single precision float in fd
  *((float*)&reg.gpr1[fd(op)])=(float)(*((int64*)&reg.gpr1[fs(op)]));
}


void eCPU_COP1_CVTDW(void) // TODO: add rounding support
{
  // convert fixed point word in reg fs to single precision float in fd
  *((double*)&reg.gpr1[fd(op)])=(double)(*((int32*)&reg.gpr1[fs(op)]));
}


void eCPU_COP1_CVTWD(void) // TODO: add rounding support
{
  // convert fixed point word in reg fs to single precision float in fd
  *((int32*)&reg.gpr1[fd(op)])=(int32)(*((double*)&reg.gpr1[fs(op)]));
}
void eCPU_COP1_CVTLD(void) // TODO: add rounding support
{
  // convert fixed point word in reg fs to single precision float in fd
  *((int64*)&reg.gpr1[fd(op)])=(int64)(*((double*)&reg.gpr1[fs(op)]));
}


void eCPU_COP1_CVTWS(void) // TODO: add rounding support
{
  *((int32*)&reg.gpr1[fd(op)])=(int32)*((float*)&reg.gpr1[fs(op)]);		 // was (uint32*) , bug ?
}

void eCPU_COP1_CVTDS(void) // TODO: add rounding support
{
  *((double*)&reg.gpr1[fd(op)])=(double)*((float*)&reg.gpr1[fs(op)]);
//  printf("cvtd.s $f%d $f%d",fd(op),fs(op));
}

void eCPU_COP1_CVTSD(void) // TODO: add rounding support
{
  *((float*)&reg.gpr1[fd(op)])=(float)*((double*)&reg.gpr1[fs(op)]);
}

void eCPU_COP1_MULS(void)
{
 (*((float*)&reg.gpr1[fd(op)]))=(float)(*((float*)&reg.gpr1[fs(op)]))*(*((float*)&reg.gpr1[ft(op)]));
}


void eCPU_COP1_MULD(void)
{
 (*((double*)&reg.gpr1[fd(op)]))=(*((double*)&reg.gpr1[fs(op)]))*(*((double*)&reg.gpr1[ft(op)]));
}

void eCPU_COP1_TRUNCWD(void)
{
 *((int32*)&reg.gpr1[fd(op)])=(int32)floor(*((double*)&reg.gpr1[fs(op)]));
}

void eCPU_COP1_TRUNCWS(void)
{
  *((int32*)&reg.gpr1[fd(op)])=(int32)floor(*((float*)&reg.gpr1[fs(op)]));
}

void eCPU_COP1_NEGS(void)
{
 *((float*)&reg.gpr1[fd(op)])=-*((float*)&reg.gpr1[fs(op)]);
}
void eCPU_COP1_NEGD(void)
{
 *((double*)&reg.gpr1[fd(op)])=-*((double*)&reg.gpr1[fs(op)]);
}



// sign extend or not ?
void eCPU_LWC1(void) {
 uint32 addr;
 addr=(uint32)((reg.gpr[base(op)]+(int16)offset(op)) & 0x1fffffff);
#ifdef DEBUG
 if(!RAM_OFFSET_MAP[addr>>16])
 { printf("LWC1: Unimplemented memory area. addy: 0x%x\n",addr);
     lerror=-2;
     return;
  }
#endif
 if(addr&0x04000000)
   reg.gpr1[rt(op)]= *((int32 *)(Check_Load(addr)));
 else // patch... 1964 doesn't seem to sign extend them mmmh yah it does
 reg.gpr1[rt(op)]=*((int32 *)(addr+RAM_OFFSET_MAP[addr>>16]));
}



void eCPU_COP1_DIVS(void)
{
  (*((float*)&reg.gpr1[fd(op)]))=(*((float*)&reg.gpr1[fs(op)]))/(*((float*)&reg.gpr1[ft(op)]));
}

void eCPU_COP1_DIVD(void)
{
  (*((double*)&reg.gpr1[fd(op)]))=(*((double*)&reg.gpr1[fs(op)]))/(*((double*)&reg.gpr1[ft(op)]));
}

void eCPU_COP1_SQRTS(void)
{
  (*((float*)&reg.gpr1[fd(op)]))=(float)sqrt((double)*((float*)&reg.gpr1[fs(op)]));
}

void eCPU_COP1_ADDS(void)
{
  (*((float*)&reg.gpr1[fd(op)]))=(*((float  *)&reg.gpr1[fs(op)]))+(*((float*)&reg.gpr1[ft(op)]));
}

void eCPU_COP1_ADDD(void)
{
  (*((double*)&reg.gpr1[fd(op)]))=(*((double*)&reg.gpr1[fs(op)]))+(*((double*)&reg.gpr1[ft(op)]));
}




void eCPU_COP1_MOVD(void)
{
  *((double*)&reg.gpr1[fd(op)])=*((double*)&reg.gpr1[fs(op)]);
}



// conditional ones


/*
void eCPU_COP1_CF(void)
void eCPU_COP1_CUN(void)
void eCPU_COP1_CEQ(void)
void eCPU_COP1_CUEQ(void)
void eCPU_COP1_COLT(void)
void eCPU_COP1_CULT(void)
void eCPU_COP1_COLE(void)
void eCPU_COP1_CULE(void)
void eCPU_COP1_CNGLE(void)
void eCPU_COP1_CSEQ(void)
void eCPU_COP1_CNGL(void)
void eCPU_COP1_CLT(void)
void eCPU_COP1_CNGE(void)
void eCPU_COP1_CLE(void)
void eCPU_COP1_CNGT(void)
*/


// all the comparisons in one go (slightly slower but so much less code
void eCPU_COP1_CCS(void)
{
	static char less,equal,unordered,cond;
	if (isnan(*((float*)&reg.gpr1[fs(op)])) ||	isnan(*((float*)&reg.gpr1[ft(op)])))
	{
		less=0;
		equal=0;
		unordered=1;

		if (op&0x8)
		{
    		printf("FPU Invalid operation exception\n");
		}	
	}
	else
	{
		less=(*((float*)&reg.gpr1[fs(op)]) < *((float*)&reg.gpr1[ft(op)]));
    equal=(*((float*)&reg.gpr1[fs(op)]) == *((float*)&reg.gpr1[ft(op)]));
		unordered=0;
  }

	cond= ( ((op&0x4) && less ) || ((op&0x2) & equal )  || ((op&0x1) & unordered ));
	
	reg.cr1[31]&=~(0x1<<23);	// clear it
	reg.cr1[31]|=cond<<23; // bit 23 is the condition flag
	
	// may need some more stuff here based on exception handling mode
}

// all the comparisons in one go (slightly slower but so much less code
void eCPU_COP1_CCD(void)
{
	static char less,equal,unordered,cond;
	if (isnan(*((double*)&reg.gpr1[fs(op)])) ||	isnan(*((double*)&reg.gpr1[ft(op)])))
	{
		less=0;
		equal=0;
		unordered=1;

		if (op&0x8)
		{
    		printf("FPU Invalid operation exception\n");
		}	
	}
	else
	{
		less=(*((double*)&reg.gpr1[fs(op)]) < *((double*)&reg.gpr1[ft(op)]));
    equal=(*((double*)&reg.gpr1[fs(op)]) == *((double*)&reg.gpr1[ft(op)]));
		unordered=0;



  }
	//printf("C.d less=%d equal=%d unorder=%d\n, 0x%llx  ? 0x%llx",less,equal,unordered, *((double*)&reg.gpr1[fs(op)]),*((double*)&reg.gpr1[ft(op)]));

	cond= ( ((op&0x4) && less ) || ((op&0x2) & equal )  || ((op&0x1) & unordered ));
	
	//printf("condition %d\n",cond);

 // printf("reg.cr1[31] 0x%x\n",reg.cr1[31]);
	reg.cr1[31]&=~(0x1<<23);	// clear it
	reg.cr1[31]|=(cond<<23); // bit 23 is the condition flag
	
//  printf("reg.cr1[31] 0x%x\n",reg.cr1[31]);
	// may need some more stuff here based on exception handling mode
}

// all the comparisons in one go (slightly slower but so much less code
void eCPU_COP1_CCW(void)
{
	static char less,equal,unordered,cond;
	if (isnan(*((float*)&reg.gpr1[fs(op)])) || isnan(*((float*)&reg.gpr1[ft(op)])))
	{
		less=0;
		equal=0;
		unordered=0;

		if (op&0x8)
		{
    		printf("FPU Invalid operation exception\n");
		}	
	}
	else
	{
		less=(*((int32*)&reg.gpr1[fs(op)]) < *((int32*)&reg.gpr1[ft(op)]));
    equal=(*((int32*)&reg.gpr1[fs(op)]) == *((int32*)&reg.gpr1[ft(op)]));
		unordered=0;
  }

	cond= ( ((op&0x4) && less ) || ((op&0x2) & equal )  || ((op&0x1) & unordered ));
	
	reg.cr1[31]&=~(0x1<<23);	// clear it
	reg.cr1[31]|=cond<<23; // bit 23 is the condition flag
	
	// may need some more stuff here based on exception handling mode
}

// all the comparisons in one go (slightly slower but so much less code
void eCPU_COP1_CCL(void)
{
	static char less,equal,unordered,cond;
	if (isnan(*((double*)&reg.gpr1[fs(op)])) || isnan(*((double*)&reg.gpr1[ft(op)])))
	{
		less=0;
		equal=0;
		unordered=0;

		if (op&0x8)
		{
    		printf("FPU Invalid operation exception\n");
		}	
	}
	else
	{
		less=(*((int64*)&reg.gpr1[fs(op)]) < *((int64*)&reg.gpr1[ft(op)]));
    equal=(*((int64*)&reg.gpr1[fs(op)]) == *((int64*)&reg.gpr1[ft(op)]));
		unordered=0;
  }

	cond= ( ((op&0x4) && less ) || ((op&0x2) & equal )  || ((op&0x1) & unordered ));
	
	reg.cr1[31]&=~(0x1<<23);	// clear it
	reg.cr1[31]|=cond<<23; // bit 23 is the condition flag
	
	// may need some more stuff here based on exception handling mode
}


void eCPU_COP1_BC(void)
{
	char cond=((reg.cr1[31]>>23)&0x1);
	char t=(op>>16)&0x3;

 // printf("reg.cr1[31] 0x%x\ncond=%d\n((op>>16)&0x3)=%d\n",reg.cr1[31],cond,t);

	switch(t) 	// switch on type
	{
		case 0: //BC1F
			if (!cond)
			{    	
				reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
				reg.CPUdelay=1;
			}
    	break;
		case 1: //BC1T
			if (cond)
			{    	
				reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
				reg.CPUdelay=1;
			}
    	break;
		case 2:  //BC1FL
			if (!cond)
			{    	
				reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
				reg.CPUdelay=1;
			}
			else
				reg.pc+=4;
    	break;
		case 3:  //BC1TL
			if (cond)
			{    	
				reg.CPUdelayPC = reg.pc+((int16)immediate(op) << 2)+4;
				reg.CPUdelay=1;
			}
			else
				reg.pc+=4;
    	break;
		default:
      printf("BC broken");
			exit(0);
	}
}








// fixed this 26/12/2001 think others prob broken too
void eCPU_LDC1(void) {
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
    *((int32*)&reg.gpr1[rt(op)])=temp.bit32[1];//           ((uint64)temp.bit32[0])<<32|((uint64)temp.bit32[1]);
    *((int32*)(&reg.gpr1[rt(op)])+1)=temp.bit32[0];
  }
}

void eCPU_SWC1(void) {
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

void eCPU_SDC1(void) {
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
      if (addr)
				*((uint64*)addr)=*((uint64*)&reg.gpr1[rt(op)]);
    }
 else
   *((uint64*)(addr+RAM_OFFSET_MAP[addr>>16]))=*((uint64*)&reg.gpr1[rt(op)]);
}

