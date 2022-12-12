#include <string>
#include <memory>
#include <utility>
#include <iostream>
#include <coroutine>
#include <cstdio>
#include <vector>
using namespace std::literals;
struct Generator {
    struct promise_type {
        int _val;
        void unhandled_exception() noexcept {}
        Generator get_return_object() { return Generator{ this }; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int val) noexcept {
            _val = val;
            return {};
        }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
    };

    using Handle = std::coroutine_handle<promise_type>;
    Handle mCoroHdl{};

    Generator(promise_type* p) :mCoroHdl{ Handle::from_promise(*p) } {} 
    Generator(Generator&& rhs) noexcept :mCoroHdl{ std::exchange(rhs.mCoroHdl, nullptr) } {}
    ~Generator() {
        if (mCoroHdl) { mCoroHdl.destroy(); }
    }
    bool finished() const {
        return mCoroHdl.done();
    }
    void resume() {
        if (not finished()) {
            mCoroHdl.resume();
        }
    }
    int value() const {
        return mCoroHdl.promise()._val;
    }
};

Generator interleaved(std::vector<int> a, std::vector<int> b)
{
    auto lambda = [](std::vector<int>& v) ->Generator {
        for (const auto& e : v) {
            co_yield e;
        }
    };
    auto x = lambda(a);
    auto y = lambda(b);
    while (not x.finished() or not y.finished()) {
        if (not x.finished()) {
            co_yield x.value();
            x.resume();
        }
        if (not y.finished()) {
            co_yield y.value();
            y.resume();
        }
    }
}

int main() {
    std::vector a{ 2,4,6,8 };
    std::vector b{ 3,5,7,9 };
    Generator g = interleaved(std::move(a), std::move(b));
    while (not g.finished()) {
        std::cout << g.value() << std::endl;
        g.resume();
    }
}