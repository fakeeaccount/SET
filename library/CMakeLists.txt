set(SRC_FILES set.cpp)

add_library(archiver_library ${SRC_FILES})
target_include_directories(archiver_library PUBLIC .)