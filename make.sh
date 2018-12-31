src="."

SRC_LIST="\
$src/romloader/byteswap.c \
$src/romloader/romloader.c \
$src/romloader/configfile.c \
$src/romloader/load_module.c \
$src/romloader/printd.c \
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
$src/cpucore_pure_c/savestate.c \
$src/sliders_video_module/libvideo.c \
$src/rs_audio_module/libaudio.c \
$src/bfs_input_module/libinput.c"

gcc -o loader_static $SRC_LIST -DSTATIC_MODULES -pg -I$src/include -I$src/romloader -I$src/cpucore_pure_c -lm -ldl -lSDL -lpthread -lGL -l GLU
