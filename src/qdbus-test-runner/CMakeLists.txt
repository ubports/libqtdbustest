
add_executable(
	qdbus-simple-test-runner
	qdbus-simple-test-runner.cpp
)

target_link_libraries(
	qdbus-simple-test-runner
	qtdbustest
)

qt5_use_modules(
	qdbus-simple-test-runner
	Core
)

install(
	TARGETS qdbus-simple-test-runner
	RUNTIME DESTINATION "bin"
)
