ACLOCAL_AMFLAGS = -I ../../m4 -Wnone
AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I $(top_srcdir)/lib/client \
	-I $(top_srcdir)/lib/ocomm \
	-I $(top_srcdir)/lib/shared \
	$(XML2_CFLAGS)

oml2incdir = $(includedir)/oml2

oml2inc_HEADERS = \
	oml2/omlc.h \
	oml2/oml_filter.h \
	oml2/oml_writer.h \
	oml2/oml_out_stream.h

lib_LTLIBRARIES = liboml2.la

liboml2_la_SOURCES = \
	api.c \
	client.h \
	filter.c \
	init.c \
	oml_writer.c \
	misc.c \
	text_writer.c \
	bin_writer.c \
	oml_out_stream.c \
	file_stream.c \
	file_stream.h \
	net_stream.c \
	net_stream.h \
	buffered_writer.c \
	buffered_writer.h \
	parse_config.c \
	filter/factory.c \
	filter/factory.h \
	filter/first_filter.c \
	filter/last_filter.c \
	filter/average_filter.c \
	filter/histogram_filter.c \
	filter/stddev_filter.c \
	filter/sum_filter.c \
	filter/delta_filter.c \
	filter/first_filter.h \
	filter/last_filter.h \
	filter/average_filter.h \
	filter/histogram_filter.h \
	filter/stddev_filter.h \
	filter/sum_filter.h \
	filter/delta_filter.h \
	$(oml2inc_HEADERS)

liboml2_la_LIBADD = \
		    $(top_builddir)/lib/ocomm/libocomm.la \
		    $(XML2_LIBS) $(PTHREAD_LIBS) $(M_LIBS)

liboml2_la_LDFLAGS = -version-info $(LIBOML2_LT_VER)
