#ifndef MODULE_H
#define MODULE_H

#ifndef GENERAL_H
	#include <general.h>
#endif

#include <printd.h>
#include <ostask.h>

#include "romheader.h"


// printd comes free with modules, thought this was a sensible idea

// these really should be uint8* etc but it breaks msvc   (why?)

typedef struct cpu_m cpu_module;
typedef struct input_m input_module;
typedef struct video_m video_module;
typedef struct audio_m audio_module;

struct module_info
{
        input_module*   input;
        audio_module*   audio;
        video_module*   video;
        cpu_module*             cpu;
        int (*printd_f)(debug_type, debug_level, char*, ...);
	int (*test_debug_f)(debug_type, debug_level);
};

struct cpu_m
{
#ifndef WIN32
	void *module;
#endif

	void (*main_cpu_loop_f)(struct rom* rom, struct module_info* modules);
	char *(*module_id_f)(void);
	void (*config_module_f)(char*);
  void* (*translate_addr_f)(unsigned int);
};

struct input_m
{
#ifndef WIN32
	void *module;
#endif

	char *(*module_id_f)(void);
	void (*config_module_f)(char*);

	void (*init_pifram_f)(struct module_info *modptr,unsigned char *here);
	void (*pifram_interrupt_f)();

};

struct video_m
{
#ifndef WIN32
	void *module;
#endif

	char *(*module_id_f)(void);
	int (*vi_init_f)(struct module_info*);
	int (*vi_deinit_f)(void);
	void (*config_module_f)(char*);
	void (*vi_display_f)(unsigned char*,unsigned short*);

	void (*vi_intr_reg_write_f)(unsigned int data);
	void (*vi_width_reg_write_f)(unsigned int data);
	void (*vi_origin_reg_write_f)(unsigned int data);
	void (*vi_status_reg_write_f)(unsigned int data);
	void (*rcp_command_f)(OSTask*);
};

struct audio_m
{
#ifndef WIN32
	void *module;
#endif
	int (*ai_getlength_f)(void);
	char *(*module_id_f)(void);
	int (*ai_init_f)(void);
	int (*ai_dma_request_f)(unsigned char *airegs,unsigned int addr);
	int (*ai_deinit_f)(void);  
	void (*config_module_f)(char*); 
};


#endif

