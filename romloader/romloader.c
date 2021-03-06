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

#ifdef STATIC_MODULES
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

struct rom *
load_n64_rom(char* filename)
{
	uint8* rom;
	struct rom *romstruct;
	FILE *fp;
	int bytes;
	int length;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		perror("load_n64_rom");
		puts("Can't open the rom...");
		exit(-1);
	}

	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	if (length > 64 * 1024 * 1024) {
		printf("Warning:  Truncating ROM size %i to 64 MiB.\n", length);
		length = 64 * 1024 * 1024;
	}

#if 0
	if (length < 0x800000)
		length = 0x800000;
	rom = (uint8 *)calloc(1, length);
#else
	rom = (length >= 0x800000) ?
		calloc(1, length) : calloc(1, 0x800000)
	;
#endif
	if (rom == NULL) {
		perror("load_n64_rom");
		puts("Cannot allocate memory...");
		exit(-1);
	}

	if (length % 8 != 0)
		puts("Warning:   ROM size isn't a multiple of 8.");
	rewind(fp);
	bytes = fread(rom, 1, length, fp);
	printf("ROM loaded:  %d/%d bytes read.\n", bytes, length);

	romstruct = (struct rom *)dmalloc(sizeof(struct rom));

	romstruct->length   = bytes;
	romstruct->image    = rom;
	romstruct->header   = rom;
	romstruct->bootcode = rom + 0x40;
	romstruct->progcode = rom + 0x1000;

	printf("image[0] before = 0x%02X\n", romstruct->image[0]);
	byteswap(romstruct->length, romstruct->image);
	printf("image[0] after  = 0x%02X\n", romstruct->image[0]);

	romstruct->PI_BSB_DOM1_LAT_REG  = romstruct->image[ 0];
	romstruct->PI_BSB_DOM1_PGS_REG  = romstruct->image[ 1];
	romstruct->PI_BSB_DOM1_PWD_REG  = romstruct->image[ 2];
	romstruct->PI_BSB_DOM1_PGS_REG2 = romstruct->image[ 3];

	romstruct->clockrate =
		((uint32)(romstruct->image[ 4]) << 24) |
		((uint32)(romstruct->image[ 5]) << 16) |
		((uint32)(romstruct->image[ 6]) <<  8) |
		((uint32)(romstruct->image[ 7]) <<  0);
	romstruct->PC =
		((uint32)(romstruct->image[ 8]) << 24) |
		((uint32)(romstruct->image[ 9]) << 16) |
		((uint32)(romstruct->image[10]) <<  8) |
		((uint32)(romstruct->image[11]) <<  0);
	romstruct->release =
		((uint32)(romstruct->image[12]) << 24) |
		((uint32)(romstruct->image[13]) << 16) |
		((uint32)(romstruct->image[14]) <<  8) |
		((uint32)(romstruct->image[15]) <<  0);

	romstruct->CRC1 =
		((uint32)(romstruct->image[16]) << 24) |
		((uint32)(romstruct->image[17]) << 16) |
		((uint32)(romstruct->image[18]) <<  8) |
		((uint32)(romstruct->image[19]) <<  0);
	romstruct->CRC2 =
		((uint32)(romstruct->image[20]) << 24) |
		((uint32)(romstruct->image[21]) << 16) |
		((uint32)(romstruct->image[22]) <<  8) |
		((uint32)(romstruct->image[23]) <<  0);
	romstruct->unknown1 =
		((uint32)(romstruct->image[24]) << 24) |
		((uint32)(romstruct->image[25]) << 16) |
		((uint32)(romstruct->image[26]) <<  8) |
		((uint32)(romstruct->image[27]) <<  0);
	romstruct->unknown2 =
		((uint32)(romstruct->image[28]) << 24) |
		((uint32)(romstruct->image[29]) << 16) |
		((uint32)(romstruct->image[30]) <<  8) |
		((uint32)(romstruct->image[31]) <<  0);

	memcpy(
		&(romstruct->name[0]),
		&(romstruct->image[32]),
		20
	);
	romstruct->name[20] = '\0';
	romstruct->unknown3 = romstruct->image[52] + romstruct->image[58];
	romstruct->manufacturer = romstruct->image[59];

	romstruct->cartridge_id =
		((uint16)(romstruct->image[60]) << 8) |
		((uint16)(romstruct->image[61]) << 0);
	romstruct->country_code = romstruct->image[62];
	romstruct->country_code >>= 8; /* Wrong.  image[63] is its own field. */
	return (romstruct);
}

void dumpheader(struct rom *rom)
{
	printf(
		"PI_BSB_DOM1_LAT_REG:  0x%x (should be 0x80)\n"\
		"PI_BSB_DOM1_PGS_REG:  0x%x (should be 0x37)\n"\
		"PI_BSB_DOM1_PWD_REG:  0x%x (should be 0x12)\n"\
		"PI_BSB_DOM1_PGS_REG2:  0x%x (should be 0x40)\n",
	
		rom->PI_BSB_DOM1_LAT_REG,
		rom->PI_BSB_DOM1_PGS_REG,
		rom->PI_BSB_DOM1_PWD_REG,
		rom->PI_BSB_DOM1_PGS_REG2
	);
	printf(
		"Clockrate:  %d\n"\
		"PC:  0x%x\n"\
		"Release:  %d\n"\
		"CRC1:  0x%x\n"\
		"CRC2:  0x%x\n",

		rom->clockrate,
		rom->PC,
		rom->release,
		rom->CRC1,
		rom->CRC2
	);

	printf("Manufacturer: 0x%x ", rom->manufacturer);
	if (rom->manufacturer==0x4e)
		puts(" (Nintendo)");
	else
		putchar('\n');

	printf(
		"Cartridge ID:  0x%04X\n"\
		"Country Code:  '%c'\n",
		rom->cartridge_id,
		rom->country_code
	);
	printf("Size: %d megabits\n",((rom->length*8)/1024)/1024);
}

int is_module(const struct dirent *name)
{ return (!strcmp(name->d_name+strlen(name->d_name)-3,".so")); }

char module_name[200];

#ifndef STATIC_MODULES
char *pick_module(char *directory)
{
    struct dirent **namelist;
    char buf[200];
    int n, i, t = 0;

    n = scandir(directory, &namelist, &is_module, alphasort);
    if (n < 0) {
        perror("scandir:");
        exit(-1);
        return NULL;
    }

    for (i = 0; i < n; i++) {
        sprintf(buf, "%s/%s", directory, namelist[i]->d_name);
        printf(
            "%d) %s / %s\n",
            i + 1, namelist[i]->d_name, get_module_id(buf)
        );
    }

    while (!t | !namelist[t - 1]) {
        printf("\nPick module:  ");
        fflush(stdout);
        fgets(buf, 198, stdin);
        t = atoi(buf);
    }
    t--;
    sprintf(module_name, "%s/%s", directory, namelist[t]->d_name);
    while (n--)
        free(namelist[n]);
    free(namelist);
    return (module_name);
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

int
main(int argc, char* argv[])
{
    struct rom *romstruct;
    char buf[200];

  modules.printd_f = _printd;
  modules.test_debug_f = _test_debug;
    init_debugging();

    romstruct = load_n64_rom(argv[1]);
    dumpheader(romstruct);
    fflush(stdout);
    read_config(0, romstruct, argv[1]);

#ifndef STATIC_MODULES
    while (modules.input == NULL) {
        modules.input = (input_module *)load_input_module(pick_module(INPUT_DIR));
        printf("Address of input module:  %p\n", modules.input);
    }
    printf("Input module loaded:  %s\n", (*modules.input->module_id_f)());

    while (modules.audio == NULL) {
        modules.audio = (audio_module *)load_audio_module(pick_module(AUDIO_DIR));
        printf("Address of audio module:  %p\n", modules.audio);
    }
    printf("Audio module loaded:  %s\n", (*modules.audio->module_id_f)());

    while (modules.video == NULL) {
        modules.video = (video_module *)load_video_module(pick_module(VIDEO_DIR));
        printf("Address of video module:  %p\n", modules.video);
    }
    printf("Video module loaded: %s\n", (*modules.video->module_id_f)());

    while (modules.cpu == NULL) {
        modules.cpu = (cpu_module *)load_cpu_core_module(pick_module(CPU_DIR));
        printf("Address of CPU core:  %p\n", modules.cpu);
    }
    printf("CPU core loaded: %s\n", (*modules.cpu->module_id_f)());

    read_config(1, romstruct, argv[1]);
#endif

#ifndef STATIC_MODULES
    (*modules.video -> vi_init_f)(&modules);
    (*modules.audio -> ai_init_f)();
    (*modules.cpu   -> main_cpu_loop_f)(romstruct,&modules);
    (*modules.audio -> ai_deinit_f)();
    (*modules.video -> vi_deinit_f)();
#else
    vi_init();
    ai_init();
    main_cpu_loop(romstruct);
    ai_deinit();
    vi_deinit();
#endif

    dfree(romstruct->image);
    dfree(romstruct);
}
