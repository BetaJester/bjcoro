#pragma once

#if __has_include(<coroutine>)
#include <coroutine>
namespace co = std;
#elif __has_include(<experimental/coroutine>)
#include <experimental/coroutine>
namespace co = std::experimental;
#else
#error No coroutine headers found
#endif