src="."

# available sub-module ("plugin") options to choose from

VIDEO_BLUEFYRE="$src/bfs_video_module/*.c" # Nothing there. :)
VIDEO_RS="$src/rs_video_module/*.c"
VIDEO_SLIDER="$src/sliders_video_module/libvideo.c"

AUDIO_BLUEFYRE="$src/bfs_audio_module/libaudio.c"
AUDIO_RS="$src/rs_audio_module/libaudio.c"

INPUT_BLUEFYRE="$src/bfs_input_module/libinput.c"

# To do:  RSP sub-module selection?

CPU_INTERPRETER="\
$src/cpucore_pure_c/libcpu.c \
$src/cpucore_pure_c/config_options.c \
$src/cpucore_pure_c/cop0_scp.c \
$src/cpucore_pure_c/cop1_fpu.c \
$src/cpucore_pure_c/debugger.c \
$src/cpucore_pure_c/disass.c \
$src/cpucore_pure_c/exec.c \
$src/cpucore_pure_c/interrupts.c \
$src/cpucore_pure_c/memory.c \
$src/cpucore_pure_c/opstruct.c \
$src/cpucore_pure_c/patch.c \
$src/cpucore_pure_c/reg_names.c \
$src/cpucore_pure_c/savestate.c"
CPU_DYNAREC="\
$src/cpucore_dynarec/libcpu.c \
$src/cpucore_dynarec/cop0_scp.c \
$src/cpucore_dynarec/cop1_fpu.c \
$src/cpucore_dynarec/debugger.c \
$src/cpucore_dynarec/disass.c \
$src/cpucore_dynarec/dyna.c \
$src/cpucore_dynarec/dynstruct.c \
$src/cpucore_dynarec/exec.c \
$src/cpucore_dynarec/interrupts.c \
$src/cpucore_dynarec/memory.c \
$src/cpucore_dynarec/opstruct.c \
$src/cpucore_dynarec/savestate.c \
$src/cpucore_dynarec/x86.c"

CPU_CORE=$CPU_INTERPRETER
VIDEO_MODULE=$VIDEO_RS
AUDIO_MODULE=$AUDIO_BLUEFYRE
INPUT_MODULE=$INPUT_BLUEFYRE

SRC_LIST="\
$src/romloader/byteswap.c \
$src/romloader/romloader.c \
$src/romloader/configfile.c \
$src/romloader/load_module.c \
$src/romloader/printd.c \
$CPU_CORE $VIDEO_MODULE $AUDIO_MODULE $INPUT_MODULE"

rm loader_static
#gcc -o loader_static $SRC_LIST -DSTATIC_MODULES -DGPROF -pg -I$src/include -I$src/romloader -I$src/cpucore_pure_c -lm -ldl -lSDL -lpthread -lGL -lGLU
gcc -o loader_static $SRC_LIST -DSTATIC_MODULES -DCLIENT_ENDIAN -I$src/include -I$src/romloader -I$src/cpucore_pure_c -lm -ldl -lSDL -lpthread -lGL -lGLU
#gcc -o loader_static $SRC_LIST -DSTATIC_MODULES -I$src/include -I$src/romloader -I$src/cpucore_pure_c -lm -ldl -lSDL -lpthread -lGL -lGLU
