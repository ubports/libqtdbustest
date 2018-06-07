
add_executable(
	watchdog
	Watchdog.cpp
	main.cpp
)

target_link_libraries(
	watchdog
	Qt5::Core
)

install(
	TARGETS watchdog
	RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_LIBEXECDIR}/libqtdbustest"
)
