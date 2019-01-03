#ifdef WIN32
	#include <windows.h>
#endif

#include <general.h>
#include "memory.h"
#include "video.h"
#include "config_options.h"

#include <GL/gl.h>
#include <GL/glu.h>

/* SDL_GL_SwapBuffers() */
#include <SDL/SDL.h>

uint8 spack[640*480][4];
extern struct screen_attributes sa;


void render2d_16(uint16* addr)
{
	uint16 pixel;
	int i;

	for (i = 0; i < sa.XRes * sa.YRes; i++) {
		pixel = ((uint16 *)addr)[i ^ HALFWORD_ADDRESS_SWAP];
		spack[i][0] = ((pixel & 0xF800) >> 11) * 8;
		spack[i][1] = ((pixel & 0x07C0) >>  6) * 8;
		spack[i][2] = ((pixel & 0x003E) >>  1) * 8;
		spack[i][3] = ((pixel & 0x0001) >>  0) * 255;
	}

	reset_stacks();

	glDisable(GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ZERO);

	glRasterPos2i(0,sa.Height-1);
//	glPixelZoom(1.0f,1.0f);
//	glPixelZoom((float)sa.Height/(float)sa.YRes,-(float)sa.Width/(float)sa.XRes);//-sa.Height/sa.YRes);
	glPixelZoom((float)sa.Width/(float)sa.XRes,-(float)sa.Height/(float)sa.YRes);
	glDrawPixels(
		sa.XRes, sa.YRes,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		spack
	);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void render2d_32(uint16* addr)
{
	GLbyte rgba[4];
	register int i;

	for (i = 0; i < sa.XRes * sa.YRes; i++) {
		spack[i][0] = ((uint8 *)addr)[4*i + BES(0)];
		spack[i][1] = ((uint8 *)addr)[4*i + BES(1)];
		spack[i][2] = ((uint8 *)addr)[4*i + BES(2)];
		spack[i][3] = ((uint8 *)addr)[4*i + BES(3)];
	}
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ZERO);

	glRasterPos2i(0,sa.Height-1);
	glPixelZoom((float)sa.Height/(float)sa.YRes,-(float)sa.Width/(float)sa.XRes);//-sa.Height/sa.YRes);
	glDrawPixels(
		sa.XRes, sa.YRes,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		spack
	);

	glEnable(GL_DEPTH_TEST);
}

void render2d(uint16* addr)
{
	if (sa.Active) {
		if (fb2d_active) { // config option video 2d
			if (sa.rbpp == 16) {
				render2d_16(addr);
			} else {
				render2d_32(addr);
			}
		}
	}
	SDL_GL_SwapBuffers();
}
