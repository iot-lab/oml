#
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([oml2],
	m4_esyscmd([build-aux/git-version-gen .tarball-version]),
	[oml-user@lists.nicta.com.au])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])
# automake 1.12 seems to require this, but automake 1.11 doesn't recognize it
m4_pattern_allow([AM_PROG_AR])
AM_PROG_AR

LT_INIT

AC_PROG_CXX
AC_PROG_CC
gl_EARLY
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_MAKE_SET

gl_INIT

OML_CHECK_MACOSX
AS_IF([test "x$is_macosx" = "xyes" ],
      AC_DEFINE([MSG_NOSIGNAL], [SO_NOSIGPIPE], [Define to system's equivalent to disable SIGPIPE when writing to unconnected sockets.])
      )

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h malloc.h netdb.h netinet/in.h stdlib.h string.h strings.h sys/ioctl.h sys/socket.h sys/time.h sys/timeb.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gethostbyname gettimeofday inet_ntoa memmove memset socket strerror])

AC_C_BIGENDIAN

# Checks for libraries.
missing_libs=""

AC_SUBST([OML_BASE_VER], [`echo "$PACKAGE_VERSION" | sed 's/^\([[0-9]]\+\.[[0-9]]\+\).*$/\1/'`])
AC_SUBST([OML_PKG_VER], [`echo "$PACKAGE_VERSION" | sed 's/^\([[0-9]]\+\(\.[[0-9]]\+\)\{1,2\}\)\([[-.a-zA-Z0-9]]\+\)\?.*$/\1~\3/;s/-/./g;s/~$//'`])

LIBOML2_LT_VER_CUR=10
LIBOML2_LT_VER_REV=1 # Revisions of LIBOML2_LT_VER_CUR
LIBOML2_LT_VER_AGE=1 # Supported APIs prior to LIBOML2_LT_VER_CUR
LIBOML2_LT_VER_MIN=$(($LIBOML2_LT_VER_CUR - $LIBOML2_LT_VER_AGE))
LIBOCOMM_LT_VER_CUR=1
LIBOCOMM_LT_VER_REV=1
LIBOCOMM_LT_VER_AGE=0
LIBOCOMM_LT_VER_MIN=$(($LIBOCOMM_LT_VER_CUR - $LIBOCOMM_LT_VER_AGE))
AC_SUBST([LIBOML2_LT_VER_CUR], [$LIBOML2_LT_VER_CUR])
AC_SUBST([LIBOML2_LT_VER_REV], [$LIBOML2_LT_VER_REV])
AC_SUBST([LIBOML2_LT_VER_AGE], [$LIBOML2_LT_VER_AGE])
AC_SUBST([LIBOML2_LT_VER_MIN], [$LIBOML2_LT_VER_MIN])
AC_SUBST([LIBOML2_LT_VER],
	 [$LIBOML2_LT_VER_CUR:$LIBOML2_LT_VER_REV:$LIBOML2_LT_VER_AGE]) dnl cur[:rev[:age]]
AC_SUBST([LIBOCOMM_LT_VER_CUR], [$LIBOCOMM_LT_VER_CUR])
AC_SUBST([LIBOCOMM_LT_VER_REV], [$LIBOCOMM_LT_VER_REV])
AC_SUBST([LIBOCOMM_LT_VER_AGE], [$LIBOCOMM_LT_VER_AGE])
AC_SUBST([LIBOCOMM_LT_VER_MIN], [$LIBOCOMM_LT_VER_MIN])
AC_SUBST([LIBOCOMM_LT_VER],
	 [$LIBOCOMM_LT_VER_CUR:$LIBOCOMM_LT_VER_REV:$LIBOCOMM_LT_VER_AGE])

AC_SUBST([pkglocalstatedir], [${localstatedir}/${PACKAGE}])

# Check for presence of libraries, but don't add them to the default LIBS,
# rather add create new Makefile variables xxx_LIBS
oldLIBS=$LIBS
AC_SEARCH_LIBS([sqrt], [m],[
		AS_IF([test "$ac_res" != "none required"],[M_LIBS="$M_LIBS $ac_res"])
	       ])
AC_SEARCH_LIBS([frexp], [m], [
		AS_IF([test "$ac_res" != "none required"],[M_LIBS="$M_LIBS $ac_res"])
	       ])
AC_SEARCH_LIBS([log10], [m], [
		AS_IF([test "$ac_res" != "none required"],[M_LIBS="$M_LIBS $ac_res"])
	       ])
AC_SEARCH_LIBS([floor], [m], [
		AS_IF([test "$ac_res" != "none required"],[M_LIBS="$M_LIBS $ac_res"])
	       ])
AS_IF([test "$LIBS" != "$oldLIBS"], [AC_SUBST([M_LIBS], $M_LIBS)])
LIBS=$oldLIBS
AC_SEARCH_LIBS([pthread_create], [pthread], [
		AC_DEFINE([HAVE_LIBPTHREAD], [1], [Define if libpthread is installed.])
		AS_IF([test "$LIBS" != "$oldLIBS"], [AC_SUBST([PTHREAD_LIBS], $ac_res)])
	       ], [missing_libs+=" libpthread"])
LIBS=$oldLIBS
AC_SEARCH_LIBS([poptGetContext], [popt], [
		AC_DEFINE([HAVE_LIBPOPT], [1], [Define if libpopt is installed.])
		AS_IF([test "$LIBS" != "$oldLIBS"], [AC_SUBST([POPT_LIBS], $ac_res)])
	       ], [missing_libs+=" libpopt"])
LIBS=$oldLIBS

# Check that libxml2 is installed, and work out how to compile/link against it
AC_SEARCH_LIBS([xmlParseFile], [xml2], [
		AC_DEFINE([HAVE_LIBXML2], [1], [Define if libxml2 is installed.])
		AS_IF([test "$LIBS" != "$oldLIBS"], [AC_SUBST([XML2_LIBS], $ac_res)])

		AS_IF([test x$cross_compiling = xno], [
			AC_PATH_PROG([HAVE_XML2CFG], [xml2-config])
			AS_IF([test "x$HAVE_XML2CFG" != "x" ],
				[
				xxCFLAGS=`$HAVE_XML2CFG --cflags`
				AC_MSG_CHECKING([how to include libxml2 headers])
				AC_MSG_RESULT([$xxCFLAGS])
				]
			     )
			], [
				xxCFLAGS="-I=/usr/include/libxml2"
				AC_MSG_WARN([skipping xml2-config; result '$xxCFLAGS' guessed because of cross compilation; override with 'CFLAGS=-I...'])
			]
		     )
		# XXX xml2-config doesn't make a difference between CFLAGS and CPPFLAGS
		oldCFLAGS=$CFLAGS
		oldCPPFLAGS=$CPPFLAGS
		CFLAGS="$CFLAGS $xxCFLAGS"
		CPPFLAGS="$CPPFLAGS $xxCFLAGS"
		AC_CHECK_HEADER([libxml/tree.h],
				[AC_SUBST([XML2_CFLAGS], [$xxCFLAGS])],
				[missing_libs+=" libxml2"])
		CPPFLAGS=$oldCPPFLAGS
		CFLAGS=$oldCFLAGS
	       ], [missing_libs+=" libxml2"])

AC_SEARCH_LIBS([sqlite3_open], [sqlite3 sqlite], [
		AC_DEFINE([HAVE_LIBSQLITE3], [1], [Define if libsqlite3 is installed.])
		AS_IF([test "$LIBS" != "$oldLIBS"], [AC_SUBST([SQLITE3_LIBS], $ac_res)])
		AC_CHECK_HEADER([sqlite3.h], [], [missing_libs+=" libsqlite3"])
	       ], [missing_libs+=" libsqlite3"])
LIBS=$oldLIBS


# Detect if PostgreSQL is around, but don't add it to the default
oldLIBS=$LIBS
AX_CHECK_POSTGRES_DB([],
		     [AS_IF([test "$pg_prefix" != ""],[missing_libs+=" libpq"])])
AX_WITH_PROG([POSTGRES],[postgres],[no])
AM_CONDITIONAL([HAVE_LIBPQ], [test x$HAVE_LIBPQ = xyes])
AM_CONDITIONAL([HAVE_POSTGRES], [test x$POSTGRES != xno])
LIBS=$oldLIBS

AC_ARG_ENABLE([debug],
	      [AS_HELP_STRING([--enable-debug],
			      [enable verbose debug code in time-sensitive parts of the code])],
	      [AC_DEFINE([DEBUG], [1],
			 [Define if verbose debug code in time-sensitive parts of the code should be enabled.])])

AC_ARG_ENABLE([packaging],
	      [AS_HELP_STRING([--enable-packaging],
			      [enable targets to create distribution-specific packages (Git clone needed)])],
	      [
	       AX_WITH_PROG([GIT],[git])
	       AS_IF([test "x$enable_packaging" != "xno" && test -n "$GIT"],
		     [
		      AC_MSG_CHECKING([whether \$srcdir is a Git repo])
		      AS_IF([test "x${srcdir}" = "x." && $GIT status >/dev/null 2>&1],
			    [
			     AC_MSG_RESULT([yes])
			     AC_MSG_CHECKING([current Git branch])
			     gitbranch=`${GIT} describe --contains --all HEAD`
			     AC_MSG_RESULT([$gitbranch])

			     AC_MSG_CHECKING([current Git tag])
			     gittag=`${GIT} describe --exact-match --match v\* 2>/dev/null`
			     AS_IF([ test "x" != "x$gittag"],
				   [
				    AC_MSG_RESULT([$gittag])
				    gitlasttag=$gittag
				    gitistag=yes
				   ],
				   [
				    gitlasttag=`${GIT} describe --tags --abbrev=0 --match v\* HEAD`
				    AC_MSG_RESULT([none, latest tag is $gitlasttag])
				    gitistag=no
				   ])

			    AC_ARG_ENABLE([android],
					  [AS_HELP_STRING([--disable-android],
							  [do not prepare Android source])],
							  [],
							  [enable_android=yes])
			    AS_IF([test "x$enable_android" == "xyes"],
				   OML_GIT_SEARCH_TREEISH([Android],
							 [gitandref], [$gitistag],
							 [android/$gittag,
							  origin/android/$gittag,
							  android/$gitbranch,
							  origin/android/$gitbranch,
							  android/master],
							  [origin/android/master])
				   )

			     AX_WITH_PROG([MAKEPKG],[makepkg])
			     AS_IF([test "x$MAKEPKG" != "x"],[
				    OML_DISTRIBUTION_IS([ArchLinux],
							[/etc/arch-release],
							[isarch],
							[],
							[AC_SUBST([MAKEPKG_FLAGS_ADD],[--nodeps])])
				    OML_GIT_SEARCH_TREEISH([ArchLinux],
							  [gitarchref], [$gitistag],
							  [archlinux/$gittag,
							   origin/archlinux/$gittag,
							   archlinux/$gitbranch,
							   origin/archlinux/$gitbranch,
							   archlinux/master],
							   [origin/archlinux/master])
				   ])
			     AX_WITH_PROG([DPKG_BUILDPACKAGE],[dpkg-buildpackage])
			     AX_WITH_PROG([DCH],[dch])
			     AS_IF([test "x$DPKG_BUILDPACKAGE" != "x" -a "x$DCH" != "x"],[
				    OML_DISTRIBUTION_IS([Debian-based],
							[/etc/debian_version],
							[isdebian],
							[],
							[AC_SUBST([DPKG_BUILDPACKAGE_FLAGS_ADD],[-d])])
				    OML_GIT_SEARCH_TREEISH([Debian],
							  [gitdebref], [$gitistag],
							  [debian/$gittag,
							   origin/debian/$gittag,
							   debian/$gitbranch,
							   origin/debian/$gitbranch,
							   debian/master],
							   [origin/debian/master])
				   ])
			     AX_WITH_PROG([RPMBUILD],[rpmbuild])
			     AS_IF([test "x$RPMBUILD" != "x"], [
				    OML_DISTRIBUTION_IS([RedHat-based],
							[/etc/redhat-release],
							[isredhat],
							[],
							[AC_SUBST([RPMBUILD_FLAGS_ADD],[--nodeps])])
				    OML_GIT_SEARCH_TREEISH([RPM],
							  [gitrpmref], [$gitistag],
							  [rpm/$gittag,
							   origin/rpm/$gittag,
							   rpm/$gitbranch,
							   origin/rpm/$gitbranch,
							   rpm/master],
							   [origin/rpm/master])
				   ])
			    ],
			    [
			     AC_MSG_RESULT([no])
			     AC_MSG_WARN([--enable-packaging needs git and a Git repo for binary distributions])
			     enable_packaging=partial
			    ])
		      ])],
	      [enable_packaging=no])
AC_SUBST([GITBRANCH],[$gitbranch])
AC_SUBST([GITTAG],[$gittag])
AC_SUBST([GITLASTTAG],[$gitlasttag])
AC_SUBST([GITHEAD],[$githead])
AC_SUBST([GITANDREF],[$gitandref])
AC_SUBST([GITARCHREF],[$gitarchref])
AC_SUBST([GITDEBREF],[$gitdebref])
AC_SUBST([GITRPMREF],[$gitrpmref])
AM_CONDITIONAL([ENABLE_PACKAGING], [test "x$enable_packaging" != "xno" && test "x$enable_packaging" != "xpartial"])
AM_CONDITIONAL([GITISTAG], [test "x$gitistag" != "xno"])
AM_CONDITIONAL([BUILD_ANDROID], [test "x$enable_android" == "xyes"])
AM_CONDITIONAL([BUILD_ARCH], [test "x$MAKEPKG" != "x"])
AM_CONDITIONAL([BUILD_DEBIAN], [test "x$DPKG_BUILDPACKAGE" != "x" -a "x$DCH" != "x"])
AM_CONDITIONAL([BUILD_RPM], [test "x$RPMBUILD" != "x"])
AM_CONDITIONAL([IS_ARCH], [test "x$isarch" != "x"])
AM_CONDITIONAL([IS_DEBIAN], [test "x$isdebian" != "x"])
AM_CONDITIONAL([IS_REDHAT], [test "x$isredhat" != "x"])

AX_WITH_PROG([PYTHON],[python])
AS_IF([test x"$PYTHON" = x], [missing_python=yes])
AM_CONDITIONAL([HAVE_PYTHON], [test x"${missing_python}" != xyes])

AX_WITH_PROG([RUBY],[ruby])

AS_IF([test x"$RUBY" = x],
      [missing_ruby=yes
       missing_rubyge187=yes
       AC_MSG_WARN([Ruby (preferably>=1.8.7) is needed to build the oml2-server])],
       AX_PROG_RUBY_VERSION([1.8.7],
			    [
			     missing_rubyge187=no
			     ],
			     [
			      AC_MSG_WARN([No Ruby>=1.8.7 found; some tests will be skipped])
			      missing_rubyge187=yes
			      ])
       )
AM_CONDITIONAL([BUILD_SERVER], [test x"${missing_ruby}" != xyes])
AM_CONDITIONAL([HAVE_RUBY_MIN_1_8_7], [test x"${missing_rubyge187}" != xyes])
AS_IF([test "x$enable_android" = "xyes" -a x"${missing_rubyge187}" = xyes],
      [AC_MSG_WARN([The Android source package will not contain a working example-oml2, please do not distribute it])])

PKG_CHECK_MODULES([CHECK], [check >= 0.9.4], [], [missing_check="yes"])
AM_CONDITIONAL([HAVE_CHECK], [test x"${missing_check}" != xyes])
AX_WITH_PROG([TIMEOUT],[timeout])

AC_ARG_ENABLE([doc],
		[AS_HELP_STRING([--disable-doc],
				[do not build documentation (man pages, manual)])],
		[],
		[enable_doc=yes])

AS_IF([test "x$enable_doc" != "xno"],
      [
       # Check for asciidoc (for generating manpages).
       AC_PATH_PROG(A2X, a2x, no)
       AC_PATH_PROG(ASCIIDOC, asciidoc, no)
       AS_IF([test "x$A2X" = "xno"],
          [AC_MSG_WARN([Test for a2x failed. Not generating man pages (but OML build should still complete ok).])])
       AS_IF([test "x$ASCIIDOC" = "xno"],
          [AC_MSG_WARN([Test for asciidoc failed. Not generating man pages (but OML build should still complete ok).])])
      ],[
          AC_MSG_WARN([Not generating man pages (but OML build should still complete ok).])
      ])
AM_CONDITIONAL([ENABLE_DOC], [test "x$enable_doc" != "xno"])
AM_CONDITIONAL([HAVE_A2X], [test "x$A2X" != "xno"])
AM_CONDITIONAL([HAVE_ASCIIDOC], [test "x$ASCIIDOC" != "xno"])
AX_WITH_PROG([PLANTUML],[plantuml],[no])
AM_CONDITIONAL([HAVE_PLANTUML], [test "x$PLANTUML" != "xno"])

DX_DOT_FEATURE(ON)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN([oml2], [], [doxygen])
AC_ARG_ENABLE([doxygen-private],
	      [AS_HELP_STRING([--enable-doxygen-private],
			      [generate Doxygen documentation for private and static elements])],
	      [],
	      [enable_doxygen_private=no])
AS_IF([test x$enable_doxygen_private = xno],
      [
       DX_ENV_APPEND(EXTRACT_PRIVATE, [NO])
       DX_ENV_APPEND(EXTRACT_STATIC, [NO])
      ],
      [
       DX_ENV_APPEND(EXTRACT_PRIVATE, [YES])
       DX_ENV_APPEND(EXTRACT_STATIC, [YES])
      ])
AC_ARG_ENABLE([doxygen-callgraph],
	      [AS_HELP_STRING([--enable-doxygen-callgraph],
			      [generate Doxygen call graphs])],
	      [],
	      [enable_doxygen_callgraph=no])
AS_IF([test x$enable_doxygen_callgraph = xno],
      [DX_ENV_APPEND(ENABLE_CALLGRAPH, [NO])],
      [DX_ENV_APPEND(ENABLE_CALLGRAPH, [YES])])
DX_ENV_APPEND(TOPSRCDIR, [\$(top_srcdir)]) dnl let make deal with it
DX_ENV_APPEND(BUILDDIR, [\$(builddir)]) dnl ditto

AC_CONFIG_FILES([Makefile
		 gnulib/Makefile
		 lib/Makefile
		 lib/client/Makefile
		 lib/shared/Makefile
		 lib/ocomm/Makefile
		 ruby/Makefile
		 ruby/oml2-scaffold
		 server/Makefile
		 proxy_server/Makefile
		 doc/Makefile
		 example/Makefile
		 test/Makefile
		 test/lib/Makefile
		 test/server/Makefile
		 test/system/Makefile
		 ])

AS_IF([test "x$missing_check" != "x"],
   [AC_MSG_WARN([could not find the `check' package, required for running unit tests. This will not stop OML from building, but it will cause `make check' to do nothing. Install the `check' package (http://check.sourceforge.net) if you wish to run the unit tests.])])

# Let the user know we're missing required libraries, and don't generate the build system.
AS_IF([test "x$missing_libs" != "x"],
   [
    for lib in $missing_libs; do
      AC_MSG_WARN([could not find required library $lib])
   done
   AC_MSG_ERROR([missing libraries, could not continue. Please install all of the following libraries and then re-run configure (you might need the development header packages (-dev) as well if you are using your system's package manager to install them): $missing_libs])
  ])

AC_OUTPUT
