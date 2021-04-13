cmake_minimum_required(VERSION 3.19)
project(bjcoro C CXX)

option(BJ_BUILD_CORO_TESTS "Build bj coroutine tests" OFF)
option(BJ_BUILD_CORO_EXAMPLES "Build bj coroutine examples" OFF)

include(FetchContent)
FetchContent_Declare(pwropt		GIT_REPOSITORY https://github.com/BetaJester/pwropt.git		GIT_TAG main)
FetchContent_Declare(fmt        GIT_REPOSITORY https://github.com/fmtlib/fmt.git			GIT_TAG master)
FetchContent_MakeAvailable(pwropt fmt)

set(thing bjcoro)
add_library(${thing} STATIC 
"include/bj/coro/impl/log.hpp"
"include/bj/coro/task.hpp"
"include/bj/coro/future_async.hpp"
"include/bj/coro/generator.hpp"
"src/dummy.cpp"
"include/bj/coro/coroutine.hpp"
 "include/bj/coro/impl/traits_n_concepts.hpp" "include/bj/coro/eztask.hpp")
add_library(bj::coro ALIAS ${thing})
target_include_directories(${thing} PUBLIC "include")
pwropt_target_cxx_latest(${thing} PUBLIC)
pwropt_target_compile_options(${thing} PUBLIC pwr_lefticus pwr_werror)
pwropt_target_compile_options(${thing} COMPILERS Clang PUBLIC -stdlib=libc++)
pwropt_target_link_options(${thing} COMPILERS Clang PUBLIC -rpath -lc++ -lc++abi -lc++experimental)
pwropt_target_compile_options(${thing} COMPILERS GNU PUBLIC -fcoroutines)
pwropt_target_link_libraries(${thing} COMPILERS GNU PUBLIC pthread)
target_link_libraries(${thing} PUBLIC fmt)

if(${BJ_BUILD_CORO_EXAMPLES})
	add_subdirectory("examples")
endif()

if(${BJ_BUILD_CORO_TESTS})
	enable_testing()
	add_subdirectory("tests")
endif()