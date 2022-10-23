cmake_minimum_required (VERSION 3.16.3)


################################################################################
# Compiler can be selected as shown at the end
################################################################################

project(ntrip)

set(CMAKE_CXX_FLAGS                "-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion")
set(CMAKE_CXX_STANDARD 17)
set(COMPILE_WARNING_MODE HIGH)

# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -O0 -fprofile-arcs -ftest-coverage")
# set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")

set(ntrip_src
	Ntrip/Src/login_encode.cpp
	Ntrip/Src/base64_encoder.cpp
	Ntrip/Src/nmea.cpp
	cli.cpp
	Ntrip/Src/async_io.cpp
	Ntrip/Src/tcp_client.cpp
	Ntrip/Src/mount_point.cpp
)

set(ntclient_src
	${ntrip_src}
	Ntrip/Src/ntrip_client.cpp
)

set(ntserver_src
	${ntrip_src}
	Ntrip/Src/ntrip_server.cpp
)

set(ntcaster_src
	Ntrip/Src/tcp_server.cpp
	Ntrip/Src/async_io.cpp
)

include_directories(
.
Ntrip/Inc)

add_executable(ntclient ntclient.cpp ${ntclient_src})
target_link_libraries(ntclient rt)

add_executable(ntserver ntserver.cpp ${ntserver_src})
target_link_libraries(ntserver rt)

add_executable(ntcaster ntcaster.cpp ${ntcaster_src})


# set(caster_src
# 	${ntrip_src}
# 	Ntrip/Src/accept_listener.cpp
# 	Ntrip/Src/tcp_server.tmpl.cpp
# )

# set(prog_src ${caster_src} prog.cpp)
# add_executable(prog ${prog_src})
# target_link_libraries(prog pthread rt)


################################################################################
# Unit Tests
################################################################################

enable_testing()

set(testcppu_src
	${ntclient_src}
	Ntrip/Src/tcp_server.cpp
	Tests/RunAllTests.cpp
	Tests/testU1.cpp
	Tests/TestEncoder.cpp
	Tests/TestTcpServer.cpp
)
add_executable(${PROJECT_NAME}_CppUtest ${testcppu_src})
target_link_libraries(${PROJECT_NAME}_CppUtest CppUTest pthread rt)
add_test(NAME ${PROJECT_NAME}_TCppU COMMAND ${PROJECT_NAME}_CppUtest)

set(testgsuite_src
	${ntclient_src}
	Tests/testGoo1.cpp
	Tests/gtestNmea.cpp
	Tests/gtestNtripClient.cpp
	Tests/gtestTcp.cpp
	Tests/gtest_cli.cpp
)
add_executable(${PROJECT_NAME}_gtest ${testgsuite_src})
target_link_libraries(${PROJECT_NAME}_gtest gtest gtest_main pthread rt)
add_test(NAME ${PROJECT_NAME}_GTest COMMAND ${PROJECT_NAME}_gtest)

add_custom_target(coverage
	COMMAND make ${PROJECT_NAME}_gtest
	COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_gtest
	COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --html --html-details -o cov.html ${CMAKE_BINARY_DIR}
)
