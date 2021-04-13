// Copyright(C) 2021 Glenn Duncan <betajester@betajester.com>
// See README.md, LICENSE, or go to https://github.com/BetaJester/bjcoro
// for details.

// Based on https://devblogs.microsoft.com/oldnewthing/20191209-00/?p=103195

#include <thread>
#include "coroutine.hpp"

namespace bj {

    struct resume_new_thread final : co::suspend_always
    {
        void await_suspend(co::coroutine_handle<> handle) noexcept
        {
            std::thread(handle).detach();
        }
    };

} // namespace bj.