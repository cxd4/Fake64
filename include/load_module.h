struct cpu_m {
   void *module;
   void (*main_cpu_loop)(struct rom *rom);
   char *(*module_id)(void);
   void (*config_module)(char*);
};

struct input_m {
   void *module;
   char *(*module_id)(void);
   void (*config_module)(char*);
};

struct video_m {
   void *module;
   char *(*module_id)(void);
   int (*vi_init)(void);
   int (*vi_deinit)(void);
   void (*config_module)(char*);
};

struct audio_m {
   void *module;
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

