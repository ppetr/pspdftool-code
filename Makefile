# 
# Projekt:   pspdftool
# Autor:     Ales Snuparek
# Datum:
#

CFLAGS = -c  -Wall -D _DEBUG -g -ansi -std=gnu99    #pg pro profiler #parametry prekladace
LDFLAGS = -lm -lz 
CC=gcc				#prekladac jazyka C
prefix=/usr/local
PREFIX=${prefix}
DESTDIR=${PREFIX}/bin
mandir=${PREFIX}/share/man
PROJECT="PSPDFtool"
PROJECT_DESC="tool for preprinting preparation"
INSTALL=install

all:.depend pspdftool

pspdftool: main.o  fileio.o cmdexec.o common.o vldoc.o vdocerror.o dummydoc.o vdocerror.o page_list.o ps_doc.o ps_lib.o pdf_doc.o pdf_lib.o vdoc.o pdf_parser.o pdf_filters.o
	$(CC) ${LDFLAGS} -o $@ main.o  fileio.o cmdexec.o vldoc.o dummydoc.o vdocerror.o page_list.o common.o ps_doc.o ps_lib.o pdf_doc.o pdf_lib.o vdoc.o pdf_parser.o pdf_filters.o

.depend:clist
	gcc -M `cat clist` | tr '\n\t\r' ' '  | sed 's/\/usr[^ ]* //g; s/\\//g ; /^[ ]*;/d; s/\([^ ]*:\)/\1/g ; s/[ ][ ]*/ /g' | tr '' '\n'> .depend

clist:
	if !  [ -f "clist" ]; then\
       		ls -1 *.c *.h > clist ;\
	fi

doxy_doc: *.c *.h
	doxygen doxygen.conf
clean:
	rm -f *.o .depend pspdftool pspdftool.1.gz clist
    
install:all
	[ -d "${DESTDIR}" ] || mkdir -p ${DESTDIR} > /dev/null
	[ -d "${mandir}/man1" ] || mkdir -p ${mandir}/man1
	${INSTALL} -m 755  pspdftool ${DESTDIR}
	gzip -c pspdftool.1 > pspdftool.1.gz
	${INSTALL} -m 644  pspdftool.1.gz ${mandir}/man1

uninstall:
	rm -f ${DESTDIR}/pspdftool
	rm -f ${PREFIX}/share/man/man1/pspdftool.1.gz
help:
	@echo "$(PROJECT) - $(PROJECT_DESC)"
	@echo
	@echo "Makefile targets:"
	@echo "all      : Build everything"
	@echo "help     : This little text"
	@echo "install  : Build & Install into ${DESTDIR}/"
	@echo "clean    : Clean the dirs to be pristine in bondage"

ifeq (.depend, $(wildcard, .depend))
include .depend
endif
