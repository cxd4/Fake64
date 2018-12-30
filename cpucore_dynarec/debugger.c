#include <general.h>
#include <romheader.h>
#include "registers.h"
#include <memory.h>
#include <decode.h>
#include <signal.h>
#include "debugger.h"
#include <module.h>

struct sdebugger debugger; // contains all the things we need to share




#include "linking.h"


#define LOG_LAST_INST 1000000
int64 lastpc[LOG_LAST_INST];
int lastpccount=0;
int logpc=0;

typedef
struct bp
{
	int line;
	int count;
}bp;

extern uint32 op;

extern struct module_info* modules;

extern void GenerateInterrupt(uint32);

int write_savestate(char *filename);
int read_savestate(char *filename);
void do_a_dump();

extern char* reg_names[];
extern struct cpu_reg reg;


char lastcommand[200];
bp breakpoints[128];
int breakpointcount=0;
uint32 tempopcode;

static volatile char searching=1;	// fo searching



void endsearch(int signal)
{
	searching=0;
}

// read/write/number watches
watch watches[100];
int   nwatches=0;

// for bactracing pc a few instructions
uint64 btpc[100];
int   ibtpc=0;

int watchreg=-1;
uint64 oldwatch=0;

extern FILE* disfd;
extern cpu_instruction dcpu_instr[64];
void sig_stop_run();

FILE *flogpc;

void debugger_step(void)
{
	char buffer[200];
  char command[100];
  int i,j;
  uint32 addr,oldop,addr2;
  char* sp; // second parameter


	// this is for btpc, hardwired to 100 instructions
  btpc[ibtpc++]=(uint32)reg.pc;
  if (ibtpc==100)
    ibtpc=0;


 if (logpc)
  {
    // log each time we see a new pc
    for (i=lastpccount;i>=0;i--)
      if (lastpc[i]==reg.pc)
         break;

    if (i==lastpccount || i==0 || i==-1)
    {
      fprintf(flogpc,"0x%x\t",(uint32)reg.pc);
      disfd=flogpc;
 	    dcpu_instr[opcode(op)>>26]();
			disfd=stdout;
 			lastpc[lastpccount++]=reg.pc;
    }

    if (lastpccount>=LOG_LAST_INST)
    {
      printf("WARNING logpc ran out of instrs\n");
    	lastpccount=0;
    }
  }

	// check for breakpoints
  i=0;
  while(i<breakpointcount)
  {
		if(breakpoints[i].line==(uint32)reg.pc)
      {
        breakpoints[i].count--;
				if (breakpoints[i].count<1)
				{
        	debugger.run=0;
        	printf("Breakpoint %d: 0x%x\n",i+1,(uint32)reg.pc&0xFFFFFFFF);
        	breakpoints[i].count=1;
        }
      }
    i++;
  }

  if (watchreg != -1) {
     if (reg.gpr[watchreg] != oldwatch) {
	printf("Watch on $%s caused breakpoint\n", reg_names[watchreg]);
	printf("Was %.16llx, is %.16llx", oldwatch, reg.gpr[watchreg]);
	watchreg = -1;
	oldwatch = 0;
	debugger.run = 0;
     }
  }
  if (debugger.run) return;
  printf("(Debugger) ");
  fflush(stdout);
  while(fgets(buffer,99,stdin))
    { buffer[strlen(buffer)-1]='\0';
      if(!strlen(buffer))
        strcpy(buffer,lastcommand);
      else
        strcpy(lastcommand,buffer);
      for(i=0;buffer[i]&&(buffer[i]!=0x20);i++)
        command[i]=buffer[i];
      command[i]='\0';
      strcpy(buffer,lastcommand);

      if(!strncmp(command,"run",strlen(command)))
        { debugger.run=1; return; }
      else if (!strncmp(command,"print",strlen(command)))
        { debugger.print=1; return; }
      else if (!strncmp(command,"next",strlen(command)))
        return;
      else if (!strncmp(command,"noprint",strlen(command)))
        { debugger.print=0; return; }

        else if (!strncmp(command,"btpc",strlen(command)))
        { int i=0;
          for (i=0;i<100;i++)
          {
              addr=btpc[(i+ibtpc)%100];
              op=*((uint32 *)(((addr)&0x1fffffff)+RAM_OFFSET_MAP[((addr)&0x1fffffff)>>16]));
              printf("0x%.8x: <0x%.8x>",(uint32)addr,op);
              dcpu_instr[opcode(op)>>26]();
          }
        }
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
              printf("0x%x (0x%x+0x%x=0x%x): 0x%x\n",i,(i&0x1fffffff),RAM_OFFSET_MAP[(i&0x1fffffff)>>16],RAM_OFFSET_MAP[(i&0x1fffffff)>>16]+(i&0x1fffffff),(uint32)*((int64 *)(RAM_OFFSET_MAP[(i&0x1fffffff)>>16]+(i&0x1fffffff))));
           }
        }
      else if (!strncmp(command,"dumpregs",strlen(command)))
         do_a_dump();
      else if (!strncmp(command,"help",strlen(command)))
        {
           printf("\nCommands:\n run\n next\n breakpoint address [count]\n show address\n dumpregs\n help\n quit\n disassemble [address] [lines]");
	         printf("\n print\n noprint\n\n");
        }
      else if (!strncmp(command, "rset", strlen(command)))
	{
	   int rnum;
	   char *curptr;
	   uint64 rval;

	   if(!(strlen(buffer) > strlen(command))) {
		printf("Please provide valid arguments\n");
		return;
	   }

	   curptr = buffer+strlen(command)+1;
	   rnum = strtoul(curptr + 1, NULL, 0);
	   rval = strtoul(strchr(curptr, ' ')+1, NULL, 0);
	   switch(curptr[0]) {
		case 'g':
			printf("Setting gpr %i to %llx\n", rnum, rval);
			reg.gpr[rnum] = rval;
			break;
		case 'f':
			printf("Setting fpr %i to %llx\n", rnum, rval);
			reg.gpr1[rnum] = rval;
			break;
		case 'c':
			printf("Setting cop0 gpr %i to %llx\n", rnum, rval);
			reg.gpr0[rnum] = rval;
			break;
		case 'o':
			printf("Setting cop0 control reg %i to %llx\n", rnum, rval);
			reg.cr0[rnum] = rval;
			break;
		case 'O':
			printf("Setting cop1 control reg %i to %llx\n", rnum, rval);
			reg.cr1[rnum] = rval;
			break;
		default:
			printf("Not a supported register type!\n");
			printf("Usage: rset regcode || regnum value\n");
			printf("Example: rset g0 0 sets gpr r0 to 0\n");
			printf("Supported codes: gfcoO\n");
			break;
	   }
        }
      else if (!strncmp(command,"oset",4))
	{
	   tempopcode = strtoul(buffer+strlen(command)+1,NULL,0);
	   printf("Temp opcode set to: %x\n", tempopcode);
	}
      else if (!strncmp(command,"odecode",7))
	{
	   uint32 holdrealop;
	   holdrealop = op;
	   op = tempopcode;
	   dcpu_instr[opcode(op)>>26]();
	   op = holdrealop;
	}
      else if (!strncmp(command,"causeint",8)) {
	   int intn;
	   intn = strtoul(buffer+strlen(command)+1,NULL,0);
	   printf("Generating %#x\n", intn);
	   GenerateInterrupt(intn);
	}
      else if (!strncmp(command,"quit",4))
             exit(1);
      else if (!strncmp(command,"qui",strlen(command)))
        { printf("If you want to quit, spell it out.\n"); }
   else if (!strncmp(command,"disassemble",strlen(command)))
        {
            oldop=op;
            addr=strtoul(buffer+strlen(command)+1,(char **)(&addr2),0); // the address
            addr&=~0x3; // instrs are aligned to 4 bytes
            j=strtoul((char *)addr2,NULL,0); // number of lines
        //    printf ("%s \n",addr2);
            if (!j)
              j=10;

            if (addr==0)
            	addr=(reg.pc&0x1fffffff);
            for (i=0;i<j;i++)
            {
              op=*((uint32 *)(((addr+(i*4))&0x1fffffff)+RAM_OFFSET_MAP[((addr+(i*4))&0x1fffffff)>>16]));
              printf("0x%.8x: <0x%.8x>",(uint32)addr+(i*4),op);
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
   else if (!strncmp(command,"search",strlen(command))) // search "string" | number
   {
     char *ra;
			char *string=NULL;		// what to search for
      sighandler_t oldh;
			char *p=buffer+strlen(command)+1;
			
			searching=1;	
			while(*p==' ')
				p++;				// skip spaces
			
			if (*p=='"')
			{
      	++p;	
        string=p;
				while(*p++!='"')
					;	
					*(p-1)=0;		// end " -> \0

				

			}
			else
				printf("only searches for strings atm\n");			

			addr=strtoul(p+1,NULL,0);
			printf("Search for %s, Address 0x%x\n",string,addr);
      ra=(uint8 *)(((addr)&0x1fffffff)+RAM_OFFSET_MAP[((addr)&0x1fffffff)>>16]);
			
			oldh=signal(11,endsearch);
      while (strncmp(ra,string,strlen(string)) )
			{
	    printf("doggy\n");
			ra++;
	if (!searching)
		break;
	//	printf("There are %x fishes in the sea\n",searching);
}
      if (searching)
				printf("Found at 0x%x\n",(uint32)ra-(((addr)&0x1fffffff)+RAM_OFFSET_MAP[((addr)&0x1fffffff)>>16])+addr);
      else
				printf("%s not found\n",string);
			signal(11,oldh);

	 }





 else if (!strncmp(command,"display",strlen(command)))
   		{
//             addr2=*((uint32 *)(VIREGS))&0x1fffffff;
//	           oldop=(VIREGS+4) + RAM_OFFSET_MAP[*(VIREGS+4)>>16];
//             printf("%x %x\n",oldop,oldop); //*((int64 *)(RAM_OFFSET_MAP[(oldop&0x1fffffff)>>16]+(oldop&0x1fffffff))));
		printf("lies: %x\n", *((uint32*)(VIREGS+4))+RAM_OFFSET_MAP[*((uint32*)(VIREGS+4))>>16]);
		modules->video->vi_display_f(VIREGS,(uint16 *)(*((uint32*)(VIREGS+4))+RAM_OFFSET_MAP[*((uint32*)(VIREGS+4))>>16]));
        }
       else if (!strncmp(command,"rwatch",strlen(command)))
        {
            uint32 tempreg;
                if((tempreg = strtoul(buffer+strlen(command)+1,NULL,0)))
          {
            watches[nwatches].Type=WATCH_READ;
            watches[nwatches++].Address=tempreg;
            printf("Read watch on 0x%x\n",tempreg);
          }
        }
      else if (!strncmp(command,"wwatch",strlen(command)))
        {
            uint32 tempreg;
                if((tempreg = strtoul(buffer+strlen(command)+1,NULL,0)))
          {
            watches[nwatches].Type=WATCH_WRITE;
            watches[nwatches++].Address=tempreg;
            printf("Write watch on 0x%x\n",tempreg);
          }
        }
      else if (!strncmp(command,"awatch",strlen(command)))
        {
            uint32 tempreg;
                if((tempreg = strtoul(buffer+strlen(command)+1,NULL,0)))
          {
            watches[nwatches].Type=WATCH_READ|WATCH_WRITE;
            watches[nwatches++].Address=tempreg;
            printf("Read/Write watch on 0x%x\n",tempreg);
          }
        }
      else if (!strncmp(command,"nwatch",strlen(command)))
        {
            uint32 tempreg;
                if((tempreg = strtoul(buffer+strlen(command)+1,NULL,0)))
          {
            watches[nwatches].Type=WATCH_NUMBER;
            watches[nwatches++].Value=tempreg;
            printf("Watch for 0x%x on read/writes\n",tempreg);
          }
        }
      else if (!strncmp(command,"save",4))
	{ 
        if(write_savestate(buffer+strlen(command)+1))
          printf("Error saving\n");	
	else
	  printf("Saved\n");
	}
      else if (!strncmp(command,"load",4))
	{
	if(read_savestate(buffer+strlen(command)+1))
	  printf("Error loading\n");
	else
	  printf("Loaded\n");
	}
      else if (!strncmp(command,"x/",2))
		  {
      		uint32 Number;
					char* pFormat;
          char tc;
          Number=strtoul(&command[2],&pFormat,0);
          addr=strtoul(buffer+strlen(command)+1,(char **)&addr2,0); // the address
	//				printf("Address 0x%x, Format %c,Number %d\n",addr,*pFormat,Number);
          if (addr)
					{
						void *ra=(uint32 *)(((addr)&0x1fffffff)+RAM_OFFSET_MAP[((addr)&0x1fffffff)>>16]);
						switch(*pFormat)
						{
						case 'b':	
							for (i=0;i<Number;i++)
							{
                if (!(i%8))
									printf("\n0x%x :\t",addr+i);
                printf("%.2x ",*((unsigned char*)ra+i));
							}
              printf("\n");
							break;
					case 'c':	
							for (i=0;i<Number;i++)
							{
                tc=*((char*)ra+i);
								if (!(i%32))
									printf("\n0x%x :\t",addr+i);
                if ((tc>='a' && tc<='z') || (tc>='A' && tc<='Z') || (tc>='0' && tc<='9'))
                  printf("%c",tc);
								else
									printf(".");
							}
              printf("\n");
							break;
					
 						case 'x':	
							for (i=0;i<Number;i++)
							{
                if (!(i%8))
									printf("\n0x%x :\t",addr+i);
                printf("%.8x ",*((uint32*)ra+i));
							}
              printf("\n");
							break;
					
						default:
							printf("Unknown format '%c'\n",*pFormat);
						}


					}
          else
						printf("Invalid address\n");

			}





else
        { printf("Unknown command \"%s\".\n",buffer); return; }
      printf("(Debugger) ");
      fflush(stdout);
    }
}

void sig_stop_run()
{
	if(debugger.run)
		debugger.run=0;
	else
	{
		printf("Bye bye\n");
		modules->video->vi_deinit_f();
    exit(0);
	}
}
