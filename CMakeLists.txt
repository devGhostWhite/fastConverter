cmake_minimum_required(VERSION 3.15)
project(ffmpeg_batch_converter CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(fconv
    main.cpp
    src/presets.cpp
    src/flags.cpp
    src/worker.cpp
    src/utils.cpp
)

find_package(Threads REQUIRED)

target_link_libraries(fconv PRIVATE Threads::Threads)

set_target_properties(
    fconv
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
