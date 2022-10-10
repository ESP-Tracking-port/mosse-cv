project(mosse)
option(MOSSE_MEM_CLONE_IMAGE_WORKING_AREA
	"Since frames prepared by a camera driver, and those processed by the
	tracking thread may reside in the same memory, copying a chunk of image
	into a separate memory chunk will resolve the resource management problem.

	If the tracker and the frame producing-thread reside on a separate physical
	cores, this will improve performance, since the latter does not have to
	wait for the buffer to get released, and may proceed with the next frame,
	while the tracker processes the previous one.
	"
	ON)
set(MOSSE_MEM_CLONE_IMAGE_WORKING_AREA_SIDE_FRACTION 4 CACHE STRING
	"The working area's size will exceed that of the ROI by a value equal to
	ROI side divided by this parameter. E.g. when the \"Fraction\" is 4, and
	the ROI side length is 64, a (64 + 64 / 4, 64 + 64 / 4) image chunk will be
	copied")
option(MOSSE_PORTABLE "Only include libraries that can be ported on an embedded platform" OFF)
option(MOSSE_PORT_USE_CASSERT "Use <cassert> from the standard library" ON)
set(MOSSE_PORT_ARROGANT_HOLD_FRAMES 0 CACHE STRING
	"Expenses on context switching may exceed the benefits from using multiple
	cores. Set this option to anything different from 0. When enabled, a
	threaded SIMD matrix ops. thread will not release control for
	`MOSSE_PORT_ARROGANT_HOLD_FRAMES` number of frames. Establish the optimal value
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
