LDFLAGS = -lmagic

mad-open: mad-open.c
	gcc -ggdb -Wl,/usr/lib64/libmagic.so mad-open.c -o program
