message (STATUS "Preparing Include Files.")

configure_file (
 ${CMAKE_CURRENT_SOURCE_DIR}/Config.hh.in
 ${PROJECT_BINARY_DIR}/include/Config.hh
 ESCAPE_QUOTES)

