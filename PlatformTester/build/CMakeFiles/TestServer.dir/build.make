# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/ahmed/TheYoctoProject/APD-R1903/PlatformTester

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ahmed/TheYoctoProject/APD-R1903/PlatformTester/build

# Include any dependencies generated for this target.
include CMakeFiles/TestServer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TestServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TestServer.dir/flags.make

CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o: CMakeFiles/TestServer.dir/flags.make
CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o: ../ServerTestEnv/server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed/TheYoctoProject/APD-R1903/PlatformTester/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o   -c /home/ahmed/TheYoctoProject/APD-R1903/PlatformTester/ServerTestEnv/server.c

CMakeFiles/TestServer.dir/ServerTestEnv/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/TestServer.dir/ServerTestEnv/server.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ahmed/TheYoctoProject/APD-R1903/PlatformTester/ServerTestEnv/server.c > CMakeFiles/TestServer.dir/ServerTestEnv/server.c.i

CMakeFiles/TestServer.dir/ServerTestEnv/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/TestServer.dir/ServerTestEnv/server.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ahmed/TheYoctoProject/APD-R1903/PlatformTester/ServerTestEnv/server.c -o CMakeFiles/TestServer.dir/ServerTestEnv/server.c.s

CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o.requires:

.PHONY : CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o.requires

CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o.provides: CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o.requires
	$(MAKE) -f CMakeFiles/TestServer.dir/build.make CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o.provides.build
.PHONY : CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o.provides

CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o.provides.build: CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o


# Object files for target TestServer
TestServer_OBJECTS = \
"CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o"

# External object files for target TestServer
TestServer_EXTERNAL_OBJECTS =

TestServer: CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o
TestServer: CMakeFiles/TestServer.dir/build.make
TestServer: CMakeFiles/TestServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ahmed/TheYoctoProject/APD-R1903/PlatformTester/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable TestServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TestServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TestServer.dir/build: TestServer

.PHONY : CMakeFiles/TestServer.dir/build

CMakeFiles/TestServer.dir/requires: CMakeFiles/TestServer.dir/ServerTestEnv/server.c.o.requires

.PHONY : CMakeFiles/TestServer.dir/requires

CMakeFiles/TestServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TestServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TestServer.dir/clean

CMakeFiles/TestServer.dir/depend:
	cd /home/ahmed/TheYoctoProject/APD-R1903/PlatformTester/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ahmed/TheYoctoProject/APD-R1903/PlatformTester /home/ahmed/TheYoctoProject/APD-R1903/PlatformTester /home/ahmed/TheYoctoProject/APD-R1903/PlatformTester/build /home/ahmed/TheYoctoProject/APD-R1903/PlatformTester/build /home/ahmed/TheYoctoProject/APD-R1903/PlatformTester/build/CMakeFiles/TestServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TestServer.dir/depend

