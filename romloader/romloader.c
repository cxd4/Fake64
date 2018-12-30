#define ROMLOADER

#include <stdio.h>
#include <stdlib.h>
#include <general.h>
#include <romheader.h>
#include <module.h>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

/* especially for me :) */

#ifdef NOT_INSTALLED
	#undef CPU_DIR
	#define CPU_DIR "./plugins/cpu"
	#undef INPUT_DIR
	#define INPUT_DIR "./plugins/input"
	#undef VIDEO_DIR
	#define VIDEO_DIR "./plugins/video"
	#undef AUDIO_DIR
	#define AUDIO_DIR "./plugins/audio"
#endif

#ifdef GPROF
	extern void vi_init();
	extern void ai_init();
	extern void main_cpu_loop(struct rom*);
	extern void vi_deinit();
	extern void ai_deinit();
#endif

struct module_info  modules;		// holds pointers to all the modules loaded
/*
cpu_module *cpu;
input_module *input;
video_module *video;
audio_module *audio;
*/

void read_config(int, struct rom*, char*);
char* get_module_id(char*);
void byteswap(int Size, uint8* Image);


void *dmalloc(int size)
{
	static void *t;
	t=(void *)calloc(1,size);
	#ifdef DEBUG
	if (t)
		printf("Allocated memblock: 0x%x\n",t);
	else
		printf("Failed to allocate block of size %d\n",size);
	#endif
	return(t);
}

void dfree(void *t)
{
#ifdef DEBUG
	printf("Freeing memblock: 0x%x\n",t);
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
  if(!(rom=(uint8 *) calloc(1,length>=0x800000 ? length : 0x800000)))
   { perror("load_n64_rom");
     printf("Cannot allocate memory.....\n");
     exit(-1);
   }
  if(length % 8)
   printf("Warning: Length of rom isn't a multiple of 8\n");
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

  printf("Cartridge ID: 0x%x\nCountry Code: 0x%x ",rom->cardridge_id,rom->country_code);
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

int is_module(const struct dirent *name)
{ return (!strcmp(name->d_name+strlen(name->d_name)-3,".so")); }

char module_name[200];

#ifndef GPROF

char *pick_module(char *directory)
{
  struct dirent **namelist;
  char buf[200];
  int n,i,t=0;

  n=scandir(directory, &namelist, &is_module, alphasort);
  if(n<0)
   { perror("scandir:"); exit(-1); }
  else
   { for(i=0;i<n;i++)
      {
       sprintf(buf,"%s/%s",directory,namelist[i]->d_name);
       printf("%d) %s / %s\n",i+1,namelist[i]->d_name,get_module_id(buf));
      }
     while(!t | !namelist[t-1])
      { 
        printf("\nPick module: ");
        fflush(stdout);
        fgets(buf,198,stdin);
        t=atoi(buf);
      }
     t--;
     sprintf(module_name,"%s/%s",directory,namelist[t]->d_name);
     while(n--)
	free(namelist[n]);
     free(namelist);
     return module_name;
   }
}

#endif

/*void call_init_pifram(uint8 *here) {

	void (*init_pifram)();

	init_pifram = (void*)dlsym(input->module, "init_pifram");

	if (!init_pifram) {
		dlerror();
	} else {
		init_pifram(here);
	}
}*/

main(int argc,char **argv)
{ struct rom *romstruct;
  char buf[200];

  modules.printd_f = _printd;
  modules.test_debug_f = _test_debug;
  init_debugging();

#ifndef GPROF

  romstruct=load_n64_rom(argv[1]);
  dumpheader(romstruct);
  fflush(stdout);
  read_config(0, romstruct, argv[1]);

  				       // add ~/.fake64rc support,eg autoselect 
				       // a module
  if (!modules.input) {
   while(!(modules.input = (input_module *)load_input_module(pick_module(INPUT_DIR))))
    {
      printf("Couldn't load input module\n");
    }
  }

  printf("Input module loaded: %s\n", (*modules.input->module_id_f)());

  if (!modules.audio) {
   while(!(modules.audio = (audio_module *)load_audio_module(pick_module(AUDIO_DIR))))
    {
      printf("Couldn't load audio module\n");
    }
  }

  printf("Audio module loaded: %s\n", (*modules.audio->module_id_f)());

  if (!modules.video) {
   while(!(modules.video = (video_module *)load_video_module(pick_module(VIDEO_DIR))))
    {
      printf("Couldn't load video module\n");
    }
  }

  printf("Video module loaded: %s\n", (*modules.video->module_id_f)());

  if (!modules.cpu) {
   while(!(modules.cpu = (cpu_module *)load_cpu_core_module(pick_module(CPU_DIR))))
    {
      printf("Couldn't load cpucore module\n");
    }
  }

  printf("Cpu core loaded: %s\n",(*modules.cpu->module_id_f)());

  read_config(1, romstruct, argv[1]);

#endif

#ifndef GPROF

  (*modules.video->vi_init_f)(&modules);

  (*modules.audio->ai_init_f)();
  
  (*modules.cpu->main_cpu_loop_f)(romstruct,&modules);

  (*modules.audio->ai_deinit_f)();

  (*modules.video->vi_deinit_f)();

#endif

#ifdef GPROF

  vi_init();
  ai_init();
  main_cpu_loop(romstruct);
  ai_deinit();
  vi_deinit();

#endif

  dfree(romstruct->image);
  dfree(romstruct);
}
