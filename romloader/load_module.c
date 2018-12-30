#include <stdio.h>
#include <dlfcn.h>
#include <general.h>
#include <romheader.h>
#include <load_module.h>
#include <version.h>

void * load_module(char *name)
{ static void *module;

  module=dlopen(name,RTLD_LAZY | RTLD_GLOBAL); // replace with rtld_now to speed things up?
  if(!module)
   { fprintf(stderr, "%s: %s\n", PROG_NAME, dlerror());
     return NULL;
   }
  return module;
}

void * load_symbol(void *module,char *name,char *modulename)
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

input_module *load_input_module(char *name) {

	void *module, *module_id;
	input_module *input;

	module = load_module(name);
	if (!module) { return 0; }

	module_id = load_symbol(module, "module_id", name);
	if (!module_id) { return 0; }

	input = (input_module *)malloc(sizeof(input_module));
	input->module = module;
	input->module_id = module_id;
	input->config_module = dlsym(module, "config_module");

	return input;
}

video_module *load_video_module(char *name) {

	void *module, *module_id, *vi_init,*vi_deinit;
	video_module *video;

  void *vi_display; // temp by rs
	module = load_module(name);
	if (!module) { return 0; }

	module_id = load_symbol(module, "module_id", name);
	if (!module_id) { return 0; }

        vi_init = load_symbol(module, "vi_init", name);
        if (!vi_init) return 0;

        vi_deinit = load_symbol(module, "vi_deinit", name);
        if (!vi_deinit) return 0;

        vi_display = load_symbol(module, "vi_display", name);
        if (!vi_display) return 0;

	video = (video_module *)malloc(sizeof(video_module));
	video->module = module;
	video->module_id = module_id;
	video->vi_init = vi_init;
	video->vi_deinit = vi_deinit;
	video->config_module = dlsym(module, "config_module");

	return video;
}

audio_module *load_audio_module(char *name) {

	void *module, *module_id,*ai_init,*ai_dma_request,*ai_deinit;
	audio_module *audio;

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
	audio->module_id = module_id;
   	audio->ai_init = ai_init;
   	audio->ai_dma_request = ai_dma_request;
   	audio->ai_deinit = ai_deinit;
	audio->config_module = dlsym(module, "config_module");

	return audio;
}

cpu_module *load_cpu_core_module(char *name)
{ void *module,*module_id,*main_cpu_loop;
  cpu_module *cpumodule;
  module=load_module(name);
  if(!module) return 0;
  module_id=load_symbol(module,"module_id",name);
  if(!module_id) return 0;
  main_cpu_loop=load_symbol(module,"main_cpu_loop",name);
  if(!main_cpu_loop) return 0;

  cpumodule=(cpu_module *)malloc(sizeof(cpu_module));
  cpumodule->module=module;
  cpumodule->module_id=module_id;
  cpumodule->main_cpu_loop=main_cpu_loop;
  cpumodule->config_module=dlsym(module, "config_module");

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
