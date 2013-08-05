
add_executable(
	watchdog
	Watchdog.cpp
	main.cpp
)

qt5_use_modules(
	watchdog
	Core
)

install(
	TARGETS watchdog
	RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_LIBEXECDIR}/libqtdbustest"
)
