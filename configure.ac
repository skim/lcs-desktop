dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT([lcs_desktop], 0.1)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CC




dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=[lcs_desktop]
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])

AM_GLIB_GNU_GETTEXT



LT_INIT



PKG_CHECK_MODULES(LCS_DESKTOP, gtk+-3.0
gio-2.0
pango
libwnck-3.0
[gdk-3.0 clutter-1.0 ])




AC_OUTPUT([
Makefile
src/Makefile
po/Makefile.in
])
