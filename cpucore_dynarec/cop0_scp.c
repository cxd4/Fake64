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

int eCPU_COP0_MOVE(void) {

//__asm__("int $3\n");

if(base(op)) {
	//MTC0
	reg.gpr0[rd(op)] = reg.gpr[rt(op)];
  //printf("MTC0 reg = %x , %llx\n",&reg.gpr0[rd(op)],reg.gpr0[rd(op)]);

 } else {
	//MFC0
	if(rd(op)==13 && !MI_INTR_REG_R) COP0_CAUSE &=0xfffffbff;
#ifdef DEBUG
	if(rd(op)==13) printf("read from cause: 0x%x\n",(uint32)COP0_CAUSE);
#endif
	reg.gpr[rt(op)] = reg.gpr0[rd(op)];
//  printf("MFC0 reg = %x , %llx\n",&reg.gpr[rt(op)],reg.gpr[rt(op)]);
 }
}

int eCPU_COP0_CACHE(void) {
 // printf("CACHE Instruction ignored! (pc:0x%x)\n",reg.pc);
}

int eCPU_COP0_TLBP(void) { }
int eCPU_COP0_TLBR(void) { }

// TLB support borrowed from 1964, which is gpl anyways

int eCPU_COP0_TLBWI(void) {
 struct tlb_struct *theTLB=&TLB[reg.gpr0[0] & 0x1F];

 printf("WARNING: TLBWI INSTRUCTION!!!!\n");
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
                fprintf(stderr,"tlbwi: invalid page size\n");
		lerror=-4;
                break;
        }
}  

int eCPU_COP0_ERET(void)
{
  if(COP0_STATUS&0x4)
  {
     reg.pc = COP0_ERROR_EPC;
     COP0_STATUS&=~0x4;
  }
  else
   {
     reg.pc = COP0_EPC;
     COP0_STATUS&=~0x2;
   }
  reg.LLbit=0;
}

