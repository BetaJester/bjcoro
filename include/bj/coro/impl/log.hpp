// Copyright(C) 2021 Glenn Duncan <betajester@betajester.com>
// See README.md, LICENSE, or go to https://github.com/BetaJester/bjcoro
// for details.

#pragma once

#ifndef NDEBUG
#include <thread>
#include <fmt/format.h>
#endif

namespace bj::impl {

    template<typename Str, typename ...Args>
    inline void log(const Str &str, Args &&...args) noexcept {
#ifndef NDEBUG
        fmt::print("[{}] {}\n", std::hash<std::thread::id>{}(std::this_thread::get_id()), fmt::format(str, args...));
#endif
    }

} // namespace bj::impl.