SUBDIRS = romloader cpucore_pure_c

all-local:
	if [ ! -f ./loader ] ; then ln -s romloader/romloader ./loader; fi

clean-local:
	rm -f ./loader
