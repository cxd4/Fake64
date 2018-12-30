libcpu_LTLIBRARIES = libcpu.la
libcpu_la_SOURCES = disass.c  libcpu.c  opstruct.c  exec.c  cop0_scp.c cop1_fpu.c memory.c interrupts.c reg_names.c debugger.c opcodes.h registers.h
CFLAGS = -g3 -O3 -finline-functions -ffast-math -fstrength-reduce -fthread-jumps -frerun-loop-opt -foptimize-register-move -fbranch-probabilities @gprof@ @fomit@
LDFLAGS = @gprof@ 
