LIBS = -lmagic
SRC = mad-open.c file.c char.c
DEBUG_FLAGS = -DNDEBUG -ggdb

mad-open: ${SRC}
	gcc ${LIBS} ${CFLAGS} ${SRC} -o mad-open

debug: ${SRC}
	gcc ${LIBS} ${DEBUG_FLAGS} ${CFLAGS} ${SRC} -o mad-open

.PHONY: install
.PHONY: clean

install: 
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f mad-open ${DESTDIR}${PREFIX}/bin
clean:
	rm -f mad-open
