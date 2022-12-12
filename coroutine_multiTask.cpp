//#include <string>
//#include <memory>
//#include <utility>
//#include <iostream>
//#include <coroutine>
//#include <cstdio>
//#include <vector>
//#include <list>
//using namespace std::literals;
//
//struct Scheduler {
//    std::list<std::coroutine_handle<>> _tasks{};
//    bool schedule()
//    {
//        std::coroutine_handle<> task = _tasks.front();
//        _tasks.pop_front();
//        if (not task.done()) { task.resume(); }
//        return not _tasks.empty();
//    }
//
//    auto suspend()
//    {
//        struct awaiter :std::suspend_always {
//            Scheduler& _sched;
//            explicit awaiter(Scheduler& sched): _sched{sched} {}
//            void await_suspend(std::coroutine_handle<>coro) const noexcept {
//                _sched._tasks.push_back(coro);
//            }
//        };
//        return awaiter(*this);
//    }
//};
//struct Task {
//    struct promise_type {
//        Task get_return_object() { return {}; }
//        std::suspend_never initial_suspend() noexcept { return {}; }
//        std::suspend_never final_suspend() noexcept { return {}; }
//        void unhandled_exception() noexcept {}
//        void return_void() noexcept {}
//    };
//};
//Task taskA(Scheduler& sched)
//{
//    std::cout << "Hello, from taskA\n";
//    co_await sched.suspend();
//    std::cout << "a is back doing work\n";
//    co_await sched.suspend();
//    std::cout << "a is back doing more work\n";
//}
//Task taskB(Scheduler& sched)
//{
//    std::cout << "Hello, from taskB\n";
//    co_await sched.suspend();
//    std::cout << "b is back doing work\n";
//    co_await sched.suspend();
//    std::cout << "b is back doing more work\n";
//}
//
//
//int main()
//{
//    Scheduler ss{};
//    taskA(ss);
//    taskB(ss);
//    while (ss.schedule()) {}
//
//}
#include <string>
#include <memory>
#include <utility>
#include <iostream>
#include <coroutine>
#include <cstdio>
using namespace std::literals;
struct Chat {
    struct promise_type {
        std::string _msgOut{}, _msgIn{};
        void unhandled_exception() noexcept {}
        Chat get_return_object() { return Chat{ this }; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always yield_value(std::string msg) noexcept {
            _msgOut = std::move(msg);
            return {};
        }
        auto await_transform(std::string) noexcept {
            struct awaiter {
                promise_type& pt;
                constexpr bool await_ready() const noexcept {
                    return true;
                }
                std::string await_resume() const noexcept {
                    return std::move(pt._msgIn);
                }
                void await_suspend(std::coroutine_handle<>) const noexcept {}
            };
            return awaiter{ *this };
        }
        void return_value(std::string msg) noexcept { _msgOut = std::move(msg); }
        std::suspend_always final_suspend() noexcept { return {}; }
    };

    using Handle = std::coroutine_handle<promise_type>;
    Handle mCoroHdl{};

    Chat(promise_type* p) :mCoroHdl{ Handle::from_promise(*p) } {}
    Chat(Chat&& rhs) noexcept :mCoroHdl{ std::exchange(rhs.mCoroHdl, nullptr) } {}
    ~Chat() {
        if (mCoroHdl) { mCoroHdl.destroy(); }
    }
    std::string listen() {
        if (not mCoroHdl.done()) { mCoroHdl.resume(); }
        return std::move(mCoroHdl.promise()._msgOut);
    }
    void answer(std::string msg) {
        mCoroHdl.promise()._msgIn = msg;
        if (not mCoroHdl.done()) { mCoroHdl.resume(); }
    }
};

Chat Fun()
{
    co_yield "Hello!\n"s;
    std::cout << co_await std::string{};
    co_return "Here\n"s;

}

int main() {
    Chat chat = Fun();
    std::cout << chat.listen();
    chat.answer("Where are you?\n"s);
    std::cout << chat.listen();
}