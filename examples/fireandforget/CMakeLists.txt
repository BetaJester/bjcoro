# Copyright(C) 2021 Glenn Duncan <betajester@betajester.com>
# See README.md, LICENSE, or go to https://github.com/BetaJester/bjcoro
# for details.

include(FetchContent)
FetchContent_Declare(fmt        GIT_REPOSITORY https://github.com/fmtlib/fmt.git			GIT_TAG master)
FetchContent_MakeAvailable(fmt)

set(thing bjcoro_fireandforget)
add_executable(${thing} "fireandforget.cpp")
target_link_libraries(${thing} PRIVATE Catch2 fmt bj::coro)
pwropt_target_cxx_latest(${thing} PRIVATE)
pwropt_target_compile_options(${thing} PRIVATE pwr_lefticus pwr_wall)
pwropt_target_compile_options(${thing} COMPILERS Clang PUBLIC -stdlib=libc++)
pwropt_target_link_options(${thing} COMPILERS Clang PUBLIC -rpath -lc++ -lc++abi -lc++experimental)
pwropt_target_compile_options(${thing} COMPILERS GNU PUBLIC -fcoroutines)
pwropt_target_link_libraries(${thing} COMPILERS GNU PUBLIC pthread)
