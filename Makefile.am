SUBDIRS = romloader cpucore_pure_c bfs_input_module

all-local:
	if [ ! -f ./loader ] ; then ln -s romloader/romloader ./loader; fi

clean-local:
	rm -f ./loader

install-exec-local:
	if [ ! -f @bindir@/fake64 ] ; then ln -s @hiddenbindir@/romloader @bindir@/fake64; fi

dist-hook:
	mkdir $(distdir)/include
	cp $(srcdir)/include/*.h $(distdir)/include
	cp $(srcdir)/include/*.in $(distdir)/include
	cp testrom.tar.bz2 $(distdir)
