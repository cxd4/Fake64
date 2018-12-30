#include <general.h>
#include <memory.h>

int patchind = 0;

uint32 patchaddr[100];
uint32 patchval[100];

void set_patch(uint32 addr, uint32 val) {

	printf("Patching %#.8x = %#.8x\n", addr, val);

	patchaddr[patchind] = addr;
	patchval[patchind] = val;
	patchind++;

}

void do_patch(uint32 addr, uint32 val) {

	uint32* ptr;

	ptr = translate_addr(addr & 0x1fffffff);
	printf("%x:%x\n", addr, ptr);
	*ptr = val;

}

void apply_patches() {

	int i;

	printf("Patchind: %i\n", patchind);

	if (patchind) {
		for (i = 0; i < (patchind); i++) {
			do_patch(patchaddr[i], patchval[i]);
			printf("Patch %i applied\n", i);
		}
	}
}
		
