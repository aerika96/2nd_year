# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /home/aerika/clion-2016.3.4/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/aerika/clion-2016.3.4/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aerika/Documents/2nd_year/OS/lab7/increment

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aerika/Documents/2nd_year/OS/lab7/increment/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/increment.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/increment.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/increment.dir/flags.make

CMakeFiles/increment.dir/main.c.o: CMakeFiles/increment.dir/flags.make
CMakeFiles/increment.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aerika/Documents/2nd_year/OS/lab7/increment/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/increment.dir/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/increment.dir/main.c.o   -c /home/aerika/Documents/2nd_year/OS/lab7/increment/main.c

CMakeFiles/increment.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/increment.dir/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/aerika/Documents/2nd_year/OS/lab7/increment/main.c > CMakeFiles/increment.dir/main.c.i

CMakeFiles/increment.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/increment.dir/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/aerika/Documents/2nd_year/OS/lab7/increment/main.c -o CMakeFiles/increment.dir/main.c.s

CMakeFiles/increment.dir/main.c.o.requires:

.PHONY : CMakeFiles/increment.dir/main.c.o.requires

CMakeFiles/increment.dir/main.c.o.provides: CMakeFiles/increment.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/increment.dir/build.make CMakeFiles/increment.dir/main.c.o.provides.build
.PHONY : CMakeFiles/increment.dir/main.c.o.provides

CMakeFiles/increment.dir/main.c.o.provides.build: CMakeFiles/increment.dir/main.c.o


# Object files for target increment
increment_OBJECTS = \
"CMakeFiles/increment.dir/main.c.o"

# External object files for target increment
increment_EXTERNAL_OBJECTS =

increment: CMakeFiles/increment.dir/main.c.o
increment: CMakeFiles/increment.dir/build.make
increment: CMakeFiles/increment.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aerika/Documents/2nd_year/OS/lab7/increment/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable increment"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/increment.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/increment.dir/build: increment

.PHONY : CMakeFiles/increment.dir/build

CMakeFiles/increment.dir/requires: CMakeFiles/increment.dir/main.c.o.requires

.PHONY : CMakeFiles/increment.dir/requires

CMakeFiles/increment.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/increment.dir/cmake_clean.cmake
.PHONY : CMakeFiles/increment.dir/clean

CMakeFiles/increment.dir/depend:
	cd /home/aerika/Documents/2nd_year/OS/lab7/increment/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aerika/Documents/2nd_year/OS/lab7/increment /home/aerika/Documents/2nd_year/OS/lab7/increment /home/aerika/Documents/2nd_year/OS/lab7/increment/cmake-build-debug /home/aerika/Documents/2nd_year/OS/lab7/increment/cmake-build-debug /home/aerika/Documents/2nd_year/OS/lab7/increment/cmake-build-debug/CMakeFiles/increment.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/increment.dir/depend

