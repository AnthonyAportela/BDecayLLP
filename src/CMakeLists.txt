message (STATUS "Preparing BDecay-LLP executable.")

add_executable (BDecayLLP
			 ./BDecay_LLP.cc
			 )

target_include_directories(BDecayLLP PRIVATE ${PROJECT_SOURCE_DIR}/include
											 ${PROJECT_BINARY_DIR}/include)
target_compile_options(BDecayLLP PRIVATE ${BDecayLLP_CompileOptions})
if(VERBOSE_DEBUG)
	target_compile_options(BDecayLLP PRIVATE ${BDecayLLP_VerboseOptions})
endif()
if(SANITIZE)
	target_compile_options(BDecayLLP PRIVATE ${BDecayLLP_SanitizeOptions})
endif()
target_link_libraries(BDecayLLP PUBLIC ThirdParty::HepMC ThirdParty::Pythia8)

if(SANITIZE)
	add_sanitizers(BDecayLLP)
endif()

install (TARGETS BDecayLLP DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

