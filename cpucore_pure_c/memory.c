#include <general.h>
#include <romheader.h>
#include <memory.h>

uint8 *RDREGS	= NULL;   // RDRAM registers
uint8 *SPDIMEM	= NULL;
uint8 *SPREGS	= NULL;   // SP registers
uint8 *SP_REG	= NULL;
uint8 *DPREGS	= NULL;   // DP registers
uint8 *DPSREGS	= NULL;   // DP span registers
uint8 *MIREGS	= NULL;   // MIPS Interface registers
uint8 *VIREGS	= NULL;   // Video Interface registers
uint8 *AIREGS	= NULL;   // Audio Interface registers
uint8 *PIREGS	= NULL;   // Peripheral Interface registers
uint8 *RIREGS	= NULL;   // RDRAM Interface registers
uint8 *SIREGS	= NULL;   // Serial Interface registers
uint8 *PIFMEM	= NULL;   // PIF area
uint8 *RDRAM	= NULL;   // rdram
uint8 *C2A1	= NULL;	  // Cartridge Domain 2 Address 1 32kb sram
uint8 *C2A2     = NULL;   // Cartridge Domain 2 Address 2 128kb flashram
uint8 *C1A1     = NULL;   // Cartridge Domain 1 Address 1 32kb sram
uint8 *C1A3     = NULL;   // Cartridge Domain 1 Address 3 32kb sram
uint8 *ROM	= NULL;   // rom(header&everythin)

int RAM_OFFSET_MAP[0x2000];

int alloc_memory(struct rom *rom)
{
	if (rom == NULL) {
		fputs("alloc_memory(NULL)\n", stderr);
		exit(EXIT_FAILURE);
		return -1;
	}
	ROM = rom->header;

	if (RDREGS == NULL) {
		RDREGS = (uint8 *)calloc(1, 1024*1024); /* 1 MB 0x03f00000-0x03fffff */
		if (RDREGS == NULL)
			return -1;
	}

	if (SPDIMEM == NULL) {
		SPDIMEM = (uint8 *)calloc(1, 0x2000);
		if (SPDIMEM == NULL)
			return -1;
	}

	if (SPREGS == NULL) {
		SPREGS = (uint8 *)calloc(1, 0x20);
		if (SPREGS == NULL)
			return -1;
	}

	if (SP_REG == NULL) {
		SP_REG = (uint8 *)calloc(1, 0x20);
		if (SP_REG == NULL)
			return -1;
	}

	if (DPREGS == NULL) {
		DPREGS = (uint8 *)calloc(1, 0x20);
		if (DPREGS == NULL)
			return -1;
	}

	if (DPSREGS == NULL) {
		DPSREGS = (uint8 *)calloc(1, 0x10);
		if (DPSREGS == NULL)
			return -1;
	}

	if (MIREGS == NULL) {
		MIREGS = (uint8 *)calloc(1, 0x10);
		if (MIREGS == NULL)
			return -1;
	}

	if (VIREGS == NULL) {
		VIREGS = (uint8 *)calloc(1, 0x38);
		if (VIREGS == NULL)
			return -1;
	}

	if (AIREGS == NULL) {
		AIREGS = (uint8 *)calloc(1, 0x18);
		if (AIREGS == NULL)
			return -1;
	}

	if (PIREGS == NULL) {
		PIREGS = (uint8 *)calloc(1, 0x34);
		if (PIREGS == NULL)
			return -1;
	}

	if (RIREGS == NULL) {
		RIREGS = (uint8 *)calloc(1, 0x20);
		if (RIREGS == NULL)
			return -1;
	}

	if (SIREGS == NULL) {
		SIREGS = (uint8 *)calloc(1, 0x1C);
		if (SIREGS == NULL)
			return -1;
	}

	if (PIFMEM == NULL) {
		PIFMEM = (uint8 *)calloc(1, 0x800);
		if (PIFMEM == NULL)
			return -1;
	}

	if (RDRAM == NULL) {
		RDRAM = (uint8 *)calloc(1, RAM_SIZE);
		if (RDRAM == NULL)
			return -1;
	}

	if (C2A1 == NULL) {
		C2A1 = (uint8 *)calloc(1, 0x8000);
		if (C2A1 == NULL)
			return -1;
	}

	if (C2A2 == NULL) {
		C2A2 = (uint8 *)calloc(1, 0x20000);
		if (C2A2 == NULL)
			return -1;
	}

	if (C1A1 == NULL) {
		C1A1 = (uint8 *)calloc(1, 0x8000);
		if (C1A1 == NULL)
			return -1;
	}

	if (C1A3 == NULL) {
		C1A3 = (uint8 *)calloc(1, 0x8000);
		if (C1A3 == NULL)
			return -1;
	}

	init_memory(rom->length);
	return 0;
}

void init_memory(int romlength)
{ int i;
  for(i=0;i<0x2000;i++)
   RAM_OFFSET_MAP[i]=0;

  for(i=0;i<=(RAM_SIZE>>16);i++)
   RAM_OFFSET_MAP[i]=(int)RDRAM; 
  
  for(i=0x1000;i<=(romlength+0x10000000)>>16;i++)   
   RAM_OFFSET_MAP[i]=(int)ROM-0x10000000;

  for(i=0x03f0;i<=0x03ff;i++)
   RAM_OFFSET_MAP[i]=(int)RDREGS-0x03f00000;

  RAM_OFFSET_MAP[0x0400]=(int)SPDIMEM-0x04000000;
  RAM_OFFSET_MAP[0x0404]=(int)SPREGS-0x04040000;
  RAM_OFFSET_MAP[0x0408]=(int)SP_REG-0x04080000;
  RAM_OFFSET_MAP[0x0410]=(int)DPREGS-0x04100000;
  RAM_OFFSET_MAP[0x0420]=(int)DPSREGS-0x04200000;
  RAM_OFFSET_MAP[0x0430]=(int)MIREGS-0x04300000;
  RAM_OFFSET_MAP[0x0440]=(int)VIREGS-0x04400000;
  RAM_OFFSET_MAP[0x0450]=(int)AIREGS-0x04500000;
  RAM_OFFSET_MAP[0x0460]=(int)PIREGS-0x04600000;
  RAM_OFFSET_MAP[0x0470]=(int)RIREGS-0x04700000;
  RAM_OFFSET_MAP[0x0480]=(int)SIREGS-0x04800000;
  RAM_OFFSET_MAP[0x0500]=(int)C2A1-0x05000000;
  RAM_OFFSET_MAP[0x0600]=(int)C1A1-0x06000000;
  RAM_OFFSET_MAP[0x0800]=(int)C2A2-0x08000000;
  RAM_OFFSET_MAP[0x0801]=(int)C2A2-0x08010000;
  RAM_OFFSET_MAP[0x1fc0]=(int)PIFMEM-0x1fc00000;
	printf(
		"%p:  %X\n",
		PIFMEM,
		RAM_OFFSET_MAP[0x1FC0] + 0x1FC00000
	);
  RAM_OFFSET_MAP[0x1fd0]=(int)C1A3-0x1fd00000;

}

void* translate_addr(uint32 addr) {

	return (void*)(addr + RAM_OFFSET_MAP[addr>>16]);

}
