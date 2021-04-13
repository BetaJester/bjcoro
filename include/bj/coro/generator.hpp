#pragma once

#include "coroutine.hpp"
#include "impl/log.hpp"

namespace bj {

    template<typename T>
    struct generator
    {
        struct promise_type;
        using handle_type = co::coroutine_handle<promise_type>;

        handle_type coro;

        generator(handle_type h) : coro{ h } { impl::log("generator::generator(handle_type)"); }
        ~generator() {
            impl::log("~generator::generator()"); 
            if (coro) coro.destroy();
        }
        generator(const generator &) = delete;
        generator& operator=(const generator &) = delete;
        generator(generator &&other) : coro{ std::exchange(other.coro, nullptr) } { impl::log("generator::generator(generator&&)"); }
        generator& operator=(generator &&rhs)
        {
            impl::log("generator::operator=(generator&&)");
            coro = std::exchange(rhs.coro, nullptr);
            return *this;
        }
        T get_next_value() // Ours, not std required.
        {
            impl::log("generator::get_next_value()");
            coro.resume();
            return coro.promise().current_value;
        }

        struct promise_type
        {
            promise_type() { impl::log("generator::promise_type::promise_type()"); }
            ~promise_type() { impl::log("generator::promise_type::~promise_type()"); }
            co::suspend_always initial_suspend() { impl::log("generator::promise_type::initial_suspend()"); return {}; }
            co::suspend_always final_suspend() noexcept { impl::log("generator::promise_type::final_suspend()"); return {}; }
            auto get_return_object()
            {
                impl::log("generator::promise_type::get_return_object()");
                return generator{ handle_type::from_promise(*this) };
            }
            co::suspend_always yield_value(T value)
            {
                impl::log("generator::promise_type::yield_value()");
                current_value = value;
                return {};
            }
            void return_void() { impl::log("generator::promise_type::return_void()"); }
            void unhandled_exception() { std::exit(1); }

            T current_value{};
        };
    };

} // namespace bj.