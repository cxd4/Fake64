#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/soundcard.h>

#include <config.h>
#include <general.h>
#include <romheader.h>
#include <memory.h>


//#define DEBUG

#define VI_NTSC_CLOCK		48681812        /* Hz = 48.681812 MHz */
#define VI_PAL_CLOCK		49656530        /* Hz = 49.656530 MHz */
#define VI_MPAL_CLOCK		48628316        /* Hz = 48.628316 MHz */

#define USE_DSP  // otherwise default to PCM

#define CHANNELS 2



uint8* airegs;

int fd_dsp;       // sound device file descriptor
int arg;      // argument for ioctl calls 
int status;   // return status of system calls 
int bitrate=0;
int dacrate=0;

#ifndef STATIC_MODULES
char
*module_id(void)
{
    return ("rs's basic audio module (DSP)");
}
#endif

int ai_init()
{
    int value;

    int n_fragments = 300; /* number of fragments */
    int fragment_size = 8; /* a buffersize of 2^8 = 256 bytes */

		printf("AI: init rs' module\n");
		printf("ai_init = %p\n", ai_init);

    fd_dsp = open("/dev/dsp", O_WRONLY,0);
    if (fd_dsp ==-1)
    {
      perror("AI: open of /dev/dsp failed");
      return 0;
    }

    arg = AFMT_U16_LE;
    status=ioctl (fd_dsp, SNDCTL_DSP_SETFMT, &arg);
    if (status == -1)
    {
      perror("AI: Error setting format");
      return 0;
    }
    value = CHANNELS-1; /* mono = 0, stereo = 1 */
    status=ioctl (fd_dsp, SNDCTL_DSP_STEREO, &value);
    if (status == -1)
    {
      perror("AI: Error setting channels");
      return 0;
    }

    value = (n_fragments << 16) | fragment_size;
    status=ioctl (fd_dsp, SNDCTL_DSP_SETFRAGMENT, &value);
    if (status == -1)
    {
      perror("AI: Error setting fragment info");
      return 0;
    }
    return 1;
}

// get how much we have buffered up
int32 ai_getlength()
{
	int32 length;
	audio_buf_info info;
	status=ioctl(fd_dsp, SNDCTL_DSP_GETOSPACE, &info);
  if (-1==status)
		perror("AI: Error trying to get length");

	length=(info.fragstotal-info.fragments)*info.fragsize;
#ifdef DEBUG
    printf("AI: Warning: Read from length, %u\n",length);
#endif

	return length;
}

int ai_dma_request(uint8 *airegs,uint32 addr) {
int newbitrate,newdacrate,value;
uint32 length=*((uint32 *)(airegs+0x04))&~0x7;	// ignore lower 3 bits (HACK should be 17:3 or 15:3)
  	
  newbitrate=1+ (*((uint32*)(airegs+0x14)) &0x0F);
	newdacrate=(*((uint32*)(airegs+0x10))&0x3FFF); // lower 14 bits
	if (newbitrate!=bitrate)
	{
		bitrate=newbitrate;
	  value = bitrate; /* number of bits */
    status=ioctl (fd_dsp, SNDCTL_DSP_SAMPLESIZE, &value);
    if (status == -1) {
    perror("AI: Error setting bitrate");
    return 0;
    }
  }

	if (newdacrate!=dacrate)
	{
		dacrate=newdacrate;
		value = VI_PAL_CLOCK/dacrate; /* frequency */
		status=ioctl (fd_dsp, SNDCTL_DSP_SPEED, &value);// do something
    if (status == -1) {
    perror("AI: Error setting frequency");
    return 0;
    }
  }

#ifdef DEBUG
	if (dacrate)
		printf("AI dma_request: Bitrate %d\tDacrate %d\t Frequency %d from 0x%x\n",bitrate,dacrate,VI_PAL_CLOCK/dacrate,addr);
    printf("Length= %u\n",length);
#endif
  //write
	status = write(fd_dsp,(char*)addr ,length);

  if (status == -1) {
    perror("AI: Error writing to /dev/dsp");
    return 0;
  }
//  if (!length)
//		GenerateInterrupt(0x4); // AI_interrupt
#ifdef DEBUG
  printf("AI wrote %d to /dev/dsp\n",status);
#endif
  return 1;
}


int ai_deinit(void) {
 printf("AI: audio deinitialized\n");
 close(fd_dsp);
 return 1;
}

