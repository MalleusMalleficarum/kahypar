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
include tests/datastructure/CMakeFiles/binary_heap_test.dir/depend.make

# Include the progress variables for this target.
include tests/datastructure/CMakeFiles/binary_heap_test.dir/progress.make

# Include the compile flags for this target's objects.
include tests/datastructure/CMakeFiles/binary_heap_test.dir/flags.make

tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o: tests/datastructure/CMakeFiles/binary_heap_test.dir/flags.make
tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o: tests/datastructure/binary_heap_test.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/andre/server-home/myHyparHypar/kahypar/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o -c /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure/binary_heap_test.cc

tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.i"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure/binary_heap_test.cc > CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.i

tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.s"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure/binary_heap_test.cc -o CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.s

tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o.requires:
.PHONY : tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o.requires

tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o.provides: tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o.requires
	$(MAKE) -f tests/datastructure/CMakeFiles/binary_heap_test.dir/build.make tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o.provides.build
.PHONY : tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o.provides

tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o.provides.build: tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o

# Object files for target binary_heap_test
binary_heap_test_OBJECTS = \
"CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o"

# External object files for target binary_heap_test
binary_heap_test_EXTERNAL_OBJECTS =

tests/datastructure/binary_heap_test: tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o
tests/datastructure/binary_heap_test: tests/datastructure/CMakeFiles/binary_heap_test.dir/build.make
tests/datastructure/binary_heap_test: external_tools/googletest/googlemock/gtest/libgtest.a
tests/datastructure/binary_heap_test: external_tools/googletest/googlemock/gtest/libgtest_main.a
tests/datastructure/binary_heap_test: external_tools/googletest/googlemock/gtest/libgtest.a
tests/datastructure/binary_heap_test: tests/datastructure/CMakeFiles/binary_heap_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable binary_heap_test"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/binary_heap_test.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Running binary_heap_test"
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure && /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure/binary_heap_test

# Rule to build all files generated by this target.
tests/datastructure/CMakeFiles/binary_heap_test.dir/build: tests/datastructure/binary_heap_test
.PHONY : tests/datastructure/CMakeFiles/binary_heap_test.dir/build

tests/datastructure/CMakeFiles/binary_heap_test.dir/requires: tests/datastructure/CMakeFiles/binary_heap_test.dir/binary_heap_test.cc.o.requires
.PHONY : tests/datastructure/CMakeFiles/binary_heap_test.dir/requires

tests/datastructure/CMakeFiles/binary_heap_test.dir/clean:
	cd /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure && $(CMAKE_COMMAND) -P CMakeFiles/binary_heap_test.dir/cmake_clean.cmake
.PHONY : tests/datastructure/CMakeFiles/binary_heap_test.dir/clean

tests/datastructure/CMakeFiles/binary_heap_test.dir/depend:
	cd /home/andre/server-home/myHyparHypar/kahypar && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andre/server-home/myHyparHypar/kahypar /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure /home/andre/server-home/myHyparHypar/kahypar /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure /home/andre/server-home/myHyparHypar/kahypar/tests/datastructure/CMakeFiles/binary_heap_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/datastructure/CMakeFiles/binary_heap_test.dir/depend

