#include <config.h>
#include <general.h>

uint8 *PIFMEM;

char *module_id(void) {
	return("Bluefyre's basic hackerific input core");
}

void init_pifram(uint8 *here) {

#ifdef DEBUG_PIF
	printf("PIFMEM inited: %x\n", here);
#endif
	PIFMEM = here;

}

void pifram_interrupt() {

#ifdef DEBUG_PIF
	int i;
	int j;

	printf("PIFMEM debug:\n");

	for (i = 0; i < 8; i++) {
		printf("Channel %i:\t", i+1);
		for (j = 0; j < 8; j++) {
			if (j == 4) { printf("| "); }
			printf("%.2x ", PIFMEM[(i*8)+j]);
		}
		printf("\n");
	}
#endif

}
