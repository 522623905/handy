#pragma once
#include "logging.h"
#include "util.h"
#include "net.h"
#include "threads.h"
#include "codec.h"
#include <utility>
#include <set>
#include <memory>
#include <unistd.h>

namespace handy {
struct Channel;
struct TcpConn;
struct TcpServer;
struct IdleIdImp;
struct EventsImp;
struct EventBase;
typedef std::unique_ptr<IdleIdImp> IdleId;
typedef std::pair<int64_t, int64_t> TimerId;

//自动管理上下文
struct AutoContext: noncopyable {
    void* ctx;
    Task ctxDel;
    AutoContext():ctx(0) {}
    //返回上下文
    template<class T> T& context() {
        //首次使用时分配
        if (ctx == NULL) {
            ctx = new T();
            ctxDel = [this] { delete (T*)ctx; }; //lambda设置析构函数
        }
        return *(T*)ctx;
    }
    ~AutoContext() { if (ctx) ctxDel(); } //析构时释放该上下文
};

}
