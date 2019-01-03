#ifndef _RENDER_H_
#define _RENDER_H_

extern void reset_stacks(void);
extern void dodrawrect(int, int, int, int);
extern void dodrawtri(int, int, int);
extern void dodrawsprite(void);

extern void render2d(uint16* addr);
extern void render2d_16(uint16* addr);
extern void render2d_32(uint16* addr);

#endif
