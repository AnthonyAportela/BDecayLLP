# - Locate Pythia8 library
# by probing pythia8-config
# or in a directory defined via  Pythia8_ROOT_DIR or based on the PYTHIA8DATA environment variable
# Defines:
#
#  Pythia8_FOUND
#  Pythia8_INCLUDE_DIR
#  Pythia8_LIBRARIES
#  Pythia8_LIBRARY_DIR
#  Pythia8_XMLDOC_DIR

find_program(PYTHIA8_CONFIG_EXECUTABLE pythia8-config
  HINTS $ENV{p}/bin $ENV{PYTHIA8DATA}/../../../bin)

if(NOT PYTHIA8_CONFIG_EXECUTABLE)
  set(Pythia8_FOUND FALSE)
else()    
  set(Pythia8_FOUND TRUE)

  execute_process(
    COMMAND ${PYTHIA8_CONFIG_EXECUTABLE} --libdir 
    OUTPUT_VARIABLE Pythia8_LIBRARY_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  execute_process(
    COMMAND ${PYTHIA8_CONFIG_EXECUTABLE} --includedir
    OUTPUT_VARIABLE Pythia8_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(
        COMMAND ${PYTHIA8_CONFIG_EXECUTABLE} --libs
        OUTPUT_VARIABLE Pythia8_LIBRARIES
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(
        COMMAND ${PYTHIA8_CONFIG_EXECUTABLE} --xmldoc
        OUTPUT_VARIABLE Pythia8_XMLDOC_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

if(NOT Pythia8_FOUND)
    find_path(Pythia8_INCLUDE_DIR Pythia8/Pythia.h
          HINTS $ENV{PYTHIA8_ROOT_DIR}/include $ENV{PYTHIA8DATA}/../../../include)

    find_library(Pythia8_LIBRARIES NAMES Pythia8
            HINTS $ENV{PYTHIA8_ROOT_DIR}/lib $ENV{PYTHIA8DATA}/../../../lib
                  $ENV{PYTHIA8_ROOT_DIR}/lib64 $ENV{PYTHIA8DATA}/../../../lib64)


endif()

set(filename "${Pythia8_INCLUDE_DIR}/Pythia8/Pythia.h")
if (EXISTS ${filename})
    file(READ "${filename}" header)
    string(REGEX REPLACE ".*#[ \t]*define[ \t]*PYTHIA\_VERSION[ \t]*8\.([0-9]*).*" "\\1" match "${header}")
    if (match STREQUAL header)
        if (NOT quiet)
            message(AUTHOR_WARNING "Unable to find \#define PYTHIA_VERSION \"<version>\" from ${Pythia8_INCLUDE_DIR}/Pythia8/Pythia.h")
        endif()
    endif()
    set(Pythia8_VERSION_STRING "8.${match}")
else ()
    set(Pythia8_VERSION_STRING)
endif()
mark_as_advanced(filename header match)

if(NOT Pythia8_FOUND)
    find_path(Pythia8_XMLDOC_DIR Index.xml
              HINTS $ENV{PYTHIA8_ROOT_DIR}/share/Pythia8/xmldoc 
                    ${Pythia8_INCLUDE_DIR}/../share/Pythia8/xmldoc
                    $ENV{PYTHIA8DATA})
endif()

# handle the QUIETLY and REQUIRED arguments and set Pythia8_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Pythia8
                                  REQUIRED_VARS Pythia8_INCLUDE_DIR
                                                Pythia8_LIBRARIES
                                                Pythia8_XMLDOC_DIR
                                  VERSION_VAR Pythia8_VERSION_STRING)
