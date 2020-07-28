cmake_minimum_required (VERSION 3.16.3)

################################################################################
# Compiler can be selected as shown at the end
################################################################################

#set (CMAKE_CXX_COMPILER             "clang++-8")
#set (CMAKE_CXX_COMPILER             "g++-8")
#set (CMAKE_CXX_COMPILER             "/home/vovan/llvm-project/build/bin/clang++")

project (ntrip)

#set (CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wno-psabi")
#set (CMAKE_CXX_FLAGS                "-Wall -Werror -Wextra -pedantic -pedantic-errors -Wconversion -Wsign-conversion")
set (CMAKE_CXX_FLAGS                "-Wall -Werror -Wextra -Wconversion -Wsign-conversion")
set (CMAKE_CXX_STANDARD 17)
set (COMPILE_WARNING_MODE HIGH)

set (ntrip_src 
	Ntrip/Src/login_encode.cpp
	Ntrip/Src/base64_encoder.cpp
	Ntrip/Src/nmea.cpp
	cli.cpp
	Ntrip/Src/async_io.cpp
	Ntrip/Src/tcp_client.cpp 
	Ntrip/Src/mount_point.cpp
)
set (ntclient_src
	${ntrip_src}
	Ntrip/Src/ntrip_client.cpp
)
set (ntserver_src
	${ntrip_src}
	Ntrip/Src/ntrip_server.cpp
)

include_directories(Ntrip/Inc)
#include <cli.hpp>

add_executable (ntclient ntclient.cpp ${ntclient_src})
target_link_libraries (ntclient rt)

add_executable (ntserver ntserver.cpp ${ntserver_src})
target_link_libraries (ntserver rt)


set (caster_src
	${ntrip_src}
	Ntrip/Src/accept_listener.cpp
	Ntrip/Src/tcp_server.tmpl.cpp
)

set (prog_src ${caster_src} prog.cpp)
add_executable (prog ${prog_src})
target_link_libraries (prog pthread rt)

################################################################################
# Unit Tests
################################################################################

enable_testing()

# CppUTest
set (testcppu_src
	${ntclient_src}
	Tests/RunAllTests.cpp
	Tests/testU1.cpp
	Tests/TestEncoder.cpp
	Tests/TestTcpServer.cpp
	Ntrip/Src/accept_listener.cpp
)
add_executable (${PROJECT_NAME}_CppUtest ${testcppu_src})
target_link_libraries (${PROJECT_NAME}_CppUtest CppUTest pthread rt)
add_test (NAME ${PROJECT_NAME}_TCppU COMMAND ${PROJECT_NAME}_CppUtest)

# Google Test
# gtbuild is output directory of googletest build
#link_directories (../gtest/googletest-release-1.8.1/gtbuild/)

set (testgsuite_src
	${ntclient_src}
	Tests/testGoo1.cpp
	Tests/gtestNmea.cpp
	Tests/gtestNtripClient.cpp
	Tests/gtestTcp.cpp
	Tests/gtest_cli.cpp
)
add_executable (${PROJECT_NAME}_gtest ${testgsuite_src})
target_link_libraries (${PROJECT_NAME}_gtest gtest gtest_main pthread rt)
add_test (NAME ${PROJECT_NAME}_GTest COMMAND ${PROJECT_NAME}_gtest)


################################################################################
# Makefile generation
################################################################################
# cmake -DCMAKE_BUILD_TYPE=Debug ../Src/
# CC=gcc-4.2 CXX=/usr/bin/g++-4.2 cmake -G "Your Generator" path/to/your/source
# cmake -G "Your Generator" -D CMAKE_C_COMPILER=gcc-4.2 -D CMAKE_CXX_COMPILER=g++-4.2 path/to/your/source
# cmake -D CMAKE_CXX_COMPILER="/home/vovan/Desktop/uboot-x15/vanilla/toolchain/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++" ../Src
# cmake -DCMAKE_CXX_COMPILER="clang++-8" -DCMAKE_BUILD_TYPE=Debug ../Src
# cmake -DCMAKE_CXX_COMPILER="/home/vovan/Desktop/uboot-x15/vanilla/toolchain/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++" -DCMAKE_BUILD_TYPE=Debug ../Src
