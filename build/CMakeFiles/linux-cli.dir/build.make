# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/guts/Projects/linux-cli

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/guts/Projects/linux-cli/build

# Include any dependencies generated for this target.
include CMakeFiles/linux-cli.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/linux-cli.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/linux-cli.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/linux-cli.dir/flags.make

CMakeFiles/linux-cli.dir/codegen:
.PHONY : CMakeFiles/linux-cli.dir/codegen

CMakeFiles/linux-cli.dir/src/main.cpp.o: CMakeFiles/linux-cli.dir/flags.make
CMakeFiles/linux-cli.dir/src/main.cpp.o: /home/guts/Projects/linux-cli/src/main.cpp
CMakeFiles/linux-cli.dir/src/main.cpp.o: CMakeFiles/linux-cli.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/guts/Projects/linux-cli/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/linux-cli.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/linux-cli.dir/src/main.cpp.o -MF CMakeFiles/linux-cli.dir/src/main.cpp.o.d -o CMakeFiles/linux-cli.dir/src/main.cpp.o -c /home/guts/Projects/linux-cli/src/main.cpp

CMakeFiles/linux-cli.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/linux-cli.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guts/Projects/linux-cli/src/main.cpp > CMakeFiles/linux-cli.dir/src/main.cpp.i

CMakeFiles/linux-cli.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/linux-cli.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guts/Projects/linux-cli/src/main.cpp -o CMakeFiles/linux-cli.dir/src/main.cpp.s

# Object files for target linux-cli
linux__cli_OBJECTS = \
"CMakeFiles/linux-cli.dir/src/main.cpp.o"

# External object files for target linux-cli
linux__cli_EXTERNAL_OBJECTS =

linux-cli: CMakeFiles/linux-cli.dir/src/main.cpp.o
linux-cli: CMakeFiles/linux-cli.dir/build.make
linux-cli: CMakeFiles/linux-cli.dir/compiler_depend.ts
linux-cli: /usr/local/lib/libtdjson.so.1.8.50
linux-cli: /usr/local/lib/libtdclient.a
linux-cli: /usr/local/lib/libtdcore.a
linux-cli: /usr/local/lib/libtdapi.a
linux-cli: /usr/local/lib/libtdutils.a
linux-cli: /usr/local/lib/libtddb.a
linux-cli: /usr/local/lib/libtdsqlite.a
linux-cli: /usr/local/lib/libtde2e.a
linux-cli: /usr/local/lib/libtdmtproto.a
linux-cli: /usr/local/lib/libtdnet.a
linux-cli: /usr/lib/x86_64-linux-gnu/libssl.so
linux-cli: /usr/local/lib/libtdactor.a
linux-cli: /usr/local/lib/libtdutils.a
linux-cli: /usr/lib/x86_64-linux-gnu/libcrypto.so
linux-cli: /usr/lib/x86_64-linux-gnu/libz.so
linux-cli: CMakeFiles/linux-cli.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/guts/Projects/linux-cli/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable linux-cli"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/linux-cli.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/linux-cli.dir/build: linux-cli
.PHONY : CMakeFiles/linux-cli.dir/build

CMakeFiles/linux-cli.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/linux-cli.dir/cmake_clean.cmake
.PHONY : CMakeFiles/linux-cli.dir/clean

CMakeFiles/linux-cli.dir/depend:
	cd /home/guts/Projects/linux-cli/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/guts/Projects/linux-cli /home/guts/Projects/linux-cli /home/guts/Projects/linux-cli/build /home/guts/Projects/linux-cli/build /home/guts/Projects/linux-cli/build/CMakeFiles/linux-cli.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/linux-cli.dir/depend

