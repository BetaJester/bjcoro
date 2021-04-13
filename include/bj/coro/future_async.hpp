#pragma once

#include <memory>
#include <thread>
#include "coroutine.hpp"
#include "impl/log.hpp"

namespace bj 
{

    template<typename T>
    struct future_async
    {
        struct promise_type;
        using handle_type = co::coroutine_handle<promise_type>;

        handle_type coro;

        future_async(handle_type h) : coro{ h } { impl::log("future_async::future_async(handle_type)"); }
        ~future_async() 
        {
            impl::log("future_async::~future_async()"); 
            if (coro) coro.destroy();
        }
        T get() 
        {
            impl::log("future_async::get()");
            std::thread t([this] { coro.resume(); });
            t.join();
            return coro.promise().result;
        }

        struct promise_type
        {
            T result;

            promise_type() { impl::log("future_async::promise_type::promise_type()"); }
            ~promise_type() { impl::log("future_async::promise_type::~promise_type()"); }
            auto get_return_object() 
            { 
                impl::log("future_async::promise_type::get_return_object()"); 
                return future_async{ handle_type::from_promise(*this) };
            }
            void return_value(T v) 
            { 
                impl::log("future_async::promise_type::return_value(T)"); 
                result = v; 
            }
            co::suspend_always initial_suspend() { impl::log("future_async::promise_type::initial_suspend()"); return {}; }
            co::suspend_always final_suspend() noexcept { impl::log("future_async::promise_type::final_suspend()"); return {}; }
            void unhandled_exception() { std::exit(1); }
            //void return_void() {}
        };
    };

} // namespace bj.