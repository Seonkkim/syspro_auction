# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/169/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/169/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sunkk97/CLionProjects/sys

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sunkk97/CLionProjects/sys/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/sys2.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/sys2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sys2.dir/flags.make

CMakeFiles/sys2.dir/client.c.o: CMakeFiles/sys2.dir/flags.make
CMakeFiles/sys2.dir/client.c.o: ../client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sunkk97/CLionProjects/sys/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/sys2.dir/client.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sys2.dir/client.c.o -c /home/sunkk97/CLionProjects/sys/client.c

CMakeFiles/sys2.dir/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sys2.dir/client.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/sunkk97/CLionProjects/sys/client.c > CMakeFiles/sys2.dir/client.c.i

CMakeFiles/sys2.dir/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sys2.dir/client.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/sunkk97/CLionProjects/sys/client.c -o CMakeFiles/sys2.dir/client.c.s

# Object files for target sys2
sys2_OBJECTS = \
"CMakeFiles/sys2.dir/client.c.o"

# External object files for target sys2
sys2_EXTERNAL_OBJECTS =

sys2: CMakeFiles/sys2.dir/client.c.o
sys2: CMakeFiles/sys2.dir/build.make
sys2: CMakeFiles/sys2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sunkk97/CLionProjects/sys/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable sys2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sys2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sys2.dir/build: sys2
.PHONY : CMakeFiles/sys2.dir/build

CMakeFiles/sys2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sys2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sys2.dir/clean

CMakeFiles/sys2.dir/depend:
	cd /home/sunkk97/CLionProjects/sys/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sunkk97/CLionProjects/sys /home/sunkk97/CLionProjects/sys /home/sunkk97/CLionProjects/sys/cmake-build-debug /home/sunkk97/CLionProjects/sys/cmake-build-debug /home/sunkk97/CLionProjects/sys/cmake-build-debug/CMakeFiles/sys2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sys2.dir/depend

