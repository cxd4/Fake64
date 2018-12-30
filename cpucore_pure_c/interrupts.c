#include <general.h>
#include <romheader.h>
#include <memory.h>
#include <registers.h>
#include "decode.h"

extern struct cpu_reg reg;
extern int error;

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
	case PI_WR_LEN_REG: PI_DMA_Transfer_WR(reg.gpr[rt(op)]); break;
	case PI_STATUS_REG:
	case PI_RD_LEN_REG:
		printf("Error: SW: Unimplemented interrupt: 0x%x\n",addr); error=-1;
		break;
/*	case SP_PC_REG:
		printf("Write to PC at 0x%x pc = 0x%x, wrote to it: 0x%x\n",addr,reg.pc,reg.gpr[rt(op)]);
		reg.pc=(reg.pc&0xfffff800)|(reg.gpr[rt(op)]&0x7ff);
		break;*/
	case SI_STATUS_REG:
		*((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16])) &= 0xffffefff;
		break;
	case MI_VERSION_REG:
		printf("Error: SW: Write to readonly register\n"); error=-1;
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
                printf("Error: LW: Unimplemented interrupt\n"); error=-1;
                break;		*/
	case RI_SELECT_REG:
		printf("Read from RI_SELECT_REG, returned 0 (1964 does) for now\n");
		reg.gpr[rt(op)]=0x0;
		break;
	case PI_STATUS_REG:
		reg.gpr[rt(op)]=0; // We do all pi dma synced so no waiting
		break;
	case SP_PC_REG:
		reg.gpr[rt(op)]=reg.pc&0x7ff;
		break;
	default:
		printf("Warning: LW: Unimplemented interrupt\n");
#ifdef EXIT_ON_UNIM
	exit(0);
#endif
        case RI_BASE_REG:
        case RI_CONFIG_REG:
        case RI_CURRENT_LOAD_REG:
        case RI_REFRESH_REG:
		reg.gpr[rt(op)]=(int64)*((int32 *)(addr+RAM_OFFSET_MAP[addr>>16]));
		break;
    }
}
