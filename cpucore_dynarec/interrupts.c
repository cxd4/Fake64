#include "general.h"

#include <romheader.h>
#include <memory.h>
#include "registers.h"
#include <decode.h>
#include "debugger.h"
#include <module.h>

#include "linking.h"

extern struct sdebugger debugger;

extern struct module_info* modules;
extern struct cpu_reg reg;
extern int lerror;
extern int othertask;


// read/write/number watches
extern watch watches[100];
extern int   nwatches;

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

#define USE_DEBUGGER 1

void Exception(uint32 type);
void GenerateInterrupt(uint32 type);
void dump_rcp_command(uint8* data, int len);

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
  OSTask *task;
 
  if(value&SP_CLR_HALT) 
     *SPS&=~SP_STATUS_HALT;
  if(value&SP_SET_HALT) *SPS|=SP_STATUS_HALT;

  if(value&SP_CLR_BROKE) *SPS&=~SP_STATUS_BROKE;

// mmmh what does this set?
  if(value&SP_CLR_INTR) 
   { 
     MI_INTR_REG_R &= ~MI_INTR_SP;
     // is this a hack? from 1964
     if(!(MI_INTR_REG_R&MI_INTR_MASK_REG_R))
      COP0_CAUSE&=~0x400;
   }
  if(value&SP_SET_INTR)
   {
     GenerateInterrupt(MI_INTR_SP);
     printf("SP Interrupt generated\n");
   }

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

  if(!(*SPS&1))
    { task=(OSTask *)(SPDIMEM+0xfc0);
      //printf("SPTask:\ntype:0x%x\nflags:0x%x\nucodeboot:0x%x\nucode_boot_size:0x%x\nucode:0x%x\nucode_size:0x%x\nucode_data:0x%x\nucode_data_size:0x%x\ndram_stack:0x%x\ndram_stack_size:0x%x\noutput_buff:0x%x\noutput_buff_size:0x%x\ndata_ptr:0x%x\ndata_size:0x%x\nyield_data_ptr:0x%x\nyield_data_size:0x%x\n",task->type,task->flags,task->ucode_boot,task->ucode_boot_size,task->ucode,task->ucode_size,task->ucode_data,task->ucode_data_size,task->dram_stack,task->dram_stack_size,task->output_buff,task->output_buff_size,task->data_ptr,task->data_size,task->yield_data_ptr,task->yield_data_size); 

	modules->video->rcp_command_f(task);    
  
      *SPS|= (SP_STATUS_HALT|SP_STATUS_TASKDONE);
      GenerateInterrupt(MI_INTR_DP);
//      debugger.run=0;
    }
}

void PI_DMA_Transfer_WR()
{ uint32 to,from,length;
  to=*((uint32 *)(PIREGS));
  from=*((uint32 *)(PIREGS+0x04));
  length=*((uint32 *)(PIREGS+0x0C));
  to&=0x1fffffff;
  from&=0x1fffffff; //
  printf("PI DMA Transfer: Cart->DRAM: 0x%x bytes from 0x%x to 0x%x at 0x%x\n",length+1,from,to,(uint32)reg.pc);

	memcpy((void*)(to+RAM_OFFSET_MAP[to>>16]),(void*)(from+RAM_OFFSET_MAP[from>>16]),length+1);
}

void PI_DMA_Transfer_RD()
{ uint32 to,from,length;
  to=*((uint32 *)(PIREGS+0x04)); 
  from=*((uint32 *)(PIREGS));
  length=*((uint32 *)(PIREGS+0x08));
  to&=0x1fffffff;
  from&=0x1fffffff;
  printf("PI DMA Transfer: DRAM->Cart: 0x%x bytes from 0x%x to 0x%x at 0x%x\n",length+1,from,to,(uint32)reg.pc);
  memcpy((void*)(to+RAM_OFFSET_MAP[to>>16]),(void*)(from+RAM_OFFSET_MAP[from>>16]),length+1);
}               

void SP_DMA_Transfer_WR()
{ uint32 to,from,length;
  to=*((uint32 *)(SPREGS+0x04));
  from=*((uint32 *)(SPREGS));
  length=*((uint32 *)(SPREGS+0x0C));
  to&=0x1fffffff;
  from&=0x1fffffff;
  printf("SP DMA Transfer: SP->RDRAM: 0x%x bytes from 0x%x to 0x%x\n",length+1,from,to);
  memcpy((void*)(to+RAM_OFFSET_MAP[to>>16]),(void*)(from+RAM_OFFSET_MAP[from>>16]),length+1);
}       
        
void SP_DMA_Transfer_RD()
{ uint32 to,from,length;
  to=*((uint32 *)(SPREGS));
  from=*((uint32 *)(SPREGS+0x04));
  length=*((uint32 *)(SPREGS+0x08));
  to&=0x1fffffff;
  from&=0x1fffffff;
  printf("SP DMA Transfer: RDRAM->SP: 0x%x bytes from 0x%x to 0x%x\n",length+1,from,to);
  memcpy((void*)(to+RAM_OFFSET_MAP[to>>16]),(void*)(from+RAM_OFFSET_MAP[from>>16]),length+1);
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
	printf("SI DMA Transfer to PIFRAM from 0x%x at 0x%x\n", *((uint32 *)SIREGS),(uint32)reg.pc);
	src = *((uint32 *)SIREGS);
	src&=0x1fffffff;
	memcpy(PIFMEM+0x07c0, (void*)(src + RAM_OFFSET_MAP[src>>16]), 64);
	modules->input->pifram_interrupt_f();
}

void si_dma_transfer_read() {
	uint32 dest;
	modules->input->pifram_interrupt_f();
	printf("SI DMA Transfer to RDRAM at 0x%x at 0x%x\n", *((uint32 *)SIREGS),(uint32)reg.pc);
	dest = *((uint32 *)SIREGS);
        dest&=0x1fffffff;
	memcpy((void*)(dest+RAM_OFFSET_MAP[dest>>16]), PIFMEM+0x07c0, 64);
}

void Exception(uint32 type)
{
  int newpc=0x180;

  if(COP0_STATUS_EXL || COP0_STATUS_ERL)	// not sure , see chart in mips book	
		return; // don't do exception
#ifdef DEBUG
  printf("Warning: exception.... pc: 0x%x, miregs:0x%x mimask:0x%x status:0x%x cause:0x%x\n",(uint32)reg.pc,MI_INTR_REG_R,MI_INTR_MASK_REG_R,(uint32)COP0_STATUS,(uint32)COP0_CAUSE);
#endif

    COP0_CAUSE&=~0xFF;  // zero lower byte and set code
    COP0_CAUSE|=type;
/*
  if (!type)
  {
    if(COP0_CAUSE&0x8000)  // timer
    {
    //  reg.gpr0[13]&=~0x8000; // clear interrupt , needed ??

    }
    else if(COP0_CAUSE&(1<<10))
    { }
    else
		{
  		printf("WARNING !!!!!!!!!!!!!!!!!!!!!!!\n");
    //  reg.gpr0[13]&=~0xFF00; // BAD ignore it
      return;
    }
  }
*/


if (reg.CPUdelay==1)
{
 printf("umm no\n");
 exit(0);
}


if (reg.CPUdelay==2)
   { COP0_CAUSE |= COP0_CAUSE_BD_BM;
     COP0_EPC = reg.pc-4 ;// reg.CPUdelayPC;
     reg.CPUdelay=0;
  	#ifdef DEBUG
			printf("Exception in delay slot at 0x%lx\n",reg.pc);
		#endif
   }
  else
   {
     COP0_CAUSE &= ~COP0_CAUSE_BD_BM;
     COP0_EPC=reg.pc;// +4;
     reg.CPUdelay=0;
  	#ifdef DEBUG
			printf("Exception at 0x%lx\n",reg.pc);
		#endif
	}

  COP0_STATUS|=COP0_STATUS_EXL_BM;

  if( (COP0_CAUSE & TLB_LOAD_EX) || (COP0_CAUSE & TLB_STORE_EX))
    newpc = 0x80;
 // COP0_CAUSE&=~COP0_CAUSE_EXCCODE_BM;
  reg.pc=0x80000000+newpc;

}


void CheckInterrupts()
{
  if (COP0_STATUS_EXL || COP0_STATUS_ERL || !(COP0_STATUS & 1))  // in kernel mode, delay   or if Interrupts disabled
    return;


   if (!(COP0_STATUS&COP0_CAUSE&0xFF00))
    return; // no interrupt really


  Exception(0);
  PossibleInterrupt=0;
}


void GenerateTimerInterrupt()
{
  PossibleInterrupt=1;
  COP0_CAUSE|=0x8000;
}

void GenerateInterrupt(uint32 type)
{
#ifdef DEBUG
 printf("Interrupt generated at 0x%x count=%d type=0x%x\n",(uint32)reg.pc,(uint32)reg.gpr0[9],type);
#endif

  MI_INTR_REG_R|=type;
  if(type==MI_INTR_VI)
  {
    modules->video->vi_display_f(VIREGS,(uint16 *)((*((uint32*)(VIREGS+4))&0x1fffffff)+RAM_OFFSET_MAP[(*((uint32*)(VIREGS+4))&0x1fffffff)>>16]));
    reg.gpr0[13]&=~0xFF00;
  }
  if(MI_INTR_REG_R&MI_INTR_MASK_REG_R)
    COP0_CAUSE|=1<<10; // normal interrupt, use 8+7 for count/compare

  PossibleInterrupt=1;
}

void*
Check_Store(int addr, uint32 value)
{ 
#if USE_DEBUGGER
  int i;
//  printf("Death by Bannannas\n");
  for (i=0;i<nwatches;i++)
  {
//     printf("watch on %d , type %d\n",watches[i].Address,watches[i].Type);
     if ((watches[i].Address==addr && (watches[i].Type & WATCH_WRITE)))
    {
			debugger.run=0;
    printf("Store 0x%x at address 0x%x, Breaking\n",value,addr);
  	}
  }
 #endif


  if ((addr >= SP_DMEM) && (addr <= SP_IMEM_END)) {
	return (addr+RAM_OFFSET_MAP[addr>>16]);
  }

  switch(addr)
    {		
	case MI_INTR_MASK_REG: Write_MI_IMR(value); break;
	case MI_MODE_REG: Write_MI_MODE(value); break;
	case SP_STATUS_REG: Write_SP_STATUS(value); break;
	case PI_WR_LEN_REG: GenerateInterrupt(PI_INTERRUPT);
			    othertask|=OTHER_DMA_WR;
 			    return(addr+RAM_OFFSET_MAP[addr>>16]);
			    break;
	case PI_RD_LEN_REG: GenerateInterrupt(PI_INTERRUPT);
			    othertask|=OTHER_DMA_RD;
                	    return(addr+RAM_OFFSET_MAP[addr>>16]);
			    break;
	case PI_STATUS_REG: PI_Status(value); break; //inline this
	case AI_CONTROL_REG: 
			    othertask|=OTHER_AI; 
			    return(addr+RAM_OFFSET_MAP[addr>>16]);
           		    break;
        case SP_RD_LEN_REG: othertask|=OTHER_SP_DMA_RD; 
			    return(addr+RAM_OFFSET_MAP[addr>>16]);
			    break;
        case SP_WR_LEN_REG: othertask|=OTHER_SP_DMA_WR; 
			    return(addr+RAM_OFFSET_MAP[addr>>16]);
			    break;
/*                printf("Error: Write: Unimplemented register: 0x%x\n",addr);
                exit(0);
                break;*/

  	case AI_STATUS_REG:
		       *((uint32 *)(MIREGS+0xc))&=~MI_INTR_AI;
	case AI_LEN_REG:
	case AI_DRAM_ADDR_REG:
	case AI_DACRATE_REG:
	case AI_BITRATE_REG:
				return(addr+RAM_OFFSET_MAP[addr>>16]);
           	break;
  	case PIF_MEM_BASE+0x7fc:
		othertask |= OTHER_PIF;
		GenerateInterrupt(SI_INTERRUPT);
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
		modules->video->vi_status_reg_write_f(value);
		return (addr+RAM_OFFSET_MAP[addr>>16]);
		break;
	case VI_CURRENT_REG:
		CLEAR_INTR(VI_INTERRUPT);
		return (addr+RAM_OFFSET_MAP[addr>>16]);
		break;
	case VI_ORIGIN_REG:
		modules->video->vi_origin_reg_write_f(value);
                return (addr+RAM_OFFSET_MAP[addr>>16]);
		break;
	case VI_WIDTH_REG:
		modules->video->vi_width_reg_write_f(value);
                return (addr+RAM_OFFSET_MAP[addr>>16]);
		break;
	case VI_INTR_REG:
		modules->video->vi_intr_reg_write_f(value);
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
	return NULL;
}

void*
Check_Load(int addr)
{
	static uint32 returnvalue;
	static int test=0xFF;

#if USE_DEBUGGER
  int i;
//  printf("Death by Ninjas\n");
  for (i=0;i<nwatches;i++)
  {
    if ((watches[i].Address==addr && (watches[i].Type & WATCH_READ)))
    {
			debugger.run=0;
    	printf("Read from address 0x%x, breaking\n",addr);
  	}
	}
#endif

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


	case VI_CURRENT_REG:
		test+=0x15;
		if (test>512)
			test=0;
		{
			uint32 viival;
			viival = *(uint32*)(VI_INTR_REG+RAM_OFFSET_MAP[VI_INTR_REG>>16]);
			if ((test > viival) && (test < (viival + 0x10))) {
				test = viival;
				printf("VI_INTR_REG matches: generating interrupt\n");
				GenerateInterrupt(VI_INTERRUPT);
			}
		}
		returnvalue = test;
		break;

		case AI_LEN_REG:
			returnvalue=modules->audio->ai_getlength_f();
			break;
case AI_STATUS_REG:
			returnvalue=(modules->audio->ai_getlength_f()<0) ? 1<<30 : 0;
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
	return &(returnvalue);
}

