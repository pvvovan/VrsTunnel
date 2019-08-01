cmake_minimum_required (VERSION 3.10.2)
project (ntrip)

#SET (CMAKE_CXX_COMPILER             "/home/vovan/Desktop/uboot-x15/vanilla/toolchain/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++")
SET (CMAKE_CXX_COMPILER             "clang++-8")
#SET (CMAKE_CXX_COMPILER             "g++-8")
#SET (CMAKE_CXX_COMPILER             "/home/vovan/llvm-project/build/bin/clang++")
set(CMAKE_CXX_FLAGS                "-Wall -Werror")
#set(CMAKE_CXX_FLAGS_DEBUG          "-g -Og")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS          "-lrt")

# SET (CMAKE_AR      "/usr/bin/llvm-ar")
# SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
# SET (CMAKE_NM      "/usr/bin/llvm-nm")
# SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
# SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")

set(ntrip_source Ntrip/Src/login_encode.cpp Ntrip/Src/base64_encoder.cpp Ntrip/Src/nmea.cpp)
set(ntrip_source ${ntrip_source} Ntrip/Src/async_io.cpp Ntrip/Src/tcp_client.cpp Ntrip/Src/NtripClient.cpp)

include_directories(Ntrip/Inc)
# add_library(Ntrip Ntrip/Src/login_encode.cpp)
add_executable(prog main.cpp ${ntrip_source})

enable_testing()

set(test1cppu_source Tests/RunAllTests.cpp Tests/testU1.cpp Tests/TestEncoder.cpp ${ntrip_source})
set(test1cppu_source ${test1cppu_source} )
add_executable(test1CppU ${test1cppu_source})
target_link_libraries(test1CppU CppUTest pthread)
add_test(NAME Test1cppu COMMAND test1CppU)

# gtbuild is output directory of googletest build
link_directories(../gtest/googletest-release-1.8.1/gtbuild/)
set(test2gsui_source Tests/testGoo1.cpp Tests/gtestNmea.cpp Tests/gtestNtripClient.cpp ${ntrip_source})
set(test2gsui_source ${test2gsui_source} Tests/gtestTcp.cpp)
add_executable(test2gsui ${test2gsui_source})
# include directory from googletest source
target_include_directories(test2gsui PUBLIC ../gtest/googletest-release-1.8.1/googletest/include/)
target_link_libraries(test2gsui gtest gtest_main pthread)
add_test(NAME Test2Gglt COMMAND test2gsui)



# Makefile generation
# cmake -DCMAKE_BUILD_TYPE=Debug ../Src/
