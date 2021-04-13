// Copyright(C) 2021 Glenn Duncan <betajester@betajester.com>
// See README.md, LICENSE, or go to https://github.com/BetaJester/bjcoro
// for details.

#pragma once

#include <memory>
#include <optional>
#include "coroutine.hpp"
#include "impl/traits_n_concepts.hpp"

namespace bj
{

    template<typename T> struct task;

    namespace impl {

        struct [[nodiscard]] promise_base
        {
            struct final_awaitable
            {
                [[nodiscard]]
                bool await_ready() const noexcept { return false; }
                template<typename Promise>
                [[nodiscard]]
                co::coroutine_handle<> await_suspend(co::coroutine_handle<Promise> h) noexcept
                {
                    return h.promise().continuation;
                }
                void await_resume() noexcept {}
            };

            [[nodiscard]]
            co::suspend_always initial_suspend() noexcept { return {}; }
            [[nodiscard]]
            final_awaitable final_suspend() noexcept { return {}; }
            [[noreturn]]
            static void unhandled_exception() { throw; }

            template<typename>
            friend struct awaitable_base;

        private:

            co::coroutine_handle<> continuation;
        };

        template<typename T>
        struct [[nodiscard]] promise_type final : promise_base
        {
            using value_type = T;

            [[nodiscard]]
            task<T> get_return_object() noexcept { return task<T>{ co::coroutine_handle<promise_type>::from_promise(*this) }; }
            void return_value(value_type v) noexcept { result = v; }
            T outcome() noexcept { return *result; }

        private:

            std::optional<value_type> result;
        };

        template<>
        struct [[nodiscard]] promise_type<void> : promise_base
        {
            [[nodiscard]]
            task<void> get_return_object() noexcept; // Definition below task.
            void return_void() noexcept {}
            void outcome() noexcept {}
        };

        template<typename T>
        struct [[nodiscard]] awaitable_base
        {
            explicit awaitable_base(co::coroutine_handle<promise_type<T>> h) noexcept : coro{ h } {}
            [[nodiscard]]
            bool await_ready() const noexcept { return !coro || coro.done(); }
            [[nodiscard]]
            std::coroutine_handle<> await_suspend(co::coroutine_handle<> h) noexcept 
            {
                coro.promise().continuation = h;
                return coro;
            }
            // await_resume, returning outcome, in child.

        protected:

            co::coroutine_handle<promise_type<T>> coro;
        };

    } // namespace impl.


    template<typename T = void>
    struct [[nodiscard]] task final
    {
        using promise_type = impl::promise_type<T>;

        task() noexcept = default;
        explicit task(const co::coroutine_handle<promise_type> h) noexcept : coro{ h } {}
        task(task &&other) noexcept : coro{ std::exchange(other.coro, {}) } {}
        [[nodiscard]]
        task &operator=(task &&rhs) noexcept
        {
            if (this != &rhs) {
                if (coro) coro.destroy();
                coro = std::exchange(rhs.coro, {});
            }
            return *this;
        }
        ~task() { if (coro) coro.destroy(); }
        task(const task &) = delete;
        task &operator=(const task &) = delete;
        [[nodiscard]]
        T get()
        {
            coro.resume();
            return coro.promise().outcome();
        }

        auto operator co_await() const & noexcept {
            struct awaitable : impl::awaitable_base<T> {
                using awaitable_base::awaitable_base;
                T await_resume()
                {
                    if (!coro) throw std::runtime_error("Resuming bad coroutine");
                    return coro.promise().outcome();
                }
            };
            return awaitable{ coro };
        }

        auto operator co_await() const && noexcept {
            struct awaitable : impl::awaitable_base<T> {
                using awaitable_base::awaitable_base;
                T await_resume()
                {
                    if (!coro) throw std::runtime_error("Resuming bad coroutine");
                    return std::move(coro.promise()).outcome();
                }
            };
            return awaitable{ coro };
        }

        co::coroutine_handle<promise_type> coro;
    };


    task<void> impl::promise_type<void>::get_return_object() noexcept { return task<void>{ co::coroutine_handle<promise_type>::from_promise(*this) }; }

} // namespace bj.