add_subdirectory(../untitled12/library)
add_subdirectory(../untitled12/tests)

add_executable(archiver
        main.cpp ../untitled12/library/set.h ../untitled12/library/set.cpp)

target_link_libraries(archiver archiver_library)