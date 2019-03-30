cmake_minimum_required(VERSION 3.4.3)
project (cmake-test)
include(CMakeToolsHelpers OPTIONAL) # Used by the CMake Tools extension in VSCode

# Options
option(BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
option(BUILD_TESTS "Build Tests" OFF)

# Set all output to the binary directory for easy runtime linking
# Also set the lib type
if(BUILD_SHARED_LIBS)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE) # Automatically export all symbols in libraries
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
else()
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# MSVC build fixes
if(MSVC)
	# Force static/dynamic linking to be followed by MSVC
	set(CompilerFlags
		CMAKE_C_FLAGS_DEBUG
		CMAKE_C_FLAGS_MINSIZEREL
		CMAKE_C_FLAGS_RELEASE
		CMAKE_C_FLAGS_RELWITHDEBINFO
		CMAKE_CXX_FLAGS_DEBUG
		CMAKE_CXX_FLAGS_MINSIZEREL
		CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_RELWITHDEBINFO)
	foreach(CompilerFlag ${CompilerFlags})
		if(BUILD_SHARED_LIBS)
			string(REPLACE "/MT" "/MD" ${CompilerFlag} "${${CompilerFlag}}")
		else()
			string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
		endif()
	endforeach()
endif()

# Add src folder
add_subdirectory(src)

if(BUILD_TESTS)
	add_subdirectory(tests)
endif()
