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
include tools/CMakeFiles/GraphToHgr.dir/depend.make

# Include the progress variables for this target.
include tools/CMakeFiles/GraphToHgr.dir/progress.make

# Include the compile flags for this target's objects.
include tools/CMakeFiles/GraphToHgr.dir/flags.make

tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o: tools/CMakeFiles/GraphToHgr.dir/flags.make
tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o: tools/graph_to_hgr_converter.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andre/server-home/myHyparHypar/kahypar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o"
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o -c /home/andre/server-home/myHyparHypar/kahypar/tools/graph_to_hgr_converter.cc

tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.i"
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andre/server-home/myHyparHypar/kahypar/tools/graph_to_hgr_converter.cc > CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.i

tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.s"
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andre/server-home/myHyparHypar/kahypar/tools/graph_to_hgr_converter.cc -o CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.s

tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o.requires:

.PHONY : tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o.requires

tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o.provides: tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o.requires
	$(MAKE) -f tools/CMakeFiles/GraphToHgr.dir/build.make tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o.provides.build
.PHONY : tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o.provides

tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o.provides.build: tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o


# Object files for target GraphToHgr
GraphToHgr_OBJECTS = \
"CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o"

# External object files for target GraphToHgr
GraphToHgr_EXTERNAL_OBJECTS =

tools/GraphToHgr: tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o
tools/GraphToHgr: tools/CMakeFiles/GraphToHgr.dir/build.make
tools/GraphToHgr: tools/CMakeFiles/GraphToHgr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/andre/server-home/myHyparHypar/kahypar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable GraphToHgr"
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GraphToHgr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/CMakeFiles/GraphToHgr.dir/build: tools/GraphToHgr

.PHONY : tools/CMakeFiles/GraphToHgr.dir/build

tools/CMakeFiles/GraphToHgr.dir/requires: tools/CMakeFiles/GraphToHgr.dir/graph_to_hgr_converter.cc.o.requires

.PHONY : tools/CMakeFiles/GraphToHgr.dir/requires

tools/CMakeFiles/GraphToHgr.dir/clean:
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && $(CMAKE_COMMAND) -P CMakeFiles/GraphToHgr.dir/cmake_clean.cmake
.PHONY : tools/CMakeFiles/GraphToHgr.dir/clean

tools/CMakeFiles/GraphToHgr.dir/depend:
	cd /home/andre/server-home/myHyparHypar/kahypar && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andre/server-home/myHyparHypar/kahypar /home/andre/server-home/myHyparHypar/kahypar/tools /home/andre/server-home/myHyparHypar/kahypar /home/andre/server-home/myHyparHypar/kahypar/tools /home/andre/server-home/myHyparHypar/kahypar/tools/CMakeFiles/GraphToHgr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/CMakeFiles/GraphToHgr.dir/depend
