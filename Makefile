LDFLAGS = -lmagic

mad-open: mad-open.c
	gcc -ggdb -Wl,/usr/lib64/libmagic.so mad-open.c -o program

.PHONY: install

install:
	install -m 557 program /usr/bin/mad-open
