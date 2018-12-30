#include <module.h>
#include <general.h>
#include <decode.h>
#include "opcodes.h"
#include "registers.h"
#include <romheader.h>
#include <memory.h>

extern struct cpu_reg reg;
struct tlb_struct        TLB[MAXTLB];
extern int lerror;
extern uint32 op;


void eCPU_COP0_MOVE(void) {
 if(base(op)) {
	//MTC0
	reg.gpr0[rd(op)] = reg.gpr[rt(op)];
  if (rd(op)==11)
  {
    // write to compare clears timer interrupt  (clear bit 15)
    COP0_CAUSE&=~0x8000;
    printd(D_CPU, D_DEBUG, "Experimental, timer interrupt cleared\n");
  }
 } else {
	//MFC0
	if(rd(op)==13 && !MI_INTR_REG_R) COP0_CAUSE &=0xfffffbff;
#ifdef DEBUG
	if(rd(op)==13) printd(D_CPU, D_DEBUG, "read from cause: 0x%x\n",(uint32)COP0_CAUSE);
#endif
	reg.gpr[rt(op)] = reg.gpr0[rd(op)];
 }
}

void eCPU_COP0_CACHE(void) {
 // printd(D_CPU, D_DEBUG, "CACHE Instruction ignored! (pc:0x%x)\n",reg.pc);
}

void eCPU_COP0_TLBP(void) {

	int i;
	uint32 tvpn2;
	uint32 tasid;
	uint32 gbit;

	tvpn2 = (uint32)VPN2(reg.gpr0[10]);
	tasid = (uint32)ASID(reg.gpr0[10]);

	reg.gpr0[0] = (1<<31); // (1 || 0^25 || undefined^6)
	
	printd(D_CPU, D_DEBUG, "Tlb probe for:\nVPN2 0x%x\n\tASID 0x%x\n",tvpn2,tasid);

	for (i = 0; i < MAXTLB; i++) {

		gbit = GBIT(TLB[i].EntryLo0 & TLB[i].EntryLo1);

    		if ((VPN2(TLB[i].EntryHi & ~TLB[i].PageMask) == \
			(tvpn2& ~TLB[i].PageMask)) && \
			(gbit || (ASID(TLB[i].EntryHi) == tasid))) {

				reg.gpr0[0] = i;
				printd(D_CPU, D_DEBUG, "TLBP i=%d\n",i);
			  	return;
		}
	}
}

void eCPU_COP0_TLBR(void) {

	uint32 index;

	index = (uint32)reg.gpr[0];

	reg.gpr[5] = TLB[index].PageMask;
	reg.gpr[10] = (TLB[index].EntryHi & ~TLB[index].PageMask);
	reg.gpr[3] = TLB[index].EntryLo1;   // add write global bit ?
	reg.gpr[2] = TLB[index].EntryLo0;
}

// TLB support borrowed from 1964, which is gpl anyways

void eCPU_COP0_TLBWI(void) {
 struct tlb_struct *theTLB=&TLB[reg.gpr0[0] & 0x1F];

 printd(D_CPU, D_WARN, "WARNING: TLBWI INSTRUCTION!!!! at 0x%x\n",reg.pc);
// lerror=-2;
   
 theTLB->valid = 1;
 theTLB->PageMask = reg.gpr0[5];
 theTLB->EntryHi = reg.gpr0[10]; 
 theTLB->EntryLo1 = reg.gpr0[3];			   
 theTLB->EntryLo0 = reg.gpr0[2];

        theTLB->MyHiMask = ~(uint32)theTLB->PageMask & 0xFFFFE000;

        switch (theTLB->PageMask) {
            case 0x00000000:    // 4k
                theTLB->LoCompare = 0x00001000;
                break;
            case 0x00006000:    // 16k
                theTLB->LoCompare = 0x00004000;
                break;
            case 0x0001E000:    // 64k
                theTLB->LoCompare = 0x00010000;
                break;
                theTLB->LoCompare = 0x00040000;
                break;
            case 0x001FE000:    // 1M
                theTLB->LoCompare = 0x00100000;
                break;
            case 0x007FE000:    // 4M
                theTLB->LoCompare = 0x00400000;
                break;
            case 0x01FFE000:    // 16M
                theTLB->LoCompare = 0x01000000;
                break;
            default:
                printd(D_CPU, D_ERROR, "tlbwi: invalid page size\n");
		lerror=-4;
                break;
        }
}  

void eCPU_COP0_ERET(void)
{
  if(COP0_STATUS&0x4)
  {
     reg.CPUdelayPC = COP0_ERROR_EPC;
     COP0_STATUS&=~0x4;
  } 
  else
   {
     reg.CPUdelayPC = COP0_EPC;
     COP0_STATUS&=~0x2;
   }
  reg.LLbit=0;
  reg.CPUdelay=2;


  printd(D_CPU, D_DEBUG, "Returned from exception from 0x%llx to 0x%llx\n",reg.pc,reg.CPUdelayPC);
  fflush(stdout);
}




void dump_tlb()
{
  int i;
  for (i = 0; i < MAXTLB; i++)
  {
     printd(D_CPU, D_DEBUG, "TLB Entry %d:\n\tEntryHi 0x%x\tEntryLo0 0x%x\tEntryLo1 0x%x\n\tPageMask 0x%llx\t Valid 0x%x\n\tLoCompare 0x%x\tMtHiMask 0x%x\n",TLB[i].EntryHi,TLB[i].EntryLo0,TLB[i].EntryLo1,TLB[i].PageMask,TLB[i].valid,TLB[i].LoCompare,TLB[i].MyHiMask);
  }

}

