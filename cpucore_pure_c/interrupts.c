#include <config.h>
#include <general.h>
#include <romheader.h>
#include <memory.h>
#include <registers.h>
#include "decode.h"


extern struct cpu_reg reg;
extern int lerror;
int interrupt_needed=0;

#define SET_INTR(x) *((uint32 *)(MIREGS+0x08)) |= x;
#define CLEAR_INTR(x) *((uint32 *)(MIREGS+0x08)) &= ~(x);

#define SP_INTERRUPT    0x1
#define SI_INTERRUPT    0x2
#define AI_INTERRUPT    0x4
#define VI_INTERRUPT    0x8
#define PI_INTERRUPT    0x10
#define DP_INTERRUPT    0x20
#define COUNT_INTERRUPT 0x80

// Exceptions

#define INTERRUPT_EX            0
#define TLB_MOD_EX              1
#define TLB_LOAD_EX             2
#define TLB_STORE_EX            3
#define SYSCALL_EX              8
#define BREAK_EX                9
#define COP_UNUSEABLE_EX   	11
#define MATH_OVERFLOW_EX   	12
#define TRAP_EX                 13


extern void pifram_interrupt();

void Exception(uint32 type);

void Write_MI_MODE(uint32 value)
{ uint32 *MODE=(uint32 *)(MIREGS);

  if(value&MI_CLR_INIT) *MODE&=~MI_MODE_INIT;
  if(value&MI_SET_INIT) *MODE|=MI_MODE_INIT;

  if(value&MI_CLR_EBUS) *MODE&=~MI_MODE_EBUS;
  if(value&MI_SET_EBUS) *MODE|=MI_MODE_EBUS;

  if(value&MI_CLR_DP_INTR) *((uint32 *)(MIREGS+0x08))&=~MI_INTR_DP;

  if(value&MI_CLR_RDRAM) *MODE&=~MI_MODE_RDRAM;
  if(value&MI_SET_RDRAM) *MODE|=MI_MODE_RDRAM;
}

void Write_MI_IMR(uint32 value)
{ uint32 *IMR=(uint32 *)(MIREGS+0xc);

  if (value&MI_INTR_MASK_CLR_SP) *IMR&=~MI_INTR_MASK_SP;
  if (value&MI_INTR_MASK_SET_SP) *IMR|=MI_INTR_MASK_SP;
  
  if (value&MI_INTR_MASK_CLR_SI) *IMR&=~MI_INTR_MASK_SI;
  if (value&MI_INTR_MASK_SET_SI) *IMR|=MI_INTR_MASK_SI; 

  if (value&MI_INTR_MASK_CLR_AI) *IMR&=~MI_INTR_MASK_AI;
  if (value&MI_INTR_MASK_SET_AI) *IMR|=MI_INTR_MASK_AI;

  if (value&MI_INTR_MASK_CLR_VI) *IMR&=~MI_INTR_MASK_VI;
  if (value&MI_INTR_MASK_SET_VI) *IMR|=MI_INTR_MASK_VI;

  if (value&MI_INTR_MASK_CLR_PI) *IMR&=~MI_INTR_MASK_PI;
  if (value&MI_INTR_MASK_SET_PI) *IMR|=MI_INTR_MASK_PI;

  if (value&MI_INTR_MASK_CLR_DP) *IMR&=~MI_INTR_MASK_DP;
  if (value&MI_INTR_MASK_SET_DP) *IMR|=MI_INTR_MASK_DP;

}
void Write_SP_STATUS(int value)
{ uint32 *SPS=(uint32 *)(SPREGS+0x10);
  
  if(value&SP_CLR_HALT) *SPS&=~SP_STATUS_HALT;
  if(value&SP_SET_HALT) *SPS|=SP_STATUS_HALT;

  if(value&SP_CLR_BROKE) *SPS&=~SP_STATUS_BROKE;

// mmmh what does this set?
//  if(value&SP_CLR_INTR) *SPS&=~????;
//  if(value&SP_SET_INTR) *SPS|=????;

  if(value&SP_CLR_SSTEP) *SPS&=~SP_STATUS_SSTEP;
  if(value&SP_SET_SSTEP) *SPS|=SP_STATUS_SSTEP;

  if(value&SP_CLR_INTR_BREAK) *SPS&=~SP_STATUS_INTR_BREAK;
  if(value&SP_SET_INTR_BREAK) *SPS|=SP_STATUS_INTR_BREAK;

  if(value&SP_CLR_SIG0) *SPS&=~SP_STATUS_SIG0;
  if(value&SP_SET_SIG0) *SPS|=SP_STATUS_SIG0;

  if(value&SP_CLR_SIG1) *SPS&=~SP_STATUS_SIG1;
  if(value&SP_SET_SIG1) *SPS|=SP_STATUS_SIG1;

  if(value&SP_CLR_SIG2) *SPS&=~SP_STATUS_SIG2;
  if(value&SP_SET_SIG2) *SPS|=SP_STATUS_SIG2;

  if(value&SP_CLR_SIG3) *SPS&=~SP_STATUS_SIG3;
  if(value&SP_SET_SIG3) *SPS|=SP_STATUS_SIG4;

  if(value&SP_CLR_SIG4) *SPS&=~SP_STATUS_SIG4;
  if(value&SP_SET_SIG4) *SPS|=SP_STATUS_SIG4;

  if(value&SP_CLR_SIG5) *SPS&=~SP_STATUS_SIG5;
  if(value&SP_SET_SIG5) *SPS|=SP_STATUS_SIG5;

  if(value&SP_CLR_SIG6) *SPS&=~SP_STATUS_SIG6;
  if(value&SP_SET_SIG6) *SPS|=SP_STATUS_SIG6;

  if(value&SP_CLR_SIG7) *SPS&=~SP_STATUS_SIG7;
  if(value&SP_SET_SIG7) *SPS|=SP_STATUS_SIG7;
}

void PI_DMA_Transfer_WR(int length)
{ uint32 to,from;
  to=*((uint32 *)(PIREGS));
  from=*((uint32 *)(PIREGS+0x04));
  printf("PI DMA Transfer: Cart->ROM: 0x%x bytes from 0x%x to 0x%x\n",length,from,to);
  memcpy(RDRAM+to,ROM+from-0x10000000,length);
}

void PI_DMA_Transfer_RD(int length)
{ uint32 to,from;
  to=*((uint32 *)(PIREGS+0x04)); 
  from=*((uint32 *)(PIREGS));
  printf("PI DMA Transfer: Cart->ROM: 0x%x bytes from 0x%x to 0x%x\n",length,from,to);  
  memcpy(RDRAM+to,ROM+from-0x10000000,length);
}               

void PI_Status(int value)
{
  if(value&PI_STATUS_RESET)
    *((uint32 *)(PIREGS+0x10))=0;
  else if (value & PI_STATUS_CLR_INTR)
    *((uint32 *)(MIREGS+0xc))&=~MI_INTR_PI;
}


// Interrupt handling very much inspired by apollo

void handle_interrupt(int interrupt)
{
  if(interrupt & COUNT_INTERRUPT) {
     interrupt_exception(COUNT_INTERRUPT);
  }
  SET_INTR((interrupt&0x3f)); // Apollo does this.... why?
			      // i get the feeling this should be CLEAR_INTR

  if (interrupt & VI_INTERRUPT) {
	// Handle video
  }
  interrupt_exception(SI_INTERRUPT); // apollo defaults to SI_interrupt? why?
}

int interrupt_exception(uint32 type)
{ uint32 t=1 << (8+type);
  COP0_CAUSE |= t;
  if(!(COP0_STATUS&1)) return 0; // interrupts are disabled
  if(!(COP0_STATUS&t)) return 0; // specific interrupt disabled
  Exception(INTERRUPT_EX);
  return 1;
}

void Exception(uint32 type)
{ uint32 newpc;
  newpc=0x180;
  COP0_CAUSE&=0x3000ff00;
  COP0_CAUSE|=(type<<2);
  if(COP0_STATUS&2) // why?
   {
     COP0_EPC=reg.pc;
     if (type==TLB_LOAD_EX || type==TLB_STORE_EX) newpc = 0x80; // apollo does this.....
   }
  COP0_STATUS|=2;
  if(reg.CPUdelay==2) COP0_CAUSE|= 0x80000000;
  reg.pc=newpc+=(COP0_STATUS&0x400000) ? 0xbfc00200 : 0x80000000;
}

void Check_SW(int addr,uint32 op)
{

  if ((addr >= SP_DMEM) && (addr <= SP_IMEM_END)) {
	*((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16]))=reg.gpr[rt(op)];
	return;
  }

  switch(addr)
    {		
	case MI_INTR_MASK_REG: Write_MI_IMR(reg.gpr[rt(op)]); break;
	case MI_MODE_REG: Write_MI_MODE(reg.gpr[rt(op)]); break;
	case SP_STATUS_REG: Write_SP_STATUS(reg.gpr[rt(op)]); break;
	case PI_WR_LEN_REG: PI_DMA_Transfer_WR(reg.gpr[rt(op)]); 
			    interrupt_needed |= PI_INTERRUPT;
			    SET_INTR(PI_INTERRUPT);
			    break;
	case PI_RD_LEN_REG: PI_DMA_Transfer_RD(reg.gpr[rt(op)]);
                            interrupt_needed |= PI_INTERRUPT;
                            SET_INTR(PI_INTERRUPT);
			    break;
	case PI_STATUS_REG: PI_Status(reg.gpr[rt(op)]); break; //inline this
		printf("Error: SW: Unimplemented interrupt: 0x%x\n",addr); lerror=-1;
		break;
        case PIF_MEM_BASE+0x7fc:
                *((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16]))=reg.gpr[rt(op)];
		pifram_interrupt();
		break;
	case SI_STATUS_REG:
		*((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16])) &= 0xffffefff;
		break;
	case MI_VERSION_REG:
		printf("Error: SW: Write to readonly register\n"); lerror=-1;
		break;
	case AI_STATUS_REG:
		*((uint32 *)(MIREGS+0xc))&=~MI_INTR_AI;
		break;
	default:
		printf("Warning: SW: Unimplemented interrupt: 0x%x\n",addr);
#ifdef EXIT_ON_UNIM
		exit(0);
#endif
	case RI_BASE_REG:
	case RI_CONFIG_REG:
	case RI_CURRENT_LOAD_REG:
	case RI_SELECT_REG:
	case RI_REFRESH_REG:
	case PI_DRAM_ADDR_REG:
	case PI_CART_ADDR_REG:
	case SP_PC_REG:
                *((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16]))=reg.gpr[rt(op)]; 
		break;
    }
}

void Check_LW(int addr,uint32 op)
{

  if ((addr >= SP_DMEM) && (addr <= SP_IMEM_END)) {
	reg.gpr[rt(op)]=(int64)*((int32 *)(addr+RAM_OFFSET_MAP[addr>>16]));
        return;
  }

   switch(addr)
    { 
	case MI_VERSION_REG:
		printf("Read from MI_VERSION_REG, returned 0x01010101\n");
		reg.gpr[rt(op)]=0x01010101;
		break;
/*	case PI_STATUS_REG:
                printf("Error: LW: Unimplemented interrupt\n"); lerror=-1;
                break;		*/
	case RI_SELECT_REG:
		printf("Read from RI_SELECT_REG, returned 0 (1964 does) for now\n");
		reg.gpr[rt(op)]=0x0;
		break;
	case PI_STATUS_REG:
		reg.gpr[rt(op)]=0; // We do all pi dma synced so no waiting
		break;
	default:
		printf("Warning: LW: Unimplemented interrupt 0x%x\n", addr);
#ifdef EXIT_ON_UNIM
	exit(0);
#endif
        case RI_BASE_REG:
        case RI_CONFIG_REG:
        case RI_CURRENT_LOAD_REG:
        case RI_REFRESH_REG:
	case SI_STATUS_REG:
	case SP_PC_REG:
		reg.gpr[rt(op)]=(int64)*((int32 *)(addr+RAM_OFFSET_MAP[addr>>16]));
		break;
    }
}
