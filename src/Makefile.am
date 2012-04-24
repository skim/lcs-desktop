## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(LCS_DESKTOP_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = lcs_desktop

lcs_desktop_SOURCES = \
	main.c \
	lcs-wm.c \
	lcs-wm.h \
	lcs-taskbar.c \
	lcs-taskbar.h \
	lcs-taskbar-task.c \
	lcs-taskbar-buttons.c \
	lcs-clutter.c \
	lcs-clutter.h

lcs_desktop_LDFLAGS =  \
	-o \
	lcs-desktop

lcs_desktop_LDADD = $(LCS_DESKTOP_LIBS)

