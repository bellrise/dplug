all: libdplug.a

libdplug.a:
	cc -c -o dplug.o dplug.c
	ar rcs libdplug.a dplug.o

install:
	cp dplug.h /usr/include/dplug.h
	cp libdplug.a /usr/lib/libdplug.a
