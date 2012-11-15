# $Id: Makefile.am 4 2008-06-22 09:19:11Z rbock $

noinst_PROGRAMS         = \
                          basics\
                          basics2\
                          logarithmic\
                          rulers\
                          keycontrol\
                          colors\
                          markers\
                          grid\
                          grid_array\
                          signals\
                          lissajous\
			              basics_libglade \
                          enable_disable\
                          addremove


basics_SOURCES          = basics.c

basics2_SOURCES         = basics2.c

logarithmic_SOURCES     = logarithmic.c

rulers_SOURCES     = rulers.c

keycontrol_SOURCES      = keycontrol.c

colors_SOURCES          = colors.c

markers_SOURCES          = markers.c

grid_SOURCES            = grid.c

grid_array_SOURCES      = grid_array.c

signals_SOURCES         = signals.c

lissajous_SOURCES       = lissajous.c

enable_disable_SOURCES  = enable_disable.c

addremove_SOURCES       = addremove.c

basics_libglade_SOURCES = basics_libglade.c
EXTRA_DIST              = basics_libglade.glade

INCLUDES		= \
			-I$(top_srcdir)/gtk

AM_CFLAGS			=\
			-O2\
			-W -Wall -pedantic\
			-DG_DISABLE_DEPRECATED\
			-DGDK_DISABLE_DEPRECATED\
			-DGDK_PIXBUF_DISABLE_DEPRECATED\
			-DGTK_DISABLE_DEPRECATED\
			@LIBGLADE_CFLAGS@ \
			`pkg-config gtk+-3.0 --cflags`


LDADD 			= $(top_builddir)/gtk/libgtkdatabox.la\
		 	@GTK_LIBS@


if BUILD_LIBGLADE
basics_libglade_LDADD	= $(top_builddir)/gtk/libgtkdatabox.la \
			$(top_builddir)/glade/libdatabox.la \
		 	@GTK_LIBS@ \
			@LIBGLADE_LIBS@
else
basics_libglade_LDADD = $(LDADD)
endif

basics : basics.o
	gcc -o basics basics.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm

basics2 : basics2.o
	gcc -o basics2 basics2.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm

addremove : addremove.o
	gcc -o addremove addremove.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm

colors : colors.o
	gcc -o colors colors.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm

enable_disable : enable_disable.o
	gcc -o enable_disable enable_disable.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm

grid : grid.o
	gcc -o grid grid.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm

grid_array : grid_array.o
	gcc -o grid_array grid_array.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm

keycontrol : keycontrol.o
	gcc -o keycontrol keycontrol.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm

lissajous : lissajous.o
	gcc -o lissajous lissajous.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm

logarithmic : logarithmic.o
	gcc -o logarithmic logarithmic.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm

markers : markers.o
	gcc -o markers markers.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm

rulers : rulers.o
	gcc -o rulers rulers.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm

signals : signals.o
	gcc -o signals signals.o ../gtk/.libs/libgtkdatabox.a `pkg-config --libs gtk+-3.0` -lm
