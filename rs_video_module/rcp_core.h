#ifndef MODULE_H
#include <module.h>
#endif

typedef void (*rcp_call)(rdp_command);

extern rcp_call rcp_dcmd[0x100];
extern rcp_call rcp_ecmd[0x100];

extern void (*rcp_core)(OSTask*);

extern void basic_rcp_core(OSTask* task);
