#ifdef WIN32
	#include <windows.h>
#endif

#include <general.h>
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
	int i;

	for (i = 0; i < (sa.XRes * sa.YRes); i++)
	{
		spack[i][0] = (((((uint16*)addr)[i^0x1] & 0xf800) >> 11) * 8);
		spack[i][1] = (((((uint16*)addr)[i^0x1] & 0x07c0) >> 6) * 8);
		spack[i][2] = (((((uint16*)addr)[i^0x1] & 0x003e) >> 1) * 8);
		spack[i][3] = ((((uint16*)addr)[i^0x1] & 0x0001) * 255);	// hmm doesn;t work otherwise though :/	(change blending modes when i get round to it)
	//	printf("%d\n",spack[i][3]);
	}

	reset_stacks();

	glDisable(GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ZERO);

	glRasterPos2i(0,sa.Height-1);
//	glPixelZoom(1.0f,1.0f);
//	glPixelZoom((float)sa.Height/(float)sa.YRes,-(float)sa.Width/(float)sa.XRes);//-sa.Height/sa.YRes);
	glPixelZoom((float)sa.Width/(float)sa.XRes,-(float)sa.Height/(float)sa.YRes);
	glDrawPixels(sa.XRes,sa.YRes,GL_RGBA, GL_UNSIGNED_BYTE, spack);


	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	

}

void render2d_32(uint16* addr)
{
	printf("render 32 untested\n");
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_ZERO);

	glRasterPos2i(0,sa.Height-1);
	glPixelZoom((float)sa.Height/(float)sa.YRes,-(float)sa.Width/(float)sa.XRes);//-sa.Height/sa.YRes);
	glDrawPixels(sa.XRes,sa.YRes,GL_RGBA, GL_UNSIGNED_BYTE, addr);


	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
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
