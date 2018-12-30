#include <general.h>
#include "decode.h"
#include "opcodes.h"
#include "registers.h"

extern struct cpu_reg reg;
struct tlb_struct        TLB[MAXTLB];
extern int lerror;

int eCPU_COP0_MOVE(uint32 op) {
 if(base(op)) {
	//MTC0
	reg.gpr0[rd(op)] = reg.gpr[rt(op)];
 } else {
	//MFC0
	reg.gpr[rt(op)] = reg.gpr0[rd(op)];
 }
}

int eCPU_COP0_CACHE(uint32 op) {
 // printf("CACHE Instruction ignored! (pc:0x%x)\n",reg.pc);
}

// TLB support borrowed from 1964, which is gpl anyways

int eCPU_COP0_TLBWI(uint32 op) {
 struct tlb_struct *theTLB=&TLB[reg.gpr0[0] & 0x1F];
   
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
                fprintf(stderr,"tlbwi: invalid page size\n");
		lerror=-4;
                break;
        }
}  
