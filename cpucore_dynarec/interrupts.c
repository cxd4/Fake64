#include <config.h>
#include <general.h>
#include <romheader.h>
#include <memory.h>
#include <registers.h>
#include <decode.h>


extern struct cpu_reg reg;
extern int lerror;
extern int othertask;


uint8 PossibleInterrupt=0;


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
#define TLB_MOD_EX              (1<<2)
#define TLB_LOAD_EX             (2<<2)
#define TLB_STORE_EX            (3<<2)
#define SYSCALL_EX              (8<<2)
#define BREAK_EX                (9<<2)
#define COP_UNUSEABLE_EX   	(11<<2)
#define MATH_OVERFLOW_EX   	(12<<2)
#define TRAP_EX                 (13<<2)


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

#ifdef DEBUG
  printf("Write to MI INTR MASK: 0x%x\n",value);
#endif
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

void PI_DMA_Transfer_WR()
{ uint32 to,from,length;
  to=*((uint32 *)(PIREGS));
  from=*((uint32 *)(PIREGS+0x04));
  length=*((uint32 *)(PIREGS+0x0C));
  printf("PI DMA Transfer: Cart->ROM: 0x%x bytes from 0x%x to 0x%x\n",length+1,from,to);
  memcpy(RDRAM+to,ROM+from-0x10000000,length+1);
}

void PI_DMA_Transfer_RD()
{ uint32 to,from,length;
  to=*((uint32 *)(PIREGS+0x04)); 
  from=*((uint32 *)(PIREGS));
  length=*((uint32 *)(PIREGS+0x08));
  printf("PI DMA Transfer: Cart->ROM: 0x%x bytes from 0x%x to 0x%x\n",length+1,from,to);  
  memcpy(RDRAM+to,ROM+from-0x10000000,length+1);
}               

void PI_Status(int value)
{
  if(value&PI_STATUS_RESET)
    *((uint32 *)(PIREGS+0x10))=0;
  else if (value & PI_STATUS_CLR_INTR)
    CLEAR_INTR(PI_INTERRUPT);
}

void si_dma_transfer_write() {
	uint32 src;
	printf("SI DMA Transfer to PIFRAM from 0x%x\n", *((uint32 *)SIREGS));
	src = *((uint32 *)SIREGS);
	memcpy(PIFMEM+0x07c0, (void*)(src + RAM_OFFSET_MAP[src>>16]), 64);
	pifram_interrupt();
}

void si_dma_transfer_read() {
	uint32 dest;
	printf("SI DMA Transfer to RDRAM at 0x%x\n", *((uint32 *)SIREGS));
	dest = *((uint32 *)SIREGS);
	memcpy((void*)(dest+RAM_OFFSET_MAP[dest>>16]), PIFMEM+0x07c0, 64);
}

void Exception(uint32 type)
{
  int newpc=0x180;

  if(COP0_STATUS_EXL || COP0_STATUS_ERL)
		return; // don't do exception

#ifdef DEBUG
  printf("Warning: exception.... pc: 0x%x, miregs:0x%x mimask:0x%x status:0x%x cause:0x%x\n",(uint32)reg.pc,MI_INTR_REG_R,MI_INTR_MASK_REG_R,(uint32)COP0_STATUS,(uint32)COP0_CAUSE);
#endif

    COP0_CAUSE&=~0xFF;  // zero lower byte and set code
    COP0_CAUSE|=type<<2;

/*  if (reg.CPUdelay==2)
   { COP0_CAUSE |= COP0_CAUSE_BD_BM;
     COP0_EPC = reg.pc-4;
     reg.CPUdelay=0;
   }
  else
   {*/
     COP0_CAUSE &= ~COP0_CAUSE_BD_BM;
     COP0_EPC=reg.pc;
/*   }*/

  COP0_STATUS|=COP0_STATUS_EXL_BM;

  if( (COP0_CAUSE & TLB_LOAD_EX) || (COP0_CAUSE & TLB_STORE_EX))
    newpc = 0x80;
  COP0_CAUSE&=~COP0_CAUSE_EXCCODE_BM;
  reg.pc=0x80000000+newpc;
}


void CheckInterrupts()
{
  if (COP0_STATUS_EXL || COP0_STATUS_ERL)  // in kernel mode, delay
    return;

   if (!(COP0_STATUS&COP0_CAUSE&0xFF00))
    return; // no interrupt really

//  lerror=-1;


 //  if ((reg.gpr0[13]&0xFF00)!=0x8000 ||(reg.gpr0[13]&0xFF00)!=) // igore all but timers atm !!!!!!! (HACK)
   // return; // no interrupt really
#ifdef DEBUG
  printf("Interrupt acted on 0x%x\n",reg.gpr0[13]&0xFF00);
#endif

  Exception(0);

 PossibleInterrupt=0;

}


void GenerateTimerInterrupt()
{
 /* if (!(COP0_STATUS & 0x8000 )   // bit 15, masked
        || !(COP0_STATUS & 1))  // all disabled

  {
    printf("Timer Masked IE %d\t I7 mask %d\n",reg.gpr0[12] & 1 ,reg.gpr0[12] & 0x8000);
    return;
  }*/
  PossibleInterrupt=1;
  COP0_CAUSE|=0x8000;
#ifdef DEBUG
  printf("Timer Fired IE %d\t I7 mask %d\n",reg.gpr0[12] & 1 ,reg.gpr0[12] & 0x8000);
#endif
}


void GenerateInterrupt(uint32 type)
{

#ifdef DEBUG
 printf("Interrupt generated at 0x%x count=%d type=0x%x\n",(uint32)reg.pc,(uint32)reg.gpr0[9],type);
#endif

  MI_INTR_REG_R|=type;
  if(type==MI_INTR_VI)
  {
    vi_display(VIREGS,(uint8*)((*((uint32*)(VIREGS+4))&0x1fffffff)+RAM_OFFSET_MAP[(*((uint32*)(VIREGS+4))&0x1fffffff)>>16]));
    reg.gpr0[13]&=~0xFF00;
  }
  if(MI_INTR_REG_R&MI_INTR_MASK_REG_R)
    COP0_CAUSE|=1<<10; // normal interrupt, use 8+7 for count/compare

  PossibleInterrupt=1;
}

int Check_Store(int addr, uint32 value)
{ uint32 addr2;

  if ((addr >= SP_DMEM) && (addr <= SP_IMEM_END)) {
	return (addr+RAM_OFFSET_MAP[addr>>16]);
  }

  switch(addr)
    {		
	case MI_INTR_MASK_REG: Write_MI_IMR(value); break;
	case MI_MODE_REG: Write_MI_MODE(value); break;
	case SP_STATUS_REG: Write_SP_STATUS(value); break;
	case PI_WR_LEN_REG: SET_INTR(PI_INTERRUPT);
			    othertask|=OTHER_DMA_WR;
 			    return(addr+RAM_OFFSET_MAP[addr>>16]);
			    break;
	case PI_RD_LEN_REG: SET_INTR(PI_INTERRUPT);
			    othertask|=OTHER_DMA_RD;
                	    return(addr+RAM_OFFSET_MAP[addr>>16]);
			    break;
	case PI_STATUS_REG: PI_Status(value); break; //inline this
	case AI_CONTROL_REG: 
			    othertask|=OTHER_AI; 
			    return(addr+RAM_OFFSET_MAP[addr>>16]);
           		    break;
  	case AI_STATUS_REG:
		       *((uint32 *)(MIREGS+0xc))&=~MI_INTR_AI;
		       break;
	case AI_LEN_REG:
	case AI_DRAM_ADDR_REG:
	case AI_DACRATE_REG:
	case AI_BITRATE_REG:
				return(addr+RAM_OFFSET_MAP[addr>>16]);
           	break;
  	case PIF_MEM_BASE+0x7fc:
		othertask |= OTHER_PIF;
                return (addr+RAM_OFFSET_MAP[addr>>16]);
		break;
	case SI_STATUS_REG:
		*((uint32 *)(addr+RAM_OFFSET_MAP[addr>>16])) &= 0xffffefff;
		break;
	case SI_PIF_ADDR_RD64B_REG:
		othertask |= OTHER_SI_DMA_RD;
                return (addr+RAM_OFFSET_MAP[addr>>16]);
		break;
	case SI_PIF_ADDR_WR64B_REG:
		othertask |= OTHER_SI_DMA_WR;
                return (addr+RAM_OFFSET_MAP[addr>>16]);
		break;
	case MI_VERSION_REG:
		printf("Error: SW: Write to readonly register\n"); lerror=-1;
		break;
	case VI_STATUS_REG:
                CLEAR_INTR(VI_INTERRUPT);
		vi_status_reg_write(value);
		return (addr+RAM_OFFSET_MAP[addr>>16]);
		break;
	case VI_CURRENT_REG:
		CLEAR_INTR(VI_INTERRUPT);
		return (addr+RAM_OFFSET_MAP[addr>>16]);
		break;
	case VI_ORIGIN_REG:
		vi_origin_reg_write(value);
                return (addr+RAM_OFFSET_MAP[addr>>16]);
		break;
	case VI_WIDTH_REG:
		vi_width_reg_write(value);
                return (addr+RAM_OFFSET_MAP[addr>>16]);
		break;
	case VI_INTR_REG:
		vi_intr_reg_write(value);
                return (addr+RAM_OFFSET_MAP[addr>>16]);
		break;
	default:
#ifdef DEBUG
		printf("Warning: Write: Unimplemented register: 0x%x\n",addr);
#endif
#ifdef EXIT_ON_UNIM
		exit(0);
#endif
			// fall through here on purpose so that we write
	case RI_BASE_REG:
	case RI_CONFIG_REG:
	case RI_CURRENT_LOAD_REG:
	case RI_SELECT_REG:
	case RI_REFRESH_REG:
	case PI_DRAM_ADDR_REG:
	case PI_CART_ADDR_REG:
	case SP_PC_REG:
	case SI_DRAM_ADDR_REG:
                return(addr+RAM_OFFSET_MAP[addr>>16]);
		break;
    }
  return 0;
}

uint32 Check_Load(int addr)
{ static uint32 returnvalue; 

  if ((addr >= SP_DMEM) && (addr <= SP_IMEM_END)) {
	return (addr+RAM_OFFSET_MAP[addr>>16]);
  }

   switch(addr)
    { 
	case MI_VERSION_REG:
#ifdef DEBUG
		printf("Read from MI_VERSION_REG, returned 0x01010101\n");
#endif
		returnvalue=0x01010101;
		break;
/*	case PI_STATUS_REG:
                printf("Error: LW: Unimplemented register\n"); lerror=-1;
                break;		*/
	case RI_SELECT_REG:
#ifdef DEBUG
		printf("Read from RI_SELECT_REG, returned 0 (1964 does) for now\n");
#endif
		returnvalue=0x0;
		break;
	case PI_STATUS_REG:
		returnvalue=0; // We do all pi dma synced so no waiting
		break;
  default:
#ifdef DEBUG
		printf("Warning: Load: Unimplemented register 0x%x at 0x%llx\n", addr,reg.pc);
#endif
#ifdef EXIT_ON_UNIM
	exit(0);
#endif
        case RI_BASE_REG:
        case RI_CONFIG_REG:
        case RI_CURRENT_LOAD_REG:
        case RI_REFRESH_REG:
	case SP_PC_REG:
	case SI_STATUS_REG:
	case MI_INTR_REG:
	case MI_INTR_MASK_REG:
		return(addr+RAM_OFFSET_MAP[addr>>16]);
		break;
    }
  return &returnvalue;
}

