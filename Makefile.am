# Don't bother us with missing files :-T
AUTOMAKE_OPTIONS = foreign

man_MANS = pspdftool.1x

bin_PROGRAMS = pspdftool

AM_CFLAGS = @DEBUG@ -std=gnu99

doc_DATA = AUTHORS README INSTALL COPYING @DEVEL@

doxy_doc:
	@DOXYGEN@

pspdftool_SOURCES = cmdexec.c cmdexec.h common.c common.h config.h             \
	doc_limits.h doc_type.h dummydoc.c dummydoc.h fileio.c fileio.h        \
	magic.h main.c main_page.c page_list.c page_list.h pdf_doc.c pdf_doc.h \
	pdf_filters.c pdf_filters.h pdf_lib.c pdf_lib.h pdf_parser.c           \
	pdf_parser.h pdf_type.h ps_doc.c ps_doc.h ps_lib.c ps_lib.h ps_type.h  \
	vdoc.c vdocerror.c vdocerror.h vdoc.h vldoc.c vldoc.h

