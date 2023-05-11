LDFLAGS = -lmagic

mad-open: mad-open.c
	gcc -ggdb -Wl,/usr/lib64/libmagic.so mad-open.c -o program

.PHONY: install

install: 
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f program ${DESTDIR}${PREFIX}/bin
