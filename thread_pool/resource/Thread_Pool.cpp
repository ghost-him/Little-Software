#include "Thread_Pool.h"

Thread_Pool* Thread_Pool::ptr = new Thread_Pool;

void Thread_Pool::init() {
    info("thread_pool: start init");
    // 获取硬件并发数
    unsigned int unit = std::thread::hardware_concurrency();
    // 设置最大并发数和最小并发数
    min_thread = unit;
    max_thread = unit * 4;
    grow_thread = unit;

    // 设置当前线程池的属性
    total_thread = unit;
    living_thread = 1;
    destroyed_thread = 0;
    checking_time = 5;

    is_inited = true;
}

void Thread_Pool::run() {
    if (!is_inited) {
        return ;
    }

    is_closed = false;

    // 启动线程
    for (int i = 0; i < min_thread - 1; i++) {
        std::thread new_thread = std::thread(&Thread_Pool::common_thread, this);
        thread.emplace(new_thread.get_id(), std::move(new_thread));
    }

    // 启动守护线程
    daemon = std::move(std::thread(&Thread_Pool::daemon_thread, this));

}

void Thread_Pool::close() {
    is_closed = true;
    daemon.join();
    task_mutex.notify_all();
    for (auto& item : thread) {
        item.second.join();
    }
    thread.clear();
    task_deque.clear();
    is_inited = false;
}


void Thread_Pool::common_thread() {
    while (1) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> thread_lock(total_mutex);
            while (task_deque.empty() && !is_closed) {
                task_mutex.wait(thread_lock);
                if (destroyed_thread > 0) {
                    destroyed_thread--;

                    if (total_thread > min_thread) {
                        thread[std::this_thread::get_id()].detach();
                        total_thread--;
                        thread.erase(std::this_thread::get_id());
                        return;
                    }
                }

            }

            if (is_closed) {
                total_thread--;
                return;
            }


            // 获取下一个要执行的函数
            task = task_deque.front();
            task_deque.pop_front();
        }

        living_thread ++;
        task();
        living_thread --;
    }
}

void Thread_Pool::daemon_thread() {
    // 获取当前的id
    while (1) {

        // 等待checking_time秒后再检测
        std::this_thread::sleep_for(std::chrono::seconds(checking_time));
        std::lock_guard<std::mutex> daemon_lock(total_mutex);
        // 如果关闭了
        if (is_closed) {
            return ;
        }
        // 检测是否需要添加或者减少线程
        double proportion = static_cast<double>(living_thread / total_thread);
        if (proportion > 0.8) {             // 工作的线程达到总线程的80%，则添加线程
            add_thread();
        } else if (proportion < 0.2) {      // 工作的线程小于总线程的20%，则减少线程
            mis_thread();
        }
    }
}

void Thread_Pool::add_thread() {
    info("thread_pool add thread");
    unsigned int cp_grow = grow_thread;
    unsigned int cp_max = max_thread;
    // 如果当前的线程数还没到最大的线程数
    for (int i = 0; i < cp_grow; i++) {
        if (total_thread >= cp_max)
            continue;
        total_thread ++;
        std::thread new_thread = std::thread(&Thread_Pool::common_thread, this);
        thread.emplace(new_thread.get_id(), std::move(new_thread));
    }
}

void Thread_Pool::mis_thread() {
    if (total_thread <= min_thread)
        return ;
    info("thread_pool mis thread");
    destroyed_thread = grow_thread;
    for (int i = 0; i < destroyed_thread; i++) {
        task_mutex.notify_one();
    }
}




