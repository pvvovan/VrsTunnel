cmake_minimum_required (VERSION 3.30)
project(ntrip LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(COMPILE_WARNING_MODE HIGH)

set(ntrip_src
	cli.cpp
	Ntrip/Src/async_io.cpp
	Ntrip/Src/base64_encoder.cpp
	Ntrip/Src/login_encode.cpp
	Ntrip/Src/mount_point.cpp
	Ntrip/Src/nmea.cpp
	Ntrip/Src/tcp_client.cpp
)
add_library(ntrip_lib STATIC ${ntrip_src})
target_include_directories(ntrip_lib PUBLIC Ntrip/Inc)

set(ntclient_src
	Ntrip/Src/ntrip_client.cpp
)
add_executable(ntclient ntclient.cpp ${ntclient_src})
target_include_directories(ntclient PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ntclient PRIVATE ntrip_lib rt)

set(ntserver_src
	Ntrip/Src/ntrip_server.cpp
)
add_executable(ntserver ntserver.cpp ${ntserver_src})
target_include_directories(ntserver PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ntserver PRIVATE ntrip_lib rt)

set(ntcaster_src
	Ntrip/Src/async_io.cpp
	Ntrip/Src/corr_consume.cpp
	Ntrip/Src/corr_supply.cpp
	Ntrip/Src/dispatcher.cpp
	Ntrip/Src/tcp_server.cpp
)
add_executable(ntcaster ntcaster.cpp ${ntcaster_src})
target_include_directories(ntcaster PRIVATE ${CMAKE_SOURCE_DIR} Ntrip/Inc)


####################################################################################################
enable_testing()

add_library(ntclient_cov OBJECT ${ntclient_src} ${ntrip_src})
target_include_directories(ntclient_cov PUBLIC ${CMAKE_SOURCE_DIR} Ntrip/Inc)
target_compile_options(ntclient_cov PRIVATE -g3 -O0 -fprofile-arcs -ftest-coverage)

set(testcppu_src
	Ntrip/Src/tcp_server.cpp
	Tests/RunAllTests.cpp
	Tests/testU1.cpp
	Tests/TestEncoder.cpp
	Tests/TestTcpServer.cpp
)
add_executable(${PROJECT_NAME}_CppUtest ${testcppu_src})
target_link_libraries(${PROJECT_NAME}_CppUtest CppUTest pthread rt ntclient_cov gcov)
add_test(NAME ${PROJECT_NAME}_TCppU COMMAND ${PROJECT_NAME}_CppUtest)

set(testgsuite_src
	Tests/testGoo1.cpp
	Tests/gtest_cli.cpp
	Tests/gtestNmea.cpp
	Tests/gtestNtripClient.cpp
	Tests/gtestTcp.cpp
)
add_executable(${PROJECT_NAME}_gtest ${testgsuite_src})
target_link_libraries(${PROJECT_NAME}_gtest gtest gtest_main pthread rt ntclient_cov gcov)
add_test(NAME ${PROJECT_NAME}_GTest COMMAND ${PROJECT_NAME}_gtest)

add_custom_target(coverage
	COMMAND make ${PROJECT_NAME}_gtest
	COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_gtest
	COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --html --html-details -o cov.html ${CMAKE_BINARY_DIR}
)
