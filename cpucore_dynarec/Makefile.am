libcpu_LTLIBRARIES = libcpudyna.la
libcpudyna_la_SOURCES = libcpu.c memory.c dynstruct.c dyna.c registers.h cop0_scp.c cop1_fpu.c exec.c x86.c interrupts.c disass.c debugger.c opstruct.c x86.h opcodes.h dynstruct.h
CFLAGS = -g3 -O3 -finline-functions -ffast-math -fstrength-reduce -fthread-jumps -frerun-loop-opt -foptimize-register-move -fbranch-probabilities @gprof@ @fomit@
LDFLAGS = @gprof@ 
