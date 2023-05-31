# set minimum required cmake version
#cmake_minimum_required (VERSION 3.7 FATAL_ERROR)
cmake_minimum_required (VERSION 3.2 FATAL_ERROR)

# make sure we are not going to build in the main source directory
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

#-------------------------------------------------------------------------------
# global settings
#-------------------------------------------------------------------------------

# set project name and version
set(BDecayLLP_VERSION "1.0.0")
if(CMAKE_VERSION VERSION_LESS "3.12.0")
	project (BDecayLLP VERSION "${BDecayLLP_VERSION}"
					LANGUAGES CXX)
else()
	project (BDecayLLP VERSION "${BDecayLLP_VERSION}"
					DESCRIPTION "Pythia 8 generator for LLP in B decays"
					LANGUAGES CXX)
endif()

include(GNUInstallDirs)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/")


# set the path where to put the generated libraries and executables
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/lib)

# option to enable/disable the tests
option (ENABLE_TESTS "Enable BDecayLLP unit tests" OFF)
option (VERBOSE_DEBUG "Turn on verbose debugging" OFF)
option (SANITIZE "Turn on sanitizers" OFF)
set(MAX_CXX_STANDARD "20" CACHE STRING "Caps the C++ dialect to a specific one. Overrides default behavior which is to use most recent supported or the one used to compile ROOT if WITH_ROOT is ON.")
set_property(CACHE MAX_CXX_STANDARD PROPERTY STRINGS "14" "17" "20")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(SANITIZE AND NOT VERBOSE_DEBUG)
	set(VERBOSE_DEBUG ON CACHE BOOL "" FORCE)
endif()

if(SANITIZE_ADDRESS OR SANITIZE_MEMORY OR SANITIZE_THREAD OR SANITIZE_UNDEFINED)
	set(SANITIZE ON CACHE BOOL "" FORCE)
endif()

if(SANITIZE)
	set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/Sanitizers" ${CMAKE_MODULE_PATH})
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(SANITIZE_LINK_STATIC ON CACHE BOOL "Try to link static against sanitizers." FORCE)
	endif()
	find_package(Sanitizers)
    if (NOT (SANITIZE_ADDRESS OR SANITIZE_MEMORY OR SANITIZE_THREAD OR
        SANITIZE_UNDEFINED))
		set(SANITIZE OFF CACHE BOOL "" FORCE)	
	endif ()
endif()

if(VERBOSE_DEBUG AND NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	message(STATUS "Verbose debugging turned on: setting build type to 'Debug'")
	set(CMAKE_BUILD_TYPE "Debug")
endif()

include(CompilerChecks)

#check for Pythia8
find_package(Pythia8)
if(NOT Pythia8_FOUND)
    message(FATAL_ERROR "Pythia8 not found. You should install it before installing BDecayLLP.")
else()
	if(${Pythia8_VERSION_STRING} VERSION_GREATER "8.300")
		set(HAVE_PY83 TRUE)
	endif()
	add_library(ThirdParty::Pythia8 INTERFACE IMPORTED)
    set_property(TARGET ThirdParty::Pythia8 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Pythia8_INCLUDE_DIR})
    set_property(TARGET ThirdParty::Pythia8 PROPERTY INTERFACE_LINK_LIBRARIES ${Pythia8_LIBRARIES})
endif()

#check for HepMC

find_package(HepMC)
if(NOT HepMC_FOUND)
	message(FATAL_ERROR "HepMC not found. You should install it before installing BDecayLLP.")
else()
	add_library(ThirdParty::HepMC INTERFACE IMPORTED)
	set_property(TARGET ThirdParty::HepMC PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${HepMC_INCLUDE_DIRS})
	set_property(TARGET ThirdParty::HepMC PROPERTY INTERFACE_LINK_LIBRARIES ${HepMC_LIBRARIES})
endif()

add_subdirectory(include)
add_subdirectory(src)

# test support
if (ENABLE_TESTS)
	ENABLE_TESTING()
	add_subdirectory(Tests)
endif()


## CPack configuration
include (InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")
set(CPACK_PACKAGE_VERSION_MAJOR "${BDecayLLP_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${BDecayLLP_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${BDecayLLP_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The BDecayLLP Package")
set(CPACK_PACKAGE_VENDOR "Michele Papucci")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_GENERATOR TGZ)
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES "\.gitignore"
							  "\.astylerc"
							  "/\.git/"
                              "/build/"
							  "/devel/"
							  "/python/"
							  "/master/"
							  "/sandbox/"
							  "/other/"
							  "^${PROJECT_SOURCE_DIR}/internal/"
							  "^${PROJECT_SOURCE_DIR}/Talks/"
							  "/\.vscode/"
							  "\.clang-format"
							  "\.DS_Store")
include (CPack)
