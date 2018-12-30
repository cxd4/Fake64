libvideo_LTLIBRARIES = librsvideo.la
librsvideo_la_SOURCES = 2d.c libvideo.c dump.c config_options.c config.c rcp_state.c dlist_exec_1.c render.c dlist_exec_c.c sprite_cache.c fps.c basic_core.c ucode00.c
CFLAGS = -Wall -g3 -fomit-frame-pointer -O3 -finline-functions -ffast-math -fstrength-reduce -fthread-jumps -frerun-loop-opt -foptimize-register-move -fbranch-probabilities @SDL_CFLAGS@
LDFLAGS = @SDL_LIBS@ -L/usr/X11R6/lib -lGLU -lGL
