cmake_minimum_required(VERSION 3.8)

# https://cmake.org/cmake/help/latest/command/project.html
# set(DISABLE_CXX_INIT "")
if (LIQUIDFUN_SIMD_NEON)
	# set(DISABLE_CXX_INIT "")
	# https://stackoverflow.com/a/71041529/5257399
	# works together with enable_language() below, to ensure C++ support 
	# is only enabled once CMAKE_CXX_SOURCE_FILE_EXTENSIONS has been updated
	project(box2d VERSION 2.4.1 LANGUAGES NONE)
else()
	project(box2d VERSION 2.4.1)
endif()

# set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo" CACHE STRING "" FORCE)

function(add_neon_extension variable access value current_list_file stack)
  if (NOT s IN_LIST value)
    list(PREPEND "${variable}" "s")
    set("${variable}" "${${variable}}" PARENT_SCOPE)
  endif ()
endfunction()

if (LIQUIDFUN_SIMD_NEON)
	# check_language(CXX)
	# list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS s)
	# set_source_files_properties(file.nonstandard_extention PROPERTIES LANGUAGE CXX)
	variable_watch(CMAKE_CXX_SOURCE_FILE_EXTENSIONS add_neon_extension)
  enable_language(CXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=softfp -mfpu=neon")
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon")
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

option(BOX2D_BUILD_UNIT_TESTS "Build the Box2D unit tests" ON)
option(BOX2D_BUILD_TESTBED "Build the Box2D testbed" ON)
option(BOX2D_BUILD_DOCS "Build the Box2D documentation" OFF)
option(BOX2D_USER_SETTINGS "Override Box2D settings with b2UserSettings.h" OFF)

option(BUILD_SHARED_LIBS "Build Box2D as a shared library" OFF)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

include(GNUInstallDirs)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (BOX2D_USER_SETTINGS)
	add_compile_definitions(B2_USER_SETTINGS)
endif()

add_subdirectory(src)

if (BOX2D_BUILD_DOCS)
    set(DOXYGEN_SKIP_DOT TRUE)
    find_package(Doxygen)
endif()

if (DOXYGEN_FOUND AND BOX2D_BUILD_DOCS)
    add_subdirectory(docs)
endif()

if (BOX2D_BUILD_UNIT_TESTS)
	add_subdirectory(unit-test)
endif()

if (BOX2D_BUILD_TESTBED)
	add_subdirectory(extern/glad)
	add_subdirectory(extern/glfw)
	add_subdirectory(extern/imgui)
	add_subdirectory(extern/sajson)
	add_subdirectory(testbed)

	# default startup project for Visual Studio
	if (MSVC)
		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT testbed)
		set_property(TARGET testbed PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/testbed")
	endif()
endif()

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/box2d"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
