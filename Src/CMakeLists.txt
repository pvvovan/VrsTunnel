cmake_minimum_required (VERSION 3.10.2)
project (ntrip)

#SET (CMAKE_CXX_COMPILER             "/home/vovan/Desktop/uboot-x15/vanilla/toolchain/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++")
SET (CMAKE_CXX_COMPILER             "clang++-8")
#SET (CMAKE_CXX_COMPILER             "g++-8")
#SET (CMAKE_CXX_COMPILER             "/home/vovan/llvm-project/build/bin/clang++")
#set(CMAKE_CXX_FLAGS                "-Wall -Werror")
#set(CMAKE_CXX_FLAGS_DEBUG          "-g -Og")
set(CMAKE_CXX_STANDARD 17)

# SET (CMAKE_AR      "/usr/bin/llvm-ar")
# SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
# SET (CMAKE_NM      "/usr/bin/llvm-nm")
# SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
# SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")

set(ntrip_source Ntrip/Src/login_encode.cpp Ntrip/Src/base64_encoder.cpp)

include_directories(Ntrip/Inc)
# add_library(Ntrip Ntrip/Src/login_encode.cpp)
add_executable(prog main.cpp ${ntrip_source})

enable_testing()

set(test1_source Tests/RunAllTests.cpp Tests/test1.cpp Tests/TestEncoder.cpp Tests/TestNmea.cpp ${ntrip_source})
add_executable(test1CppU ${test1_source})

target_link_libraries(test1CppU CppUTest pthread)

add_test(NAME Test1 COMMAND test1CppU)

# Makefile generation
# cmake -DCMAKE_BUILD_TYPE=Debug ../Src/
