cmake_minimum_required(VERSION 3.15)

project(oopc LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(project/animal)

add_executable(oopc
    project/app/main.c
)

target_link_libraries(oopc PRIVATE
    animal
)
