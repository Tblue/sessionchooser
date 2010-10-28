DESTDIR = /
CC = gcc
CFLAGS = -Wextra -Wall -O2 $(shell pkg-config --cflags glib-2.0)
LIBS = -lncurses $(shell pkg-config --libs glib-2.0)

.PHONY: depend distclean clean i18n install

sessionchooser: desktopentry.o error.o i18n.o main.o session.o util.o
	$(CC) $(CFLAGS) $(LIBS) -o sessionchooser $^

%.o: %.c
	$(CC) $(CFLAGS) $(LIBS) -c $<

depend:
	makedepend -- $(CFLAGS) -- *.c

distclean:
	-rm *.o sessionchooser

clean:
	-rm *.o

i18n:
	cd i18n && \
		xgettext -k_ -kQ_ -kC_ -kN_ -kNC_ --from-code=UTF-8 ../*.c ../*.h

install: sessionchooser
	install -D sessionchooser $(DESTDIR)usr/bin/sessionchooser

# DO NOT DELETE

desktopentry.o: /usr/include/string.h /usr/include/features.h
desktopentry.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
desktopentry.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
desktopentry.o: /usr/include/xlocale.h desktopentry.h
desktopentry.o: /usr/include/glib-2.0/glib.h
desktopentry.o: /usr/include/glib-2.0/glib/galloca.h
desktopentry.o: /usr/include/glib-2.0/glib/gtypes.h
desktopentry.o: /usr/lib/glib-2.0/include/glibconfig.h
desktopentry.o: /usr/include/glib-2.0/glib/gmacros.h /usr/include/limits.h
desktopentry.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
desktopentry.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
desktopentry.o: /usr/include/glib-2.0/glib/garray.h
desktopentry.o: /usr/include/glib-2.0/glib/gasyncqueue.h
desktopentry.o: /usr/include/glib-2.0/glib/gthread.h
desktopentry.o: /usr/include/glib-2.0/glib/gerror.h
desktopentry.o: /usr/include/glib-2.0/glib/gquark.h
desktopentry.o: /usr/include/glib-2.0/glib/gutils.h
desktopentry.o: /usr/include/glib-2.0/glib/gatomic.h
desktopentry.o: /usr/include/glib-2.0/glib/gbacktrace.h /usr/include/signal.h
desktopentry.o: /usr/include/bits/sigset.h /usr/include/bits/types.h
desktopentry.o: /usr/include/bits/typesizes.h /usr/include/bits/signum.h
desktopentry.o: /usr/include/time.h /usr/include/bits/siginfo.h
desktopentry.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
desktopentry.o: /usr/include/asm/sigcontext.h /usr/include/linux/types.h
desktopentry.o: /usr/include/asm/types.h /usr/include/asm-generic/types.h
desktopentry.o: /usr/include/asm-generic/int-ll64.h
desktopentry.o: /usr/include/asm/bitsperlong.h
desktopentry.o: /usr/include/asm-generic/bitsperlong.h
desktopentry.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
desktopentry.o: /usr/include/asm/posix_types.h
desktopentry.o: /usr/include/asm/posix_types_32.h
desktopentry.o: /usr/include/bits/sigstack.h /usr/include/sys/ucontext.h
desktopentry.o: /usr/include/bits/pthreadtypes.h
desktopentry.o: /usr/include/bits/sigthread.h
desktopentry.o: /usr/include/glib-2.0/glib/gbase64.h
desktopentry.o: /usr/include/glib-2.0/glib/gbitlock.h
desktopentry.o: /usr/include/glib-2.0/glib/gbookmarkfile.h
desktopentry.o: /usr/include/glib-2.0/glib/gcache.h
desktopentry.o: /usr/include/glib-2.0/glib/glist.h
desktopentry.o: /usr/include/glib-2.0/glib/gmem.h
desktopentry.o: /usr/include/glib-2.0/glib/gslice.h
desktopentry.o: /usr/include/glib-2.0/glib/gchecksum.h
desktopentry.o: /usr/include/glib-2.0/glib/gcompletion.h
desktopentry.o: /usr/include/glib-2.0/glib/gconvert.h
desktopentry.o: /usr/include/glib-2.0/glib/gdataset.h
desktopentry.o: /usr/include/glib-2.0/glib/gdate.h
desktopentry.o: /usr/include/glib-2.0/glib/gdatetime.h
desktopentry.o: /usr/include/glib-2.0/glib/gtimezone.h
desktopentry.o: /usr/include/glib-2.0/glib/gdir.h
desktopentry.o: /usr/include/glib-2.0/glib/gfileutils.h
desktopentry.o: /usr/include/glib-2.0/glib/ghash.h
desktopentry.o: /usr/include/glib-2.0/glib/ghook.h
desktopentry.o: /usr/include/glib-2.0/glib/ghostutils.h
desktopentry.o: /usr/include/glib-2.0/glib/giochannel.h
desktopentry.o: /usr/include/glib-2.0/glib/gmain.h
desktopentry.o: /usr/include/glib-2.0/glib/gpoll.h
desktopentry.o: /usr/include/glib-2.0/glib/gslist.h
desktopentry.o: /usr/include/glib-2.0/glib/gstring.h
desktopentry.o: /usr/include/glib-2.0/glib/gunicode.h
desktopentry.o: /usr/include/glib-2.0/glib/gkeyfile.h
desktopentry.o: /usr/include/glib-2.0/glib/gmappedfile.h
desktopentry.o: /usr/include/glib-2.0/glib/gmarkup.h
desktopentry.o: /usr/include/glib-2.0/glib/gmessages.h
desktopentry.o: /usr/include/glib-2.0/glib/gnode.h
desktopentry.o: /usr/include/glib-2.0/glib/goption.h
desktopentry.o: /usr/include/glib-2.0/glib/gpattern.h
desktopentry.o: /usr/include/glib-2.0/glib/gprimes.h
desktopentry.o: /usr/include/glib-2.0/glib/gqsort.h
desktopentry.o: /usr/include/glib-2.0/glib/gqueue.h
desktopentry.o: /usr/include/glib-2.0/glib/grand.h
desktopentry.o: /usr/include/glib-2.0/glib/grel.h
desktopentry.o: /usr/include/glib-2.0/glib/gregex.h
desktopentry.o: /usr/include/glib-2.0/glib/gscanner.h
desktopentry.o: /usr/include/glib-2.0/glib/gsequence.h
desktopentry.o: /usr/include/glib-2.0/glib/gshell.h
desktopentry.o: /usr/include/glib-2.0/glib/gspawn.h
desktopentry.o: /usr/include/glib-2.0/glib/gstrfuncs.h
desktopentry.o: /usr/include/glib-2.0/glib/gtestutils.h
desktopentry.o: /usr/include/glib-2.0/glib/gthreadpool.h
desktopentry.o: /usr/include/glib-2.0/glib/gtimer.h
desktopentry.o: /usr/include/glib-2.0/glib/gtree.h
desktopentry.o: /usr/include/glib-2.0/glib/gurifuncs.h
desktopentry.o: /usr/include/glib-2.0/glib/gvarianttype.h
desktopentry.o: /usr/include/glib-2.0/glib/gvariant.h error.h
error.o: error.h /usr/include/glib-2.0/glib.h
error.o: /usr/include/glib-2.0/glib/galloca.h
error.o: /usr/include/glib-2.0/glib/gtypes.h
error.o: /usr/lib/glib-2.0/include/glibconfig.h
error.o: /usr/include/glib-2.0/glib/gmacros.h /usr/include/limits.h
error.o: /usr/include/features.h /usr/include/sys/cdefs.h
error.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
error.o: /usr/include/gnu/stubs-32.h /usr/include/bits/posix1_lim.h
error.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
error.o: /usr/include/bits/posix2_lim.h /usr/include/glib-2.0/glib/garray.h
error.o: /usr/include/glib-2.0/glib/gasyncqueue.h
error.o: /usr/include/glib-2.0/glib/gthread.h
error.o: /usr/include/glib-2.0/glib/gerror.h
error.o: /usr/include/glib-2.0/glib/gquark.h
error.o: /usr/include/glib-2.0/glib/gutils.h
error.o: /usr/include/glib-2.0/glib/gatomic.h
error.o: /usr/include/glib-2.0/glib/gbacktrace.h /usr/include/signal.h
error.o: /usr/include/bits/sigset.h /usr/include/bits/types.h
error.o: /usr/include/bits/typesizes.h /usr/include/bits/signum.h
error.o: /usr/include/time.h /usr/include/bits/siginfo.h
error.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
error.o: /usr/include/asm/sigcontext.h /usr/include/linux/types.h
error.o: /usr/include/asm/types.h /usr/include/asm-generic/types.h
error.o: /usr/include/asm-generic/int-ll64.h /usr/include/asm/bitsperlong.h
error.o: /usr/include/asm-generic/bitsperlong.h
error.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
error.o: /usr/include/asm/posix_types.h /usr/include/asm/posix_types_32.h
error.o: /usr/include/bits/sigstack.h /usr/include/sys/ucontext.h
error.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
error.o: /usr/include/glib-2.0/glib/gbase64.h
error.o: /usr/include/glib-2.0/glib/gbitlock.h
error.o: /usr/include/glib-2.0/glib/gbookmarkfile.h
error.o: /usr/include/glib-2.0/glib/gcache.h
error.o: /usr/include/glib-2.0/glib/glist.h /usr/include/glib-2.0/glib/gmem.h
error.o: /usr/include/glib-2.0/glib/gslice.h
error.o: /usr/include/glib-2.0/glib/gchecksum.h
error.o: /usr/include/glib-2.0/glib/gcompletion.h
error.o: /usr/include/glib-2.0/glib/gconvert.h
error.o: /usr/include/glib-2.0/glib/gdataset.h
error.o: /usr/include/glib-2.0/glib/gdate.h
error.o: /usr/include/glib-2.0/glib/gdatetime.h
error.o: /usr/include/glib-2.0/glib/gtimezone.h
error.o: /usr/include/glib-2.0/glib/gdir.h
error.o: /usr/include/glib-2.0/glib/gfileutils.h
error.o: /usr/include/glib-2.0/glib/ghash.h
error.o: /usr/include/glib-2.0/glib/ghook.h
error.o: /usr/include/glib-2.0/glib/ghostutils.h
error.o: /usr/include/glib-2.0/glib/giochannel.h
error.o: /usr/include/glib-2.0/glib/gmain.h
error.o: /usr/include/glib-2.0/glib/gpoll.h
error.o: /usr/include/glib-2.0/glib/gslist.h
error.o: /usr/include/glib-2.0/glib/gstring.h
error.o: /usr/include/glib-2.0/glib/gunicode.h
error.o: /usr/include/glib-2.0/glib/gkeyfile.h
error.o: /usr/include/glib-2.0/glib/gmappedfile.h
error.o: /usr/include/glib-2.0/glib/gmarkup.h
error.o: /usr/include/glib-2.0/glib/gmessages.h
error.o: /usr/include/glib-2.0/glib/gnode.h
error.o: /usr/include/glib-2.0/glib/goption.h
error.o: /usr/include/glib-2.0/glib/gpattern.h
error.o: /usr/include/glib-2.0/glib/gprimes.h
error.o: /usr/include/glib-2.0/glib/gqsort.h
error.o: /usr/include/glib-2.0/glib/gqueue.h
error.o: /usr/include/glib-2.0/glib/grand.h /usr/include/glib-2.0/glib/grel.h
error.o: /usr/include/glib-2.0/glib/gregex.h
error.o: /usr/include/glib-2.0/glib/gscanner.h
error.o: /usr/include/glib-2.0/glib/gsequence.h
error.o: /usr/include/glib-2.0/glib/gshell.h
error.o: /usr/include/glib-2.0/glib/gspawn.h
error.o: /usr/include/glib-2.0/glib/gstrfuncs.h
error.o: /usr/include/glib-2.0/glib/gtestutils.h
error.o: /usr/include/glib-2.0/glib/gthreadpool.h
error.o: /usr/include/glib-2.0/glib/gtimer.h
error.o: /usr/include/glib-2.0/glib/gtree.h
error.o: /usr/include/glib-2.0/glib/gurifuncs.h
error.o: /usr/include/glib-2.0/glib/gvarianttype.h
error.o: /usr/include/glib-2.0/glib/gvariant.h
i18n.o: /usr/include/libintl.h /usr/include/features.h
i18n.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
i18n.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
i18n.o: /usr/include/locale.h /usr/include/bits/locale.h
i18n.o: /usr/include/xlocale.h defines.h i18n.h /usr/include/glib-2.0/glib.h
i18n.o: /usr/include/glib-2.0/glib/galloca.h
i18n.o: /usr/include/glib-2.0/glib/gtypes.h
i18n.o: /usr/lib/glib-2.0/include/glibconfig.h
i18n.o: /usr/include/glib-2.0/glib/gmacros.h /usr/include/limits.h
i18n.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
i18n.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
i18n.o: /usr/include/glib-2.0/glib/garray.h
i18n.o: /usr/include/glib-2.0/glib/gasyncqueue.h
i18n.o: /usr/include/glib-2.0/glib/gthread.h
i18n.o: /usr/include/glib-2.0/glib/gerror.h
i18n.o: /usr/include/glib-2.0/glib/gquark.h
i18n.o: /usr/include/glib-2.0/glib/gutils.h
i18n.o: /usr/include/glib-2.0/glib/gatomic.h
i18n.o: /usr/include/glib-2.0/glib/gbacktrace.h /usr/include/signal.h
i18n.o: /usr/include/bits/sigset.h /usr/include/bits/types.h
i18n.o: /usr/include/bits/typesizes.h /usr/include/bits/signum.h
i18n.o: /usr/include/time.h /usr/include/bits/siginfo.h
i18n.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
i18n.o: /usr/include/asm/sigcontext.h /usr/include/linux/types.h
i18n.o: /usr/include/asm/types.h /usr/include/asm-generic/types.h
i18n.o: /usr/include/asm-generic/int-ll64.h /usr/include/asm/bitsperlong.h
i18n.o: /usr/include/asm-generic/bitsperlong.h
i18n.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
i18n.o: /usr/include/asm/posix_types.h /usr/include/asm/posix_types_32.h
i18n.o: /usr/include/bits/sigstack.h /usr/include/sys/ucontext.h
i18n.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
i18n.o: /usr/include/glib-2.0/glib/gbase64.h
i18n.o: /usr/include/glib-2.0/glib/gbitlock.h
i18n.o: /usr/include/glib-2.0/glib/gbookmarkfile.h
i18n.o: /usr/include/glib-2.0/glib/gcache.h
i18n.o: /usr/include/glib-2.0/glib/glist.h /usr/include/glib-2.0/glib/gmem.h
i18n.o: /usr/include/glib-2.0/glib/gslice.h
i18n.o: /usr/include/glib-2.0/glib/gchecksum.h
i18n.o: /usr/include/glib-2.0/glib/gcompletion.h
i18n.o: /usr/include/glib-2.0/glib/gconvert.h
i18n.o: /usr/include/glib-2.0/glib/gdataset.h
i18n.o: /usr/include/glib-2.0/glib/gdate.h
i18n.o: /usr/include/glib-2.0/glib/gdatetime.h
i18n.o: /usr/include/glib-2.0/glib/gtimezone.h
i18n.o: /usr/include/glib-2.0/glib/gdir.h
i18n.o: /usr/include/glib-2.0/glib/gfileutils.h
i18n.o: /usr/include/glib-2.0/glib/ghash.h /usr/include/glib-2.0/glib/ghook.h
i18n.o: /usr/include/glib-2.0/glib/ghostutils.h
i18n.o: /usr/include/glib-2.0/glib/giochannel.h
i18n.o: /usr/include/glib-2.0/glib/gmain.h /usr/include/glib-2.0/glib/gpoll.h
i18n.o: /usr/include/glib-2.0/glib/gslist.h
i18n.o: /usr/include/glib-2.0/glib/gstring.h
i18n.o: /usr/include/glib-2.0/glib/gunicode.h
i18n.o: /usr/include/glib-2.0/glib/gkeyfile.h
i18n.o: /usr/include/glib-2.0/glib/gmappedfile.h
i18n.o: /usr/include/glib-2.0/glib/gmarkup.h
i18n.o: /usr/include/glib-2.0/glib/gmessages.h
i18n.o: /usr/include/glib-2.0/glib/gnode.h
i18n.o: /usr/include/glib-2.0/glib/goption.h
i18n.o: /usr/include/glib-2.0/glib/gpattern.h
i18n.o: /usr/include/glib-2.0/glib/gprimes.h
i18n.o: /usr/include/glib-2.0/glib/gqsort.h
i18n.o: /usr/include/glib-2.0/glib/gqueue.h
i18n.o: /usr/include/glib-2.0/glib/grand.h /usr/include/glib-2.0/glib/grel.h
i18n.o: /usr/include/glib-2.0/glib/gregex.h
i18n.o: /usr/include/glib-2.0/glib/gscanner.h
i18n.o: /usr/include/glib-2.0/glib/gsequence.h
i18n.o: /usr/include/glib-2.0/glib/gshell.h
i18n.o: /usr/include/glib-2.0/glib/gspawn.h
i18n.o: /usr/include/glib-2.0/glib/gstrfuncs.h
i18n.o: /usr/include/glib-2.0/glib/gtestutils.h
i18n.o: /usr/include/glib-2.0/glib/gthreadpool.h
i18n.o: /usr/include/glib-2.0/glib/gtimer.h
i18n.o: /usr/include/glib-2.0/glib/gtree.h
i18n.o: /usr/include/glib-2.0/glib/gurifuncs.h
i18n.o: /usr/include/glib-2.0/glib/gvarianttype.h
i18n.o: /usr/include/glib-2.0/glib/gvariant.h
i18n.o: /usr/include/glib-2.0/glib/gi18n.h /usr/include/string.h
main.o: /usr/include/glib-2.0/glib.h /usr/include/glib-2.0/glib/galloca.h
main.o: /usr/include/glib-2.0/glib/gtypes.h
main.o: /usr/lib/glib-2.0/include/glibconfig.h
main.o: /usr/include/glib-2.0/glib/gmacros.h /usr/include/limits.h
main.o: /usr/include/features.h /usr/include/sys/cdefs.h
main.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
main.o: /usr/include/gnu/stubs-32.h /usr/include/bits/posix1_lim.h
main.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
main.o: /usr/include/bits/posix2_lim.h /usr/include/glib-2.0/glib/garray.h
main.o: /usr/include/glib-2.0/glib/gasyncqueue.h
main.o: /usr/include/glib-2.0/glib/gthread.h
main.o: /usr/include/glib-2.0/glib/gerror.h
main.o: /usr/include/glib-2.0/glib/gquark.h
main.o: /usr/include/glib-2.0/glib/gutils.h
main.o: /usr/include/glib-2.0/glib/gatomic.h
main.o: /usr/include/glib-2.0/glib/gbacktrace.h /usr/include/signal.h
main.o: /usr/include/bits/sigset.h /usr/include/bits/types.h
main.o: /usr/include/bits/typesizes.h /usr/include/bits/signum.h
main.o: /usr/include/time.h /usr/include/bits/siginfo.h
main.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
main.o: /usr/include/asm/sigcontext.h /usr/include/linux/types.h
main.o: /usr/include/asm/types.h /usr/include/asm-generic/types.h
main.o: /usr/include/asm-generic/int-ll64.h /usr/include/asm/bitsperlong.h
main.o: /usr/include/asm-generic/bitsperlong.h
main.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
main.o: /usr/include/asm/posix_types.h /usr/include/asm/posix_types_32.h
main.o: /usr/include/bits/sigstack.h /usr/include/sys/ucontext.h
main.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
main.o: /usr/include/glib-2.0/glib/gbase64.h
main.o: /usr/include/glib-2.0/glib/gbitlock.h
main.o: /usr/include/glib-2.0/glib/gbookmarkfile.h
main.o: /usr/include/glib-2.0/glib/gcache.h
main.o: /usr/include/glib-2.0/glib/glist.h /usr/include/glib-2.0/glib/gmem.h
main.o: /usr/include/glib-2.0/glib/gslice.h
main.o: /usr/include/glib-2.0/glib/gchecksum.h
main.o: /usr/include/glib-2.0/glib/gcompletion.h
main.o: /usr/include/glib-2.0/glib/gconvert.h
main.o: /usr/include/glib-2.0/glib/gdataset.h
main.o: /usr/include/glib-2.0/glib/gdate.h
main.o: /usr/include/glib-2.0/glib/gdatetime.h
main.o: /usr/include/glib-2.0/glib/gtimezone.h
main.o: /usr/include/glib-2.0/glib/gdir.h
main.o: /usr/include/glib-2.0/glib/gfileutils.h
main.o: /usr/include/glib-2.0/glib/ghash.h /usr/include/glib-2.0/glib/ghook.h
main.o: /usr/include/glib-2.0/glib/ghostutils.h
main.o: /usr/include/glib-2.0/glib/giochannel.h
main.o: /usr/include/glib-2.0/glib/gmain.h /usr/include/glib-2.0/glib/gpoll.h
main.o: /usr/include/glib-2.0/glib/gslist.h
main.o: /usr/include/glib-2.0/glib/gstring.h
main.o: /usr/include/glib-2.0/glib/gunicode.h
main.o: /usr/include/glib-2.0/glib/gkeyfile.h
main.o: /usr/include/glib-2.0/glib/gmappedfile.h
main.o: /usr/include/glib-2.0/glib/gmarkup.h
main.o: /usr/include/glib-2.0/glib/gmessages.h
main.o: /usr/include/glib-2.0/glib/gnode.h
main.o: /usr/include/glib-2.0/glib/goption.h
main.o: /usr/include/glib-2.0/glib/gpattern.h
main.o: /usr/include/glib-2.0/glib/gprimes.h
main.o: /usr/include/glib-2.0/glib/gqsort.h
main.o: /usr/include/glib-2.0/glib/gqueue.h
main.o: /usr/include/glib-2.0/glib/grand.h /usr/include/glib-2.0/glib/grel.h
main.o: /usr/include/glib-2.0/glib/gregex.h
main.o: /usr/include/glib-2.0/glib/gscanner.h
main.o: /usr/include/glib-2.0/glib/gsequence.h
main.o: /usr/include/glib-2.0/glib/gshell.h
main.o: /usr/include/glib-2.0/glib/gspawn.h
main.o: /usr/include/glib-2.0/glib/gstrfuncs.h
main.o: /usr/include/glib-2.0/glib/gtestutils.h
main.o: /usr/include/glib-2.0/glib/gthreadpool.h
main.o: /usr/include/glib-2.0/glib/gtimer.h
main.o: /usr/include/glib-2.0/glib/gtree.h
main.o: /usr/include/glib-2.0/glib/gurifuncs.h
main.o: /usr/include/glib-2.0/glib/gvarianttype.h
main.o: /usr/include/glib-2.0/glib/gvariant.h
main.o: /usr/include/glib-2.0/glib/gprintf.h /usr/include/stdio.h
main.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
main.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
main.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
main.o: /usr/include/sys/types.h /usr/include/sys/select.h
main.o: /usr/include/bits/select.h /usr/include/bits/time.h
main.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
main.o: /usr/include/string.h /usr/include/xlocale.h /usr/include/unistd.h
main.o: /usr/include/bits/posix_opt.h /usr/include/bits/environments.h
main.o: /usr/include/bits/confname.h /usr/include/getopt.h
main.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
main.o: /usr/include/bits/stat.h defines.h error.h i18n.h
main.o: /usr/include/glib-2.0/glib/gi18n.h /usr/include/libintl.h session.h
main.o: /usr/include/glib-2.0/glib/gstdio.h /usr/include/sys/stat.h util.h
session.o: /usr/include/string.h /usr/include/features.h
session.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
session.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
session.o: /usr/include/xlocale.h desktopentry.h /usr/include/glib-2.0/glib.h
session.o: /usr/include/glib-2.0/glib/galloca.h
session.o: /usr/include/glib-2.0/glib/gtypes.h
session.o: /usr/lib/glib-2.0/include/glibconfig.h
session.o: /usr/include/glib-2.0/glib/gmacros.h /usr/include/limits.h
session.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
session.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
session.o: /usr/include/glib-2.0/glib/garray.h
session.o: /usr/include/glib-2.0/glib/gasyncqueue.h
session.o: /usr/include/glib-2.0/glib/gthread.h
session.o: /usr/include/glib-2.0/glib/gerror.h
session.o: /usr/include/glib-2.0/glib/gquark.h
session.o: /usr/include/glib-2.0/glib/gutils.h
session.o: /usr/include/glib-2.0/glib/gatomic.h
session.o: /usr/include/glib-2.0/glib/gbacktrace.h /usr/include/signal.h
session.o: /usr/include/bits/sigset.h /usr/include/bits/types.h
session.o: /usr/include/bits/typesizes.h /usr/include/bits/signum.h
session.o: /usr/include/time.h /usr/include/bits/siginfo.h
session.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
session.o: /usr/include/asm/sigcontext.h /usr/include/linux/types.h
session.o: /usr/include/asm/types.h /usr/include/asm-generic/types.h
session.o: /usr/include/asm-generic/int-ll64.h /usr/include/asm/bitsperlong.h
session.o: /usr/include/asm-generic/bitsperlong.h
session.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
session.o: /usr/include/asm/posix_types.h /usr/include/asm/posix_types_32.h
session.o: /usr/include/bits/sigstack.h /usr/include/sys/ucontext.h
session.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
session.o: /usr/include/glib-2.0/glib/gbase64.h
session.o: /usr/include/glib-2.0/glib/gbitlock.h
session.o: /usr/include/glib-2.0/glib/gbookmarkfile.h
session.o: /usr/include/glib-2.0/glib/gcache.h
session.o: /usr/include/glib-2.0/glib/glist.h
session.o: /usr/include/glib-2.0/glib/gmem.h
session.o: /usr/include/glib-2.0/glib/gslice.h
session.o: /usr/include/glib-2.0/glib/gchecksum.h
session.o: /usr/include/glib-2.0/glib/gcompletion.h
session.o: /usr/include/glib-2.0/glib/gconvert.h
session.o: /usr/include/glib-2.0/glib/gdataset.h
session.o: /usr/include/glib-2.0/glib/gdate.h
session.o: /usr/include/glib-2.0/glib/gdatetime.h
session.o: /usr/include/glib-2.0/glib/gtimezone.h
session.o: /usr/include/glib-2.0/glib/gdir.h
session.o: /usr/include/glib-2.0/glib/gfileutils.h
session.o: /usr/include/glib-2.0/glib/ghash.h
session.o: /usr/include/glib-2.0/glib/ghook.h
session.o: /usr/include/glib-2.0/glib/ghostutils.h
session.o: /usr/include/glib-2.0/glib/giochannel.h
session.o: /usr/include/glib-2.0/glib/gmain.h
session.o: /usr/include/glib-2.0/glib/gpoll.h
session.o: /usr/include/glib-2.0/glib/gslist.h
session.o: /usr/include/glib-2.0/glib/gstring.h
session.o: /usr/include/glib-2.0/glib/gunicode.h
session.o: /usr/include/glib-2.0/glib/gkeyfile.h
session.o: /usr/include/glib-2.0/glib/gmappedfile.h
session.o: /usr/include/glib-2.0/glib/gmarkup.h
session.o: /usr/include/glib-2.0/glib/gmessages.h
session.o: /usr/include/glib-2.0/glib/gnode.h
session.o: /usr/include/glib-2.0/glib/goption.h
session.o: /usr/include/glib-2.0/glib/gpattern.h
session.o: /usr/include/glib-2.0/glib/gprimes.h
session.o: /usr/include/glib-2.0/glib/gqsort.h
session.o: /usr/include/glib-2.0/glib/gqueue.h
session.o: /usr/include/glib-2.0/glib/grand.h
session.o: /usr/include/glib-2.0/glib/grel.h
session.o: /usr/include/glib-2.0/glib/gregex.h
session.o: /usr/include/glib-2.0/glib/gscanner.h
session.o: /usr/include/glib-2.0/glib/gsequence.h
session.o: /usr/include/glib-2.0/glib/gshell.h
session.o: /usr/include/glib-2.0/glib/gspawn.h
session.o: /usr/include/glib-2.0/glib/gstrfuncs.h
session.o: /usr/include/glib-2.0/glib/gtestutils.h
session.o: /usr/include/glib-2.0/glib/gthreadpool.h
session.o: /usr/include/glib-2.0/glib/gtimer.h
session.o: /usr/include/glib-2.0/glib/gtree.h
session.o: /usr/include/glib-2.0/glib/gurifuncs.h
session.o: /usr/include/glib-2.0/glib/gvarianttype.h
session.o: /usr/include/glib-2.0/glib/gvariant.h i18n.h
session.o: /usr/include/glib-2.0/glib/gi18n.h /usr/include/libintl.h
session.o: session.h /usr/include/glib-2.0/glib/gstdio.h
session.o: /usr/include/glib-2.0/glib/gprintf.h /usr/include/stdio.h
session.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
session.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
session.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
util.o: /usr/include/glob.h /usr/include/sys/cdefs.h
util.o: /usr/include/bits/wordsize.h /usr/include/string.h
util.o: /usr/include/features.h /usr/include/gnu/stubs.h
util.o: /usr/include/gnu/stubs-32.h /usr/include/xlocale.h
util.o: /usr/include/curses.h /usr/include/ncurses_dll.h /usr/include/stdio.h
util.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
util.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
util.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
util.o: /usr/include/unctrl.h /usr/include/term.h /usr/include/termios.h
util.o: /usr/include/bits/termios.h /usr/include/sys/ttydefaults.h
util.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
util.o: /usr/include/bits/environments.h /usr/include/bits/confname.h
util.o: /usr/include/getopt.h /usr/include/stropts.h
util.o: /usr/include/bits/xtitypes.h /usr/include/bits/stropts.h
util.o: /usr/include/linux/vt.h defines.h util.h /usr/include/glib-2.0/glib.h
util.o: /usr/include/glib-2.0/glib/galloca.h
util.o: /usr/include/glib-2.0/glib/gtypes.h
util.o: /usr/lib/glib-2.0/include/glibconfig.h
util.o: /usr/include/glib-2.0/glib/gmacros.h /usr/include/limits.h
util.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
util.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
util.o: /usr/include/glib-2.0/glib/garray.h
util.o: /usr/include/glib-2.0/glib/gasyncqueue.h
util.o: /usr/include/glib-2.0/glib/gthread.h
util.o: /usr/include/glib-2.0/glib/gerror.h
util.o: /usr/include/glib-2.0/glib/gquark.h
util.o: /usr/include/glib-2.0/glib/gutils.h
util.o: /usr/include/glib-2.0/glib/gatomic.h
util.o: /usr/include/glib-2.0/glib/gbacktrace.h /usr/include/signal.h
util.o: /usr/include/bits/sigset.h /usr/include/bits/signum.h
util.o: /usr/include/time.h /usr/include/bits/siginfo.h
util.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
util.o: /usr/include/asm/sigcontext.h /usr/include/linux/types.h
util.o: /usr/include/asm/types.h /usr/include/asm-generic/types.h
util.o: /usr/include/asm-generic/int-ll64.h /usr/include/asm/bitsperlong.h
util.o: /usr/include/asm-generic/bitsperlong.h
util.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
util.o: /usr/include/asm/posix_types.h /usr/include/asm/posix_types_32.h
util.o: /usr/include/bits/sigstack.h /usr/include/sys/ucontext.h
util.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
util.o: /usr/include/glib-2.0/glib/gbase64.h
util.o: /usr/include/glib-2.0/glib/gbitlock.h
util.o: /usr/include/glib-2.0/glib/gbookmarkfile.h
util.o: /usr/include/glib-2.0/glib/gcache.h
util.o: /usr/include/glib-2.0/glib/glist.h /usr/include/glib-2.0/glib/gmem.h
util.o: /usr/include/glib-2.0/glib/gslice.h
util.o: /usr/include/glib-2.0/glib/gchecksum.h
util.o: /usr/include/glib-2.0/glib/gcompletion.h
util.o: /usr/include/glib-2.0/glib/gconvert.h
util.o: /usr/include/glib-2.0/glib/gdataset.h
util.o: /usr/include/glib-2.0/glib/gdate.h
util.o: /usr/include/glib-2.0/glib/gdatetime.h
util.o: /usr/include/glib-2.0/glib/gtimezone.h
util.o: /usr/include/glib-2.0/glib/gdir.h
util.o: /usr/include/glib-2.0/glib/gfileutils.h
util.o: /usr/include/glib-2.0/glib/ghash.h /usr/include/glib-2.0/glib/ghook.h
util.o: /usr/include/glib-2.0/glib/ghostutils.h
util.o: /usr/include/glib-2.0/glib/giochannel.h
util.o: /usr/include/glib-2.0/glib/gmain.h /usr/include/glib-2.0/glib/gpoll.h
util.o: /usr/include/glib-2.0/glib/gslist.h
util.o: /usr/include/glib-2.0/glib/gstring.h
util.o: /usr/include/glib-2.0/glib/gunicode.h
util.o: /usr/include/glib-2.0/glib/gkeyfile.h
util.o: /usr/include/glib-2.0/glib/gmappedfile.h
util.o: /usr/include/glib-2.0/glib/gmarkup.h
util.o: /usr/include/glib-2.0/glib/gmessages.h
util.o: /usr/include/glib-2.0/glib/gnode.h
util.o: /usr/include/glib-2.0/glib/goption.h
util.o: /usr/include/glib-2.0/glib/gpattern.h
util.o: /usr/include/glib-2.0/glib/gprimes.h
util.o: /usr/include/glib-2.0/glib/gqsort.h
util.o: /usr/include/glib-2.0/glib/gqueue.h
util.o: /usr/include/glib-2.0/glib/grand.h /usr/include/glib-2.0/glib/grel.h
util.o: /usr/include/glib-2.0/glib/gregex.h
util.o: /usr/include/glib-2.0/glib/gscanner.h
util.o: /usr/include/glib-2.0/glib/gsequence.h
util.o: /usr/include/glib-2.0/glib/gshell.h
util.o: /usr/include/glib-2.0/glib/gspawn.h
util.o: /usr/include/glib-2.0/glib/gstrfuncs.h
util.o: /usr/include/glib-2.0/glib/gtestutils.h
util.o: /usr/include/glib-2.0/glib/gthreadpool.h
util.o: /usr/include/glib-2.0/glib/gtimer.h
util.o: /usr/include/glib-2.0/glib/gtree.h
util.o: /usr/include/glib-2.0/glib/gurifuncs.h
util.o: /usr/include/glib-2.0/glib/gvarianttype.h
util.o: /usr/include/glib-2.0/glib/gvariant.h
util.o: /usr/include/glib-2.0/glib/gstdio.h
util.o: /usr/include/glib-2.0/glib/gprintf.h /usr/include/sys/stat.h
util.o: /usr/include/bits/stat.h
