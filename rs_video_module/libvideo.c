#ifdef WIN32
	#include <windows.h>
#else
	#include <config.h>
#endif
#include <general.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include <module.h>

#include <GL/gl.h>                                                              // Header File For The OpenGL32 Library
#include <GL/glu.h>
			
#include "config_options.h"
#include "video.h"
#include "fps.h"

#define DEBUG_VI

#define XRES 1280
#define	YRES 1024
#define BPP 16

struct module_info* modules;
struct screen_attributes sa;


SDL_Surface *Screen;

#ifndef STATIC_MODULES
char *module_id(void)
{
	return ("rs' killer video module");
}
#endif


void vi_status_reg_write(uint32 data)
{
	sa.Status=data;

	printd(D_VI, D_DEBUG, "VI status write: %x\n", data);

	if ((data&0x3)==2 && (sa.rbpp!=16)) // use 15 when its really 16
	{
		sa.rbpp=16;
		sa.Active=1;
	}

	if ((data&0x3)==3 && (sa.rbpp!=32))
	{
		sa.rbpp=32;
		sa.Active=1;
	}
}

void vi_origin_reg_write(uint32 data)
{
	printd(D_VI, D_DEBUG, "VI frame buffer origin: %#x\n", data);
}

void vi_width_reg_write(uint32 data)
{
	printd(D_VI, D_DEBUG, "VI line width: %d pixels\n", data);

	if (((data&0x7FF)==320) && (sa.XRes!=320)) // use 15 when its really 16
	{
		sa.XRes=320;
		sa.YRes=240;
		sa.Active=1;
	}

	if (((data&0x7FF)==640) && (sa.XRes!=640))
	{
		sa.XRes=640;
		sa.YRes=480;
		sa.Active=1;
	}
}

void vi_intr_reg_write(uint32 data)
{
	printd(D_VI, D_DEBUG, "VI interrupt requested at: %d halflines\n", data);
}


void init_material() {

	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess[] = { 50.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

int vi_init(struct module_info* modptr)
{
  float ratio;
	SDL_VideoInfo *Info;

	memset(&sa,0,sizeof(sa));

	modules = modptr;

	init_fdiv_tables();

	if(SDL_Init(SDL_INIT_VIDEO)<0)
	{
		printd(D_VI, D_CRIT, "VI: Initialisation failed\n");
		exit(-1);
	}


	Info=SDL_GetVideoInfo();

	if(!Info)
	{
		printd(D_VI, D_ERROR, "VI: Failed to get current mode info\n");
		exit(-1);
	}

	sa.Width=win_width;
	sa.Height=win_height;
	sa.ubpp=BPP;//			Info->vfmt->BitsPerPixel;
  ratio = (float) sa.Width / (float) sa.Height;


	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	if(!(SDL_SetVideoMode(sa.Width,sa.Height, sa.ubpp,SDL_OPENGL)))
	{
		printd(D_VI, D_FATAL, "VI: SetVideoMode failed because %s (%d x %d,%d bpp)\n",SDL_GetError(),sa.Width,sa.Height,sa.ubpp);
		exit(-1);
	}

	printd(D_VI, D_DEBUG, "VI: SetVideoMode succeded (%d x %d, %d bpp)\n",sa.Width,sa.Height,sa.ubpp);
	SDL_WM_SetCaption("Fake64", NULL);

	glClearColor(0.1, 0.0, 0.0, 0.0);
//	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glFrustum(-3000.0, 3000.0, -3000.0, 3000.0, 500.0, 3000.0);
//	gluPerspective(45.0, (320.0/240.0), 750.0, 7500.0);
	glOrtho(0.0, 640.0, 0.0, 480.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
//	glEnable(GL_POLYGON_SMOOTH);
//	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

//	init_material();
	init_sprite_cache();
	init_fps_counter();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	win_scalex = (float)win_width / 320.0;
	win_scaley = (float)win_height / 240.0;

	

	printd(D_VIDEO, D_INFO, "Video initialized\n");
}

void vi_display(uint8* regs, uint16* addr)
{
	render2d(addr);
	frame_done();
}


int vi_deinit(void)
{
	exit_print_fps();
	printd(D_VIDEO, D_INFO, "Video deinitialized\n");
	SDL_Quit();
}

