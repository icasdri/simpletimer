CC=gcc
CFLAGS=`pkg-config --cflags --libs gtk+-3.0`
DESTDIR=/usr

.PHONY: install

simpletimer: simpletimer.c resources.c
	$(CC) $^ -o $@ $(CFLAGS)

resources.c: resources.gresource.xml main_window.xml row.xml
	glib-compile-resources $< --generate-source

install: simpletimer
	install -Dm755 simpletimer "$(DESTDIR)/bin/simpletimer"
