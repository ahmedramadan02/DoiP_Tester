cmake_minimum_required(VERSION 2.6)
project(PlatformTester)

include(GNUInstallDirs)

# executable for the client (the tester)
add_executable(platform_tester main.c PlatformTester.c UTest/UTest.c)
target_link_libraries(platform_tester rt pthread)

if(${TEST_LOCAL} MATCHES "ON")
install(TARGETS
       	platform_tester
       	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
   	)
message (STATUS "TEST_LOCAL is ${TEST_LOCAL}")
message (STATUS "TESTER INSTALLED in ${CMAKE_INSTALL_BINDIR}")
else()
# executable for the test envirnoment server before integrating to the adaptive platform
add_executable(TestServer ServerTestEnv/server.c)
message (STATUS "ARAM ${TEST_LOCAL}")
endif()
