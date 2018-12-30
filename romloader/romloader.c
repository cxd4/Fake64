#include <stdio.h>
#include <general.h>
#include <romheader.h>
#include <load_module.h>

void *dmalloc(int size)
{ void *t;
  t=(void *)calloc(1,size);
  #ifdef DEBUG
   fprintf(stderr,"Allocated memblock: 0x%x\n",t);
  #endif
  return(t);
}

void dfree(void *t)
{
  #ifdef DEBUG
   fprintf(stderr,"Freeing memblock: 0x%x\n",t);
  #endif
  free(t);
}

void fix_rom_name(char *name)
{ int k;
  char swap[5];
       for(k=20;k>=0;k-=4) {
         //Converts 0x12408037 to 0x80371240
         // 0x37 0x80 0x40 0x12 to 0x40 0x12 0x37 0x80
         // <opcode> <src> <dest>
           swap[0]=name[k];
           swap[1]=name[k+1];
           swap[2]=name[k+2];
           swap[3]=name[k+3];

           name[k]=swap[3]; // godddd this needs checking
           name[k+1]=swap[2];
           name[k+2]=swap[1];
           name[k+3]=swap[0];
        }
}

struct rom * load_n64_rom(char *filename)
{ char *rom;
  struct rom *romstruct;
  FILE *fp;
  int bytes;
  int length;
  if(!(fp=fopen(filename,"rb")))
   { perror("load_n64_rom");
     printf("Can't open the rom.....\n");
     exit(-1);
   }
  fseek(fp,0,SEEK_END);
  length=ftell(fp);
  if(!(rom=(uint8 *) dmalloc(length)))
   { perror("load_n64_rom");
     printf("Cannot allocate memory.....\n");
     exit(-1);
   }
  if(length % 8)
   fprintf(stderr,"Warning: Length of rom isn't a multiple of 8\n");
  rewind(fp);
  bytes=fread(rom,1,length,fp);
  printf("Rom loaded.. %d/%d bytes read\n",bytes,length);
   
    // TODO: convert endian... see endian.c
  romstruct=(struct rom *)dmalloc(sizeof(struct rom));
  romstruct->length=bytes;
  romstruct->image=rom;
  romstruct->header=rom;
  romstruct->bootcode=rom+0x40;
  romstruct->progcode=rom+0x1000;
  byteswap(romstruct->length,romstruct->image);
  memcpy(romstruct,rom,sizeof(struct rom)-20);  
  fix_rom_name(romstruct->name);
  return(romstruct);
}

void dumpheader(struct rom *rom)
{
  printf("PI_BSB_DOM1_LAT_REG: 0x%x (should be 0x80)\nPI_BSB_DOM1_PGS_REG: 0x%x (should be 0x37)\nPI_BSB_DOM1_PWD_REG: 0x%x (should be 0x12)\nPI_BSB_DOM1_PGS_REG2: 0x%x (should be 0x40)\n",rom->PI_BSB_DOM1_LAT_REG,rom->PI_BSB_DOM1_PGS_REG,rom->PI_BSB_DOM1_PWD_REG,rom->PI_BSB_DOM1_PGS_REG2);
  printf("Clockrate: %d\nPC: 0x%x\nRelease: %d\nCRC1: 0x%x\nCRC2: 0x%x\n",rom->clockrate,rom->PC,rom->release,rom->CRC1,rom->CRC2);

  printf("Name: %s\nManufacturer: 0x%x ",rom->name,rom->manufacturer);
  if (rom->manufacturer==0x4e)
    printf("(Nintendo)");
  printf("\n");

  printf("Cardridge ID: 0x%x\nCountry Code: 0x%x ",rom->cardridge_id,rom->country_code);
  switch(rom->country_code)
   {
     case 0x4400: printf("(Germany)"); break;
     case 0x4500: printf("(USA)"); break;
     case 0x4A00: printf("(Japan)"); break;
     case 0x5000: printf("(Europe)"); break;
     case 0x5500: printf("(Australia)"); break;     
   }
  printf("\n");
  printf("Size: %d megabits\n",((rom->length*8)/1024)/1024);
}

main(int argc,char **argv)
{ struct rom *romstruct;
  cpu_module *cpu;
  romstruct=load_n64_rom(argv[1]); 
  dumpheader(romstruct);
  if(!(cpu = (cpu_module *)load_cpu_core_module\
	("./cpucore_pure_c/.libs/libcpu.so"))) {
	fprintf(stderr, "maybe you're in cpucore_pure_c/, lets try there\n");
	cpu = (cpu_module *)load_cpu_core_module\
		("./.libs/libcpu.so");
  }
  if(cpu)
   { printf("Cpu core loaded: %s\n",(*cpu->module_id)());
    (*cpu->main_cpu_loop)(romstruct);
   }
  dfree(romstruct->image);
  dfree(romstruct);
}
