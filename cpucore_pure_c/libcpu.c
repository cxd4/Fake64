#include <signal.h>
#include <general.h>
#include <romheader.h>
#include <decode.h>
#include "registers.h"
#include "opcodes.h"
#include <memory.h>

#include "module.h"
#include "debugger.h"
#include "config_options.h"

#include "linking.h"

//#define DEBUG 1
//#define EXCESSIVE_DEBUG 1

void sig_stop_run();
extern void apply_patches();

extern struct sdebugger debugger;

void do_a_dump();
void debug_do_a_dump();

#ifndef STATIC_MODULES
char *module_id(void)
{
    return (
        "N64 pure C interpreter cpu core (Version 0.00000002 by Bluefyre and HCl)"
    );
}
#endif

extern cpu_instruction dcpu_instr[64];
extern cpu_instruction dcpu_special[64];
extern cpu_instruction dcpu_regimm[32];

extern cpu_instruction ecpu_instr[64];
extern cpu_instruction ecpu_special[64];
extern cpu_instruction ecpu_regimm[32];

extern FILE* disfd; // fp for disassembled output

extern void CheckInterrupts();
extern uint8 PossibleInterrupt;


struct module_info* modules;

struct cpu_reg reg;
struct rom *currentrom;
int lerror=0;
int othertask=0;
uint32 op;
uint64 rpc=0;

extern char* reg_names[];

void debugger_step(void);

void main_cpu_loop(struct rom *rom,struct module_info* mods)
{
	uint32 i=0,crc=0;
#ifdef DEBUG
	int disasmcount=0;
#endif
	int ret;
	uint32 addr2;

	currentrom = rom;
	alloc_memory(rom);

	modules=mods;

	init_pifram(modules, PIFMEM);

// setup disassemble output to stdout
  disfd=stdout;

	signal(SIGINT, sig_stop_run);

	// setup default register values

                reg.gpr[0x00] = 0;
                reg.gpr[0x01] = 0;
                reg.gpr[0x02] = 0xffffffffd1731be9;
                reg.gpr[0x03] = 0xffffffffd1731be9;
                reg.gpr[0x04] = 0x01be9;
                reg.gpr[0x05] = 0xfffffffff45231e5;
                reg.gpr[0x06] = 0xffffffffa4001f0c;
                reg.gpr[0x07] = 0xffffffffa4001f08;
                reg.gpr[0x08] = 0x070;
                reg.gpr[0x09] = 0;
                reg.gpr[0x0a] = 0x040;
                reg.gpr[0x0b] = 0xffffffffa4000040;
                reg.gpr[0x0c] = 0xffffffffd1330bc3;
                reg.gpr[0x0d] = 0xffffffffd1330bc3;
                reg.gpr[0x0e] = 0x025613a26;
                reg.gpr[0x0f] = 0x02ea04317;
                reg.gpr[0x10] = 0;
                reg.gpr[0x11] = 0;
                reg.gpr[0x12] = 0;
                reg.gpr[0x13] = 0;
		reg.gpr[0x14] = 1; // For now ...
//                reg.gpr[0x14] = (rom->country_code==0x45||rom->country_code==0x4a);
                reg.gpr[0x15] = 0;
                reg.gpr[0x16] = 0x3f;
                reg.gpr[0x17] = 0x06;
                reg.gpr[0x18] = 0;
                reg.gpr[0x19] = 0xffffffffd73f2993;
                reg.gpr[0x1a] = 0;
                reg.gpr[0x1b] = 0;
                reg.gpr[0x1c] = 0;
                reg.gpr[0x1d] = 0xffffffffa4001ff0;
                reg.gpr[0x1e] = 0;
                reg.gpr[0x1f] = 0xffffffffa4001554;

                reg.gpr0[12] = 0x70400004;
        	reg.gpr0[1] = 0x0000001f;
        	reg.gpr0[16] = 0x0006e463; 
        	reg.gpr0[15]   = 0x00000b00;
					reg.VInextInt=625000;
//        gHardwareState.COP1Con[0]      = 0x00000511;
	        *((uint32 *)MIREGS+4)=0x01010101;
//    MI_VERSION_REG_R = 0x01010101;
	reg.CPUdelayPC=0; reg.CPUdelay=0;



	// copy bootcode to SP_DMEM
	memcpy((void *)(RAM_OFFSET_MAP[0x400]+0x4000000),rom->header,0x1000); 
	


  //reg.pc=0xFFFFFFFF00000000+ *((int32*)(RAM_OFFSET_MAP[0x400]+0x4000008));
  reg.pc=0xffffffffa4000040; // bootcode
	ret = 0;
	reg.gpr0[9] = 0;
	lerror=0;


// do crc of boot code and set CIC accordinly
	crc=0;
	for (i=0;i<1008;i++)
	{
		crc+=*((uint32*)(RAM_OFFSET_MAP[0x400]+0x4000040+i));
	}			
  switch(crc)
	{
	case 0x98f85f89:
		reg.gpr[0x16] = 0x3f;
  		break;
	case 0x13fd446f:				
		reg.gpr[0x16] = 0x91;
  		break;
	case 0xb31279a3:
		reg.gpr[0x16] = 0x78;
  		break;
	case 0xe645acd6:
		reg.gpr[0x16] = 0x85;
  		break;
	default:
		printf("Unknown bootloader crc 0x%lx\n",crc);
		break;
	}

	if (patches_active) {
		apply_patches();
	}

	while(lerror==0)
	{

#if 0
		if(!op)
	 	 {
#ifdef DEBUG
		 printf("0x%.8x: <0x%.8x>        NOP\n",reg.pc,op);
#endif
   	         switch (CPUdelay)
 	          {
        	    case 0 :    reg.pc += cpu_step; break;
	            case 1 :    reg.pc += cpu_step; CPUdelay = 2; break;
	            default:    reg.pc = reg.CPUdelayPC; reg.CPUdelay = 0; break;
	          }
		   continue;  // skip nops.. might speed things up
		 }
#endif




                rpc=reg.pc&0x1fffffff;
                op=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));

#ifdef DEBUG
	if(debugger.print) {
		printf("0x%.8x: <0x%.8x> ",(uint32)reg.pc,op);
        	dcpu_instr[opcode(op)>>26]();
		fflush(stdout);
		disasmcount++;
	  }
#endif

#ifdef DEBUG
		debugger_step();
#endif
    		ecpu_instr[opcode(op)>>26]();
		reg.gpr0[9]+=counter_factor;

          switch (reg.CPUdelay)
           {
            case 0 :    reg.pc += cpu_step; break;
            case 1 :    reg.pc += cpu_step; reg.CPUdelay = 2; break;
            default:    reg.pc = reg.CPUdelayPC; reg.CPUdelay = 0; break;
           }

	if(reg.gpr0[9]==reg.VInextInt)
	{
    	  GenerateInterrupt(MI_INTR_VI);
    	  reg.VInextInt+=625000;
  	}

  	if ((uint32)reg.gpr0[9]==(uint32)reg.gpr0[11]&&(uint32)reg.gpr0[9])
  	  { printf("Timer interrupt generated: count: 0x%x compare:0x%x\n",(uint32)reg.gpr0[9],(uint32)reg.gpr0[11]);
 	    GenerateTimerInterrupt();
	  }

        if (PossibleInterrupt)
          CheckInterrupts();

    while(othertask)
     {
	if(othertask&OTHER_PIF)
	 {
	   pifram_interrupt();
	   othertask&=~OTHER_PIF;
	 } 
	else if (othertask&OTHER_DMA_RD)
	 {
	   PI_DMA_Transfer_RD();
	   othertask&=~OTHER_DMA_RD;
	 }
	else if (othertask&OTHER_DMA_WR)
	 {
	   PI_DMA_Transfer_WR();
	   othertask&=~OTHER_DMA_WR;
	 }
        else if (othertask&OTHER_SP_DMA_RD)
         {
	   SP_DMA_Transfer_RD();
           othertask&=~OTHER_SP_DMA_RD;
         }
        else if (othertask&OTHER_SP_DMA_WR)
         {
	   SP_DMA_Transfer_WR();
           othertask&=~OTHER_SP_DMA_WR;
         }
	else if (othertask&OTHER_SI_DMA_RD)
	 {
           si_dma_transfer_read();
           othertask&=~OTHER_SI_DMA_RD;
	 }
        else if (othertask&OTHER_SI_DMA_WR)
         {
           si_dma_transfer_write();
           othertask&=~OTHER_SI_DMA_WR;
         }
	else if (othertask&OTHER_AI)
	 {
	   addr2=(*((uint32 *)(AIREGS)))&0x1fffffff;
           ai_dma_request(AIREGS,addr2+RAM_OFFSET_MAP[addr2>>16]);
			othertask&=~OTHER_AI;
	 }
     }


#ifdef EXCESSIVE_DEBUG
		debug_do_a_dump();
#endif


// deal with interrupts/exceptions after updating delay so we can really tell if we're in a slot



//	  vi_display(VIREGS,(uint8*)(*((uint32*)(VIREGS+4))+RAM_OFFSET_MAP[*((uint32*)(VIREGS+4))>>16]));
	}
	do_a_dump();
	printf("execution ended at 0x%x\n",reg.pc);
	printf("Last opcode: "); fflush(stdout);
        dcpu_instr[opcode(op)>>26]();
        fflush(stdout);	
	printf("%i instructions executed\n", reg.gpr0[9]);
#ifdef DEBUG
        printf("%i instructions disassembled\n",disasmcount);
#endif
}

void debug_do_a_dump() {

	int i;
	printf("GPRs:\n");
	for(i = 0; i <= 31; i++) {
                printf("$%s: %16llx   ", reg_names[i], reg.gpr[i]);
                if ((i % 3)) {
                        printf("\n");
                }
        }
	printf("\n");
}

void do_a_dump()
{
	int i;
	fprintf(stdout, "GPRs:\n");
	for(i = 0; i < 16; i++) {
		fprintf(stdout, "$%s: 0x%.16llx   ", reg_names[i], reg.gpr[i]);
		fprintf(stdout, "$%s: 0x%.16llx   ",reg_names[i+16],reg.gpr[i+16]);
			fprintf(stdout, "\n");
	}

	fprintf(stdout, "FPRs:\n");
	for(i = 0; i < 16; i++) {
		fprintf(stdout, "$f%d: 0x%.8x   ", i, reg.gpr1[i]);
		fprintf(stdout, "$f%d: 0x%.8x   ",i+16,reg.gpr1[i+16]);
			fprintf(stdout, "\n");
	}


	fprintf(stdout,"\n");
        fprintf(stdout, "Hi:  0x%.16llx   Lo:  0x%.16llx   \n",reg.HI,reg.LO);
	fprintf(stdout, "\n-----------\n\n");

	fprintf(stdout, "COP0 GPRs:\n");
	for(i = 0; i <= 31; i++) {
		fprintf(stdout, "$%2i: 0x%.16llx   ", i, reg.gpr0[i]);
		if ((i % 3) == 2) {
			fprintf(stdout, "\n");
		}
	}
	fprintf(stdout, "\n");
}
/* old one
int do_a_dump()
{
	int i;
	fprintf(stderr, "GPRs:\n");
	for(i = 0; i <= 31; i++) {
		fprintf(stderr, "$%s: 0x%.16llx   ", reg_names[i], reg.gpr[i]);
		if ((i % 3)==2) {
			fprintf(stderr, "\n");
		}
	}
	fprintf(stderr,"\n");
        fprintf(stderr, "Hi:  0x%.16llx   Lo:  0x%.16llx   \n",reg.HI,reg.LO);
	fprintf(stderr, "\n-----------\n\n");

	fprintf(stderr, "COP0 GPRs:\n");
	for(i = 0; i <= 31; i++) {
		fprintf(stderr, "$%2i: 0x%.16llx   ", i, reg.gpr0[i]);
		if ((i % 3) == 2) {
			fprintf(stderr, "\n");
		}
	}
	fprintf(stderr, "\n");
}
*/
