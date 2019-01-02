#include "general.h"
#include "memory.h"

void
byteswap(int Size, uint8* Image)
{
	uint8 swap[4];
	register int k;
	const uint32 PI_init =
		((uint32)Image[0] << 24) |
		((uint32)Image[1] << 16) |
		((uint32)Image[2] <<  8) |
		((uint32)Image[3] <<  0);

	if (PI_init == 0x80371240) {
#ifdef SERVER_ENDIAN
		return; /* nothing to do...byte order correct as-is */
#else
		for (k = Size; k >= 0; k -= 4) {
			memcpy(&swap[0], &Image[k], sizeof(swap));
			Image[k + 0] = swap[BES(0)];
			Image[k + 1] = swap[BES(1)];
			Image[k + 2] = swap[BES(2)];
			Image[k + 3] = swap[BES(3)];
		}
#endif
	} else if (PI_init == 0x37804012) { /* only supported for legacy */
		puts("Warning:  Dumb V64 endian found.  Fix your ROM....");
		for (k = Size; k >= 0; k -= 4) {
			memcpy(&swap[0], &Image[k], sizeof(swap));
			Image[k + 0] = swap[BES(1)];
			Image[k + 1] = swap[BES(0)];
			Image[k + 2] = swap[BES(3)];
			Image[k + 3] = swap[BES(2)];
		}
	} else {
		puts("Corrupt or invalid ROM image.");
		return;
	}
}
