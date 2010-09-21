#########################################################################
# vim: ts=8 sw=8
#########################################################################
# Author:   tf135c (James Reynolds)
# Filename: Makefile
# Date:     2006-11-27 13:31:53
#########################################################################
# Note that we use '::' rules to allow multiple rule sets for the same
# target.  Read that as "modularity exemplarized".
#########################################################################

PREFIX	:=${HOME}/opt/$(shell uname -m)
BINDIR	=${PREFIX}/bin

TEMPLATES:=$(wildcard t_*)

TARGETS	=all clean distclean clobber check install uninstall tags
TARGET	=all

SUBDIRS	=

.PHONY:	${TARGETS} ${SUBDIRS}

CC	=ccache gcc4 -march=i686 -std=gnu99
DEFS	=-D_FORTIFY_SOURCE=2
OPT	=-Os
INCS	=-I.
CFLAGS	=${OPT} -Wall -Wextra -Werror -pedantic -pipe -g ${DEFS} ${INCS}
LDFLAGS	=-g
LDLIBS	=-lSegFault

all::	col80

${TARGETS}::

clean::
	${RM} a.out *.o core.* lint tags

distclean clobber:: clean
	${RM} col80

check::	col80 image
	./col80 ${ARGS} image

install:: col80
	install -d ${BINDIR}
	install -c -s col80 ${BINDIR}/

uninstall::
	${RM} ${BINDIR}/col80

tags::
	ctags -R .

# ${TARGETS}::
# 	${MAKE} TARGET=$@ ${SUBDIRS}

# ${SUBDIRS}::
# 	${MAKE} -C $@ ${TARGET}
