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
CMAKE_SOURCE_DIR = /home/aerika/Documents/2nd_year/OS/lab7/two_threads

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aerika/Documents/2nd_year/OS/lab7/two_threads/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/two_threads.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/two_threads.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/two_threads.dir/flags.make

CMakeFiles/two_threads.dir/main.c.o: CMakeFiles/two_threads.dir/flags.make
CMakeFiles/two_threads.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aerika/Documents/2nd_year/OS/lab7/two_threads/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/two_threads.dir/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/two_threads.dir/main.c.o   -c /home/aerika/Documents/2nd_year/OS/lab7/two_threads/main.c

CMakeFiles/two_threads.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/two_threads.dir/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/aerika/Documents/2nd_year/OS/lab7/two_threads/main.c > CMakeFiles/two_threads.dir/main.c.i

CMakeFiles/two_threads.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/two_threads.dir/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/aerika/Documents/2nd_year/OS/lab7/two_threads/main.c -o CMakeFiles/two_threads.dir/main.c.s

CMakeFiles/two_threads.dir/main.c.o.requires:

.PHONY : CMakeFiles/two_threads.dir/main.c.o.requires

CMakeFiles/two_threads.dir/main.c.o.provides: CMakeFiles/two_threads.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/two_threads.dir/build.make CMakeFiles/two_threads.dir/main.c.o.provides.build
.PHONY : CMakeFiles/two_threads.dir/main.c.o.provides

CMakeFiles/two_threads.dir/main.c.o.provides.build: CMakeFiles/two_threads.dir/main.c.o


# Object files for target two_threads
two_threads_OBJECTS = \
"CMakeFiles/two_threads.dir/main.c.o"

# External object files for target two_threads
two_threads_EXTERNAL_OBJECTS =

two_threads: CMakeFiles/two_threads.dir/main.c.o
two_threads: CMakeFiles/two_threads.dir/build.make
two_threads: CMakeFiles/two_threads.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aerika/Documents/2nd_year/OS/lab7/two_threads/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable two_threads"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/two_threads.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/two_threads.dir/build: two_threads

.PHONY : CMakeFiles/two_threads.dir/build

CMakeFiles/two_threads.dir/requires: CMakeFiles/two_threads.dir/main.c.o.requires

.PHONY : CMakeFiles/two_threads.dir/requires

CMakeFiles/two_threads.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/two_threads.dir/cmake_clean.cmake
.PHONY : CMakeFiles/two_threads.dir/clean

CMakeFiles/two_threads.dir/depend:
	cd /home/aerika/Documents/2nd_year/OS/lab7/two_threads/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aerika/Documents/2nd_year/OS/lab7/two_threads /home/aerika/Documents/2nd_year/OS/lab7/two_threads /home/aerika/Documents/2nd_year/OS/lab7/two_threads/cmake-build-debug /home/aerika/Documents/2nd_year/OS/lab7/two_threads/cmake-build-debug /home/aerika/Documents/2nd_year/OS/lab7/two_threads/cmake-build-debug/CMakeFiles/two_threads.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/two_threads.dir/depend

