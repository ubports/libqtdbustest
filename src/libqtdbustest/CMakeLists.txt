
set(
	QTDBUSTEST_SRC
	DBusTestRunner.cpp
	DBusService.cpp
	QProcessDBusService.cpp
	SuicidalProcess.cpp
)

set(
	QTDBUSTEST_PUBLIC_HEADERS
	"${CMAKE_CURRENT_BINARY_DIR}/config.h"
	DBusTestRunner.h
	DBusService.h
	QProcessDBusService.h
	SuicidalProcess.h
)

add_library(
	qtdbustest
	SHARED
	${QTDBUSTEST_SRC}
)

qt5_use_modules(
	qtdbustest
	Core
	DBus
	Test
)

set_target_properties(
	qtdbustest
	PROPERTIES
	VERSION ${API_VERSION}.0.0
	SOVERSION ${ABI_VERSION}
	PUBLIC_HEADER "${QTDBUSTEST_PUBLIC_HEADERS}"
)

install(
	TARGETS qtdbustest
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libqtdbustest-${API_VERSION}/libqtdbustest"
)

# Package config

set(LIBQTDBUSTEST_PC "${CMAKE_CURRENT_BINARY_DIR}/libqtdbustest-${API_VERSION}.pc")
configure_file("libqtdbustest.pc.in" "${LIBQTDBUSTEST_PC}" @ONLY)

install(
	FILES "${LIBQTDBUSTEST_PC}"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)
