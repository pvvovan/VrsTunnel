cmake_minimum_required (VERSION 3.10.2)
project (ntrip)
set(CMAKE_CXX_STANDARD 17)

set(ntrip_source Ntrip/src/login_encode.cpp Ntrip/src/lefe_encoder.cpp)

include_directories(Ntrip/inc)
# add_library(Ntrip Ntrip/src/login_encode.cpp)
add_executable(mainapp main.cpp ${ntrip_source})

enable_testing()

set(test1_source Tests/RunAllTests.cpp Tests/test1.cpp Tests/TestEncoder.cpp ${ntrip_source})
add_executable(test1 ${test1_source})

target_link_libraries(test1 CppUTest pthread)

add_test(NAME Test1 COMMAND test1)

