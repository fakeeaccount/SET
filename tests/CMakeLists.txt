find_library(archiver_library ../../untitled11/library)

add_executable(tests tests.cpp)

target_link_libraries(tests archiver_library)