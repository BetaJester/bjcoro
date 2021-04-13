// Copyright(C) 2021 Glenn Duncan <betajester@betajester.com>
// See README.md, LICENSE, or go to https://github.com/BetaJester/bjcoro
// for details.

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