all: libdplug.a

libdplug.a:
	cc -c -o dplug.o dplug.c
	ar r libdplug.a dplug.o

install: libdplug.a
	cp dplug.h /usr/include/dplug.h
	cp libdplug.a /usr/lib/libdplug.a

.PHONY : libdplug.a
