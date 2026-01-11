cmake_minimum_required(VERSION 4.0)
project(arena VERSION 0 LANGUAGES C) # target = arena

set(needed_features c_std_11)

add_executable(test
    EXCLUDE_FROM_ALL
    src/test.c
)
# so simple. I wonder how hard it can get (I'm sure it can get much harder, though)
add_library(arena
    src/arena.c
)

set(public_headers
    src/align.h
    src/arena.h
)
# target_sources(arena
#     PRIVATE
#         src/arena.c
#     PUBLIC
#         FILE_SET arena
#         TYPE HEADERS
#         # BASE_DIRS
#         #     include
#         FILES
#             src/arena.h
#             #include/header.h
# )
target_compile_features(arena PUBLIC ${needed_features})
target_compile_features(test PUBLIC ${needed_features})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # target_compile_definitions(test PRIVATE ARENA_DEBUG)
    target_compile_definitions(arena PUBLIC ARENA_DEBUG)
endif()

target_link_libraries(test arena) # might be convenient for later

file(COPY ${public_headers} DESTINATION "include")