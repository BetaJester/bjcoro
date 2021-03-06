# Copyright(C) 2021 Glenn Duncan <betajester@betajester.com>
# See README.md, LICENSE, or go to https://github.com/BetaJester/bjcoro
# for details.

include(FetchContent)
FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v2.x)
FetchContent_Declare(fmt        GIT_REPOSITORY https://github.com/fmtlib/fmt.git			GIT_TAG master)
FetchContent_MakeAvailable(Catch2 fmt)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${Catch2_SOURCE_DIR}/contrib")
include(Catch)
include(CTest)

set(thing bjcoro_tests)
add_executable(${thing} "main.cpp" "task.cpp")
target_link_libraries(${thing} PRIVATE Catch2 fmt bj::coro)
pwropt_target_cxx_latest(${thing} PRIVATE)
pwropt_target_compile_options(${thing} PRIVATE pwr_lefticus pwr_wall)
pwropt_target_compile_options(${thing} COMPILERS Clang PUBLIC -stdlib=libc++)
pwropt_target_link_options(${thing} COMPILERS Clang PUBLIC -rpath -lc++ -lc++abi -lc++experimental)
pwropt_target_compile_options(${thing} COMPILERS GNU PUBLIC -fcoroutines)
pwropt_target_link_libraries(${thing} COMPILERS GNU PUBLIC pthread)

catch_discover_tests(bjcoro_tests)
