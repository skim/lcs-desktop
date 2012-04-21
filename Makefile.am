## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src po

lcs_desktopdocdir = ${prefix}/doc/lcs_desktop
lcs_desktopdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(lcs_desktopdoc_DATA) \
	$(INTLTOOL_FILES)

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(lcs_desktopdocdir)
