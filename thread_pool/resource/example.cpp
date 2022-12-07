//
// Created by ghost-him on 22-9-28.
//

#include <iostream>
#include <Thread_Pool.h>

void print(int i) {
    printf("----%d\n", i);
    // 模拟执行一个两秒的任务
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

void log(const char * str) {
    std::cout << "自定义 " << str << std::endl;
}


int main() {
    auto threadpool = Thread_Pool::ptr;

    // 初始化 + 运行线程池
    threadpool->init();
    threadpool->run();

    // 设置自定义的日志函数
    threadpool->set_info_function(log);

    // 添加200个任务
    for (int i = 0; i < 200; i++) {
        threadpool->commit(print, i);
    }

    std::this_thread::sleep_for(std::chrono::seconds(20));

    threadpool->close();

}
