Simple alternative to xdg-open written in C.

I got annoyed by xdg-opens configuration files (mimeapps.list, *desktop), so I decided to write my own program to do the same thing.

I'm new to C programming so if there are any deficiencies in this program
please make an issue or a pull request, thanks.

rules file syntax
[mimetype] [program] [rules...]
Current rules are:
noclose - Doesn't close stdout and stderr, useful when launching command line programs like vim

Dependencies:
libxdg-basedir (https://github.com/devnev/libxdg-basedir)
On Linux:
libbsd (https://libbsd.freedesktop.org/wiki/)

Tested and working on:
Linux amd64
OpenBSD amd64

Build instructions:
sh fakeconfigure
make
