cmake_minimum_required (VERSION 3.30)
project(ntrip LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(COMPILE_WARNING_MODE HIGH)

include(FetchContent)
FetchContent_Declare(
	cpputest
	GIT_REPOSITORY https://github.com/cpputest/cpputest.git
	GIT_TAG        b6d49e4846963d40eb8e5ff24228e63ccf2221d8
)
set(CPPUTEST_MEM_LEAK_DETECTION_DISABLED ON) # cpputest compilation error workaround
FetchContent_MakeAvailable(cpputest)

add_library(cli_lib OBJECT)
add_library(cli::lib ALIAS cli_lib)
target_sources(cli_lib PUBLIC
	FILE_SET cli_set
	TYPE CXX_MODULES
	FILES cli.cppm
)

set(ntrip_src
	Ntrip/Src/async_io.cpp
	Ntrip/Src/base64_encoder.cpp
	Ntrip/Src/login_encode.cpp
	Ntrip/Src/mount_point.cpp
	Ntrip/Src/nmea.cpp
	Ntrip/Src/tcp_client.cpp
)
add_library(ntrip_lib OBJECT ${ntrip_src})
add_library(ntrip::lib ALIAS ntrip_lib)
target_include_directories(ntrip_lib PUBLIC Ntrip/Inc)
target_link_libraries(ntrip_lib PUBLIC rt)

set(ntclient_src
	Ntrip/Src/ntrip_client.cpp
)
add_executable(ntclient ntclient.cpp ${ntclient_src})
target_link_libraries(ntclient PRIVATE ntrip::lib cli::lib)

set(ntserver_src
	Ntrip/Src/ntrip_server.cpp
)
add_executable(ntserver ntserver.cpp ${ntserver_src})
target_link_libraries(ntserver PRIVATE ntrip::lib cli::lib)

set(ntcaster_src
	Ntrip/Src/corr_consume.cpp
	Ntrip/Src/corr_supply.cpp
	Ntrip/Src/dispatcher.cpp
	Ntrip/Src/tcp_server.cpp
)
add_executable(ntcaster ntcaster.cpp ${ntcaster_src})
target_link_libraries(ntcaster PRIVATE ntrip::lib)


####################################################################################################
enable_testing()

add_library(ntclient_cov OBJECT ${ntclient_src} ${ntrip_src})
add_library(ntclient::cov ALIAS ntclient_cov)
target_sources(ntclient_cov PUBLIC
	FILE_SET cli_set
	TYPE CXX_MODULES
	FILES cli.cppm
)
target_include_directories(ntclient_cov PUBLIC Ntrip/Inc)
target_compile_options(ntclient_cov PRIVATE -g3 -O0 -fprofile-arcs -ftest-coverage)
target_link_libraries(ntclient_cov PUBLIC pthread rt gcov)
target_link_options(ntclient_cov INTERFACE --coverage)

set(testcppu_src
	Ntrip/Src/tcp_server.cpp
	Tests/RunAllTests.cpp
	Tests/testU1.cpp
	Tests/TestEncoder.cpp
	Tests/TestTcpServer.cpp
)
add_executable(${PROJECT_NAME}_CppUtest ${testcppu_src})
target_link_libraries(${PROJECT_NAME}_CppUtest PRIVATE
	CppUTest::CppUTest
	CppUTest::CppUTestExt
	ntclient::cov
)
add_test(NAME ${PROJECT_NAME}_TCppU COMMAND ${PROJECT_NAME}_CppUtest)

set(testgsuite_src
	Tests/testGoo1.cpp
	Tests/gtest_cli.cpp
	Tests/gtestNmea.cpp
	Tests/gtestNtripClient.cpp
	Tests/gtestTcp.cpp
)
add_executable(${PROJECT_NAME}_gtest ${testgsuite_src})
target_link_libraries(${PROJECT_NAME}_gtest PRIVATE gtest gtest_main ntclient::cov)
add_test(NAME ${PROJECT_NAME}_GTest COMMAND ${PROJECT_NAME}_gtest)

add_custom_target(coverage
	COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_CppUtest
	COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_gtest
	COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --html --html-details -o cov.html ${CMAKE_BINARY_DIR}
)
add_dependencies(coverage ${PROJECT_NAME}_CppUtest ${PROJECT_NAME}_gtest)
