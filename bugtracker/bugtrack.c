#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>


// #define DEBUG

interp(int input[2],int output[2])
{
  close(0);
  close(1);
  dup(input[0]);
  dup(output[1]);
  execl("/usr/local/bin/fake64","fake64","/home/HCl/src/emu/n64stars.v64",0);
}

dynap(int input[2],int output[2])
{
  close(0);
  close(1);
  dup(input[0]);
  dup(output[1]);
  execl("/usr/local/bin/fake64","fake64","/home/HCl/src/emu/n64stars.v64",0);
}

struct fake64 {
     unsigned int PC;
     char dumpinfo[100][200];
     char readdump;
};

read_dump(struct fake64 *dest,int fd)
{ char buf[200];
  char *p;
  int i=0;
  memset(buf,0,200);
  while(strncmp(buf,"$30:",4))
   { memset(buf,0,200);
     for(p=buf;*(p-1)!='\n';read(fd,p++,1));
     strcpy((*dest).dumpinfo[i++],buf);
   }
 printf("read_dump done\n");
}

compare(int inter_in[2],int inter_out[2],int dyna_in[2],int dyna_out[2])
{ char inter_buf[1000],dyna_buf[1000],*p,temp[200];
  int inter_pos,dyna_pos;
  fd_set rfds;
  struct timeval tv;
  struct fake64 interpreter,dynarec;
  int i;
  inter_pos=0;
  dyna_pos=0;
  memset(&interpreter,0,sizeof(struct fake64));
  memset(&dynarec,0,sizeof(struct fake64));
  write(inter_in[1],"1\n",2);
  write(dyna_in[1],"2\n",2);
  write(dyna_in[1],"next\n",5);
  while(1)
   { 

     FD_ZERO(&rfds);
     FD_SET(inter_out[0], &rfds);     
     FD_SET(dyna_out[0],&rfds);
     tv.tv_sec = 5;
     tv.tv_usec = 0;
     fflush(stdout);
     if(select(100, &rfds, NULL, NULL, &tv))
      { if(FD_ISSET(inter_out[0],&rfds))
	{ read(inter_out[0],inter_buf+inter_pos,1);
	  if(*(inter_buf+inter_pos++)=='\n')
	   { 
// interpreter parser, regard inter_buf as our parameter

        while(!strncmp(inter_buf,"(Debugger) ",11))
          for(i=0;i<strlen(inter_buf);i++)
           inter_buf[i]=inter_buf[i+11];
	if(!strncmp(inter_buf,"Breakpoint ",11))
	 {
	   interpreter.PC=strtoul(inter_buf+14,NULL,0);
	   printf("INTERPRETER PC: 0x%x\n",interpreter.PC);
	   if(interpreter.PC!=dynarec.PC)
	    printf("Error: this isn't the correct breakpoint?\n");
	   else
	    {
	      write(inter_in[1],"clearbreakpoints\n",17);
	      write(inter_in[1],"dump\n",5);
	      write(dyna_in[1],"dump\n",5);
	    }
	 }
	else if(!strncmp(inter_buf,"GPRs:",5))
	 { read_dump(&interpreter,inter_out[0]);
	   interpreter.readdump=1;
	 }
#ifdef DEBUG
	else
	 printf("inter: ignored data %s",inter_buf);
#endif
// end of parser
	     inter_pos=0;
	     memset(inter_buf,0,1000);
	   }
	}
       if(FD_ISSET(dyna_out[0],&rfds))
	{ read(dyna_out[0],dyna_buf+dyna_pos,1);
          if(*(dyna_buf+dyna_pos++)=='\n') 
           { 
// dynarec parser, regard dyna_buf as parameter

	while(!strncmp(dyna_buf,"(Debugger) ",11))
	  for(i=0;i<strlen(dyna_buf);i++)
	   dyna_buf[i]=dyna_buf[i+11];
	if(!strncmp(dyna_buf,"Recompiling block of code...",strlen("Recompiling block of code...")))
	  {
	    dynarec.PC=strtoul(dyna_buf+strlen("Recompiling block of code... "),NULL,0);
	    printf("DYNA PC: 0x%x\n",dynarec.PC);
	    if(dynarec.PC!=0xA4000040)
	     {
	      sprintf(temp,"break 0x%x\nrun\n",dynarec.PC);
	      write(inter_in[1],temp,strlen(temp));
	     }
	  }
        else if(!strcmp(dyna_buf,"GPRs:\n"))
         { read_dump(&dynarec,dyna_out[0]);
           dynarec.readdump=1;
         }
#ifdef DEBUG
	else
	  printf("dyna: ignored data %s",dyna_buf);
#endif
// end of parser
             dyna_pos=0;
             memset(dyna_buf,0,1000);
           }
	}
      }
    if(dynarec.readdump&&interpreter.readdump)
     { i=0;
       while(i<100&&!strcmp(dynarec.dumpinfo[i],interpreter.dumpinfo[i])) i++;
       if(i!=100)
	{ printf("inconsistency in line %d:\n",i+1);
	  printf("interpr: %s\ndynarec: %s\n",interpreter.dumpinfo[i],dynarec.dumpinfo[i]);
	  exit(1);
	}
       printf("correct up to pc 0x%X\n",interpreter.PC==dynarec.PC ? interpreter.PC : 0);
       memset(interpreter.dumpinfo,0,20000);
       memset(dynarec.dumpinfo,0,20000);
       interpreter.readdump=dynarec.readdump=0;
       write(dyna_in[1],"next\n",5);
     }
   }
}

main()
{ int inter_in[2],inter_out[2],dyna_in[2],dyna_out[2];
  int interproc,dynaproc;

  if(pipe(inter_in))
   { perror("interpreter input pipe"); exit(1); }
  if(pipe(inter_out))
   { perror("interpreter output pipe"); exit(1); }

  if(pipe(dyna_in))
   { perror("dynarec input pipe"); exit(1); }
  if(pipe(dyna_out))
   { perror("dynarec output pipe"); exit(1); }

//  setvbuf(stdout,NULL,_IONBF,0);

  switch(interproc=fork())
   {
	case 0: interp(inter_in,inter_out); break;
	default: switch(dynaproc=fork())
		  {
		    case 0: dynap(dyna_in,dyna_out); break;
		    default: compare(inter_in,inter_out,dyna_in,dyna_out);
			     break;
		  }
   }
}
