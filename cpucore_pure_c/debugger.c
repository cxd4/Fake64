#include <general.h>
#include <romheader.h>
#include "registers.h"
#include <memory.h>

extern char* reg_names[];
extern struct cpu_reg reg;
char lastcommand[200];
int run=0,print=0;
unsigned int breakpoints[10];
int breakpointcount=0;

int watchreg=-1;
uint64 oldwatch=0;

void sig_stop_run();

void debugger_step(void)
{ char buffer[200];
  char command[100];
  int i,i6;
  i6=0;
  while(i6<breakpointcount)
  { if(breakpoints[i6]==(uint32)reg.pc)
      { run=0;
        printf("Breakpoint %d: 0x%x\n",i6+1,reg.pc);
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
      else if (!strncmp(command,"breakpoint",strlen(command)))
        {
          if(!strtoul(buffer+strlen(command)+1,NULL,16))
           { printf("Invalid address: %s.\n",buffer+strlen(command)+1); }
          else
            breakpoints[breakpointcount++]=strtoul(buffer+strlen(command)+1,NULL,16);
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
           printf("Commands:\n\nrun next breakpoint show dumpregs help quit\n");
	   printf("print noprint\n");
        }
      else if (!strncmp(command,"quit",4))
             exit(1);
      else if (!strncmp(command,"qui",strlen(command)))
        { printf("If you want to quit, spell it out.\n"); }
      else
        { printf("Unknown command \"%s\".",buffer); return; }
      printf("(Debugger) ");
      fflush(stdout);
    }
}

void sig_stop_run() {
	run = 0;
}
