# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/andre/server-home/myHyparHypar/kahypar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andre/server-home/myHyparHypar/kahypar/release

# Include any dependencies generated for this target.
include tools/CMakeFiles/HgrToPaToH.dir/depend.make

# Include the progress variables for this target.
include tools/CMakeFiles/HgrToPaToH.dir/progress.make

# Include the compile flags for this target's objects.
include tools/CMakeFiles/HgrToPaToH.dir/flags.make

tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o: tools/CMakeFiles/HgrToPaToH.dir/flags.make
tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o: ../tools/hgr_to_patoh_converter.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andre/server-home/myHyparHypar/kahypar/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o"
	cd /home/andre/server-home/myHyparHypar/kahypar/release/tools && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o -c /home/andre/server-home/myHyparHypar/kahypar/tools/hgr_to_patoh_converter.cc

tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.i"
	cd /home/andre/server-home/myHyparHypar/kahypar/release/tools && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andre/server-home/myHyparHypar/kahypar/tools/hgr_to_patoh_converter.cc > CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.i

tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.s"
	cd /home/andre/server-home/myHyparHypar/kahypar/release/tools && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andre/server-home/myHyparHypar/kahypar/tools/hgr_to_patoh_converter.cc -o CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.s

tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o.requires:

.PHONY : tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o.requires

tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o.provides: tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o.requires
	$(MAKE) -f tools/CMakeFiles/HgrToPaToH.dir/build.make tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o.provides.build
.PHONY : tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o.provides

tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o.provides.build: tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o


# Object files for target HgrToPaToH
HgrToPaToH_OBJECTS = \
"CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o"

# External object files for target HgrToPaToH
HgrToPaToH_EXTERNAL_OBJECTS =

tools/HgrToPaToH: tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o
tools/HgrToPaToH: tools/CMakeFiles/HgrToPaToH.dir/build.make
tools/HgrToPaToH: tools/CMakeFiles/HgrToPaToH.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/andre/server-home/myHyparHypar/kahypar/release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable HgrToPaToH"
	cd /home/andre/server-home/myHyparHypar/kahypar/release/tools && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HgrToPaToH.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/CMakeFiles/HgrToPaToH.dir/build: tools/HgrToPaToH

.PHONY : tools/CMakeFiles/HgrToPaToH.dir/build

tools/CMakeFiles/HgrToPaToH.dir/requires: tools/CMakeFiles/HgrToPaToH.dir/hgr_to_patoh_converter.cc.o.requires

.PHONY : tools/CMakeFiles/HgrToPaToH.dir/requires

tools/CMakeFiles/HgrToPaToH.dir/clean:
	cd /home/andre/server-home/myHyparHypar/kahypar/release/tools && $(CMAKE_COMMAND) -P CMakeFiles/HgrToPaToH.dir/cmake_clean.cmake
.PHONY : tools/CMakeFiles/HgrToPaToH.dir/clean

tools/CMakeFiles/HgrToPaToH.dir/depend:
	cd /home/andre/server-home/myHyparHypar/kahypar/release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andre/server-home/myHyparHypar/kahypar /home/andre/server-home/myHyparHypar/kahypar/tools /home/andre/server-home/myHyparHypar/kahypar/release /home/andre/server-home/myHyparHypar/kahypar/release/tools /home/andre/server-home/myHyparHypar/kahypar/release/tools/CMakeFiles/HgrToPaToH.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/CMakeFiles/HgrToPaToH.dir/depend

