SRCZ = blackhole.cpp \
       crossfader.cpp \
       oggfiledecoder.cpp \
       pitch.cpp \
       silencer.cpp \
       volume.cpp \
       delayextrastereo.cpp \
       demultiplexer.cpp \
       flanger.cpp \
       fragmenter.cpp \
       datapicker.cpp \
       bandfilter.cpp \
       whitenoise.cpp

INCZ = izsound/blackhole.h \
       izsound/crossfader.h \
       izsound/oggfiledecoder.h \
       izsound/pitch.h \
       izsound/silencer.h \
       izsound/volume.h \
       izsound/delayextrastereo.h \
       izsound/demultiplexer.h \
       izsound/flanger.h \
       izsound/fragmenter.h \
       izsound/datapicker.h \
       izsound/bandfilter.h \
       izsound/whitenoise.h

LIBZ = ${top_builddir}/src/izsound/libizsound.la \
       @VORBIS_LIBS@ \
       @OGG_LIBS@ \
       @VORBISFILE_LIBS@

FLAGZ = ${CPPFLAGS} @VORBIS_CFLAGS@ @OGG_CFLAGS@
INCLUDES = -I${top_srcdir}/src/izsound/izsound \
           -Iizsound
           ${all_includes}

if USE_LIBAO
  SRCZ  += libaooutput.cpp
  INCZ  += izsound/libaooutput.h
  LIBZ  += @AO_LIBS@
  FLAGZ += @AO_CFLAGS@
endif

if MAKE_OSS_DSP
  SRCZ += ossoutput.cpp
  INCZ += izsound/ossoutput.h
endif

if USE_LIBMAD
  SRCZ += maddecoder.cpp
  INCZ += izsound/maddecoder.h
endif

lib_LTLIBRARIES = libstdizdsp.la
libstdizdsp_la_LDFLAGS  = -version-info @SHARED_VERSION@
libstdizdsp_la_SOURCES  = ${SRCZ}
nobase_include_HEADERS  = ${INCZ}
libstdizdsp_la_LIBADD   = ${LIBZ}
libstdizdsp_la_CPPFLAGS = ${FLAGZ}
