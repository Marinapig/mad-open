LIBS = -lmagic -lxdg-basedir
SRC = src/mad-open.c src/file.c src/magic.c
DEBUG_FLAGS = -ggdb -O0 
#LINUX_FLAGS = -lbsd
#OPENBSD_FLAGS = -I/usr/local/include -L/usr/local/lib
RELEASE_FLAGS = -DNDEBUG
FLAGS_COMMON = -pedantic -Werror
COMMON_BUILD = $(CC) ${SRC} ${CFLAGS} ${FLAGS_COMMON} ${LIBS} ${LINUX_FLAGS} ${OPENBSD_FLAGS}

mad-open: ${SRC}
	${COMMON_BUILD} ${RELEASE_FLAGS} -o mad-open

debug: ${SRC}
	${COMMON_BUILD} ${DEBUG_FLAGS} -o mad-open

.PHONY: install
.PHONY: clean

install: 
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f mad-open ${DESTDIR}${PREFIX}/bin
clean:
	rm -f mad-open
