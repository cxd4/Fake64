#include <config.h>
#include <general.h>
#include <romheader.h>
#include <memory.h>

#ifndef GPROF

char *module_id(void) {
	return("Bluefyre's lovely dummy audio module");
}

#endif

int ai_init(void) {
 printf("audio initialized\n");
}

int ai_dma_request(uint64 value,uint8 *airegs,uint32 addr) {
char play[100];
static int c=0;


 int i; 
 FILE *fp;
 uint32 length=*((uint32 *)(airegs+0x04));
c+=1;
sprintf(play,"file%d.pcm",c);

 fp=fopen(play,"w");
 for(i=0;i<=length;i++)
  {
    fputc(*((uint8 *)(addr+i)),fp);
  }
 fclose(fp);
 printf("Dumped  audio buffer encountered\n");
}

int ai_deinit(void) {
 printf("audio deinitialized\n");
}
