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
include tools/CMakeFiles/MtxToHgr.dir/depend.make

# Include the progress variables for this target.
include tools/CMakeFiles/MtxToHgr.dir/progress.make

# Include the compile flags for this target's objects.
include tools/CMakeFiles/MtxToHgr.dir/flags.make

tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o: tools/CMakeFiles/MtxToHgr.dir/flags.make
tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o: tools/mtx_to_hgr_converter.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andre/server-home/myHyparHypar/kahypar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o"
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o -c /home/andre/server-home/myHyparHypar/kahypar/tools/mtx_to_hgr_converter.cc

tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.i"
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andre/server-home/myHyparHypar/kahypar/tools/mtx_to_hgr_converter.cc > CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.i

tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.s"
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andre/server-home/myHyparHypar/kahypar/tools/mtx_to_hgr_converter.cc -o CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.s

tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o.requires:

.PHONY : tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o.requires

tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o.provides: tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o.requires
	$(MAKE) -f tools/CMakeFiles/MtxToHgr.dir/build.make tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o.provides.build
.PHONY : tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o.provides

tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o.provides.build: tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o


tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o: tools/CMakeFiles/MtxToHgr.dir/flags.make
tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o: tools/mtx_to_hgr_conversion.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andre/server-home/myHyparHypar/kahypar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o"
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o -c /home/andre/server-home/myHyparHypar/kahypar/tools/mtx_to_hgr_conversion.cc

tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.i"
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andre/server-home/myHyparHypar/kahypar/tools/mtx_to_hgr_conversion.cc > CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.i

tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.s"
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andre/server-home/myHyparHypar/kahypar/tools/mtx_to_hgr_conversion.cc -o CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.s

tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o.requires:

.PHONY : tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o.requires

tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o.provides: tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o.requires
	$(MAKE) -f tools/CMakeFiles/MtxToHgr.dir/build.make tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o.provides.build
.PHONY : tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o.provides

tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o.provides.build: tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o


# Object files for target MtxToHgr
MtxToHgr_OBJECTS = \
"CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o" \
"CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o"

# External object files for target MtxToHgr
MtxToHgr_EXTERNAL_OBJECTS =

tools/MtxToHgr: tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o
tools/MtxToHgr: tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o
tools/MtxToHgr: tools/CMakeFiles/MtxToHgr.dir/build.make
tools/MtxToHgr: tools/CMakeFiles/MtxToHgr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/andre/server-home/myHyparHypar/kahypar/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable MtxToHgr"
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MtxToHgr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/CMakeFiles/MtxToHgr.dir/build: tools/MtxToHgr

.PHONY : tools/CMakeFiles/MtxToHgr.dir/build

tools/CMakeFiles/MtxToHgr.dir/requires: tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_converter.cc.o.requires
tools/CMakeFiles/MtxToHgr.dir/requires: tools/CMakeFiles/MtxToHgr.dir/mtx_to_hgr_conversion.cc.o.requires

.PHONY : tools/CMakeFiles/MtxToHgr.dir/requires

tools/CMakeFiles/MtxToHgr.dir/clean:
	cd /home/andre/server-home/myHyparHypar/kahypar/tools && $(CMAKE_COMMAND) -P CMakeFiles/MtxToHgr.dir/cmake_clean.cmake
.PHONY : tools/CMakeFiles/MtxToHgr.dir/clean

tools/CMakeFiles/MtxToHgr.dir/depend:
	cd /home/andre/server-home/myHyparHypar/kahypar && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andre/server-home/myHyparHypar/kahypar /home/andre/server-home/myHyparHypar/kahypar/tools /home/andre/server-home/myHyparHypar/kahypar /home/andre/server-home/myHyparHypar/kahypar/tools /home/andre/server-home/myHyparHypar/kahypar/tools/CMakeFiles/MtxToHgr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/CMakeFiles/MtxToHgr.dir/depend

