#include <windows.h>
#include <stdio.h>
#include <general.h>
#include <romheader.h>
#include <module.h>
#include <version.h>


HMODULE load_module(char *name)
{
	static HMODULE Module;

	Module=LoadLibrary(name);
	if(!Module)
		fprintf(stderr, "%s: error %d\n", name, GetLastError());	// fix to string sometime
	return Module;
}


void *load_symbol(HMODULE Module,char *name,char *modulename)
{
	static void *symbol;
	if (!(symbol=GetProcAddress(Module,name)))
		fprintf(stderr,"Couldn't find %s in library %s: %d\n",name,modulename,GetLastError());
	return symbol;
}



input_module *load_input_module(char *name)
{
	HMODULE module;
	input_module *input;

	if (!name || !(module=load_module(name)))
		return NULL;

	input = (input_module *)malloc(sizeof(input_module));

	input->module_id_f = (char*(*)(void))load_symbol(module, "module_id", name);
	input->config_module_f = (void(*)(char*))load_symbol(module,"config_module",name);


	input->init_pifram_f=load_symbol(module,"init_pifram",name);
	input->pifram_interrupt_f=load_symbol(module,"pifram_interrupt",name);

	if (!*input->config_module_f | !*input->module_id_f)
	{
		dfree(input);
		return NULL;
	}
	return input;
}

video_module *load_video_module(char *name)
{
	HMODULE module;
	video_module *video;

	if (!name || !(module=load_module(name)))
		return NULL;

	video = (video_module *)malloc(sizeof(video_module));

	video->module_id_f = (char*(*)(void))	load_symbol(module, "module_id", name);
	video->vi_init_f = (int(*)(void)) load_symbol(module, "vi_init", name);
	video->vi_deinit_f = (int(*)(void))load_symbol(module, "vi_deinit", name);
	video->config_module_f = (void(*)(char*))load_symbol(module, "config_module",name);

	video->vi_display_f=load_symbol(module,"vi_display",name);
	video->vi_intr_reg_write_f=load_symbol(module,"vi_intr_reg_write",name);
	video->vi_width_reg_write_f=load_symbol(module,"vi_width_reg_write",name);
	video->vi_origin_reg_write_f=load_symbol(module,"vi_origin_reg_write",name);
	video->vi_status_reg_write_f=load_symbol(module,"vi_status_reg_write",name);
	video->dump_rcp_command_f=load_symbol(module,"dump_rcp_command",name);

	if (!*video->config_module_f || !*video->module_id_f || !*video->vi_deinit_f || !*video->vi_init_f)
	{
		dfree(video);
		return NULL;
	}
	return video;
}



audio_module *load_audio_module(char *name)
{
	HMODULE module;
	audio_module *audio;

	if (!name || !(module=load_module(name)))
		return NULL;

	audio = (audio_module *)malloc(sizeof(audio_module));

   	audio->ai_init_f = (int(*)(void))load_symbol(module, "ai_init",name);
   	audio->ai_dma_request_f = (int(*)(int))load_symbol(module, "ai_dma_request",name);
   	audio->ai_deinit_f = (int(*)(void))load_symbol(module, "ai_deinit",name);
	audio->ai_getlength_f= load_symbol(module,"ai_getlength",name);
	
	audio->module_id_f = (char*(*)(void))load_symbol(module, "module_id",name);
	audio->config_module_f = (void(*)(char*))load_symbol(module, "config_module",name);

	if (!*audio->config_module_f || !*audio->module_id_f || !*audio->ai_deinit_f || !*audio->ai_init_f || !*audio->ai_dma_request_f)
	{
		dfree(audio);
		return NULL;
	}
	return audio;
}

cpu_module *load_cpu_core_module(char *name)
{
	HMODULE module;
	cpu_module *cpumodule;

	if (!name || !(module=load_module(name)))
		return NULL;
	
	cpumodule=(cpu_module *)malloc(sizeof(cpu_module));

	cpumodule->module_id_f=(char*(*)(void))load_symbol(module, "module_id",name);
	cpumodule->main_cpu_loop_f=(void(*)(struct rom*,struct module_info* modules))load_symbol(module, "main_cpu_loop",name);
	cpumodule->config_module_f=(void(*)(char*))load_symbol(module, "config_module",name);
	cpumodule->translate_addr_f=load_symbol(module,"translate_addr",name);

	if (!*cpumodule->config_module_f || !*cpumodule->main_cpu_loop_f || !*cpumodule->module_id_f)
	{
		dfree(cpumodule);
		return NULL;
	}
	return cpumodule;
}


char *get_module_id(char *filename)
{
	HMODULE module;
	char *(*module_id_func)(void);
	static char buf[1024];

	if (!filename || !(module=load_module(filename)))
		return NULL;
	
	if (!(module_id_func=(char*(*)(void))load_symbol(module,"module_id",filename)))
		return NULL;

	strncpy(buf,(*module_id_func)(),1024);

	FreeLibrary(module);
	return buf;
}

char*  pick_module(char* Directory)
{
	char input[256];
	char Search[MAX_PATH];
	static char FullPath[MAX_PATH];
	int i,modules;
	WIN32_FIND_DATA FindFileData; 
	HANDLE FindFileHandle;

	modules=0;

	sprintf(Search,"%s\\*.dll",Directory);
	FindFileHandle=FindFirstFile(Search,&FindFileData);	

	if (FindFileHandle==INVALID_HANDLE_VALUE)
		return NULL;
	do
	{
		modules++;
		sprintf(FullPath,"%s\\%s",Directory,FindFileData.cFileName);
		printf("\t%d:  %s, %s\n",modules,FindFileData.cFileName,get_module_id(FullPath));
	} while(FindNextFile(FindFileHandle,&FindFileData));

	FindClose(FindFileHandle);

	if (modules==1)			// only one module to choose from :)
		return FullPath;

	do
	{
		fgets(input,256,stdin);
		i=atoi(input);
		
	} while(i<1 || i>modules); 

	FindFileHandle=FindFirstFile(Search,&FindFileData);	

	if (FindFileHandle==INVALID_HANDLE_VALUE)
		return NULL;
	do
	{
		i--;
		if (i==0)
		{
			sprintf(FullPath,"%s\\%s",Directory,FindFileData.cFileName);
			FindClose(FindFileHandle);
			return FullPath;
		}
	} while(FindNextFile(FindFileHandle,&FindFileData));

	FindClose(FindFileHandle);
	return NULL;
}
