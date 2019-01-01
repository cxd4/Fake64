#include "general.h"
#include <romheader.h>
#include <memory.h>

int ai_getlength(void)
{
	return 0;
}

#ifndef STATIC_MODULES
char *module_id(void)
{
#ifdef _DEBUG
	return("Bluefyre's lovely dummy audio module (DEBUG)");
#else
	return("Bluefyre's lovely dummy audio module");
#endif
}
#endif

int ai_init(void)
{
	printf("audio initialized\n");
	return 0;
}

int ai_dma_request(uint8 *airegs,uint32 addr)
{

	char play[100];
	static int c=0;
	FILE *fp;
	uint32 i,length;
	
	length=*((uint32 *)(airegs+0x04));

	printf("AI playing from 0x%x, length %d (%d)\nairegs 0x%x\n",addr,*(airegs+0x04),length,airegs);

/*
#ifdef WIN32
	return;
#endif
  */
  c+=1;
/*	sprintf(play,"file%d.pcm",c);
	fp=fopen(play,"w");

	for(i=0;i<=length;i++)
	    fputc(*((uint8 *)(addr+i)),fp);
	fclose(fp);
	printf("Dumped  audio buffer encountered\n");
*/	return 0;
}

int ai_deinit(void)
{
	printf("audio deinitialized\n");
	return 0;
}

/* dummy config function */
#ifndef STATIC_MODULES
void config_module(char* conf)
{
}
#endif
