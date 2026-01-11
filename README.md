# Arena allocator in C

This is just a simple arena allocator implementation I made to
learn a few things about arena allocators and memory alignment (among other things)

This implementation uses malloc instead of asking the system for
virtual memory directly. It was made mostly for myself, as an experiment.

## Building

> [!WARNING]
> This project has only been tested on Linux. The implementation isn't particularly
> platform specific, but the generation step hasn't been tested on other platforms.

As long as CMake is installed, the following commands may be ran to
build this project:

```bash
$ cmake -B build
$ cmake --build build
```

This will build a static library. To build a shared library version, define the
BUILD_SHARED_LIBS variable in the generation step like this:

```bash
$ cmake -DBUILD_SHARED_LIBS -B build
```

In order to choose between Debug and Release builds, It is necessary to first
declare the variable CMAKE_BUILD_TYPE with the value of the desired build as a string.

```bash
$ CMAKE_BUILD_TYPE=Debug cmake -B build # this is case sensitive
or 
$ cmake -DCMAKE_BUILD_TYPE=Debug -B build
```

It is also possible to select a preset
in order to enable compilation warnings by settings the parameter `--preset` in the generation step.
Check `CMakePresets.json` for a list of defined presets.

```bash
$ cmake -B build --preset=clang
```

There is a target called `test` that builds a little program that can be used to mess around with the implementation.
It's not a proper unit test implementation. It can be built with:

```bash
$ cmake --build build --target test
```