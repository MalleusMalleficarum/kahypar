# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/andre/server-home/myHyparHypar/kahypar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andre/server-home/myHyparHypar/kahypar

# Include any dependencies generated for this target.
include tests/partition/CMakeFiles/partitioner_test.dir/depend.make

# Include the progress variables for this target.
include tests/partition/CMakeFiles/partitioner_test.dir/progress.make

# Include the compile flags for this target's objects.
include tests/partition/CMakeFiles/partitioner_test.dir/flags.make

tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o: tests/partition/CMakeFiles/partitioner_test.dir/flags.make
tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o: tests/partition/partitioner_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andre/server-home/myHyparHypar/kahypar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/partition && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/partitioner_test.dir/partitioner_test.cc.o -c /home/andre/server-home/myHyparHypar/kahypar/tests/partition/partitioner_test.cc

tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/partitioner_test.dir/partitioner_test.cc.i"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/partition && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andre/server-home/myHyparHypar/kahypar/tests/partition/partitioner_test.cc > CMakeFiles/partitioner_test.dir/partitioner_test.cc.i

tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/partitioner_test.dir/partitioner_test.cc.s"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/partition && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andre/server-home/myHyparHypar/kahypar/tests/partition/partitioner_test.cc -o CMakeFiles/partitioner_test.dir/partitioner_test.cc.s

tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o.requires:

.PHONY : tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o.requires

tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o.provides: tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o.requires
	$(MAKE) -f tests/partition/CMakeFiles/partitioner_test.dir/build.make tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o.provides.build
.PHONY : tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o.provides

tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o.provides.build: tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o


# Object files for target partitioner_test
partitioner_test_OBJECTS = \
"CMakeFiles/partitioner_test.dir/partitioner_test.cc.o"

# External object files for target partitioner_test
partitioner_test_EXTERNAL_OBJECTS =

tests/partition/partitioner_test: tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o
tests/partition/partitioner_test: tests/partition/CMakeFiles/partitioner_test.dir/build.make
tests/partition/partitioner_test: external_tools/googletest/googlemock/gtest/libgtest.a
tests/partition/partitioner_test: external_tools/googletest/googlemock/gtest/libgtest_main.a
tests/partition/partitioner_test: external_tools/googletest/googlemock/gtest/libgtest.a
tests/partition/partitioner_test: tests/partition/CMakeFiles/partitioner_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/andre/server-home/myHyparHypar/kahypar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable partitioner_test"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/partition && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/partitioner_test.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Running partitioner_test"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/partition && /home/andre/server-home/myHyparHypar/kahypar/tests/partition/partitioner_test

# Rule to build all files generated by this target.
tests/partition/CMakeFiles/partitioner_test.dir/build: tests/partition/partitioner_test

.PHONY : tests/partition/CMakeFiles/partitioner_test.dir/build

tests/partition/CMakeFiles/partitioner_test.dir/requires: tests/partition/CMakeFiles/partitioner_test.dir/partitioner_test.cc.o.requires

.PHONY : tests/partition/CMakeFiles/partitioner_test.dir/requires

tests/partition/CMakeFiles/partitioner_test.dir/clean:
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/partition && $(CMAKE_COMMAND) -P CMakeFiles/partitioner_test.dir/cmake_clean.cmake
.PHONY : tests/partition/CMakeFiles/partitioner_test.dir/clean

tests/partition/CMakeFiles/partitioner_test.dir/depend:
	cd /home/andre/server-home/myHyparHypar/kahypar && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andre/server-home/myHyparHypar/kahypar /home/andre/server-home/myHyparHypar/kahypar/tests/partition /home/andre/server-home/myHyparHypar/kahypar /home/andre/server-home/myHyparHypar/kahypar/tests/partition /home/andre/server-home/myHyparHypar/kahypar/tests/partition/CMakeFiles/partitioner_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/partition/CMakeFiles/partitioner_test.dir/depend

