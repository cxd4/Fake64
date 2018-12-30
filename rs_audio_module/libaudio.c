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

#ifndef GPROF

char *module_id(void) {
	return("rs's basic audio module (DSP)");
}

#endif

int ai_init()
{
    int value;

    int n_fragments = 16; /* number of fragments */
    int fragment_size = 8; /* a buffersize of 2^8 = 256 bytes */

		printf("AI: init rs' module\n");

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

/*
int ai_init() {
printf("rs' AI module (PCM Mode)\n");
fd_dsp = open("/dev/dsp", O_RDWR| O_SYNC);
  if (fd_dsp ==-1) {
   perror("open of /dev/dsp failed");
   exit(1);
  }


arg = CHANNELS-1;
  status = ioctl(fd_dsp, SOUND_PCM_WRITE_CHANNELS, &arg);
  if (status == -1)
    perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
  if (arg != CHANNELS)
    perror("unable to set number of channels");

  arg = 22000;
  status = ioctl(fd_dsp, SOUND_PCM_WRITE_RATE, &arg);
  if (status == -1) {
    perror("error from SOUND_PCM_WRITE_RATE ioctl");
    exit(1);
  }

 arg = SIZE;
      status = ioctl(fd_dsp, SOUND_PCM_WRITE_BITS, &arg);
  if (status == -1)
    perror("SOUND_PCM_WRITE_BITS ioctl failed");
  if (arg != SIZE)
    perror("unable to set sample size");
}
*/

int ai_dma_request(uint8 *airegs,uint32 addr) {
int newbitrate,newdacrate,value;
uint32 length=*((uint32 *)(airegs+0x04));
  	
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
	printf("ai_dma_request: Bitrate %d\tDacrate %d\t Frequency %d\n",bitrate,dacrate,VI_PAL_CLOCK/dacrate);
    printf("Length= %u\n",length);
#endif
  //write
	status = write(fd_dsp,(char*)addr ,length);

  if (status == -1) {
    perror("AI: Error writing to /dev/dsp");
    return 0;
  }
#ifdef DEBUG
  printf("AI wrote %d to /dev/dsp\n",status);
#endif
  return 1;
}


int ai_deinit(void) {
 printf("audio deinitialized\n");
 close(fd_dsp);
 return 1;
}

