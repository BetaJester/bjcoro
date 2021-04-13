#pragma once

#include <memory>
#include <optional>
#include "coroutine.hpp"
#include "impl/traits_n_concepts.hpp"

namespace bj
{

    template<typename T> struct eztask;

    namespace impl {

        struct [[nodiscard]] ezpromise_base
        {
            [[nodiscard]]
            co::suspend_always initial_suspend() noexcept { return {}; }
            [[nodiscard]]
            co::suspend_always final_suspend() noexcept { return {}; }
            [[noreturn]]
            static void unhandled_exception() { throw; }

        };

        template<typename T>
        struct [[nodiscard]] ezpromise_type final : ezpromise_base
        {
            using value_type = T;

            [[nodiscard]]
            eztask<T> get_return_object() noexcept { return eztask<T>{ co::coroutine_handle<ezpromise_type>::from_promise(*this) }; }
            void return_value(value_type v) noexcept { result = v; }
            T outcome() noexcept { return *result; }

        private:

            std::optional<value_type> result;
        };

        template<>
        struct [[nodiscard]] ezpromise_type<void> : ezpromise_base
        {
            [[nodiscard]]
            eztask<void> get_return_object() noexcept; // Definition below task.
            void return_void() noexcept {}
            void outcome() noexcept {}
        };

        template<typename T>
        struct [[nodiscard]] ezawaitable_base
        {
            explicit ezawaitable_base(co::coroutine_handle<ezpromise_type<T>> h) noexcept : coro{ h } {}
            [[nodiscard]]
            bool await_ready() const noexcept { return !coro || coro.done(); }
            void await_suspend(co::coroutine_handle<>) noexcept {}
            // await_resume, returning outcome, in child.

        protected:

            co::coroutine_handle<ezpromise_type<T>> coro;
        };

    } // namespace impl.


    template<typename T = void>
    struct [[nodiscard]] eztask final
    {
        using promise_type = impl::ezpromise_type<T>;

        eztask() noexcept = default;
        explicit eztask(const co::coroutine_handle<promise_type> h) noexcept : coro{ h } {}
        eztask(eztask &&other) noexcept : coro{ std::exchange(other.coro, {}) } {}
        [[nodiscard]]
        eztask &operator=(eztask &&rhs) noexcept
        {
            if (this != &rhs) {
                if (coro) coro.destroy();
                coro = std::exchange(rhs.coro, {});
            }
            return *this;
        }
        ~eztask() { if (coro) coro.destroy(); }
        eztask(const eztask &) = delete;
        eztask &operator=(const eztask &) = delete;
        [[nodiscard]]
        T get()
        {
            coro.resume();
            return coro.promise().outcome();
        }

        auto operator co_await() const & noexcept {
            struct awaitable : impl::ezawaitable_base<T> {
                using ezawaitable_base::ezawaitable_base;
                T await_resume()
                {
                    if (!coro) throw std::runtime_error("Resuming bad coroutine");
                    return coro.promise().outcome();
                }
            };
            return awaitable{ coro };
        }

        auto operator co_await() const && noexcept {
            struct awaitable : impl::ezawaitable_base<T> {
                using ezawaitable_base::ezawaitable_base;
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


    eztask<void> impl::ezpromise_type<void>::get_return_object() noexcept { return eztask<void>{ co::coroutine_handle<ezpromise_type>::from_promise(*this) }; }

} // namespace bj.