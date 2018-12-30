struct cpu_m {
   void *module;
   void (*main_cpu_loop)(struct rom *rom);
   char *(*module_id)(void);
};

struct input_m {
   void *module;
   char *(*module_id)(void);
   void *(*init_pifram)(uint8*);
};

typedef struct cpu_m cpu_module;
typedef struct input_m input_module;
