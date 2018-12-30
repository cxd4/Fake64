#include <stdio.h>
#include <general.h>
#include <romheader.h>
#include <module.h>
#include <version.h>
#include <dlfcn.h>


void *load_module(char *name)
{
	static void *module;
  module=dlopen(name,RTLD_GLOBAL | RTLD_NOW); // replace with rtld_now to speed things up? (yes possibly)
  if(!module)
   { fprintf(stderr, "%s: %s\n", PROG_NAME, dlerror());
     return NULL;
   }
  return module;
}


void *load_symbol(void *module,char *name,char *modulename)
{ void *symbol;
  const char *error;
  dlerror();
  symbol=dlsym(module,name);
  if((error=dlerror())) {
   fprintf(stderr,"Couldn't find %s in library %s: %s\n",name,modulename,error);
   return NULL;
  }
  return symbol;
}



input_module *load_input_module(char *name)
{
	input_module *input;
	
	if(!name[0]) return 0;	

	input = (input_module *)malloc(sizeof(input_module));

	input->module = load_module(name);
	if (!input->module) { free(input); return 0; }

	input->module_id_f = load_symbol(input->module, "module_id", name);
	if (!input->module_id_f) { free(input); return 0; }

        input->init_pifram_f = load_symbol(input->module, "init_pifram", name);
        if (!input->init_pifram_f) { free(input); return 0; }

        input->pifram_interrupt_f = load_symbol(input->module, "pifram_interrupt", name);
        if (!input->pifram_interrupt_f) { free(input); return 0; }

	input->config_module_f = dlsym(input->module, "config_module");

	return input;
}

video_module *load_video_module(char *name)
{
	video_module *video;

        if(!name[0]) return 0;

        video = (video_module *)malloc(sizeof(video_module));

	video->module = load_module(name);
	if (!video->module) { free(video); return 0; }

        video->module_id_f = load_symbol(video->module, "module_id", name);
        if (!video->module_id_f) { free(video); return 0; }

        video->vi_init_f = load_symbol(video->module, "vi_init", name);
        if (!video->vi_init_f) { free(video); return 0; }

        video->vi_deinit_f = load_symbol(video->module, "vi_deinit", name);
        if (!video->vi_deinit_f) { free(video); return 0; }

        video->vi_display_f = load_symbol(video->module, "vi_display", name);
        if (!video->vi_display_f) { free(video); return 0; }

        video->vi_intr_reg_write_f = load_symbol(video->module, "vi_intr_reg_write", name);
        if (!video->vi_intr_reg_write_f) { free(video); return 0; }

        video->vi_width_reg_write_f = load_symbol(video->module, "vi_width_reg_write", name);
        if (!video->vi_width_reg_write_f) { free(video); return 0; }

        video->vi_origin_reg_write_f = load_symbol(video->module, "vi_origin_reg_write", name);
        if (!video->vi_origin_reg_write_f) { free(video); return 0; }

        video->vi_status_reg_write_f = load_symbol(video->module, "vi_status_reg_write", name);
        if (!video->vi_status_reg_write_f) { free(video); return 0; }

        video->rcp_command_f = load_symbol(video->module, "rcp_command", name);
        if (!video->rcp_command_f) { free(video); return 0; }

	video->config_module_f = dlsym(video->module, "config_module");

	return video;
}



audio_module *load_audio_module(char *name) {

	void *module, *module_id,*ai_init,*ai_dma_request,*ai_deinit;
	audio_module *audio;

        if(!name[0]) return 0;

	module = load_module(name);
	if (!module) return 0;

	module_id = load_symbol(module, "module_id", name);
	if (!module_id) return 0;

	ai_init = load_symbol(module, "ai_init", name);
	if (!ai_init) return 0;

        ai_dma_request = load_symbol(module, "ai_dma_request", name);
        if (!ai_init) return 0;

        ai_deinit = load_symbol(module, "ai_deinit", name);
        if (!ai_deinit) return 0;

	audio = (audio_module *)malloc(sizeof(audio_module));
	audio->module = module;
	audio->module_id_f = module_id;
   	audio->ai_init_f = ai_init;
   	audio->ai_dma_request_f = ai_dma_request;
   	audio->ai_deinit_f = ai_deinit;
	audio->config_module_f = dlsym(module, "config_module");

	return audio;
}

cpu_module *load_cpu_core_module(char *name)
{ void *module,*module_id,*main_cpu_loop,*translate_addr;
  cpu_module *cpumodule;

  if(!name[0]) return 0;

  module=load_module(name);
  if(!module) return 0;
  module_id=load_symbol(module,"module_id",name);
  if(!module_id) return 0;
  main_cpu_loop=load_symbol(module,"main_cpu_loop",name);
  if(!main_cpu_loop) return 0;
  translate_addr=load_symbol(module,"translate_addr",name);
  if(!translate_addr) return 0;

  cpumodule=(cpu_module *)malloc(sizeof(cpu_module));
  cpumodule->module=module;
  cpumodule->module_id_f=module_id;
  cpumodule->main_cpu_loop_f=main_cpu_loop;
  cpumodule->config_module_f=dlsym(module, "config_module");
  cpumodule->translate_addr_f=translate_addr;

  return cpumodule;
}

char *get_module_id(char *filename)
{ void *module;
  char *(*module_id_func)(void);
  char buf[1024];
  module=load_module(filename);
  if(!module)
   return 0;
  module_id_func=load_symbol(module,"module_id",filename);
  if(!module_id_func)
   return 0;
  strcpy(buf,(*module_id_func)());
  dlclose(module);
  
  return buf;
}
