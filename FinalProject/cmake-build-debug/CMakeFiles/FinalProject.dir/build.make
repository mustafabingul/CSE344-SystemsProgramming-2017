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
CMAKE_SOURCE_DIR = /home/mustafa/Desktop/SP/homeworks/FinalProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mustafa/Desktop/SP/homeworks/FinalProject/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/FinalProject.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FinalProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FinalProject.dir/flags.make

CMakeFiles/FinalProject.dir/server.c.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/server.c.o: ../server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mustafa/Desktop/SP/homeworks/FinalProject/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/FinalProject.dir/server.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FinalProject.dir/server.c.o   -c /home/mustafa/Desktop/SP/homeworks/FinalProject/server.c

CMakeFiles/FinalProject.dir/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FinalProject.dir/server.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mustafa/Desktop/SP/homeworks/FinalProject/server.c > CMakeFiles/FinalProject.dir/server.c.i

CMakeFiles/FinalProject.dir/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FinalProject.dir/server.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mustafa/Desktop/SP/homeworks/FinalProject/server.c -o CMakeFiles/FinalProject.dir/server.c.s

CMakeFiles/FinalProject.dir/server.c.o.requires:

.PHONY : CMakeFiles/FinalProject.dir/server.c.o.requires

CMakeFiles/FinalProject.dir/server.c.o.provides: CMakeFiles/FinalProject.dir/server.c.o.requires
	$(MAKE) -f CMakeFiles/FinalProject.dir/build.make CMakeFiles/FinalProject.dir/server.c.o.provides.build
.PHONY : CMakeFiles/FinalProject.dir/server.c.o.provides

CMakeFiles/FinalProject.dir/server.c.o.provides.build: CMakeFiles/FinalProject.dir/server.c.o


CMakeFiles/FinalProject.dir/client.c.o: CMakeFiles/FinalProject.dir/flags.make
CMakeFiles/FinalProject.dir/client.c.o: ../client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mustafa/Desktop/SP/homeworks/FinalProject/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/FinalProject.dir/client.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FinalProject.dir/client.c.o   -c /home/mustafa/Desktop/SP/homeworks/FinalProject/client.c

CMakeFiles/FinalProject.dir/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FinalProject.dir/client.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mustafa/Desktop/SP/homeworks/FinalProject/client.c > CMakeFiles/FinalProject.dir/client.c.i

CMakeFiles/FinalProject.dir/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FinalProject.dir/client.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mustafa/Desktop/SP/homeworks/FinalProject/client.c -o CMakeFiles/FinalProject.dir/client.c.s

CMakeFiles/FinalProject.dir/client.c.o.requires:

.PHONY : CMakeFiles/FinalProject.dir/client.c.o.requires

CMakeFiles/FinalProject.dir/client.c.o.provides: CMakeFiles/FinalProject.dir/client.c.o.requires
	$(MAKE) -f CMakeFiles/FinalProject.dir/build.make CMakeFiles/FinalProject.dir/client.c.o.provides.build
.PHONY : CMakeFiles/FinalProject.dir/client.c.o.provides

CMakeFiles/FinalProject.dir/client.c.o.provides.build: CMakeFiles/FinalProject.dir/client.c.o


# Object files for target FinalProject
FinalProject_OBJECTS = \
"CMakeFiles/FinalProject.dir/server.c.o" \
"CMakeFiles/FinalProject.dir/client.c.o"

# External object files for target FinalProject
FinalProject_EXTERNAL_OBJECTS =

FinalProject: CMakeFiles/FinalProject.dir/server.c.o
FinalProject: CMakeFiles/FinalProject.dir/client.c.o
FinalProject: CMakeFiles/FinalProject.dir/build.make
FinalProject: CMakeFiles/FinalProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mustafa/Desktop/SP/homeworks/FinalProject/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable FinalProject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FinalProject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FinalProject.dir/build: FinalProject

.PHONY : CMakeFiles/FinalProject.dir/build

CMakeFiles/FinalProject.dir/requires: CMakeFiles/FinalProject.dir/server.c.o.requires
CMakeFiles/FinalProject.dir/requires: CMakeFiles/FinalProject.dir/client.c.o.requires

.PHONY : CMakeFiles/FinalProject.dir/requires

CMakeFiles/FinalProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FinalProject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FinalProject.dir/clean

CMakeFiles/FinalProject.dir/depend:
	cd /home/mustafa/Desktop/SP/homeworks/FinalProject/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mustafa/Desktop/SP/homeworks/FinalProject /home/mustafa/Desktop/SP/homeworks/FinalProject /home/mustafa/Desktop/SP/homeworks/FinalProject/cmake-build-debug /home/mustafa/Desktop/SP/homeworks/FinalProject/cmake-build-debug /home/mustafa/Desktop/SP/homeworks/FinalProject/cmake-build-debug/CMakeFiles/FinalProject.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FinalProject.dir/depend

