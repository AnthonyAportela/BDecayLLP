# - Locate HepMC library
# in a directory defined via  HEPMC_ROOT_DIR or HEPMC_DIR environment variable
# Defines:
#
#  HEPMC_FOUND
#  HEPMC_INCLUDE_DIR
#  HEPMC_INCLUDE_DIRS (not cached)
#  HEPMC_LIBRARIES
#  HEPMC_LIBRARY_DIR
#  HEPMC_FIO_LIBRARIES

find_path(HepMC_INCLUDE_DIR HepMC/GenEvent.h
          HINTS $ENV{HEPMC_ROOT_DIR}/include ${HepMC_ROOT_DIR}/include
          $ENV{HEPMC_DIR}/include ${HepMC_DIR}/include)

find_library(HepMC_LIBRARY NAMES HepMC
             HINTS $ENV{HEPMC_ROOT_DIR}/lib ${HepMC_ROOT_DIR}/lib
             HINTS $ENV{HEPMC_DIR}/lib ${HepMC_DIR}/lib)

find_library(HepMC_FIO_LIBRARY NAMES HepMCfio
             HINTS $ENV{HEPMC_ROOT_DIR}/lib ${HepMC_ROOT_DIR}/lib
             HINTS $ENV{HEPMC_DIR}/lib ${HepMC_DIR}/lib)


set(filename "${HepMC_INCLUDE_DIR}/HepMC/HepMCDefs.h")
if (EXISTS ${filename})
    file(READ "${filename}" header)
    string(REGEX REPLACE ".*#[ \t]*define[ \t]*HEPMC\_VERSION[ \t]*\"([^\n]*)\".*" "\\1" match "${header}")
    if (match STREQUAL header)
        if (NOT quiet)
            message(AUTHOR_WARNING "Unable to find \#define HEPMC_VERSION \"<version>\" from ${HepMC_INCLUDE_DIR}/HepMC/HepMCDefs.h")
        endif()
    endif()
    set(HepMC_VERSION_STRING "${match}")
else ()
    set(HepMC_VERSION_STRING)
endif()
mark_as_advanced(filename header match)

set(HepMC_INCLUDE_DIRS ${HepMC_INCLUDE_DIR})
set(HepMC_LIBRARIES ${HepMC_LIBRARY})

# handle the QUIETLY and REQUIRED arguments and set HEPMC_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(HepMC
                                  REQUIRED_VARS HepMC_INCLUDE_DIRS
                                                HepMC_LIBRARIES
                                  VERSION_VAR HepMC_VERSION_STRING)
