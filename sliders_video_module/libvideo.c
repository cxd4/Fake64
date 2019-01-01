#ifdef WIN32
	#include <SDL.h>
#else
	#include <SDL/SDL.h>
#endif

#include "general.h"
#include <stdio.h>
#include <stdlib.h>

//#define DEBUG_VI
//#define X2         // temporarily unsupported

uint32 frames = 0;

struct screen_attributes {
    int XRes;
    int YRes;
    char rbpp;    // requested
    char ubpp;    // actaul used, we don't need alpha atm
    char Active;  // whether we have a current mode
    uint32 Status;
} sa;
SDL_Surface *screen;

#ifndef STATIC_MODULES
char
*module_id(void)
{
    return ("rs/slider's *basic* video module");
}
#endif

void vi_status_reg_write(uint32 data) {

sa.Status=data;
#ifdef DEBUG_VI
	printf("VI status write: %x\n", data);
#endif
//__asm__("int $3\n");
if ((data&0x3)==2 && (sa.rbpp!=16)) // use 15 when its really 16
 {
   sa.rbpp=16;
   sa.ubpp=15;
   sa.Active=1;
   if (sa.XRes)
   {
   if (!(screen=SDL_SetVideoMode(sa.XRes,sa.YRes,sa.ubpp,0)))
     printf("SetVideomode %d,%d\t%d bpp Failed!\n",sa.XRes,sa.YRes,sa.ubpp);
#ifdef DEBUG_VI
   else
     printf("SetVideomode %d,%d\t%d bpp\n",sa.XRes,sa.YRes,sa.ubpp);
#endif
   }
 }

 if ((data&0x3)==3 && (sa.rbpp!=32))
 {
   sa.rbpp=32;
   sa.ubpp=32;
   sa.Active=1;
   if (sa.XRes)
   {
   if (!(screen=SDL_SetVideoMode(sa.XRes,sa.YRes,sa.ubpp,0)))
     printf("SetVideomode %d,%d\t%d bpp Failed!\n",sa.XRes,sa.YRes,sa.ubpp);
#ifdef DEBUG_VI
   else
     printf("SetVideomode %d,%d\t%d bpp\n",sa.XRes,sa.YRes,sa.ubpp);
#endif
   }
 }
}

void vi_origin_reg_write(uint32 data) {

#ifdef DEBUG_VI
	printf("VI frame buffer origin: %#x\n", data);
#endif
}

void vi_width_reg_write(uint32 data) {
//__asm__("int $3\n");

#ifdef DEBUG_VI
	printf("VI line width: %d pixels\n", data);
#endif

if (((data&0x7FF)==320) && (sa.XRes!=320)) // use 15 when its really 16
 {
   sa.XRes=320;
   sa.YRes=240;
   if (sa.ubpp)
   {
   if ((screen=SDL_SetVideoMode(sa.XRes,sa.YRes,sa.ubpp, 0)))
     printf("SetVideomode %d,%d\t%d bpp Failed!\n",sa.XRes,sa.YRes,sa.ubpp);
#ifdef DEBUG_VI
   else
     printf("SetVideomode %d,%d\t%d bpp\n",sa.XRes,sa.YRes,sa.ubpp);
#endif
   }
   sa.Active=1;
 }

 if (((data&0x7FF)==640) && (sa.XRes!=640))
 {
   sa.XRes=640;
   sa.YRes=480;
   if (sa.ubpp)
   {
     if (!(screen=SDL_SetVideoMode(sa.XRes,sa.YRes,sa.ubpp,0)))
     printf("SetVideomode %d,%d\t%d bpp Failed!\n",sa.XRes,sa.YRes,sa.ubpp);
#ifdef DEBUG_VI
   else
     printf("SetVideomode %d,%d\t%d bpp\n",sa.XRes,sa.YRes,sa.ubpp);
#endif
   }
   sa.Active=1;
 }
}

void vi_intr_reg_write(uint32 data) {

#ifdef DEBUG_VI
	printf("VI interrupt requested at: %d halflines\n", data);
#endif
}


int vi_init(void)
{
//int x,y;


if((SDL_Init(SDL_INIT_VIDEO)==-1)) {
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }

#ifdef X2
//screen = SDL_SetVideoMode(640,480,15,0); /* program dependent? */       // (15 is not a bug (well it may be)  rs) it selects a different kind of 16 bit pixel mode (HCl)
#else
 // screen = SDL_SetVideoMode(320,240,15,0); /* program dependent? */       // (15 is not a bug (well it may be)  rs) it selects a different kind of 16 bit pixel mode (HCl)
//	sa.XRes=320;
 // sa.bpp=16;
#endif
sa.XRes=0;
sa.YRes=0;
sa.rbpp=sa.ubpp=0;
sa.Active=0;


//printf("%x %x %x %x\n",screen->format->Rmask,screen->format->Gmask,screen->format->Bmask,screen->format->Amask);
SDL_WM_SetCaption("Fake64", NULL);
printf("Video (SDL) initialized\n");

	return 0;
}

void vi_display(uint8* regs,uint16* addr)
{
  Uint16 *raw_pixels;
  int i;


	if ((uint32)addr>=0x80000000)
	{
#ifdef DEBUG_VI
		printf("ODD address , ignored\n");
#endif
		return;
	}
  if (!sa.Active)
	 return;

  SDL_LockSurface(screen);

  raw_pixels = (Uint16*) screen->pixels;

if (!raw_pixels)
  {
#ifdef DEBUG_VI
    printf("raw_pixels==0\n");
#endif
    return;	// no display
  }

// four bytes at once xx yy -> yy xx
//	for (;raw_pixels<end;raw_pixels+=1,addr+=1)
 //				*raw_pixels = (*(addr+1))>>1;
	//				*(raw_pixels+1) = (*addr)>>1;

if (sa.ubpp==15)
{
	if (sa.XRes==320)
	{
		for (i=0;i<320*240;i+=4)
		{

#ifdef WIN32
		/*__asm
		{
			movq	mm0,raw_pixels[i]
			movq	mm1,mm0
			psrld	mm1,16
			pslld	mm0,16
			por		mm1,mm0
			psrld	mm1,1		// shift out alpha
			movq	addr[i],mm1
	
		};
		*/
			raw_pixels[i]=addr[i+1]>>1;
			raw_pixels[i+1]=addr[i]>>1;
			raw_pixels[i+2]=addr[i+3]>>1;
			raw_pixels[i+3]=addr[i+2]>>1;



#else

		__asm__ ("movq %1,%%mm0\n\t"
               "movq %%mm0,%%mm1\n\t"
               "psrld $16,%%mm1\n\t"
               "pslld $16,%%mm0\n\t"
               "por %%mm0,%%mm1\n\t"
               "psrld $1,%%mm1\n\t"
               "movq %%mm1,%0 \n\t"
               :"=m" (raw_pixels[i]):"m"(addr[i]));
#endif    
	
	
	
	
	}
  }
  else if(sa.XRes==640)
  {
/*    int nj=0;
    if (sa.Status&0x40)
    {
    for (i=0,j=0;i<320;i+=1,j+=480,nj=j+960)
      for (;j<nj;j+=2)
      {
 	  			raw_pixels[j] = addr[j+1]>>1;
					raw_pixels[j+1] = addr[j]>>1;
      }
    }*/
	  /*
	  for (i=0;i<640*480;i+=4)
    {
      __asm__ ("movq %1,%%mm0\n\t"
              "movq %%mm0,%%mm1\n\t"
                "psrld $16,%%mm1\n\t"
                 "pslld $16,%%mm0\n\t"
                 "por %%mm0,%%mm1\n\t"
                 "psrld $1,%%mm1\n\t"
                 "movq %%mm1,%0 \n\t"
                 :"=m" (raw_pixels[i]):"m"(addr[i]));
    }
	*/


 }

 frames++;
}
else if(sa.ubpp==24)
{
	if (sa.XRes==320)
	{
#ifdef DEBUG_VI
		printf("Screen copy\n");
#endif
		memcpy(screen,raw_pixels,320*240);
	}
else if(sa.XRes==640)
{
#ifdef DEBUG_VI
printf("Screen copy\n");
#endif
memcpy(screen,raw_pixels,640*480*2);
}
}


#ifdef WIN32
	__asm emms;
#else
	__asm__("emms\n\t");
#endif




  SDL_UnlockSurface(screen);
  SDL_UpdateRect(screen, 0, 0, 0, 0);
}


int vi_deinit(void)
{
	printf("Video deinitialized\n"); /* nothing else needed yet, since init doesnt really do anything ;p */
	SDL_Quit();
	return 0;
}


/* dummy config function */
#ifndef STATIC_MODULES
void config_module(char* conf)
{
}
#endif

void rcp_command(uint8* data, int len)
{
	printf("Not implemented in this module\n");
}

/*	old pixel copying code

#ifdef X2

for (x = 0; x < 480; x+=2,raw_pixels+=640,addr+=0)
{
	for (y = 0; y < 640; y+=4,raw_pixels+=4,addr+=2) {
					pixel=*(addr+1)>>1;
					*raw_pixels=pixel ;
					*(raw_pixels+1)=pixel;
					*(raw_pixels+640)=pixel;
					*(raw_pixels+641)=pixel;

			 		pixel=*(addr)>>1;
					*(raw_pixels+2) = pixel;
					*(raw_pixels+3)=pixel;
					*(raw_pixels+642)=pixel;
					*(raw_pixels+643)=pixel;

}
  }


#else
*/
