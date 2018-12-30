/*
struct cpu_m
{
#ifndef WIN32
	void *module;
#endif

	void (*main_cpu_loop)(struct rom *rom,struct module_info* modules);
	char *(*module_id)(void);
	void (*config_module)(char*);
=======
struct cpu_m {
   void *module;
   void (*main_cpu_loop)(struct rom *rom);
   char *(*module_id)(void);
   void (*config_module)(char*);
   void* (*translate_addr)(uint32);
>>>>>>> 1.8
};

struct input_m
{
#ifndef WIN32
	void *module;
#endif

	char *(*module_id)(void);
	void (*config_module)(char*);
};

struct video_m
{
#ifndef WIN32
	void *module;
#endif

	char *(*module_id)(void);
	int (*vi_init)(void);
	int (*vi_deinit)(void);
	void (*config_module)(char*);
};

struct audio_m
{
#ifndef WIN32
	void *module;
#endif
	int (*ai_getlength)(void);
	char *(*module_id)(void);
	int (*ai_init)(void);
	int (*ai_dma_request)(int value);
	int (*ai_deinit)(void);  
	void (*config_module)(char*); 
};

typedef struct cpu_m cpu_module;
typedef struct input_m input_module;
typedef struct video_m video_module;
typedef struct audio_m audio_module;



struct module_info
{
	input_module*	input;
	audio_module*	audio;
	video_module*	video;
	cpu_module*		cpu;
};
*/