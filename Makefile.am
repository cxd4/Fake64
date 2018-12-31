SUBDIRS = cpucore_pure_c bfs_input_module rs_video_module sliders_video_module bfs_audio_module rs_audio_module romloader cpucore_dynarec testsuite
EXTRA_DIST = detectmodules.sh fake64rc
all-local:
	if [ ! -f ./loader ] ; then ln -s romloader/romloader ./loader; fi
	chmod u+x ./detectmodules.sh
	./detectmodules.sh

clean-local:
	rm -f ./loader
	rm -rf ./plugins

install-exec-local:
	if [ ! -f @bindir@/fake64 ] ; then ln -s @hiddenbindir@/romloader @bindir@/fake64; fi
	cp ./fake64rc /etc/fake64rc
	chmod 644 /etc/fake64rc

dist-hook:
	mkdir $(distdir)/include
	cp $(srcdir)/include/*.h $(distdir)/include
	cp $(srcdir)/include/*.in $(distdir)/include
	cp testrom.tar.bz2 $(distdir)

static:
	gcc -DSTATIC_MODULES romloader/byteswap.c romloader/romloader.c romloader/configfile.c romloader/load_module.c romloader/printd.c cpucore_pure_c/libcpu.c cpucore_pure_c/config_options.c cpucore_pure_c/cop0_scp.c cpucore_pure_c/cop1_fpu.c cpucore_pure_c/debugger.c cpucore_pure_c/disass.c cpucore_pure_c/exec.c cpucore_pure_c/interrupts.c cpucore_pure_c/memory.c cpucore_pure_c/opstruct.c cpucore_pure_c/patch.c cpucore_pure_c/reg_names.c cpucore_pure_c/savestate.c sliders_video_module/libvideo.c rs_audio_module/libaudio.c bfs_input_module/libinput.c -o loader_static -pg -I./include -I./romloader -I./cpucore_pure_c -lm -ldl -lSDL -lpthread -lGL -l GLU
