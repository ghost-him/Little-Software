#include <iostream>
#include <atomic>
#include <mutex>
#include <functional>
#include <memory>
#include <thread>
#include <deque>
#include <condition_variable>
#include <map>

class Thread_Pool final {
public:
    static Thread_Pool* ptr;
    // 用于初始化线程池
    void init();

    // 运行线程池
    void run();

    // 关闭线程池
    void close();

    // 添加任务
    template <class Func, class ...Arg>
    void commit(Func && function, Arg&& ... args ) {
        info("add new commit");
        // 创建的任务
        auto task = std::bind(std::forward<Func>(function), std::forward<Arg>(args)...);
        total_mutex.lock();
        task_deque.push_back(task);
        total_mutex.unlock();
        // 让一个线程来执行该函数
        task_mutex.notify_one();
    }

    void set_max_thread(int x) {max_thread = x;}

    void set_min_thread(int x) {min_thread = x;}

    void set_grow_thread(int x) {grow_thread = x;}

    void set_checking_time(int x) {checking_time = x;}

    unsigned int get_max_thread() {return max_thread;}

    unsigned int get_min_thread() {return min_thread;}

    unsigned int get_grow_thread() {return grow_thread;}

    unsigned int get_total_thread() {return total_thread;}

    unsigned int get_living_thread() {return living_thread;}

    unsigned int get_checking_time() {return checking_time;}

    void set_info_function(void (*func)(const char *)) {function = func;};

    void info(const char * str) {
        if (function == nullptr)
            std::cerr << str << "\n";
        else
            function(str);
    }


private:
    /*
     * 线程池的属性
     */
    // 最大的线程数
    std::atomic<unsigned int> max_thread;
    // 最小的线程数
    std::atomic<unsigned int> min_thread;
    // 增长的线程数
    std::atomic<unsigned int> grow_thread;

    // 总线程数
    std::atomic<unsigned int> total_thread;
    // 工作的线程数
    std::atomic<unsigned int> living_thread;
    // 即将被销毁的线程数
    unsigned int destroyed_thread;
    // 检查的时间间隔
    std::atomic<unsigned int> checking_time;

    // 线程池的锁
    std::mutex total_mutex;

    // 任务锁
    std::condition_variable task_mutex;

    std::deque<std::function<void()>> task_deque;

    // 维护普通线程
    std::map<std::thread::id, std::thread> thread;

    // 维护守护线程
    std::thread daemon;

    // 是否被初始化了
    bool is_inited = false;

    // 是否被关闭了
    bool is_closed = true;

    // 日志函数
    void (*function)(const char *) = nullptr;

    // 普通线程执行的函数
    void common_thread();
    // 守护线程执行的函数
    void daemon_thread();
    // 添加线程
    void add_thread();
    // 减少线程
    void mis_thread();

};

