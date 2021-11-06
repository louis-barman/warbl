cmake_minimum_required(VERSION 3.5)

#project(warble-code LANGUAGES CXX)
project(warble-code)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

# see https://google.github.io/googletest/quickstart-cmake.html
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


include_directories(../warbl_firmware)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNIT_TESTS DEBUG_ENABLED=0)

enable_testing()

add_executable(register_ctrl_test
    ../warbl_firmware/register_ctrl.cpp ../warbl_firmware/register_ctrl.h
    ../warbl_firmware/warbl.h
    ../warbl_firmware/debug.h
    ../warbl_firmware/fingering.cpp ../warbl_firmware/fingering.h
    register_ctrl_test.cpp
    fingering_test.cpp
    warbl_mock.cpp warbl_mock.h
)


target_link_libraries(
  register_ctrl_test
  gtest_main
)

include(GoogleTest)
