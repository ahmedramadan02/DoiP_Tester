cmake_minimum_required(VERSION 2.6)
project(PlatformTester)

# executable for the client (the tester)
add_executable(platform_tester main.cpp PlatformLogger.cpp PlatformTester.cpp ApplicationManagement.cpp UTest/UTest.cpp)
target_link_libraries(platform_tester rt pthread)

# executable for the test envirnoment server before integrating to the adaptive platform
add_executable(TestServer ServerTestEnv/server.c)

#install(TARGETS platform_tester DESTINATION bin)
