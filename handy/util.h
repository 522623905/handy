#pragma once
#include <string>
#include <functional>
#include <utility>
#include <vector>
#include <string.h>
#include <stdlib.h>

namespace handy {

//用于给其他类继承,使得其不能拷贝和赋值
struct noncopyable {
    noncopyable() {};
    //=delete,表示禁用该函数
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

struct util {
    static std::string format(const char* fmt, ...);
    static int64_t timeMicro();
    static int64_t timeMilli() { return timeMicro()/1000; }
    static int64_t steadyMicro();
    static int64_t steadyMilli() { return steadyMicro()/1000; }
    static std::string readableTime(time_t t);
    static int64_t atoi(const char* b, const char* e) { return strtol(b, (char**)&e, 10); }
    static int64_t atoi2(const char* b, const char* e) {
        char** ne = (char**)&e;
        int64_t v = strtol(b, ne, 10);
        return ne == (char**)&e ? v : -1;
    }
    static int64_t atoi(const char* b) { return atoi(b, b+strlen(b)); }
    static int addFdFlag(int fd, int flag);
};

//析构时调用某个functor
struct ExitCaller: private noncopyable {
    ~ExitCaller() { functor_(); }
    //注意:不可以去掉std::move!
    //因为右值引用本身是一个左值，如果不使用move，传递给functor_的参数是一个左值，
    //会调用std::function的拷贝构造函数，而不是移动拷贝构造函数
    ExitCaller(std::function<void()>&& functor): functor_(std::move(functor)) {}
private:
    std::function<void()> functor_;
};

}
