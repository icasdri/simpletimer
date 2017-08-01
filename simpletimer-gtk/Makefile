# simpletimer can be compiled with either gcc or clang (compatible with gcc flags)
CC=gcc
#CC=tcc
#CC=clang 
DESTDIR=/usr
GTKFLAGS=`pkg-config --cflags --libs gtk+-3.0`
CFLAGS=-O2 -Wall -Wextra -Wconversion -Wsign-conversion -Wformat -Wformat-security -Werror -fstack-protector-all -Wstack-protector --param ssp-buffer-size=4 -pie -fPIE -ftrapv -D_FORTIFY_SOURCE=2 -z relro -z now

.PHONY: install

simpletimer: simpletimer.c resources.c
	$(CC) $^ -o $@ $(GTKFLAGS) $(CFLAGS)

resources.c: resources.gresource.xml main_window.xml row.xml
	glib-compile-resources $< --generate-source

install: simpletimer simpletimer.desktop
	install -Dm755 simpletimer "$(DESTDIR)/bin/simpletimer"
	install -Dm644 simpletimer.desktop "$(DESTDIR)/share/applications/simpletimer.desktop"
