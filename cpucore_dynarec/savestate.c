#include <signal.h>
#include <general.h>
#include <romheader.h>
#include <decode.h>
#include "registers.h"
#include "opcodes.h"
#include <memory.h>

extern struct cpu_reg reg;

int write_savestate(char *filename)
{ FILE *fp;
  if(!(fp=fopen(filename,"w")))
    return -1;
  fwrite("F64",3,1,fp);
  fwrite(&reg,sizeof(struct cpu_reg),1,fp);
  fwrite(RDREGS,1024*1024,1,fp);
  fwrite(SPDIMEM,0x2000,1,fp);
  fwrite(SPREGS,0x20,1,fp);
  fwrite(SP_REG,0x20,1,fp);
  fwrite(DPREGS,0x20,1,fp);
  fwrite(DPSREGS,0x10,1,fp);
  fwrite(MIREGS,0x10,1,fp);
  fwrite(VIREGS,0x38,1,fp);
  fwrite(AIREGS,0x18,1,fp);
  fwrite(PIREGS,0x34,1,fp);
  fwrite(RIREGS,0x20,1,fp);
  fwrite(SIREGS,0x1c,1,fp);
  fwrite(PIFMEM,0x800,1,fp);
  fwrite(RDRAM,RAM_SIZE,1,fp);
  fclose(fp);
  return 0;
}

int read_savestate(char *filename)
{ FILE *fp;
  char t[4];
  if(!(fp=fopen(filename,"r")))
    return -1;
  fread(t,3,1,fp);
  if(strncmp(t,"F64",3)) { printf("Not a fake64 savestate file\n"); return -1;}
  fread(&reg,sizeof(struct cpu_reg),1,fp);
  fread(RDREGS,1024*1024,1,fp);
  fread(SPDIMEM,0x2000,1,fp);
  fread(SPREGS,0x20,1,fp);
  fread(SP_REG,0x20,1,fp);
  fread(DPREGS,0x20,1,fp);
  fread(DPSREGS,0x10,1,fp);
  fread(MIREGS,0x10,1,fp);
  fread(VIREGS,0x38,1,fp);
  fread(AIREGS,0x18,1,fp);
  fread(PIREGS,0x34,1,fp);
  fread(RIREGS,0x20,1,fp);
  fread(SIREGS,0x1c,1,fp);
  fread(PIFMEM,0x800,1,fp);
  fread(RDRAM,RAM_SIZE,1,fp);
  fclose(fp);
  return 0;
}

