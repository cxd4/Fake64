struct cpu_m {
   void *module;
   void (*main_cpu_loop)(struct rom *rom);
   char *(*module_id)(void);
};

typedef struct cpu_m cpu_module;
