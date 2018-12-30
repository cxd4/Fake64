#include <signal.h>
#include <general.h>
#include <romheader.h>
#include "decode.h"
#include "registers.h"
#include "opcodes.h"
#include <memory.h>

#define USE_DEBUGGER 1

#undef DEBUG 1
#undef EXCESSIVE_DEBUG 1

extern void sig_stop_run();

char *module_id(void)
{ return("N64 pure C interpreter cpu core (Version 0.00000002 by Bluefyre and HCl)"); }

extern cpu_instruction dcpu_instr[64];
extern cpu_instruction dcpu_special[64];
extern cpu_instruction dcpu_regimm[32];

extern cpu_instruction ecpu_instr[64];
extern cpu_instruction ecpu_special[64];
extern cpu_instruction ecpu_regimm[32];


extern int print;

struct cpu_reg reg;
struct rom *currentrom;
int lerror=0;
uint32 rpc=0;

extern void init_pifram(uint8*);
extern char* reg_names[];

void debugger_step(void);

void main_cpu_loop(struct rom *rom)
{
        uint32 op;
#if DEBUG || USE_DEBUGGER
	int disasmcount=0;
#endif
	int ret;
	currentrom=rom;
	alloc_memory(rom);
	init_pifram(PIFMEM);

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
//        gHardwareState.COP1Con[0]      = 0x00000511;
	        *((uint32 *)MIREGS+4)=0x01010101;
//    MI_VERSION_REG_R = 0x01010101;

	reg.CPUdelayPC=reg.CPUdelay=0;

	// copy bootcode to SP_DMEM
	memcpy((void *)(RAM_OFFSET_MAP[0x400]+0x4000000),rom->header,0x1000); 
	reg.pc=0xffffffffa4000040; // bootcode
	ret = 0;
	reg.gpr0[9] = 0;
	lerror=0;

	while(lerror==0) {
#ifdef DEBUG
		if(((reg.pc&0x1fffffff)-pcbase)<0)
		 { printf("ERROR! PC isn't valid!: pc: %d\n",reg.pc);
		   exit(-1);
		 }
#endif
		rpc=reg.pc&0x1fffffff;
		op=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
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
	            default:    reg.pc = CPUdelayPC; CPUdelay = 0; break;
	          }
		   continue;  // skip nops.. might speed things up
		 }
#endif

#if DEBUG || USE_DEBUGGER
	if(print) {
		printf("0x%.8x: <0x%.8x>	",(uint32)reg.pc,op);
        	dcpu_instr[opcode(op)>>26](op);
		fflush(stdout);
		disasmcount++;
	  }
#endif

#ifdef USE_DEBUGGER
		debugger_step();
#endif

                ecpu_instr[opcode(op)>>26](op);
		if (MI_INTR_REG_R & MI_INTR_MASK_REG_R & 0x3f)
		 printf("Pending interrupt! pc:0x%x\n",reg.pc);

#ifdef EXCESSIVE_DEBUG
		debug_do_a_dump();
#endif
          switch (reg.CPUdelay)
           {
            case 0 :    reg.pc += cpu_step; break;
            case 1 :    reg.pc += cpu_step; reg.CPUdelay = 2; break;
            default:    reg.pc = reg.CPUdelayPC; reg.CPUdelay = 0; break;
           }
	reg.gpr0[9]++;
	}
	do_a_dump();
	printf("execution ended at 0x%x\n",reg.pc);
	printf("Last opcode: "); fflush(stdout);
        dcpu_instr[opcode(op)>>26](op);
        fflush(stdout);	
	printf("%i instructions executed\n", reg.gpr0[9]);
#if DEBUG || USE_DEBUGGER
        printf("%i instructions disassembled\n",disasmcount);
#endif
}

int debug_do_a_dump() {

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
