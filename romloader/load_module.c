#include <stdio.h>
#include <dlfcn.h>
#include <general.h>
#include <romheader.h>
#include <load_module.h>
#include <version.h>

void * load_module(char *name)
{ void *module;

  module=dlopen(name,RTLD_LAZY); // replace with rtld_now to speed things up?
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

cpu_module *load_cpu_core_module(char *name)
{ void *module,*module_id,*main_cpu_loop;
  cpu_module *cpumodule;
  module=load_module(name);
  if(!module)
   return 0;
  module_id=load_symbol(module,"module_id",name);
  if(!module_id)
   return 0;
  main_cpu_loop=load_symbol(module,"main_cpu_loop",name);
  if(!main_cpu_loop)
   return 0;
  cpumodule=(cpu_module *)malloc(sizeof(cpu_module));
  cpumodule->module=module;
  cpumodule->module_id=module_id;
  cpumodule->main_cpu_loop=main_cpu_loop;
  return cpumodule;
}

/*main(int argc,char **argv)
{ cpu_module *cpu;
  cpu=load_cpu_core_module(argv[1]);
  printf("%s\n",(*cpu->module_id)());
}*/
