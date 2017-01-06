
set(CMAKE_AUTOMOC OFF)
find_package(GMock)
set(CMAKE_AUTOMOC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

add_definitions(-DTEST_DBUS_SESSION_CONFIG_FILE="${DATA_DIR}/session.conf")
add_definitions(-DTEST_DBUS_SYSTEM_CONFIG_FILE="${DATA_DIR}/system.conf")
add_definitions(-DTEST_QTDBUSTEST_WATCHDOG_BIN="${CMAKE_BINARY_DIR}/src/watchdog/watchdog")

set(
	UNIT_TESTS_SRC
	main.cpp
	libqtdbustest/TestDBusTestRunner.cpp
	libqtdbustest/TestQProcessDBusService.cpp
	libqtdbustest/TestSuicidalProcess.cpp
)

add_executable(
	unit-tests
	${UNIT_TESTS_SRC}
)

qt5_use_modules(
	unit-tests
	Core
	DBus
	Test
)

target_link_libraries(
	unit-tests
	qtdbustest
	${GTEST_LIBRARIES}
	${GMOCK_LIBRARIES}
)

add_test(
	unit-tests
	unit-tests
)

