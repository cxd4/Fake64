#include <signal.h>
#include <general.h>
#include <romheader.h>
#include <memory.h>
#include "registers.h"
#include <decode.h>
#include "x86.h"

#include "module.h"
#include "linking.h"

#define DEBUGMODE 2
#define STORE_BLOCKS

char* reg_names[] = {"r0", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0",
        "t1", "t2", "t3", "t4", "t5", "t6", "t7", "s0", "s1", "s2",
        "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp",
        "sp", "s8", "ra"};

char *module_id(void)
{ return("N64 dynarec x86 cpu core (Version 0.00000001 by HCl) (BROKEN)"); }

#ifdef STORE_BLOCKS
uint32 recompiledblocks[10000][3];
uint32 recompiledblockcounter=0;
#endif 
extern cpu_instruction dyna_instr[64];
extern cpu_instruction dcpu_instr[64];
extern int print;
extern int run;
struct cpu_reg reg;
uint32 op;
uint32 dynop;
uint32 lerror;

struct module_info* modules;

char *codeblockpointer;
int codeblocksize;
int othertask;


// stuff for new memory allocating routines
#define BLOCKSIZE		1024*1024 //
#define MINSIZELEFT		16384
char*	blocks[1000];
uint16	numblocks=0;
char*	nextcodestart;		// 32 byte aligned leaving at least MINSIZELEFT space in a block

uint32 Recompile_Block(void);
void sig_stop_run();
void debugger_step(void);
void CheckInterrupts();
void PI_DMA_Transfer_WR();
void PI_DMA_Transfer_RD();
void SP_DMA_Transfer_WR();
void SP_DMA_Transfer_RD();
void si_dma_transfer_read();
void si_dma_transfer_write();
void GenerateTimerInterrupt();
void GenerateInterrupt(uint32 type);

extern uint8 PossibleInterrupt;


void init_regs(void)
{
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
                *((uint32 *)MIREGS+4)=0x01010101;
        	reg.pc=0xffffffffa4000040;
	

	reg.VInextInt=625000;
}

typedef int (*CodeBlock)(void);

void printcount(int signal)
{
 printf("count as 32 bit is 0x%llx\n",reg.gpr0[9]);
 printf("count as 64 0x%llx\n",reg.gpr0[9]);
 printf("nextvi as 32 bit is 0x%llx\n",reg.VInextInt);
 printf("nextvi as 64 0x%llx\n",reg.VInextInt);
}

void main_cpu_loop(struct rom *rom,struct module_info* mods)
{	uint32 *lop,rpc;
	uint32 addr2;
	uint32 countstate;

	modules=mods;

	signal(SIGINT, sig_stop_run);

    alloc_memory(rom);
	init_regs();
    modules->input->init_pifram_f(modules,PIFMEM);

	// allocate first block
	blocks[0]=malloc(BLOCKSIZE);
	nextcodestart=(char *)(((uint32)(blocks[0]+32))&~31);
	numblocks++;


	// copy bootcode to SP_DMEM
	memcpy((void *)(RAM_OFFSET_MAP[0x400]+0x4000000),rom->header,0x1000);		
	while(1) {
           rpc=reg.pc&0x1fffffff;
           lop=(uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]);
	   countstate=(uint32)reg.gpr0[9];
	   if ((*lop>>26)==59) // if it's our special opcode, call the block
       {
		   
			PUSHALLREGS();
	//			printf("calling block at 0x%x\n",((*lop&0x03FFFFFF)<<5) );
				((int (*)()) ((*lop&0x03FFFFFF)<<5) )();
			POPALLREGS();
 	   
	   }
	    else
             { *lop=Recompile_Block();
#if DEBUGMODE==2
	     debugger_step();
#endif
	     }
#if DEBUGMODE==1      
                debugger_step();
#endif  

	    while(othertask)
	     {
	        if(othertask&OTHER_PIF)
        	 {
	           modules->input->pifram_interrupt_f();
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
	           addr2=*((uint32 *)(AIREGS))&0x1fffffff;
	           modules->audio->ai_dma_request_f(AIREGS,addr2+RAM_OFFSET_MAP[addr2>>16]);
        	                othertask&=~OTHER_AI;
	         }     
	     }

/*	    if (((uint32)reg.gpr0[9]>=(uint32)reg.gpr0[11])&&countstate<=reg.gpr0[11])
             {  printf("Timer interrupt: count: 0x%x compare: 0x%x\n",(uint32)reg.gpr0[9],(uint32)reg.gpr0[11]);
		GenerateTimerInterrupt();}*/
	    if( ((uint32)reg.gpr0[9]>=(uint32)reg.VInextInt)&&countstate<=reg.VInextInt)
	     {
          GenerateInterrupt(MI_INTR_VI);
	        reg.VInextInt=reg.gpr0[9]+625000;
	     }
	    if(PossibleInterrupt) 
		CheckInterrupts();
	}
}
uint32 jumped;
uint32 currentpc;
uint32 count;

uint32 Recompile_Block(void)
{ char *Newcodeblock;
  uint32 rpc;

	if (nextcodestart-blocks[numblocks-1]>BLOCKSIZE-MINSIZELEFT)	// if not enough space
	{
		printf("New Block\n");
		// new block
		blocks[numblocks]=malloc(BLOCKSIZE);
		nextcodestart=(char *)(((uint32)(blocks[numblocks]+32))&~31);
		numblocks++;
	}

  codeblockpointer=nextcodestart;
  Newcodeblock=codeblockpointer;
  count=0;

  printf("Recompiling block of code... 0x%x @ 0x%x\n",(uint32)reg.pc,(uint32)Newcodeblock);

  // Begin block with:

  // nothing atm
  jumped=0;
  currentpc=reg.pc;
  while(!jumped)
   { rpc=currentpc&0x1fffffff;
     dynop=*((uint32 *)(rpc+RAM_OFFSET_MAP[(rpc)>>16]));
  //   printf("PC= 0x%x %x\n",(uint32)rpc,&reg.pc);
     if (dynop)
      dyna_instr[opcode(dynop)>>26]();
     count++;
     currentpc+=4;
   }
  // End block with:
  // increase of countreg
/*
  ADD_ImmToMem(&COP0_COUNT,count);
  if(jumped==1)
    {
     // modification of reg.pc
     currentpc+=4; // to skip delay slot
     MOV_Imm32ToMem(&reg.pc,currentpc);
     MOV_Imm32ToMem((uint8*)&reg.pc+4,(currentpc & 0x80000000) ? 0xFFFFFFFF : 0x00000000);
    }
     // ret or a jump to the next block (ret for now)    
     RET();
*/


#ifdef STORE_BLOCKS
  recompiledblocks[recompiledblockcounter][0]=(uint32)reg.pc;
  recompiledblocks[recompiledblockcounter][1]=(uint32)Newcodeblock;
  recompiledblocks[recompiledblockcounter++][2]=(uint32)codeblockpointer-(uint32)Newcodeblock;
#endif

  nextcodestart=(char *)(((uint32)(codeblockpointer+32))&~31); // setup where to start next

  
  return ((59<<26)|(((uint32)Newcodeblock)>>5));
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

// dummy config function
void config_module(char* conf)
{
}
