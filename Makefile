LIBS = -lmagic -lXDGdirs
SRC = mad-open.c file.c char.c magic.c
DEBUG_FLAGS = -ggdb -Og
RELEASE_FLAGS = -DNDEBUG
FLAGS_COMMON = -Wpedantic -pedantic

mad-open: ${SRC}
	$(CC) ${SRC} ${CFLAGS} ${FLAGS_COMMON} ${RELEASE_FLAGS} ${LIBS}  -o mad-open

debug: ${SRC}
	$(CC) ${FLAGS_COMMON} ${DEBUG_FLAGS} ${CFLAGS} ${SRC} ${LIBS} -o mad-open

.PHONY: install
.PHONY: clean

install: 
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f mad-open ${DESTDIR}${PREFIX}/bin
clean:
	rm -f mad-open
