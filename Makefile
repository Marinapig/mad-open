LIBS = -lmagic
SRC = mad-open.c file.c char.c magic.c
DEBUG_FLAGS = -DNDEBUG -ggdb
FLAGS_COMMON = -Wpedantic

mad-open: ${SRC}
	gcc ${LIBS} ${CFLAGS} ${SRC} ${FLAGS_COMMON} -o mad-open

debug: ${SRC}
	gcc ${FLAGS_COMMON} ${LIBS} ${DEBUG_FLAGS} ${CFLAGS} ${SRC} -o mad-open

.PHONY: install
.PHONY: clean

install: 
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f mad-open ${DESTDIR}${PREFIX}/bin
clean:
	rm -f mad-open
