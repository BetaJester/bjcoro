// Copyright(C) 2021 Glenn Duncan <betajester@betajester.com>
// See README.md, LICENSE, or go to https://github.com/BetaJester/bjcoro
// for details.

// Based on https://devblogs.microsoft.com/oldnewthing/20191209-00/?p=103195

#include <fmt/format.h>
#include <bj/coro/fire_and_forget.hpp>
#include <bj/coro/resume_new_thread.hpp>


bj::fire_and_forget continue_in_new_thread()
{
    fmt::print("Line {}, thread {}\n", __LINE__, std::hash<std::thread::id>{}(std::this_thread::get_id()));
    co_await bj::resume_new_thread{};
    fmt::print("Line {}, thread {}\n", __LINE__, std::hash<std::thread::id>{}(std::this_thread::get_id()));
}

int main() 
{
    continue_in_new_thread();
    std::this_thread::sleep_for(std::chrono::seconds{ 3 });
}