# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

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
CMAKE_COMMAND = /home/andre/server-software/cmake-3.0.1/bin/cmake

# The command to remove a file.
RM = /home/andre/server-software/cmake-3.0.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/andre/server-home/myHyparHypar/kahypar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andre/server-home/myHyparHypar/kahypar

# Include any dependencies generated for this target.
include tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/depend.make

# Include the progress variables for this target.
include tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/progress.make

# Include the compile flags for this target's objects.
include tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/flags.make

tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o: tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/flags.make
tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o: tests/partition/initial_partitioning/gain_computation_test.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/andre/server-home/myHyparHypar/kahypar/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o -c /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning/gain_computation_test.cc

tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.i"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning/gain_computation_test.cc > CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.i

tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.s"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning/gain_computation_test.cc -o CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.s

tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o.requires:
.PHONY : tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o.requires

tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o.provides: tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o.requires
	$(MAKE) -f tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/build.make tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o.provides.build
.PHONY : tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o.provides

tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o.provides.build: tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o

# Object files for target gain_computation_test
gain_computation_test_OBJECTS = \
"CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o"

# External object files for target gain_computation_test
gain_computation_test_EXTERNAL_OBJECTS =

tests/partition/initial_partitioning/gain_computation_test: tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o
tests/partition/initial_partitioning/gain_computation_test: tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/build.make
tests/partition/initial_partitioning/gain_computation_test: external_tools/googletest/googlemock/gtest/libgtest.a
tests/partition/initial_partitioning/gain_computation_test: external_tools/googletest/googlemock/gtest/libgtest_main.a
tests/partition/initial_partitioning/gain_computation_test: external_tools/googletest/googlemock/gtest/libgtest.a
tests/partition/initial_partitioning/gain_computation_test: tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable gain_computation_test"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gain_computation_test.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Running gain_computation_test"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning && /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning/gain_computation_test

# Rule to build all files generated by this target.
tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/build: tests/partition/initial_partitioning/gain_computation_test
.PHONY : tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/build

tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/requires: tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/gain_computation_test.cc.o.requires
.PHONY : tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/requires

tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/clean:
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning && $(CMAKE_COMMAND) -P CMakeFiles/gain_computation_test.dir/cmake_clean.cmake
.PHONY : tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/clean

tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/depend:
	cd /home/andre/server-home/myHyparHypar/kahypar && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andre/server-home/myHyparHypar/kahypar /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning /home/andre/server-home/myHyparHypar/kahypar /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning /home/andre/server-home/myHyparHypar/kahypar/tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/partition/initial_partitioning/CMakeFiles/gain_computation_test.dir/depend

