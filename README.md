# cpp_build_project
Proof-of-concept of a C++ build system whose main language is C++ (instead of Lua, Python, custom language, etc)

**NOTE: Currently only supporting Unix platforms**

## Motivation

Why another build system for C++?
I think the biggest problem with the current C++ build systems is the lack of familiarity.
First, when writing we need to learn a new language, which involves new syntax, semantics, etc.
This is time-consuming, and we will probably not become experts of the language.
Moreover, this is an obstacle for beginners.

The aim is to provide not another tool, but instead a library to perform builds, using C++ as a language.
As a C++ library, it will be (hopefully) easy to provide other libraries upon it to add functionalities such as package management.
If accepted, this library would be part of the standard library to be easy enough to access.

## Principle

A C++ project using this build system would have at its root a file named `__cppbuild__.cpp`.
This file describes a C++ program, containing a `main(int argc, char** argv)` entry point.
After compilation of this file, a program is created, named `build_project`.
By executing this program, the project is built.

The execution of `build_project` may modify the content of a `__cppcache__` directory.
This directory is used for incremental building, such as storing object files between builds.
Deleting the `__cppcache__` directory will effectively trigger a complete rebuild.

## How to build

You will need CMake to build the library, ironically.
(I could have provided shell scripts because actually this is not a complex usage of CMake,
but CMake does provide better support for customizing compiler than shell scripts)

```bash
mkdir build     # create directory in which we will build
cd build        # go to that directory
cmake ..        # invoke cmake
cmake --build . # build the library
cd ..           # return to root directory
# now, each example has been populated with a single program "build_project"
cd examples/WHICH_EXAMPLE # go to the example you want to experiment
./build_project           # execute the "build_project" for this project
```