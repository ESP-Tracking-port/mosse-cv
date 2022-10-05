project(mosse)
option(MOSSE_PORTABLE "Only include libraries that can be ported on an embedded platform" OFF)
option(MOSSE_PORT_USE_CASSERT "Use <cassert> from the standard library" ON)
set(MOSSE_ARROGANT_HOLD_FRAMES 0 CACHE STRING
	"Expenses on context switching may exceed the benefits from using multiple
	cores. Set this option to anything different from 0. When enabled, a
	threaded SIMD matrix ops. thread will not release control for
	`MOSSE_ARROGANT_HOLD_FRAMES` number of frames. Establish the optimal value
	experimentally, but do not set it too high, as it may cause priority
	inversion-like situation and trigger a watchdog timer.")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
cmake_minimum_required(VERSION 2.8)
add_subdirectory(lib)
add_subdirectory(embmosse)

if (NOT MOSSE_PORTABLE)
	message(STATUS "Building non portable version of embmosse")
	# Executable
	find_package(OpenCV REQUIRED PATHS /home/dmurashov/bin/libopencv/lib/cmake NO_DEFAULT_PATH)
	include_directories(src)
	FILE(GLOB_RECURSE sourcefiles "src/*.cpp")
	add_executable( mosse ${sourcefiles} )
	target_link_libraries( mosse ${OpenCV_LIBS} mosselibs embmosse)
	target_compile_options(mosse PRIVATE -g)
	file(GLOB SRCTEST "test/*.cpp")
	# Tests
	add_executable(mossetest ${SRCTEST})
	target_link_libraries(mossetest PRIVATE embmosse ${OpenCV_LIBS})
	target_compile_options(mossetest PRIVATE "-g")
endif()
