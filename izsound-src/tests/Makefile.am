if MAKE_TEST_PROG

INCLUDES = ${all_includes} \
           -I${top_srcdir}/src/izsound/izsound \
           -I${top_srcdir}/src/std-dsp/izsound
bin_PROGRAMS = testizsound
testizsound_SOURCES = main.cpp
testizsound_CPPFLAGS = @AO_CFLAGS@ @OGG_CFLAGS@ @VORBIS_CFLAGS@
testizsound_LDADD = ${all_libraries} @AO_LIBS@ @OGG_LIBS@ @VORBIS_LIBS@ \
                    ${top_builddir}/src/izsound/libizsound.la \
                    ${top_builddir}/src/std-dsp/libstdizdsp.la

endif
