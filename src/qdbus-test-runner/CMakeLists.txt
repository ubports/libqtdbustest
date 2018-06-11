
add_executable(
	qdbus-simple-test-runner
	qdbus-simple-test-runner.cpp
)

target_link_libraries(
	qdbus-simple-test-runner
	qtdbustest
)

target_link_libraries(
	qdbus-simple-test-runner
	Qt5::Core
)

install(
	TARGETS qdbus-simple-test-runner
	RUNTIME DESTINATION "bin"
)
