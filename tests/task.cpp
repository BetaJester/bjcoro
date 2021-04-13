#include <bj/coro/eztask.hpp>
#include <catch2/catch.hpp>

bj::eztask<int> simple_task_usage() {
    co_return 42;
}

bj::eztask<> void_task_usage(int &x) {
    x = co_await simple_task_usage();
}

TEST_CASE("simple future usage", "[coro,future]") {

    //auto fut = simple_task_usage();
    //
    //REQUIRE(fut.get() == 42);

    int x = 1;
    auto t = void_task_usage(x);
    t.get();
    REQUIRE(x == 42);

}
