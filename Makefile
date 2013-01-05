GITREV = $(shell git describe --dirty --tags --always 2>/dev/null)

DESTDIR = /
CC = gcc
CFLAGS = -Wextra -Wall -O2 $(shell pkg-config --cflags glib-2.0) \
		 $(if $(strip $(GITREV)),-DSESS_GITREV='"$(GITREV)"')
LIBS = -lncurses $(shell pkg-config --libs glib-2.0)

.PHONY: distclean clean i18n install

sessionchooser: desktopentry.o error.o i18n.o main.o session.o util.o
	$(CC) $(CFLAGS) $(LIBS) -o sessionchooser $^

# main.o does not depend on a header file
main.o:: main.c
	$(CC) $(CFLAGS) $(LIBS) -c $<

%.o: %.c %.h
	$(CC) $(CFLAGS) $(LIBS) -c $<

distclean:
	-rm *.o sessionchooser

clean:
	-rm *.o

i18n:
	cd i18n && \
		xgettext -k_ -kQ_ -kC_ -kN_ -kNC_ --from-code=UTF-8 ../*.c ../*.h

install: sessionchooser
	install -D sessionchooser $(DESTDIR)usr/bin/sessionchooser
