EXTRA_DIST = doxyfile

if BUILD_DOXY_DOC

BASE_DIR   = ${datadir}/doc/@PACKAGE@-@PACKAGE_VERSION@
TARGET_DIR = ${BASE_DIR}/classes

all-local: doxyfile html

html:
	@DOXYGEN@ doxyfile

install-data-local:
	[ -d ${DESTDIR}${TARGET_DIR} ] || mkdir -p ${DESTDIR}${TARGET_DIR}
	cp -R html ${DESTDIR}${TARGET_DIR}

uninstall-hook:
	rm -rf ${DESTDIR}${BASE_DIR}

clean-local:
	rm -rf html

endif
