project(malloccounter)
add_library(malloccounter MallocCounter.cpp)
target_link_libraries(stdc++)
target_include_directories(malloccounter PUBLIC .)

