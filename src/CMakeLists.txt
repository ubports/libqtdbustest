include(GenerateExportHeader)
add_compiler_export_flags()

add_subdirectory(watchdog)
add_subdirectory(libqtdbustest)
add_subdirectory(qdbus-test-runner)