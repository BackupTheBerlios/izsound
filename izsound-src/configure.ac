# Initialisations
AC_INIT(IzSound, 0.1.0, julien@izforge.com)
AC_CONFIG_SRCDIR(src/Makefile.am)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_SANITY_CHECK

# Libtool shared versioning (current:revision:age)
# current  -> bumped when the interfaces change (revision and age go to 0)
# revision -> bumped on each release if the interfaces are stable
# age      -> bumped on each release if the interfaces are stable and new
#             interfaces have been added
#
# We start at 0:2:0 (as of the 0.2.0 release)
SHARED_VERSION="0:2:0"
AC_SUBST(SHARED_VERSION)

# Checks for programs.
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_RANLIB
AC_PROG_MAKE_SET

# Checks for libraries.
XIPH_PATH_OGG([], AC_MSG_ERROR([Ogg libraries not found.]))
XIPH_PATH_VORBIS([], AC_MSG_ERROR([Vorbis libraries not found.]))

# LibAO library
AC_ARG_ENABLE(libao_dsp,
              AC_HELP_STRING([--disable-libao_dsp],
                             [Don't use the libao features]),
              [case ${enableval} in
                yes) uselibao=true;;
                no)  uselibao=false;;
                *) AC_MSG_ERROR([bad value ${enableval} for --disable-libao_dsp]);;
               esac],
              [uselibao=true])
if test x${uselibao} = xtrue; then
  XIPH_PATH_AO([], AC_MSG_ERROR([libao not found.]))
fi
AM_CONDITIONAL(USE_LIBAO, test x${uselibao} = xtrue)

# LibMad library
AC_ARG_ENABLE(libmad_dsp,
              AC_HELP_STRING([--disable-libmad_dsp],
                             [Don't use the libmad features]),
              [case ${enableval} in
                yes) uselibmad=true;;
                no)  uselibmad=false;;
                *) AC_MSG_ERROR([bad value ${enableval} for --disable-libmad_dsp]);;
               esac],
              [uselibmad=true])
if test x${uselibmad} = xtrue; then
  AC_CHECK_HEADER([mad.h], [], AC_MSG_ERROR([mad.h not found.]))
  AC_CHECK_LIB([mad], [main], [], AC_MSG_ERROR([libmad not found.]))
fi
AM_CONDITIONAL(USE_LIBMAD, test x${uselibmad} = xtrue)

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_CHECK_HEADERS(sys/stat.h sys/fcntl.h sys/ioctl.h sys/types.h sys/soundcard.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_PROG_CXX
AC_C_CONST
AC_C_INLINE
AC_C_BIGENDIAN

# Checks for library functions.

# Doxygen documentation generation
AC_ARG_ENABLE(doxydoc,
              AC_HELP_STRING([--enable-doxydoc],
                             [Build the Doxygen classes documentation]),
              [case ${enableval} in
                yes) usedoxy=true;;
                no)  usedoxy=false;;
                *) AC_MSG_ERROR([bad value ${enableval} for --enable-doxydoc]);;
               esac],
               [usedoxy=false])
if test x${usedoxy} = xtrue; then
  AC_PATH_PROG(DOXYGEN, doxygen, none)
  AM_CONDITIONAL(BUILD_DOXY_DOC, test x${DOXYGEN} != xnone)
else
  AM_CONDITIONAL(BUILD_DOXY_DOC, test 1 = 2)
fi

# OSS DSP building conditional
AC_ARG_ENABLE(oss_dsp,
              AC_HELP_STRING([--disable-oss_dsp], [Don't build an oss dsp]),
              [case ${enableval} in
                yes) makeoss=true;;
                no)  makeoss=false;;
                *) AC_MSG_ERROR([bad value ${enableval} for --disable-oss_dsp]);;
               esac],
               [makeoss=true])
AM_CONDITIONAL(MAKE_OSS_DSP, test x${makeoss} = xtrue)

# Test program building conditional
AC_ARG_ENABLE(testprog,
              AC_HELP_STRING([--enable-testprog],
                             [Build the tests program]),
              [case ${enableval} in
                yes) maketestprog=true;;
                no)  maketestprog=false;;
                *) AC_MSG_ERROR([bad value ${enableval} for --enable-testprog]);;
               esac],
               [maketestprog=false])
AM_CONDITIONAL(MAKE_TEST_PROG, test x${maketestprog} = xtrue)

# Debug informations generation
AC_ARG_ENABLE(debug,
              AC_HELP_STRING([--enable-debug],
                             [Turn on debugging informations generation]),
              [case ${enableval} in
               yes) debug=true;;
               no)  debug=false;;
               *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]);;
              esac],
              [debug=false])
if test x${debug} = xtrue; then
  DFLAGS='-Wall -g -O0 -pipe -fno-rtti -fno-exceptions'
else
  DFLAGS=`echo ${CXXFLAGS} | sed -e s/-g//g`
  DFLAGS="${DFLAGS} -Wall -pipe -fomit-frame-pointer -fno-rtti -fno-exceptions"
fi
AC_SUBST(CFLAGS, ${DFLAGS})
AC_SUBST(CXXFLAGS, ${DFLAGS})

# Output
AC_CONFIG_HEADERS(src/config.h)
AC_CONFIG_FILES(Makefile \
                src/Makefile \
                src/izsound/Makefile \
                src/std-dsp/Makefile \
                doc/Makefile \
                doc/classes/Makefile \
                doc/manual/Makefile \
                doc/manual/images/Makefile \
                tests/Makefile)
AC_OUTPUT

