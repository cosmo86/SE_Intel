# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/SE_Intel/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/SE_Intel/src/build

# Include any dependencies generated for this target.
include include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/compiler_depend.make

# Include the progress variables for this target.
include include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/progress.make

# Include the compile flags for this target's objects.
include include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/flags.make

include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/numeric/int128.cc.o: include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/flags.make
include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/numeric/int128.cc.o: /root/clickhouse-app/contribs/clickhouse-cpp/contrib/absl/absl/numeric/int128.cc
include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/numeric/int128.cc.o: include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/root/SE_Intel/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/numeric/int128.cc.o"
	cd /root/SE_Intel/src/build/include_clickhouse/contrib/absl/absl && /opt/rh/devtoolset-8/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/numeric/int128.cc.o -MF CMakeFiles/absl_int128.dir/numeric/int128.cc.o.d -o CMakeFiles/absl_int128.dir/numeric/int128.cc.o -c /root/clickhouse-app/contribs/clickhouse-cpp/contrib/absl/absl/numeric/int128.cc

include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/numeric/int128.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/absl_int128.dir/numeric/int128.cc.i"
	cd /root/SE_Intel/src/build/include_clickhouse/contrib/absl/absl && /opt/rh/devtoolset-8/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/clickhouse-app/contribs/clickhouse-cpp/contrib/absl/absl/numeric/int128.cc > CMakeFiles/absl_int128.dir/numeric/int128.cc.i

include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/numeric/int128.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/absl_int128.dir/numeric/int128.cc.s"
	cd /root/SE_Intel/src/build/include_clickhouse/contrib/absl/absl && /opt/rh/devtoolset-8/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/clickhouse-app/contribs/clickhouse-cpp/contrib/absl/absl/numeric/int128.cc -o CMakeFiles/absl_int128.dir/numeric/int128.cc.s

# Object files for target absl_int128
absl_int128_OBJECTS = \
"CMakeFiles/absl_int128.dir/numeric/int128.cc.o"

# External object files for target absl_int128
absl_int128_EXTERNAL_OBJECTS =

include_clickhouse/contrib/absl/absl/libabsl_int128.a: include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/numeric/int128.cc.o
include_clickhouse/contrib/absl/absl/libabsl_int128.a: include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/build.make
include_clickhouse/contrib/absl/absl/libabsl_int128.a: include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/root/SE_Intel/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libabsl_int128.a"
	cd /root/SE_Intel/src/build/include_clickhouse/contrib/absl/absl && $(CMAKE_COMMAND) -P CMakeFiles/absl_int128.dir/cmake_clean_target.cmake
	cd /root/SE_Intel/src/build/include_clickhouse/contrib/absl/absl && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/absl_int128.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/build: include_clickhouse/contrib/absl/absl/libabsl_int128.a
.PHONY : include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/build

include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/clean:
	cd /root/SE_Intel/src/build/include_clickhouse/contrib/absl/absl && $(CMAKE_COMMAND) -P CMakeFiles/absl_int128.dir/cmake_clean.cmake
.PHONY : include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/clean

include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/depend:
	cd /root/SE_Intel/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/SE_Intel/src /root/clickhouse-app/contribs/clickhouse-cpp/contrib/absl/absl /root/SE_Intel/src/build /root/SE_Intel/src/build/include_clickhouse/contrib/absl/absl /root/SE_Intel/src/build/include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : include_clickhouse/contrib/absl/absl/CMakeFiles/absl_int128.dir/depend
