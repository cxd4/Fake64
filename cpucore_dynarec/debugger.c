#include <general.h>
#include <romheader.h>
#include "registers.h"
#include <memory.h>
#include <decode.h>


#define LOG_LAST_INST 1000000
//#define STORE_BLOCKS

long long lastpc[LOG_LAST_INST];
int lastpccount=0;
int logpc=0;
#ifdef STORE_BLOCKS
extern uint32 recompiledblocks[10000][3];
extern uint32 recompiledblockcounter;
#endif
typedef
struct bp
{
	int line;
	int count;
}bp;

extern uint32 op;

extern void vi_display(uint8* regs,uint16 *addr);

extern char* reg_names[];
extern struct cpu_reg reg;
char lastcommand[200];
int run=0,print=0;
bp breakpoints[128];
int breakpointcount=0;

int watchreg=-1;
uint64 oldwatch=0;

extern FILE* disfd;
extern cpu_instruction dcpu_instr[64];
void sig_stop_run();

FILE *flogpc;

void debugger_step(void)
{ char buffer[200];
  char command[100];
  int i,i6=0,j;
  FILE *fp;
  uint32 addr,oldop,addr2;
  char* sp; // second parameter
  void *crap,*newblock;
  if (logpc)
  {
    // log each time we see a new pc
    for (i=lastpccount;i>=0;i--)
    {
      if (lastpc[i]==reg.pc)
         break;
    }

    if (i==lastpccount || i==0 || i==-1)
    {
      fprintf(flogpc,"0x%lx\t",reg.pc);
      disfd=flogpc;
 	    dcpu_instr[opcode(op)>>26]();
			disfd=stdout;
 			lastpc[lastpccount++]=reg.pc;
    }
  //  printf("%d\n",lastpccount);
    if (lastpccount>=LOG_LAST_INST)
    {
      printf("WARNING logpc ran out of instrs\n");
    	lastpccount=0;
    }

  }
  while(i6<breakpointcount)
  { if(breakpoints[i6].line==(uint32)reg.pc)
      {
        breakpoints[i6].count--;
				if (breakpoints[i6].count<1) {
        run=0;
        printf("Breakpoint %d: 0x%x\n",i6+1,reg.pc&0xFFFFFFFF);
        breakpoints[i6].count=1;
        }
     //   else
       // printf("Breakpoint %d passed: 0x%x\n",i6+1,reg.pc);

      }
    i6++;
  }
  if (watchreg != -1) {
     if (reg.gpr[watchreg] != oldwatch) {
	printf("Watch on $%s caused breakpoint\n", reg_names[watchreg]);
	printf("Was %.16llx, is %.16llx", oldwatch, reg.gpr[watchreg]);
	watchreg = -1;
	oldwatch = 0;
	run = 0;
     }
  }
  if (run) return;
  printf("(Debugger) ");
  fflush(stdout);
  while(fgets(buffer,99,stdin))
    { buffer[strlen(buffer)-1]='\0';
#ifdef DEBUG
      printf("Buffer:%s Lastcommand:%s\n",buffer,lastcommand);
#endif
      if(!strlen(buffer))
        strcpy(buffer,lastcommand);
      else
        strcpy(lastcommand,buffer);
      for(i=0;buffer[i]&&(buffer[i]!=0x20);i++)
        command[i]=buffer[i];
      command[i]='\0';
      strcpy(buffer,lastcommand);
#ifdef DEBUG
      printf("Buffer:%s Lastcommand:%s command:%s\n",buffer,lastcommand,command);
#endif

      if(!strncmp(command,"run",strlen(command)))
        { run=1; return; }
      else if (!strncmp(command,"print",strlen(command)))
        { print=1; return; }
      else if (!strncmp(command,"next",strlen(command)))
        return;
      else if (!strncmp(command,"noprint",strlen(command)))
        { print=0; return; }

      else if (!strncmp(command,"delete",strlen(command)))
        {
         int bp=strtoul(buffer+strlen(command)+1,&sp,0);
 		     breakpoints[bp].count=0;
				}
 			else if (!strncmp(command,"breakpoint",strlen(command)))
        {
          if(!strtoul(buffer+strlen(command)+1,&sp,16))
           { printf("Invalid address: %s.\n",buffer+strlen(command)+1); }
          else
            {
            	breakpoints[breakpointcount].line=strtoul(buffer+strlen(command)+1,NULL,16);
              breakpoints[breakpointcount].count=strtoul(sp,NULL,0);
	            printf("breaking on %x, on a count of %d\n",breakpoints[breakpointcount].line,breakpoints[breakpointcount].count);
              breakpointcount+=1;
            }
        }
      else if (!strncmp(command,"watch",strlen(command)))
	{
	  int tempreg;
	  if((tempreg = strtoul(buffer+strlen(command)+1,NULL,10)) || !strncmp(buffer+strlen(command)+1,"0",2)) {
	     if (tempreg > -1 && tempreg < 33) {
		watchreg=tempreg;
		oldwatch=reg.gpr[watchreg];
		printf("Watching $%s\n", reg_names[watchreg]);
	     } else {
		printf("No such register!\n");
	     }
	  } else {
		printf("Specify a register number!\n");
	  }
	}
      else if (!strncmp(command,"show",strlen(command)))
        {
          if(!strtoul(buffer+strlen(command)+1,NULL,16))
            { printf("Invalid address: %s.\n",buffer+strlen(command)+1); }
          else
           {  i=strtoul(buffer+strlen(command)+1,NULL,16);
              printf("0x%x (0x%x+0x%x=0x%x): 0x%x\n",i,(i&0x1fffffff),RAM_OFFSET_MAP[(i&0x1fffffff)>>16],RAM_OFFSET_MAP[(i&0x1fffffff)>>16]+(i&0x1fffffff),*((int64 *)(RAM_OFFSET_MAP[(i&0x1fffffff)>>16]+(i&0x1fffffff))));
           }
        }
      else if (!strncmp(command,"dumpregs",strlen(command)))
         do_a_dump();
      else if (!strncmp(command,"help",strlen(command)))
        {
           printf("\nCommands:\n run\n next\n breakpoint address [count]\n show address\n dumpregs\n help\n quit\n disassemble [address] [lines]");
	         printf("\n print\n noprint\n\n");
        }
      else if (!strncmp(command,"quit",4))
             exit(1);
      else if (!strncmp(command,"qui",strlen(command)))
        { printf("If you want to quit, spell it out.\n"); }
   else if (!strncmp(command,"disassemble",strlen(command)))
        {
            oldop=op;
            addr=strtoul(buffer+strlen(command)+1,&addr2,0); // the address
            addr&=~0x3; // instrs are aligned to 4 bytes
            j=strtoul(addr2,NULL,0); // number of lines
        //    printf ("%s \n",addr2);
            if (!j)
              j=10;

            if (addr==0)
            	addr=(reg.pc&0x1fffffff);
            for (i6=0;i6<j;i6++)
            {
              op=*((uint32 *)(((addr+(i6*4))&0x1fffffff)+RAM_OFFSET_MAP[((addr+(i6*4))&0x1fffffff)>>16]));
              printf("0x%.8x: <0x%.8x>",(uint32)addr+(i6*4),op,addr);
        	    dcpu_instr[opcode(op)>>26]();
            }
            op=oldop; // maybe needed
        }
      else if (!strncmp(command,"logpc",strlen(command)))
			{
        logpc=1;
      	flogpc=fopen("log.pc","w");
        printf("logging pc\n");
       }
       else if (!strncmp(command,"nologpc",strlen(command)))
       {
        logpc=0;
        fclose(flogpc);
       }
#ifdef STORE_BLOCKS
      else if (!strncmp(command,"dumpcode",8))
	{ 
	  printf("Dumping code...\n");
	  fp=fopen("/tmp/dumpedcode","w");
	  for(i=0;i<recompiledblockcounter;i++)
	   {
	     fprintf(fp,"++++ 0x%x 0x%x\n",recompiledblocks[i][0],recompiledblocks[i][2]);
	     fwrite(recompiledblocks[i][1],recompiledblocks[i][2],1,fp);	
	     fprintf(fp,"----\n");
	   }
	  fclose(fp);
	  printf("Code dumped\n");
	}
      else if (!strncmp(command,"readcode",8))
        { printf("Reading code....\n");
	  fp=fopen("/tmp/dumpedcode","r");
	  while(!feof(fp))
	   {
    	     crap=0; 
	     i=1;
  	     codeblock =(void *) calloc(1,32768); // default to this
  	     while((((uint32)codeblock)&0x1f)) // things should be aligned... just checking          
	      { if(crap) free(crap);
	        free(codeblock);    // might go into an endless loop too
	        crap=calloc(1,i++);
	        codeblock =(void *) calloc(1,32768); // default to this
	      }
	     
	   }	  
        }
#endif
      else if (!strncmp(command,"display",strlen(command)))
   		{
//             addr2=*((uint32 *)(VIREGS))&0x1fffffff;
//	           oldop=(VIREGS+4) + RAM_OFFSET_MAP[*(VIREGS+4)>>16];
//             printf("%x %x\n",oldop,oldop); //*((int64 *)(RAM_OFFSET_MAP[(oldop&0x1fffffff)>>16]+(oldop&0x1fffffff))));
		printf("lies: %x\n", *((uint32*)(VIREGS+4))+RAM_OFFSET_MAP[*((uint32*)(VIREGS+4))>>16]);
		vi_display(VIREGS,(uint16*)(*((uint32*)(VIREGS+4))+RAM_OFFSET_MAP[*((uint32*)(VIREGS+4))>>16]));
        }
   else
        { printf("Unknown command \"%s\".",buffer); return; }
      printf("(Debugger) ");
      fflush(stdout);
    }
}

void sig_stop_run() {
	if(run) {
		run = 0;
	} else {
		printf("Bye bye\n");
		exit(0);
	}
}
