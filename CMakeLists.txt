cmake_minimum_required(VERSION 3.1)

project(cpp_build_project)


set(CMAKE_CXX_STANDARD 17)

# enable warnings
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

#### LIBRARY ####

add_library(cpp_build_project
  lib_sources/build_project.cpp
) 
target_include_directories(cpp_build_project PUBLIC include)

#### TESTS ####




#### EXAMPLES ####

add_executable(example_one_file_builder
  examples/one_file/__cppbuild__.cpp
)

target_link_libraries(example_one_file_builder PRIVATE cpp_build_project)
# add a symbolic link to the program from the source directory
add_custom_command(TARGET example_one_file_builder POST_BUILD
  COMMAND cmake -E create_symlink $<TARGET_FILE:example_one_file_builder> ${CMAKE_SOURCE_DIR}/examples/one_file/build_project
)

