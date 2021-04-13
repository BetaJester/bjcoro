// Copyright(C) 2021 Glenn Duncan <betajester@betajester.com>
// See README.md, LICENSE, or go to https://github.com/BetaJester/bjcoro
// for details.

#include "coroutine.hpp"

namespace bj
{
    struct fire_and_forget {};
} // namespace bj.

template <typename... Args>
struct co::coroutine_traits<bj::fire_and_forget, Args...>
{
    struct promise_type
    {
        constexpr void return_void() const noexcept {}
        static constexpr void unhandled_exception() { throw; }
        [[nodiscard]] constexpr bj::fire_and_forget get_return_object() const noexcept { return {}; }
        [[nodiscard]] constexpr suspend_never initial_suspend() const noexcept { return {}; }
        [[nodiscard]] constexpr suspend_never final_suspend() const noexcept { return {}; }
    };
};
