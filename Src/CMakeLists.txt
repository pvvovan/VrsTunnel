cmake_minimum_required (VERSION 3.18)

project(ntrip)

set(CMAKE_CXX_STANDARD 23)
set(COMPILE_WARNING_MODE HIGH)

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
	Ntrip/Src/dispatcher.cpp
	Ntrip/Src/corr_supply.cpp
	Ntrip/Src/corr_consume.cpp
)

set(ntrip_inc
	${CMAKE_SOURCE_DIR}
	Ntrip/Inc
)

add_executable(ntclient ntclient.cpp ${ntclient_src})
target_include_directories(ntclient PRIVATE ${ntrip_inc})
target_link_libraries(ntclient rt)

add_executable(ntserver ntserver.cpp ${ntserver_src})
target_include_directories(ntserver PRIVATE ${ntrip_inc})
target_link_libraries(ntserver rt)

add_executable(ntcaster ntcaster.cpp ${ntcaster_src})
target_include_directories(ntcaster PRIVATE ${ntrip_inc})


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
target_include_directories(${PROJECT_NAME}_CppUtest PRIVATE ${ntrip_inc})
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
target_include_directories(${PROJECT_NAME}_gtest PRIVATE ${ntrip_inc})
target_link_libraries(${PROJECT_NAME}_gtest gtest gtest_main pthread rt)
add_test(NAME ${PROJECT_NAME}_GTest COMMAND ${PROJECT_NAME}_gtest)

add_custom_target(coverage
	COMMAND make ${PROJECT_NAME}_gtest
	COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_gtest
	COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --html --html-details -o cov.html ${CMAKE_BINARY_DIR}
)
