# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/mustafa/Programs/clion-2017.3.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/mustafa/Programs/clion-2017.3.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mustafa/Desktop/SP/homeworks/h1/tifff

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mustafa/Desktop/SP/homeworks/h1/tifff/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/tifff.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tifff.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tifff.dir/flags.make

CMakeFiles/tifff.dir/main.c.o: CMakeFiles/tifff.dir/flags.make
CMakeFiles/tifff.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mustafa/Desktop/SP/homeworks/h1/tifff/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/tifff.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tifff.dir/main.c.o   -c /home/mustafa/Desktop/SP/homeworks/h1/tifff/main.c

CMakeFiles/tifff.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tifff.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mustafa/Desktop/SP/homeworks/h1/tifff/main.c > CMakeFiles/tifff.dir/main.c.i

CMakeFiles/tifff.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tifff.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mustafa/Desktop/SP/homeworks/h1/tifff/main.c -o CMakeFiles/tifff.dir/main.c.s

CMakeFiles/tifff.dir/main.c.o.requires:

.PHONY : CMakeFiles/tifff.dir/main.c.o.requires

CMakeFiles/tifff.dir/main.c.o.provides: CMakeFiles/tifff.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/tifff.dir/build.make CMakeFiles/tifff.dir/main.c.o.provides.build
.PHONY : CMakeFiles/tifff.dir/main.c.o.provides

CMakeFiles/tifff.dir/main.c.o.provides.build: CMakeFiles/tifff.dir/main.c.o


# Object files for target tifff
tifff_OBJECTS = \
"CMakeFiles/tifff.dir/main.c.o"

# External object files for target tifff
tifff_EXTERNAL_OBJECTS =

tifff: CMakeFiles/tifff.dir/main.c.o
tifff: CMakeFiles/tifff.dir/build.make
tifff: CMakeFiles/tifff.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mustafa/Desktop/SP/homeworks/h1/tifff/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable tifff"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tifff.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tifff.dir/build: tifff

.PHONY : CMakeFiles/tifff.dir/build

CMakeFiles/tifff.dir/requires: CMakeFiles/tifff.dir/main.c.o.requires

.PHONY : CMakeFiles/tifff.dir/requires

CMakeFiles/tifff.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tifff.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tifff.dir/clean

CMakeFiles/tifff.dir/depend:
	cd /home/mustafa/Desktop/SP/homeworks/h1/tifff/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mustafa/Desktop/SP/homeworks/h1/tifff /home/mustafa/Desktop/SP/homeworks/h1/tifff /home/mustafa/Desktop/SP/homeworks/h1/tifff/cmake-build-debug /home/mustafa/Desktop/SP/homeworks/h1/tifff/cmake-build-debug /home/mustafa/Desktop/SP/homeworks/h1/tifff/cmake-build-debug/CMakeFiles/tifff.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tifff.dir/depend

