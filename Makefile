LIBS = -lmagic -lXDGdirs
SRC = mad-open.c file.c char.c magic.c
DEBUG_FLAGS = -DNDEBUG -ggdb
FLAGS_COMMON = -Wpedantic

mad-open: ${SRC}
	gcc ${CFLAGS} ${SRC} ${FLAGS_COMMON} ${LIBS} -o mad-open

debug: ${SRC}
	gcc ${FLAGS_COMMON} ${DEBUG_FLAGS} ${CFLAGS} ${SRC} ${LIBS} -o mad-open

.PHONY: install
.PHONY: clean

install: 
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f mad-open ${DESTDIR}${PREFIX}/bin
clean:
	rm -f mad-open
